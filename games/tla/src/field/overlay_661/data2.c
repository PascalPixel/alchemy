#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000280(void);
void Func_020003b8(void);
void Func_020004bc(void);
void Func_02000550(void);
void Func_020005a8(void);
void Func_02000600(void);
void Func_02000620(void);
void Func_02000634(void);
void Func_02000648(void);
void Func_02000670(void);
void Func_02000684(void);
void Func_020006ac(void);
void Func_020007e0(void);
void Func_02000994(void);
void Func_02000a10(void);
void Func_02000a8c(void);
void Func_02000c34(void);
void Func_02000d50(void);
void Func_02000e78(void);
void Func_02000fc8(void);
void Func_02001288(void);
void Func_020012dc(void);
void Func_020013f8(void);
void Func_02001518(void);
void Func_0200157c(void);
void Func_020015d4(void);
void Func_02001628(void);
void Func_0200163c(void);
void Func_02001690(void);
void Func_02002e14(void);
void Func_02002e98(void);

extern u8 Data_02004200[];
extern u8 Data_0200423c[];
extern u8 Data_02004278[];

extern u32 Data_020045d4[];
extern u32 Data_02004630[];

s32 Data_020045c8[] = {
	(s32)Data_02004200,
	(s32)Data_0200423c,
	(s32)Data_02004278,
};

u32 Data_020045d4[] = {
	22, 3, 0xb000, 0, 80, 22,
	3, 0, 0, 80, 22, 3,
	0xd000, 0, 20, 22, 3, 0,
	0, 40, 13, 0xffff, 0,
};

u32 Data_02004630[] = {
	22, 3, 0xd000, 0, 20, 22,
	3, 0, 0, 40, 22, 3,
	0xb000, 0, 80, 22, 3, 0,
	0, 80, 13, 0xffff, 0,
};

u32 Data_0200468c[] = {
	39, 2, 4, 0xf80000, 0, 0xf80000,
	1, 4, 0xf80000, 0, 0x1180000, 1,
	4, 0x1080000, 0, 0x1900000, 1, 39,
	1, 17,
};

u32 Data_020046dc[] = {
	39, 2, 4, 0xf00000, 0, 0x1180000,
	1, 4, 0x1080000, 0, 0x1900000, 1,
	39, 1, 17,
};

u32 Data_02004718[] = {
	39, 2, 4, 0xd80000, 0, 0xe80000,
	1, 4, 0xf00000, 0, 0x1180000, 1,
	4, 0x1080000, 0, 0x1900000, 1, 39,
	1, 17,
};

s16 Data_02004768[] = {
	8, 0, 512, 9, 0, 513, 10, 0,
	514, 11, 0, 515, 12, 0, 516, 13,
	0, 517, -1,
};

s16 Data_0200478e[] = {
	8, 0, 512, 9, 0, 513, 10, 0,
	514, 11, 0, 515, 12, 0, 516, 13,
	0, 517, 14, 0, 518, 15, 0, 519,
	16, 0, 520, 17, 0, 521, 18, 0,
	522, -1,
};

s16 Data_020047d2[] = {
	8, 0, 512, 9, 0, 513, 10, 0,
	514, 11, 0, 515, -1,
};

s16 Data_020047ec[] = {
	8, 0, 512, 9, 0, 513, 10, 0,
	514, -1,
};

s32 Data_02004800[] = {
	8, -0xfe00,
};

u32 Data_02004808[] = {
	0,
};

u32 Data_0200480c[] = {
	0,
};

s32 Data_02004810[] = {
	-0x10000, 172, 0x40000095, -0x10000, -1, 0xffff,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_02004840[] = {
	0xffff0001, 408, 0xc00001e0, 0x1200000, 0x2100178, 536,
	0xffff0002, 408, 0x400001c8, 0x1200000, 0x2100178, 536,
	0xffff0003, 120, 0x400001a8, 0, 0x1c00010, 496,
	0xffff0004, 376, 0x40000058, 0, 0x1c00010, 496,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_020048b8[] = {
	0x2c00d8, 0xf800b8, 0xd8004c, 0x1ffff, 0xffff, 0,
	0, 0,
};

s16 Data_020048d8[] = {
	112, 24, 416, 128, 40, 432, -1, 3,
	368, 24, 80, 384, 40, 96, -1, 4,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
	SCENE_EXITS(67),
	SCENE_EXIT(1, 2, 17),
	SCENE_EXIT(2, 68, 1),
	SCENE_EXITS(68),
	SCENE_EXIT(1, 67, 2),
	SCENE_EXIT(2, 72, 1),
	SCENE_EXIT(3, 77, 1),
	SCENE_EXIT(4, 69, 1),
	SCENE_EXIT(5, 77, 3),
	SCENE_EXITS(69),
	SCENE_EXIT(1, 68, 4),
	SCENE_EXIT(2, 70, 1),
	SCENE_EXITS(70),
	SCENE_EXIT(1, 69, 2),
	SCENE_EXIT(2, 70, 3),
	SCENE_EXIT(3, 70, 2),
	SCENE_EXIT(4, 71, 1),
	SCENE_EXITS(71),
	SCENE_EXIT(1, 70, 4),
	SCENE_EXIT(2, 71, 3),
	SCENE_EXIT(3, 71, 2),
	SCENE_EXIT(4, 76, 1),
	SCENE_EXITS(72),
	SCENE_EXIT(1, 68, 2),
	SCENE_EXIT(2, 78, 1),
	SCENE_EXITS(73),
	SCENE_EXIT(1, 78, 2),
	SCENE_EXIT(2, 74, 1),
	SCENE_EXITS(74),
	SCENE_EXIT(1, 73, 2),
	SCENE_EXIT(2, 2, 18),
	SCENE_EXIT(3, 75, 1),
	SCENE_EXIT(4, 79, 1),
	SCENE_EXITS(75),
	SCENE_EXIT(1, 74, 3),
	SCENE_EXIT(2, 2, 19),
	SCENE_EXITS(76),
	SCENE_EXIT(1, 71, 4),
	SCENE_EXITS(77),
	SCENE_EXIT(1, 68, 3),
	SCENE_EXIT(2, 77, 4),
	SCENE_EXIT(3, 68, 5),
	SCENE_EXIT(4, 77, 2),
	SCENE_EXITS(78),
	SCENE_EXIT(1, 72, 2),
	SCENE_EXIT(2, 73, 1),
	SCENE_EXITS_END,
};

u32 Data_020049c4[] = {
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_020049dc[] = {
	0xffff0196, 1, 0x1280000, 0, 0xd80000, 0x24000,
	0xffff0196, 1, 0x1a80000, 0, 0xa80000, 0x24000,
	0xffff0196, 1, 0x1c80000, 0, 0x880000, 0x24000,
	0xffff0196, 1, 0x2680000, 0, 0xa80000, 0x24000,
	0xffff0196, 1, 0x2780000, 0, 0x1880000, 0x24000,
	0xffff0196, 1, 0x2a80000, 0, 0x1a80000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_02004a84[] = {
	(s32)0xffff0058,
	(s32)Data_020045d4,
	0xc80000,
	0,
	0xd80000,
	0xd000,
	(s32)0xffff0058,
	(s32)Data_02004630,
	0xd80000,
	0,
	0xc80000,
	0xd000,
	(s32)0xffff0072,
	1,
	0x980000,
	0,
	0xc80000,
	0x23000,
	(s32)0xffff005d,
	1,
	0xa20000,
	0,
	0xe80000,
	0x20000,
	(s32)0xffff0070,
	1,
	0xd40000,
	0,
	0xf60000,
	0x20000,
	(s32)0xffff0013,
	1,
	0xe60000,
	0,
	0xf80000,
	0x20000,
	(s32)0xffff0014,
	1,
	0xea0000,
	0,
	0xe60000,
	0x20000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u32 Data_02004b44[] = {
	0xffff0058, 1, 0x2080000, 0, 0xf20000, 0x1b000,
	0xffff0058, 1, 0x2150000, 0, 0x10f0000, 0x8000,
	0xffff0072, 1, 0x1b30000, 0, 0x1430000, 0x13000,
	0xffff005d, 1, 0x1d30000, 0, 0xf80000, 0x25000,
	0xffff0070, 1, 0x1ce0000, 0, 0x1540000, 0x1d000,
	0xffff0013, 1, 0x1e20000, 0, 0x1540000, 0x1d000,
	0xffff0014, 1, 0x1e90000, 0, 0x1400000, 0x1b000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_02004c04[] = {
	0xffff0196, 1, 0xd80000, 0, 0xb80000, 0x24000,
	0xffff0196, 1, 0x1780000, 0, 0xd80000, 0x24000,
	0xffff0196, 1, 0xd80000, 0, 0x1580000, 0x24000,
	0xffff0196, 1, 0x980000, 0, 0x1780000, 0x24000,
	0xffff0196, 1, 0xa80000, 0, 0x2080000, 0x24000,
	0xffff0196, 1, 0x1280000, 0, 0x1c80000, 0x24000,
	0xffff0196, 1, 0x1480000, 0, 0x1c80000, 0x24000,
	0xffff0196, 1, 0x1580000, 0, 0x2280000, 0x24000,
	0xffff0196, 1, 0x1280000, 0, 0x2680000, 0x24000,
	0xffff0196, 1, 0x1f80000, 0, 0x1780000, 0x24000,
	0xffff0196, 1, 0x2180000, 0, 0x2d80000, 0x24000,
	0x7400f6, 1, 0x1f80000, 0, 0x2380000, 0x4000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_02004d3c[] = {
	0xffff0196, 1, 0x880000, 0, 0x2380000, 0x24000,
	0xffff0196, 1, 0xe80000, 0, 0x2580000, 0x24000,
	0xffff0196, 1, 0x1280000, 0, 0x2380000, 0x24000,
	0xffff0196, 1, 0xb80000, 0, 0xe80000, 0x24000,
	0xffff0196, 1, 0xb80000, 0, 0xb80000, 0x24000,
	0xffff0196, 1, 0x1280000, 0, 0xc80000, 0x1024000,
	0xffff0196, 1, 0xa80000, 0, 0x1080000, 0x1024000,
	0xffff0196, 1, 0x1180000, 0, 0x1180000, 0x1024000,
	0xffff0067, 1, 0xe80000, 0, 0xc80000, 0x24000,
	0xffff00fb, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_02004e44[] = {
	0xffff0196, 1, 0xe80000, 0, 0x2d80000, 0x24000,
	0xffff0196, 1, 0xc80000, 0, 0x1280000, 0x24000,
	0xffff0196, 1, 0x1180000, 0, 0x1580000, 0x24000,
	0xffff0196, 1, 0xe80000, 0, 0x2880000, 0x24000,
	0xffff0196, 1, 0xb80000, 0, 0x2b80000, 0x1024000,
	0xffff0196, 1, 0xa80000, 0, 0x2d80000, 0x1024000,
	0xffff0196, 1, 0xd80000, 0, 0x2f80000, 0x1024000,
	0xffff0196, 1, 0x1480000, 0, 0x2980000, 0x1024000,
	0xffff0196, 1, 0x1280000, 0, 0x2c80000, 0x1024000,
	0xffff0196, 1, 0x1380000, 0, 0x2e80000, 0x1024000,
	0xffff0196, 1, 0x1280000, 0, 0x3080000, 0x1024000,
	0xffff0196, 1, 0x1180000, 0, 0x3080000, 0x1024000,
	0xffff0196, 1, 0, 0, 0, 0x1024000,
	0xffff0067, 1, 0, 0, 0, 0x24000,
	0xffff00fb, 1, 0, 0, 0, 0x24000,
	0xffff0162, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_02004fdc[] = {
	0xffff0197, 1, 0x1980000, 0, 0x1680000, 0x24000,
	0xffff0197, 1, 0x1680000, 0, 0x1680000, 0x1024000,
	0xffff01c2, 1, 0x3000000, 0, 0x2680000, 0x24000,
	0xffff0151, 1, 0x3380000, 0, 0x1b80000, 0x24000,
	0xffff0151, 1, 0x3380000, 0, 0x1a80000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_0200506c[] = {
	0xffff0067, 1, 0, 0, 0, 0x24000,
	0xffff01e9, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_020050b4[] = {
	0xffff0196, 1, 0x2080000, 0, 0x980000, 0x24000,
	0xffff0151, 1, 0x1e80000, 0, 0xb80000, 0x24000,
	0xffff0151, 1, 0x1f80000, 0, 0xb80000, 0x1024000,
	0xffff0151, 1, 0x2080000, 0, 0xb80000, 0x1024000,
	0xffff0151, 1, 0x2080000, 0, 0xc80000, 0x1024000,
	0xffff0151, 1, 0x2180000, 0, 0xd80000, 0x1024000,
	0xffff0151, 1, 0x2480000, 0, 0x1080000, 0x1024000,
	0xffff0151, 1, 0x3080000, 0, 0x1080000, 0x1024000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_0200518c[] = {
	-1, 0, 0,
};

s32 Data_02005198[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x50008615,
	0x2000008,
	(s32)Func_02000620,
	0x50008615,
	0x2010009,
	(s32)Func_02000620,
	0x50008615,
	0x202000a,
	(s32)Func_02000620,
	0x50008615,
	0x203000b,
	(s32)Func_02000620,
	0x50008615,
	0x204000c,
	(s32)Func_02000620,
	0x50008615,
	0x205000d,
	(s32)Func_02000620,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	-1,
	0,
	0,
};

s32 Data_0200521c[] = {
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
	514,
	(s32)0xffff0005,
	(s32)Func_02002e14,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	0x50008805,
	0x88d000a,
	(s32)Func_020015d4,
	0,
	0x8f00008,
	(s32)Func_020004bc,
	0,
	(s32)0xffff0008,
	MSGNO(MsgAtLastWe),
	0x8d15,
	0x8f00008,
	MSGNO(MsgBriggsAndThe),
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgOhNotAgain),
	0,
	0x8f00009,
	MSGNO(MsgIfThatAccursed),
	0,
	(s32)0xffff0009,
	MSGNO(MsgIWasCertain),
	0x8d15,
	0x8f00009,
	MSGNO(MsgThatIsSo),
	0x8d15,
	(s32)0xffff0009,
	MSGNO(MsgMyJobIs),
	0,
	0x8f0000a,
	MSGNO(MsgOooohThisDesert),
	0,
	(s32)0xffff000a,
	MSGNO(MsgIMJust),
	0x8d15,
	0x8f0000a,
	MSGNO(MsgSureItS),
	0x8d15,
	(s32)0xffff000a,
	MSGNO(MsgMaybeWeShould),
	0,
	0x8f0000b,
	MSGNO(MsgWaterMyThroat),
	0,
	(s32)0xffff000b,
	MSGNO(MsgOhManThis),
	0x8d15,
	0x8f0000b,
	MSGNO(MsgThereSA),
	0x8d15,
	(s32)0xffff000b,
	MSGNO(MsgIDonTWant),
	0,
	0x8f0000c,
	(s32)Func_02000550,
	0,
	(s32)0xffff000c,
	MSGNO(MsgOseniaIsSo),
	0x8d15,
	0x88f040c,
	(s32)Func_02000550,
	0x8d15,
	0x8f0000c,
	MSGNO(MsgWeVeReached),
	0x8d15,
	(s32)0xffff000c,
	MSGNO(MsgHowMuchLonger),
	0,
	0x8f0000d,
	(s32)Func_020005a8,
	0,
	(s32)0xffff000d,
	MSGNO(MsgMySonHas),
	0x8d15,
	0x88f040d,
	(s32)Func_020005a8,
	0x8d15,
	0x8f0000d,
	MSGNO(MsgMySonIs),
	0x8d15,
	(s32)0xffff000d,
	MSGNO(MsgRegardlessOfWhat),
	0,
	0x8f0000e,
	MSGNO(MsgFatherHowAm),
	0,
	(s32)0xffff000e,
	MSGNO(MsgIfWeRemain),
	0x8d15,
	0x8f0000e,
	MSGNO(MsgMyFatherS),
	0x8d15,
	(s32)0xffff000e,
	MSGNO(MsgHoooICan),
	2,
	0x8f0000f,
	(s32)Func_02001690,
	-1,
	0,
	0,
};

s32 Data_020053fc[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x50008615,
	0x2000008,
	(s32)Func_02000634,
	0x50008615,
	0x2010009,
	(s32)Func_02000634,
	0x50008615,
	0x202000a,
	(s32)Func_02000634,
	0x50008615,
	0x203000b,
	(s32)Func_02000634,
	0x50008615,
	0x204000c,
	(s32)Func_02000634,
	0x50008615,
	0x205000d,
	(s32)Func_02000634,
	0x50008615,
	0x206000e,
	(s32)Func_02000634,
	0x50008615,
	0x207000f,
	(s32)Func_02000634,
	0x50008615,
	0x2080010,
	(s32)Func_02000634,
	0x50008615,
	0x2090011,
	(s32)Func_02000634,
	0x50008615,
	0x20a0012,
	(s32)Func_02000634,
	2,
	0x20b000b,
	(s32)Func_020006ac,
	2,
	0x120b000a,
	(s32)Func_020007e0,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	0,
	(s32)0xffff0013,
	(s32)Func_02000280,
	-1,
	0,
	0,
};

s32 Data_020054e0[] = {
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
	0x50008615,
	0x2000008,
	(s32)Func_02000648,
	0x50008615,
	0x2010009,
	(s32)Func_02000648,
	0x50008615,
	0x202000a,
	(s32)Func_02000648,
	0x50008615,
	0x203000b,
	(s32)Func_02000648,
	2,
	0x20b000b,
	(s32)Func_020006ac,
	2,
	0x120b000a,
	(s32)Func_020007e0,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	6,
	(s32)0xffff00d2,
	(s32)Func_02000a8c,
	6,
	(s32)0xffff00c8,
	(s32)Func_02000c34,
	-1,
	0,
	0,
};

s32 Data_02005594[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	1,
	(s32)0xffff0003,
	3,
	0x50008615,
	0x2000008,
	(s32)Func_02000684,
	0x50008615,
	0x2010009,
	(s32)Func_02000684,
	0x50008615,
	0x202000a,
	(s32)Func_02000684,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	6,
	0x88b00d2,
	(s32)Func_02000a8c,
	6,
	(s32)0xffff00c9,
	(s32)Func_02000d50,
	6,
	(s32)0xffff00ca,
	(s32)Func_02000e78,
	2,
	0x88c000a,
	(s32)Func_02000fc8,
	-1,
	0,
	0,
};

s32 Data_02005630[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	0x50008805,
	0x302000c,
	(s32)Func_020003b8,
	-1,
	0,
	0,
};

s32 Data_02005678[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	-1,
	0,
	0,
};

s32 Data_020056b4[] = {
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
	0x8515,
	0x20c0008,
	0,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	0x50009085,
	-0x10000,
	(s32)Func_020012dc,
	0x40009085,
	-0x10000,
	(s32)Func_020013f8,
	0,
	(s32)0xffff000a,
	(s32)Func_02000600,
	2,
	0x120d0007,
	(s32)Func_0200163c,
	2,
	0x20d0008,
	(s32)Func_02001628,
	-1,
	0,
	0,
};

s32 Data_02005750[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x50009085,
	-0x10000,
	(s32)Func_02001518,
	0x40009085,
	-0x10000,
	(s32)Func_0200157c,
	-1,
	0,
	0,
};

s32 Data_0200578c[] = {
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	0x50008805,
	0x8ed000a,
	(s32)Func_02001288,
	-1,
	0,
	0,
};

s32 Data_020057bc[] = {
	1,
	(s32)0xffff0001,
	1,
	0xc602,
	(s32)0xffff0002,
	(s32)Func_02002e98,
	-1,
	0,
	0,
};

s32 Data_020057e0[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0x50008615,
	0x2000008,
	(s32)Func_02000670,
	0x8b85,
	-0x10000,
	(s32)Func_02000994,
	0x40008b85,
	-0x10000,
	(s32)Func_02000a10,
	-1,
	0,
	0,
};

u32 Data_02005828[] = {
	0, 1, 4, 0xe80000, 0, 0xba0000,
	1, 38,
};
