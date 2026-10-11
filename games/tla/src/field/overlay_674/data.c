#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_020000d8(void);
void Func_02000210(void);
void Func_020002ac(void);
void Func_0200030c(void);
void Func_02000380(void);
void Func_0200040c(void);
void Func_02000440(void);
void Func_020004f4(void);
void Func_02000520(void);
void Func_02000568(void);
void Func_020006f4(void);
void Func_02000e38(void);
void Func_020014e0(void);
void Func_02001a68(void);
void Func_02001bf4(void);
void Func_02001c38(void);
void Func_02001db0(void);
void Func_02001f74(void);
void Func_0200208c(void);
void Func_020020b4(void);
void Func_020020d0(void);
void Func_02002144(void);
void Func_020021d0(void);
void Func_02002430(void);
void Func_020024c8(void);
void Func_020024f0(void);
void Func_0200253c(void);
void Func_020025a4(void);
void Func_020025b0(void);
void Func_020025bc(void);
void Func_020025dc(void);
void Func_020025fc(void);
void Func_0200261c(void);
void Func_0200263c(void);
void Func_0200265c(void);
void Func_0200267c(void);
void Func_0200274c(void);
void Func_02002780(void);
void Func_02003620(void);
void Func_0200367c(void);

extern u32 Data_020040d4[];
extern u32 Data_02004198[];
extern u32 Data_02004640[];
extern s16 Data_020046a4[];
extern s16 Data_020046ba[];

u32 Data_02004004[] = {
	23, 3, 0x1000, 0, 1, 13,
	16, 0, 17,
};

s32 Data_02004028[] = {
	22, 26, 0, 23, 7, -0x8000,
	23, 9, -1024, 23, 10, -1024,
	0, 1, 13, 24, 0, 17,
};

u16 Data_02004070[] = {
	258, 259, 293, 294, 332, 331,
};

u32 Data_0200407c[] = {
	0x820f8e0, 0x2008e0f8, 0x20f0e0, 0x2008e0f8, 0x820f8e0, 0x2008e0f8,
};

s16 Data_02004094[] = {
	0, 16, 0, 16, 0, 16, 16, 0,
	16, 0, 16, 0, 16, 0, 0, -16,
	0, -16, 0, -16, 0, -16, -16, 0,
	-16, 0, -16, 0, -16, 0, 0, 16,
};

u32 Data_020040d4[] = {
	22, 30, 129, 22, 16, 0x6666,
	22, 15, 0x6666, 39, 2, 4,
	0x1780000, 0, 0xd80000, 1, 4, 0x1880000,
	0, 0xe00000, 1, 39, 1, 0,
	1, 22, 3, 0x3000, 0, 80,
	39, 2, 4, 0x1780000, 0, 0xd80000,
	1, 4, 0x1780000, 0, 0xb80000, 1,
	39, 1, 0, 40, 13, 0xffff,
	0,
};

u32 Data_02004198[] = {
	39, 4, 22, 15, 0xcccc, 22,
	16, 0x3333, 22, 6, 0x400000, 0x80010000,
	4, 0x1600000, 0x400000, 0x1500000, 1, 4,
	0x1700000, 0x400000, 0x1500000, 1, 4, 0x1780000,
	0x400000, 0x1480000, 1, 4, 0x1780000, 0x400000,
	0x1380000, 1, 4, 0x1700000, 0x400000, 0x1300000,
	1, 4, 0x1600000, 0x400000, 0x1300000, 1,
	4, 0x1580000, 0x400000, 0x1380000, 1, 4,
	0x1580000, 0x400000, 0x1480000, 1, 13, 0xffff,
	0xc0010000,
};

u32 Data_02004274[] = {
	39, 2, 4, 0xea0000, 0, 0xf00000,
	1, 4, 0x1140000, 0, 0xde0000, 1,
	4, 0x1340000, 0, 0x1000000, 1, 4,
	0xfc0000, 0, 0x1000000, 1, 4, 0x1080000,
	0, 0xd00000, 1, 4, 0xe80000, 0,
	0xb80000, 1, 13, 0xffff, 0,
};

s32 Data_02004300[] = {
	39, 3, 41, 152, 22, 13,
	0x40000, 5, 0, 0, -0x100000, 1,
	41, 152, 22, 13, 0x40000, 5,
	-0x200000, 0, 0, 1, 41, 152,
	22, 13, 0x40000, 5, 0, 0,
	0x200000, 1, 41, 152, 22, 13,
	0x40000, 5, 0x200000, 0, 0, 1,
	41, 152, 22, 13, 0x40000, 4,
	0x1180000, 0, 0x3580000, 1, 39, 1,
	0, 1, 22, 3, 0x8000, 0,
	1, 17,
};

s32 Data_020043f8[] = {
	39, 3, 41, 152, 22, 13,
	0x40000, 5, 0, 0, 0x100000, 1,
	41, 152, 22, 13, 0x40000, 5,
	0x200000, 0, 0, 1, 41, 152,
	22, 13, 0x40000, 5, 0, 0,
	-0x200000, 1, 41, 152, 22, 13,
	0x40000, 5, -0x200000, 0, 0, 1,
	41, 152, 22, 13, 0x40000, 4,
	0x1180000, 0, 0x3580000, 1, 39, 1,
	0, 1, 22, 3, 0, 0,
	1, 17,
};

u32 Data_020044f0[] = {
	0, 15, 3, 0, 1, 38,
};

u32 Data_02004508[] = {
	4, 0xf80000, 0, 0x2ae0000, 1, 4,
	0xf80000, 0, 0x3000000, 1, 4, 0x980000,
	0, 0x3000000, 1, 22, 40, 1,
	17,
};

u32 Data_02004554[] = {
	39, 2, 4, 0xf80000, 0, 0x2ae0000,
	1, 4, 0xf80000, 0, 0x3000000, 1,
	4, 0xb00000, 0, 0x3090000, 1, 39,
	1, 17,
};

u32 Data_020045a4[] = {
	4, 0xf80000, 0, 0x3000000, 1, 4,
	0xf80000, 0, 0x2ae0000, 1, 4, 0xf80000,
	0, 0x2480000, 1, 22, 40, 1,
	17,
};

u32 Data_020045f0[] = {
	39, 2, 4, 0xf80000, 0, 0x3000000,
	1, 4, 0xf80000, 0, 0x2ae0000, 1,
	4, 0xf00000, 0, 0x2580000, 1, 39,
	1, 17,
};

u32 Data_02004640[] = {
	39, 2, 4, 0xb80000, 0, 0x3a80000,
	1, 39, 1, 1, 80, 39,
	2, 4, 0x580000, 0, 0x3a80000, 1,
	39, 1, 0, 80, 13, 0xffff,
	0,
};

s16 Data_020046a4[] = {
	92, 10, 1, 2, 6, 92, 8, 1,
	2, 6, -1,
};

s16 Data_020046ba[] = {
	-1,
};

s32 Data_020046bc[] = {
	(s32)Data_020046ba,
	0x400040,
	(s32)Data_020046a4,
	0xa0052,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 208, 0, 800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(131),
	SCENE_EXIT(1, 2, 28),
	SCENE_EXIT(2, 183, 9),
	SCENE_EXITS(132),
	SCENE_EXIT(1, 2, 29),
	SCENE_EXIT(2, 57, 5),
	SCENE_EXIT(3, 132, 1),
	SCENE_EXITS(133),
	SCENE_EXIT(1, 2, 30),
	SCENE_EXIT(2, 2, 80),
	SCENE_EXIT(3, 11, 7),
	SCENE_EXIT(4, 2, 81),
	SCENE_EXITS(134),
	SCENE_EXIT(10, 2, 31),
	SCENE_EXIT(11, 183, 8),
	SCENE_EXITS(135),
	SCENE_EXIT(1, 2, 32),
	SCENE_EXIT(2, 156, 6),
	SCENE_EXIT(3, 135, 4),
	SCENE_EXIT(4, 135, 3),
	SCENE_EXIT(5, 136, 1),
	SCENE_EXITS_END,
};

u32 Data_02004754[] = {
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_0200476c[] = {
	(s32)0xffff0049,
	1,
	0x580000,
	0,
	0x2e80000,
	0x18000,
	(s32)0xffff004d,
	1,
	0xb80000,
	0,
	0x3980000,
	0x2a000,
	(s32)0xffff004d,
	(s32)Data_02004640,
	0x580000,
	0,
	0x3a80000,
	0x10000,
	(s32)0xffff00bd,
	1,
	0x1180000,
	0,
	0x3580000,
	0x18000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

s32 Data_020047e4[] = {
	(s32)0xffff0047,
	(s32)Data_020040d4,
	0x1780000,
	0,
	0xb80000,
	0x4000,
	(s32)0xffff018f,
	1,
	0xe80000,
	0,
	0xb80000,
	0x24000,
	(s32)0xffff0102,
	7,
	0xb00000,
	0,
	0x8c0000,
	0x24000,
	(s32)0xffff0192,
	1,
	0x480000,
	0,
	0x1280000,
	0x24000,
	(s32)0xffff0192,
	1,
	0xa80000,
	0,
	0x1280000,
	0x1024000,
	(s32)0xffff0192,
	1,
	0xc80000,
	0,
	0x1280000,
	0x1024000,
	(s32)0xffff0192,
	1,
	0x1080000,
	0,
	0x1280000,
	0x1024000,
	(s32)0xffff0192,
	1,
	0x880000,
	0,
	0x1480000,
	0x1024000,
	(s32)0xffff0192,
	1,
	0xa80000,
	0,
	0x1480000,
	0x1024000,
	(s32)0xffff0192,
	1,
	0x680000,
	0,
	0x1680000,
	0x1024000,
	(s32)0xffff0192,
	1,
	0xe80000,
	0,
	0x1680000,
	0x1024000,
	(s32)0xffff0190,
	1,
	0x780000,
	0,
	0xb80000,
	0x24000,
	(s32)0xffff01e9,
	1,
	0x80000,
	0,
	0x80000,
	0x24000,
	(s32)0xffff00c9,
	1,
	0x1380000,
	0,
	0x1180000,
	0x3000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u32 Data_0200494c[] = {
	0xffff0047, 1, 0x1480000, 0, 0xf80000, 0x4000,
	0xffff0049, 1, 0x1080000, 0, 0xb80000, 0x12000,
	0xffff0050, 1, 0x840000, 0, 0xb00000, 0x18000,
	0xffff012f, 7, 0x1460000, 0, 0x1180000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_020049c4[] = {
	(s32)0xffff00bb,
	1,
	0xe80000,
	0,
	0x1280000,
	0x1c000,
	(s32)0xffff00bb,
	1,
	0x1180000,
	0,
	0x1180000,
	0x14000,
	(s32)0xffff0195,
	(s32)Data_02004198,
	0x1680000,
	0,
	0x1380000,
	0x14000,
	(s32)0xffff01e9,
	1,
	0x80000,
	0,
	0x80000,
	0x4000,
	(s32)0xffff0120,
	1,
	0xd80000,
	0,
	0x1780000,
	0x24000,
	(s32)0xffff0120,
	1,
	0x780000,
	0,
	0x1a80000,
	0x24000,
	(s32)0xffff0122,
	1,
	0x580000,
	0,
	0x1d80000,
	0x24000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u32 Data_02004a84[] = {
	0xffff004b, 2, 0xa80000, 0, 0x1280000, 0x24000,
	0xffff00a2, 1, 0x1380000, 0, 0x1b80000, 0x4000,
	0xffff00a3, 7, 0, 0, 0, 0x24000,
	0xffff01e9, 1, 0xf80000, 0, 0xca0000, 0x24000,
	0xffff01e9, 1, 0x10a0000, 0, 0xb80000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_02004b14[] = {
	-1, 0, 0,
};

s32 Data_02004b20[] = {
	1,
	(s32)0xffff0001,
	1,
	0xc602,
	(s32)0xffff0002,
	(s32)Func_02002780,
	0x602,
	(s32)0xffff0010,
	(s32)Func_0200367c,
	0,
	(s32)0xffff0008,
	MSGNO(MsgEverSinceThe),
	0,
	(s32)0xffff0009,
	(s32)Func_02001bf4,
	0,
	(s32)0xffff000a,
	MSGNO(MsgYouReLucky),
	0,
	(s32)0xffff000b,
	MSGNO(MsgArffArff2),
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgTheWaterwaysNorth),
	0x8d15,
	(s32)0xffff0009,
	MSGNO(MsgICanTWaitUntil),
	0x8d15,
	(s32)0xffff000a,
	MSGNO(MsgICanTWaitUntilThe),
	0x8d15,
	(s32)0xffff000b,
	(s32)Func_02000e38,
	-1,
	0,
	0,
};

s32 Data_02004bb0[] = {
	1,
	(s32)0xffff0001,
	1,
	0xc602,
	(s32)0xffff0002,
	(s32)Func_0200267c,
	0x4602,
	(s32)0xffff000a,
	(s32)Func_0200208c,
	0x4602,
	(s32)0xffff000b,
	(s32)Func_0200367c,
	0x4602,
	0xa2c001e,
	(s32)Func_020000d8,
	2,
	0xaa20014,
	(s32)Func_02001db0,
	2,
	(s32)0xffff0007,
	(s32)Func_020024f0,
	2,
	(s32)0xffff0008,
	(s32)Func_0200253c,
	0,
	(s32)0xffff0008,
	MSGNO(MsgMyMomSays),
	0,
	(s32)0xffff0009,
	(s32)Func_02000210,
	0,
	(s32)0xffff0013,
	(s32)Func_0200030c,
	6,
	(s32)0xffff00c8,
	(s32)Func_02003620,
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgWhoInWeyard),
	0x8d15,
	(s32)0xffff0009,
	(s32)Func_020002ac,
	0x8d15,
	(s32)0xffff0013,
	(s32)Func_02000380,
	0,
	(s32)0xffff0015,
	(s32)Func_0200040c,
	0x8d15,
	(s32)0xffff0015,
	MSGNO(MsgMayTheseTravelers),
	0x50009085,
	-0x10000,
	(s32)Func_020025a4,
	0x40009085,
	-0x10000,
	(s32)Func_020025b0,
	-1,
	0,
	0,
};

s32 Data_02004ca0[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	1,
	(s32)0xffff0003,
	3,
	1,
	(s32)0xffff0004,
	4,
	0,
	(s32)0xffff0008,
	MSGNO(MsgIJustWant),
	0,
	(s32)0xffff0009,
	MSGNO(MsgTheSeaOf),
	0,
	(s32)0xffff000a,
	MSGNO(MsgMooMoo2),
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgDadAndGrandpa),
	0x8d15,
	(s32)0xffff0009,
	MSGNO(MsgIRememberOnce),
	0x8d15,
	(s32)0xffff000a,
	(s32)Func_020006f4,
	0x8b85,
	-0x10000,
	(s32)Func_020020b4,
	0x40008b85,
	-0x10000,
	(s32)Func_020020d0,
	0x50008905,
	(s32)0xffff000a,
	(s32)Func_020025bc,
	0x50008905,
	(s32)0xffff000b,
	(s32)Func_020025dc,
	0x50008905,
	(s32)0xffff000c,
	(s32)Func_020025fc,
	0x50008905,
	(s32)0xffff000d,
	(s32)Func_0200261c,
	0x50008905,
	(s32)0xffff000e,
	(s32)Func_0200263c,
	0x50008905,
	(s32)0xffff000f,
	(s32)Func_0200265c,
	-1,
	0,
	0,
};

s32 Data_02004d84[] = {
	1,
	(s32)0xffff000a,
	10,
	0xce01,
	(s32)0xffff000b,
	11,
	0x4602,
	0x2120015,
	(s32)Func_02002430,
	8,
	0x2120000,
	(s32)Func_02002144,
	0x10008c15,
	0x212000c,
	(s32)Func_02002144,
	0x10008c15,
	0x212000d,
	(s32)Func_02002144,
	9,
	0x2120000,
	(s32)Func_020021d0,
	0x8c15,
	0x212000c,
	(s32)Func_020021d0,
	0x8c15,
	0x212000d,
	(s32)Func_020021d0,
	0x1815,
	0x213000e,
	(s32)Func_020024c8,
	0,
	(s32)0xffff0008,
	(s32)Func_02000440,
	0,
	(s32)0xffff0009,
	(s32)Func_020004f4,
	0,
	(s32)0xffff000a,
	(s32)Func_02000520,
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgIfWeRe),
	0x8d15,
	(s32)0xffff0009,
	MSGNO(MsgSureThereAre),
	0x8d15,
	(s32)0xffff000a,
	(s32)Func_02000568,
	2,
	0x2150014,
	(s32)Func_02001f74,
	-1,
	0,
	0,
};

s32 Data_02004e5c[] = {
	2,
	(s32)0xffff0001,
	(s32)Func_0200274c,
	2,
	(s32)0xffff0002,
	(s32)Func_0200274c,
	49,
	(s32)0xffff0005,
	5,
	0,
	(s32)0xffff0008,
	MSGNO(MsgYouUsedTo),
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgIRememberHow),
	0,
	0xaa50009,
	MSGNO(MsgBlank3),
	0,
	(s32)0xffff0009,
	MSGNO(MsgYup2),
	0x8d15,
	0x2170009,
	(s32)Func_020014e0,
	0x8d15,
	(s32)0xffff0009,
	(s32)Func_02001a68,
	0,
	(s32)0xffff000b,
	(s32)Func_02001c38,
	0,
	(s32)0xffff000c,
	(s32)Func_02001c38,
	0x8b85,
	-0x10000,
	(s32)Func_020020b4,
	0x40008b85,
	-0x10000,
	(s32)Func_020020d0,
	-1,
	0,
	0,
};
