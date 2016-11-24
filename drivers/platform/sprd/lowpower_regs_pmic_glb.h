
//-----------aon apb register---------------//
//reg,deep_golden,light_golden
static unsigned int pmic_glb_deepsleep_regs[][3] = {
	{(0x40038000+0x00000800), 0x0000020A,0x0000037e},
	{(0x40038000+0x00000804), 0x00000000,0x00000318},
	{(0x40038000+0x00000808), 0x0000000B,0x000010ef},
	{(0x40038000+0x0000080C), 0x00000000,0x00000000},
	{(0x40038000+0x00000810), 0x0000C080,0x00006000},
	{(0x40038000+0x00000814), 0x0000EDFF,0x00006df7},
	{(0x40038000+0x00000818), 0x000068D2,0x000068d2},
	{(0x40038000+0x0000081C), 0x00006838,0x00006838},
	{(0x40038000+0x00000820), 0x00000068,0x00000068},
	{(0x40038000+0x00000824), 0x000068A0,0x000068a0},
	{(0x40038000+0x00000828), 0x00003CB4,0x00003cb4},
	{(0x40038000+0x0000082C), 0x00003C3C,0x0000a000},
	{(0x40038000+0x00000830), 0x0000B4A0,0x0000b4a0},
	{(0x40038000+0x00000834), 0x0000B4A0,0x0000b4a0},
	{(0x40038000+0x00000838), 0x00003C3C,0x00003c3c},
	{(0x40038000+0x0000083C), 0x000068D2,0x000068d3},
	{(0x40038000+0x00000840), 0x000000A0,0x0000009e},
	{(0x40038000+0x00000844), 0x00001000,0x00001000},
	{(0x40038000+0x00000848), 0x00004210,0x00004210},
	{(0x40038000+0x0000084C), 0x00001A80,0x00001a80},
	{(0x40038000+0x00000850), 0x00001A80,0x00001a80},
	{(0x40038000+0x00000854), 0x00001C80,0x00001c80},
	{(0x40038000+0x00000858), 0x00001A80,0x00001a80},
	{(0x40038000+0x0000085C), 0x00001A80,0x00001a80},
	{(0x40038000+0x00000860), 0x00001A80,0x00001a80},
	{(0x40038000+0x00000864), 0x00001A80,0x00001a80},
	{(0x40038000+0x00000868), 0x00000808,0x00000808},
	{(0x40038000+0x0000086C), 0x00000A08,0x00000a08},
	{(0x40038000+0x00000870), 0x00000808,0x00000808},
	{(0x40038000+0x00000874), 0x00003000,0x00003008},
	{(0x40038000+0x00000878), 0x00000840,0x00000840},
	{(0x40038000+0x0000087C), 0x00000C0C,0x00000c0c},
	{(0x40038000+0x00000880), 0x00000C0C,0x00000c0c},
	{(0x40038000+0x00000884), 0x00000C0C,0x00000c0c},
	{(0x40038000+0x00000888), 0x0000000C,0x0000000c},
	{(0x40038000+0x0000088C), 0x0000098A,0x00000708},
	{(0x40038000+0x00000890), 0x0000B7F8,0x00008300},
	{(0x40038000+0x00000894), 0x0000C3FF,0x0000c1f8},
	{(0x40038000+0x00000898), 0x00007F01,0x00001802},
	{(0x40038000+0x0000089C), 0x00000000,0x00000000},
	{(0x40038000+0x000008A0), 0x00000000,0x00000000},
	{(0x40038000+0x000008A4), 0x00003FFF,0x00003fff},
	{(0x40038000+0x000008A8), 0x00000303,0x00000300},
	{(0x40038000+0x000008AC), 0x00000260,0x00000360},
	{(0x40038000+0x000008B0), 0x00000842,0x00000842},
	{(0x40038000+0x000008B4), 0x00000042,0x00000042},
	{(0x40038000+0x000008B8), 0x000049A8,0x000049a8},
	{(0x40038000+0x000008BC), 0x00000397,0x00000397},
	{(0x40038000+0x000008C0), 0x00000000,0x00000048},
	{(0x40038000+0x000008C4), 0x00008000,0x00008fc0},
	{(0x40038000+0x000008C8), 0x00000000,0x00007140},
	{(0x40038000+0x000008CC), 0x00000000,0x00000e3f},
	{(0x40038000+0x000008D0), 0x00000000,0x000001ff},
	{(0x40038000+0x000008D4), 0x00000000,0x00000000},
	{(0x40038000+0x000008D8), 0x00000000,0x00000000},
	{(0x40038000+0x000008DC), 0x00000000,0x00000000},
	{(0x40038000+0x000008E0), 0x00000000,0x00000000},
	{(0x40038000+0x000008E4), 0x00000000,0x00000000},
	{(0x40038000+0x000008E8), 0x00000232,0x00000132},
	{(0x40038000+0x000008EC), 0x00000805,0x00000805},
	{(0x40038000+0x000008F0), 0x00008001,0x00008001},
	{(0x40038000+0x000008F4), 0x00000BD2,0x00000bd2},
	{(0x40038000+0x000008F8), 0x000003B4,0x000003b4},
	{(0x40038000+0x000008FC), 0x00001908,0x00001908},
	{(0x40038000+0x00000900), 0x0000000C,0x0000000c},
	{(0x40038000+0x00000904), 0x00000400,0x00008001},
	{(0x40038000+0x00000908), 0x0000000E,0x00000009},
	{(0x40038000+0x0000090C), 0x00000000,0x00000022},
	{(0x40038000+0x00000910), 0x00000110,0x00000110},
	{(0x40038000+0x00000914), 0x00000000,0x00000000},
	{(0x40038000+0x00000918), 0x00000300,0x00000300},
	{(0x40038000+0x0000091C), 0x0000F000,0x0000f000},
	{(0x40038000+0x00000920), 0x00000000,0x00000080},
	{(0x40038000+0x00000924), 0x00000000,0x00000000},
	{(0x40038000+0x00000928), 0x0000FB5B,0x00000000},
	{(0x40038000+0x0000092C), 0x00000000,0x00000000},
	{(0x40038000+0x00000930), 0x00000102,0x00000102},
	{(0x40038000+0x00000934), 0x00000165,0x00000164},
	{(0x40038000+0x00000938), 0x0000FE00,0x0000ee00},
	{(0x40038000+0x0000093C), 0x0000A090,0x0000a090},
	{(0x40038000+0x00000940), 0x00002723,0x00002723},
	{(0x40038000+0x00000944), 0x00000000,0x00000000},
	{(0x40038000+0x00000948), 0x00000001,0x00000001},
	{(0x40038000+0x0000094C), 0x00000000,0x00000000},
	{(0x40038000+0x00000950), 0x00000000,0x00000000},
	{(0x40038000+0x00000954), 0x00000000,0x00000000},
	{(0x40038000+0x00000958), 0x00000000,0x00000000},
	{(0x40038000+0x0000095C), 0x00000000,0x00000000},
	{(0x40038000+0x00000960), 0x00000000,0x00000000},
	{(0x40038000+0x00000964), 0x0000A596,0x0000a500},
	{(0x40038000+0x00000968), 0x00000000,0x00000000},
	{(0x40038000+0x0000096C), 0x00000000,0x00000000},
	{(0x40038000+0x00000970), 0x00000000,0x00000000},
	{(0x40038000+0x00000974), 0x00000000,0x00000000},
	{(0x40038000+0x00000978), 0x00000000,0x00000000},
	{(0x40038000+0x0000097C), 0x00000000,0x00000000},
	{(0x40038000+0x00000980), 0x00000000,0x00000000},
	{(0x40038000+0x00000984), 0x00000000,0x00000000},
	{(0x40038000+0x00000988), 0x00000000,0x00000000},
	{(0x40038000+0x0000098C), 0x00000000,0x00000001},
	{(0x40038000+0x00000990), 0x00000000,0x00000000},
	{(0x40038000+0x00000994), 0x00000010,0x00000010},
	{(0x40038000+0x00000998), 0x0000000D,0x00000007},
	{(0x40038000+0x0000099C), 0x00000000,0x00000000},
	{(0x40038000+0x000009A0), 0x00000000,0x00000000},
	{(0x40038000+0x000009A4), 0x00000000,0x00000000},
	{(0x40038000+0x000009A8), 0x00000000,0x00000000},
	{(0x40038000+0x000009AC), 0x00000000,0x00000000},
	{(0x40038000+0x000009B0), 0x00000000,0x00000000},
	{(0x40038000+0x000009B4), 0x00000000,0x00000000},
	{(0x40038000+0x000009B8), 0x00000000,0x00000000},
	{(0x40038000+0x000009BC), 0x00000000,0x00000000},
	{(0x40038000+0x000009C0), 0x00000000,0x00000000},
	{(0x40038000+0x000009C4), 0x00000000,0x00000000},
	{(0x40038000+0x000009C8), 0x00000000,0x00000000},
	{(0x40038000+0x000009CC), 0x00000000,0x00000000},
	{(0x40038000+0x000009D0), 0x00000000,0x00000000},
	{(0x40038000+0x000009D4), 0x00000000,0x00000000},
	{(0x40038000+0x000009D8), 0x00000000,0x00000000},
	{(0x40038000+0x000009DC), 0x00000000,0x00000000},
	{(0x40038000+0x000009E0), 0x00000000,0x00000000},
	{(0x40038000+0x000009E4), 0x00000000,0x00000000},
	{(0x40038000+0x000009E8), 0x00000000,0x00000000},
	{(0x40038000+0x000009EC), 0x00000000,0x00000000},
	{(0x40038000+0x000009F0), 0x00000000,0x00000000},
	{(0x40038000+0x000009F4), 0x00000000,0x00000000},
	{(0x40038000+0x000009F8), 0x00000000,0x00000000},
	{(0x40038000+0x000009FC), 0x00000000,0x00000000},
	{(0x40038000+0x00000A00), 0x0000005E,0x00000060},
	{(0x40038000+0x00000A04), 0x0000005E,0x00000060},
	{(0x40038000+0x00000A08), 0x000000C0,0x000000ab},
	{(0x40038000+0x00000A0C), 0x000001E0,0x000001e0},
	{(0x40038000+0x00000A10), 0x00000000,0x00000000},
	{(0x40038000+0x00000A14), 0x00000000,0x00000000},
	{(0x40038000+0x00000A18), 0x00000140,0x00000140},
	{(0x40038000+0x00000A1C), 0x00000120,0x00000180},
	{0, 0, 0},
};



