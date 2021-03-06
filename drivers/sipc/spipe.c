/*
 * Copyright (C) 2012 Spreadtrum Communications Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/poll.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#ifdef CONFIG_OF
#include <linux/of_device.h>
#endif

#include <linux/sipc.h>
#include <linux/spipe.h>

struct spipe_device {
	struct spipe_init_data	*init;
	int			major;
	int			minor;
	struct cdev		cdev;
};

struct spipe_sbuf {
	uint8_t			dst;
	uint8_t			channel;
	uint32_t		bufid;
};

static struct class		*spipe_class;

#ifdef CONFIG_SPRD_SIMDET_IOCTL
#define SIM_NOTIFY_CH	9
#define IOCTL_MODEM_STATUS	_IO('o', 0x27)
enum modem_state {
	STATE_SIM_ATTACH = 7,
	STATE_SIM_DETACH,
};

static int sim_state_changed;
static int current_sim_state;
extern void sbuf_ring_rx_wakeup(uint8_t dst, uint8_t chnnel, uint32_t bufnum);
void sprd_simdet_set_state(uint8_t dst, uint8_t channel, uint32_t bufnum,
			  int state)
{
	printk(KERN_INFO "%s: state = %d\n", __func__, state);
	sim_state_changed = 1;
	current_sim_state = state;

	sbuf_ring_rx_wakeup(dst, channel, bufnum);
}
EXPORT_SYMBOL(sprd_simdet_set_state);
#endif

static int spipe_open(struct inode *inode, struct file *filp)
{
	int minor = iminor(filp->f_path.dentry->d_inode);
	struct spipe_device *spipe;
	struct spipe_sbuf *sbuf;

	spipe = container_of(inode->i_cdev, struct spipe_device, cdev);
	if (sbuf_status(spipe->init->dst, spipe->init->channel) != 0) {
		printk(KERN_ERR "spipe %d-%d not ready to open!\n", spipe->init->dst, spipe->init->channel);
		filp->private_data = NULL;
		return -ENODEV;
	}

	sbuf = kmalloc(sizeof(struct spipe_sbuf), GFP_KERNEL);
	if (!sbuf) {
		return -ENOMEM;
	}
	filp->private_data = sbuf;

	sbuf->dst = spipe->init->dst;
	sbuf->channel = spipe->init->channel;
	sbuf->bufid = minor - spipe->minor;

	return 0;
}

static int spipe_release(struct inode *inode, struct file *filp)
{
	struct spipe_sbuf *sbuf = filp->private_data;

	if (sbuf) {
		kfree(sbuf);
	}

	return 0;
}

static ssize_t spipe_read(struct file *filp,
		char __user *buf, size_t count, loff_t *ppos)
{
	struct spipe_sbuf *sbuf = filp->private_data;
	int timeout = -1;

	if (filp->f_flags & O_NONBLOCK) {
		timeout = 0;
	}

	return sbuf_read(sbuf->dst, sbuf->channel, sbuf->bufid,
			(void *)buf, count, timeout);
}

static ssize_t spipe_write(struct file *filp,
		const char __user *buf, size_t count, loff_t *ppos)
{
	struct spipe_sbuf *sbuf = filp->private_data;
	int timeout = -1;

	if (filp->f_flags & O_NONBLOCK) {
		timeout = 0;
	}

	return sbuf_write(sbuf->dst, sbuf->channel, sbuf->bufid,
			(void *)buf, count, timeout);
}

static unsigned int spipe_poll(struct file *filp, poll_table *wait)
{
	struct spipe_sbuf *sbuf = filp->private_data;
	int ret;

#ifdef CONFIG_SPRD_SIMDET_IOCTL
	if (sim_state_changed && sbuf->bufid == SIM_NOTIFY_CH) {
		printk(KERN_INFO "%s: sim status update\n", __func__);
		return POLLHUP;
	}
#endif
	ret = sbuf_poll_wait(sbuf->dst, sbuf->channel, sbuf->bufid,
			filp, wait);

#ifdef CONFIG_SPRD_SIMDET_IOCTL
	if (sim_state_changed && sbuf->bufid == SIM_NOTIFY_CH) {
		ret |= POLLHUP;
		printk(KERN_INFO "%s: sim status changed\n", __func__);
	}
#endif
	return ret;
}

static long spipe_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
#ifdef CONFIG_SPRD_SIMDET_IOCTL
	struct spipe_sbuf *sbuf = filp->private_data;
	switch (cmd) {
		case IOCTL_MODEM_STATUS:
			if (sim_state_changed && sbuf->bufid == SIM_NOTIFY_CH) {
				printk(KERN_INFO "%s: sim status = %d\n",
						__func__, current_sim_state);
				sim_state_changed = 0;
				return current_sim_state?
					STATE_SIM_ATTACH : STATE_SIM_DETACH;
			}
			break;
		default:
			printk(KERN_ERR "ERR! undefined cmd 0x%X\n", cmd);
			return -EINVAL;
	}
#endif
	return 0;
}

static const struct file_operations spipe_fops = {
	.open		= spipe_open,
	.release	= spipe_release,
	.read		= spipe_read,
	.write		= spipe_write,
	.poll		= spipe_poll,
	.unlocked_ioctl	= spipe_ioctl,
	.owner		= THIS_MODULE,
	.llseek		= default_llseek,
};

static int spipe_parse_dt(struct spipe_init_data **init, struct device *dev)
{
#ifdef CONFIG_OF
	struct device_node *np = dev->of_node;
	struct spipe_init_data *pdata = NULL;
	int ret;
	uint32_t data;

	pdata = kzalloc(sizeof(struct spipe_init_data), GFP_KERNEL);
	if (!pdata) {
		printk(KERN_ERR "Failed to allocate pdata memory\n");
		return -ENOMEM;
	}

	ret = of_property_read_string(np, "sprd,name", (const char**)&pdata->name);
	if (ret) {
		goto error;
	}

	ret = of_property_read_u32(np, "sprd,dst", (uint32_t *)&data);
	if (ret) {
		goto error;
	}
	pdata->dst = (uint8_t)data;

	ret = of_property_read_u32(np, "sprd,channel", (uint32_t *)&data);
	if (ret) {
		goto error;
	}
	pdata->channel = (uint8_t)data;

	ret = of_property_read_u32(np, "sprd,ringnr", (uint32_t *)&pdata->ringnr);
	if (ret) {
		goto error;
	}

	ret = of_property_read_u32(np, "sprd,size-rxbuf", (uint32_t *)&pdata->rxbuf_size);
	if (ret) {
		goto error;
	}

	ret = of_property_read_u32(np, "sprd,size-txbuf", (uint32_t *)&pdata->txbuf_size);
	if (ret) {
		goto error;
	}
	*init = pdata;
	return ret;
error:
	kfree(pdata);
	*init = NULL;
	return ret;
#else
	return -ENODEV;
#endif
}

static inline void spipe_destroy_pdata(struct spipe_init_data **init)
{
#ifdef CONFIG_OF
	struct spipe_init_data *pdata = *init;

	if (pdata) {
		kfree(pdata);
	}

	*init = NULL;
#else
	return;
#endif
}

static int spipe_probe(struct platform_device *pdev)
{
	struct spipe_init_data *init = pdev->dev.platform_data;
	struct spipe_device *spipe;
	dev_t devid;
	int i, rval;

	if (pdev->dev.of_node && !init) {
		rval = spipe_parse_dt(&init, &pdev->dev);
		if (rval) {
			printk(KERN_ERR "Failed to parse spipe device tree, ret=%d\n", rval);
			return rval;
		}
	}
	pr_info("spipe: after parse device tree, name=%s, dst=%u, channel=%u, ringnr=%u, rxbuf_size=%u, txbuf_size=%u\n",
		init->name, init->dst, init->channel, init->ringnr, init->rxbuf_size, init->txbuf_size);


	rval = sbuf_create(init->dst, init->channel, init->ringnr,
		init->txbuf_size, init->rxbuf_size);
	if (rval != 0) {
		printk(KERN_ERR "Failed to create sbuf: %d\n", rval);
		spipe_destroy_pdata(&init);
		return rval;
	}

	spipe = kzalloc(sizeof(struct spipe_device), GFP_KERNEL);
	if (spipe == NULL) {
		sbuf_destroy(init->dst, init->channel);
		spipe_destroy_pdata(&init);
		printk(KERN_ERR "Failed to allocate spipe_device\n");
		return -ENOMEM;
	}

	rval = alloc_chrdev_region(&devid, 0, init->ringnr, init->name);
	if (rval != 0) {
		sbuf_destroy(init->dst, init->channel);
		kfree(spipe);
		spipe_destroy_pdata(&init);
		printk(KERN_ERR "Failed to alloc spipe chrdev\n");
		return rval;
	}

	cdev_init(&(spipe->cdev), &spipe_fops);
	rval = cdev_add(&(spipe->cdev), devid, init->ringnr);
	if (rval != 0) {
		sbuf_destroy(init->dst, init->channel);
		kfree(spipe);
		unregister_chrdev_region(devid, init->ringnr);
		spipe_destroy_pdata(&init);
		printk(KERN_ERR "Failed to add spipe cdev\n");
		return rval;
	}

	spipe->major = MAJOR(devid);
	spipe->minor = MINOR(devid);
	if (init->ringnr > 1) {
		for (i = 0; i < init->ringnr; i++) {
			device_create(spipe_class, NULL,
				MKDEV(spipe->major, spipe->minor + i),
				NULL, "%s%d", init->name, i);
		}
	} else {
		device_create(spipe_class, NULL,
			MKDEV(spipe->major, spipe->minor),
			NULL, "%s", init->name);
	}

	spipe->init = init;

	platform_set_drvdata(pdev, spipe);

	return 0;
}

static int  spipe_remove(struct platform_device *pdev)
{
	struct spipe_device *spipe = platform_get_drvdata(pdev);
	int i;

	for (i = 0; i < spipe->init->ringnr; i++) {
		device_destroy(spipe_class,
				MKDEV(spipe->major, spipe->minor + i));
	}
	cdev_del(&(spipe->cdev));
	unregister_chrdev_region(
		MKDEV(spipe->major, spipe->minor), spipe->init->ringnr);

	sbuf_destroy(spipe->init->dst, spipe->init->channel);

	spipe_destroy_pdata(&spipe->init);

	kfree(spipe);

	platform_set_drvdata(pdev, NULL);

	return 0;
}

static const struct of_device_id spipe_match_table[] = {
	{.compatible = "sprd,spipe", },
	{ },
};

static struct platform_driver spipe_driver = {
	.driver = {
		.owner = THIS_MODULE,
		.name = "spipe",
		.of_match_table = spipe_match_table,
	},
	.probe = spipe_probe,
	.remove = spipe_remove,
};

static int __init spipe_init(void)
{
	spipe_class = class_create(THIS_MODULE, "spipe");
	if (IS_ERR(spipe_class))
		return PTR_ERR(spipe_class);

	return platform_driver_register(&spipe_driver);
}

static void __exit spipe_exit(void)
{
	class_destroy(spipe_class);
	platform_driver_unregister(&spipe_driver);
}

module_init(spipe_init);
module_exit(spipe_exit);

MODULE_AUTHOR("Chen Gaopeng");
MODULE_DESCRIPTION("SIPC/SPIPE driver");
MODULE_LICENSE("GPL");
