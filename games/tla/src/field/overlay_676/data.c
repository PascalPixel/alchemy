#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000044(void);
void Func_02000270(void);
void Func_02000328(void);
void Func_0200036c(void);
void Func_0200038c(void);
void Func_0200041c(void);
void Func_0200043c(void);
void Func_020004cc(void);
void Func_02000544(void);
void Func_02000584(void);
void Func_02000614(void);
void Func_020022f8(void);

void Func_020022f4(void);

extern s32 Data_020025a8[];
extern s32 Data_020025d4[];
extern s32 Data_020025f8[];
extern u32 Data_02002634[];
extern u32 Data_020026d8[];
extern s16 Data_02002c8c[];
extern s16 Data_02002ca2[];
extern s16 Data_02002cb8[];
extern s16 Data_02002cce[];

s32 Data_020025a8[] = {
	39,
	3,
	22,
	26,
	0,
	23,
	6,
	0x20000,
	46,
	(s32)Func_02000044,
	17,
};

s32 Data_020025d4[] = {
	22,
	26,
	0,
	23,
	6,
	-0x100000,
	46,
	(s32)Func_02000044,
	17,
};

s32 Data_020025f8[] = {
	22,
	9,
	0x9999,
	22,
	10,
	0x9999,
	22,
	26,
	0,
	23,
	6,
	-0x100000,
	46,
	(s32)Func_02000044,
	17,
};

u32 Data_02002634[] = {
	39, 2, 22, 15, 0x10000, 22,
	16, 0x6666, 4, 0x1480000, 0, 0x2f00000,
	1, 4, 0x1680000, 0, 0x2f00000, 1,
	4, 0x1780000, 0, 0x3080000, 1, 4,
	0x1680000, 0, 0x3180000, 1, 4, 0x1480000,
	0, 0x3180000, 1, 4, 0x1380000, 0,
	0x3080000, 1, 13, 0xffff, 0,
};

u32 Data_020026d8[] = {
	39, 2, 22, 15, 0x10000, 22,
	16, 0x6666, 4, 0x1680000, 0, 0x3180000,
	1, 4, 0x1480000, 0, 0x3180000, 1,
	4, 0x1380000, 0, 0x3080000, 1, 4,
	0x1480000, 0, 0x2f00000, 1, 4, 0x1680000,
	0, 0x2f00000, 1, 4, 0x1780000, 0,
	0x3080000, 1, 13, 0xffff, 0,
};

u32 Data_0200277c[] = {
	39, 2, 4, 0x1500000, 0, 0x3180000,
	1, 39, 1, 0, 1, 22,
	3, 0x5000, 0, 10, 17,
};

u32 Data_020027c0[] = {
	39, 2, 4, 0x1600000, 0, 0x3180000,
	1, 39, 1, 0, 1, 22,
	3, 0x5000, 0, 10, 17,
};

u32 Data_02002804[] = {
	39, 2, 4, 0x13c0000, 0, 0x3780000,
	1, 4, 0x1500000, 0, 0x3780000, 1,
	4, 0x1580000, 0, 0x3680000, 1, 39,
	1, 0, 1, 22, 3, 0x8000,
	0, 10, 17,
};

u32 Data_02002870[] = {
	39, 2, 4, 0x16a0000, 0, 0x3180000,
	1, 4, 0x1800000, 0, 0x3500000, 1,
	4, 0x17e0000, 0, 0x3780000, 1, 39,
	1, 0, 1, 22, 3, 0x8000,
	0, 10, 17,
};

u32 Data_020028dc[] = {
	39, 2, 4, 0x16c0000, 0, 0x3280000,
	1, 4, 0x1760000, 0, 0x36e0000, 1,
	4, 0x1680000, 0, 0x3780000, 1, 39,
	1, 0, 1, 22, 3, 0x8000,
	0, 10, 17,
};

u32 Data_02002948[] = {
	39, 2, 4, 0x1120000, 0, 0x3780000,
	1, 4, 0x1480000, 0, 0x3780000, 1,
	39, 1, 22, 3, 0x8000, 0,
	10, 17,
};

u32 Data_02002998[] = {
	39, 2, 4, 0x1580000, 0, 0x3480000,
	1, 39, 1, 17,
};

u32 Data_020029c0[] = {
	39, 2, 4, 0x1580000, 0, 0x3780000,
	1, 4, 0x1580000, 0, 0x3580000, 1,
	39, 1, 0, 1, 17,
};

u32 Data_02002a04[] = {
	39, 2, 4, 0x1580000, 0, 0x3780000,
	1, 4, 0x1580000, 0, 0x36a0000, 1,
	39, 1, 0, 1, 17,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(139),
	SCENE_EXIT(1, 141, 1),
	SCENE_EXIT(2, 141, 2),
	SCENE_EXIT(3, 142, 1),
	SCENE_EXIT(4, 141, 4),
	SCENE_EXIT(5, 142, 3),
	SCENE_EXIT(6, 142, 2),
	SCENE_EXIT(7, 141, 3),
	SCENE_EXIT(8, 58, 4),
	SCENE_EXIT(9, 140, 6),
	SCENE_EXIT(10, 2, 34),
	SCENE_EXIT(11, 2, 33),
	SCENE_EXIT(12, 139, 15),
	SCENE_EXIT(13, 140, 7),
	SCENE_EXIT(15, 139, 12),
	SCENE_EXITS(140),
	SCENE_EXIT(6, 139, 9),
	SCENE_EXIT(7, 139, 13),
	SCENE_EXITS_END,
};

s32 Data_02002ac4[] = {
	(s32)0xffff0146,
	(s32)Data_020025d4,
	0xb80000,
	0,
	0xc80000,
	0x8000,
	(s32)0xffff0146,
	(s32)Data_020025a8,
	0xb80000,
	0,
	0xc80000,
	0x8000,
	(s32)0xffff0147,
	(s32)Data_020025f8,
	0x80000,
	0,
	0x80000,
	0x8000,
	(s32)0xffff0073,
	1,
	0x2680000,
	0,
	0xd80000,
	0x32000,
	(s32)0xffff0074,
	1,
	0x2c80000,
	0,
	0xe80000,
	0x14000,
	(s32)0xffff0072,
	3,
	0x2280000,
	0,
	0xf80000,
	0x16000,
	(s32)0xffff0080,
	2,
	0x1680000,
	0,
	0x1680000,
	0x1a000,
	(s32)0xffff0070,
	1,
	0x1c80000,
	0,
	0x1180000,
	0x12000,
	(s32)0xffff0071,
	1,
	0xd80000,
	0,
	0x1780000,
	0x1e000,
	(s32)0xffff00e2,
	1,
	0x1d00000,
	0,
	0x880000,
	0x16000,
	(s32)0xffff00e2,
	1,
	0x1880000,
	0,
	0x1080000,
	0x14000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u32 Data_02002be4[] = {
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_02002bfc[] = {
	(s32)0xffff0088,
	1,
	0x1380000,
	0,
	0x3680000,
	0x18000,
	(s32)0xffff0088,
	(s32)Data_02002634,
	0x1380000,
	0,
	0x3080000,
	0x2c000,
	(s32)0xffff008a,
	(s32)Data_020026d8,
	0x1780000,
	0,
	0x3080000,
	0x24000,
	(s32)0xffff008a,
	1,
	0x1100000,
	0,
	0x3200000,
	0xb000,
	(s32)0xffff0005,
	1,
	0,
	0,
	0,
	0x24000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

s16 Data_02002c8c[] = {
	93, 28, 1, 2, 6, 92, 28, 1,
	2, 6, -1,
};

s16 Data_02002ca2[] = {
	95, 28, 1, 2, 6, 94, 28, 1,
	2, 6, -1,
};

s16 Data_02002cb8[] = {
	89, 28, 1, 2, 6, 90, 28, 1,
	2, 6, -1,
};

s16 Data_02002cce[] = {
	92, 30, 2, 2, 6, 94, 30, 2,
	2, 6, -1,
};

s32 Data_02002ce4[] = {
	(s32)Data_02002c8c,
	0x130062,
	(s32)Data_02002ca2,
	0x14005c,
	(s32)Data_02002cb8,
	0x120054,
	(s32)Data_02002c8c,
	0x7004f,
	(s32)Data_02002cb8,
	0x100050,
	(s32)Data_02002cb8,
	0x11004d,
	(s32)Data_02002ca2,
	0x80063,
	(s32)Data_02002cce,
	0x6005a,
};

s32 Data_02002d24[] = {
	0xc602,
	(s32)0xffff0001,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0002,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0003,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0004,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0005,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0006,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0007,
	(s32)Func_02000270,
	0xc602,
	(s32)0xffff0008,
	(s32)Func_02000270,
	0xc401,
	(s32)0xffff0009,
	9,
	1,
	(s32)0xffff000a,
	10,
	1,
	(s32)0xffff000b,
	11,
	1,
	(s32)0xffff000c,
	12,
	0x4401,
	(s32)0xffff000d,
	13,
	0x50008905,
	(s32)0xffff0014,
	(s32)Func_02000328,
	0x50008905,
	(s32)0xffff0015,
	(s32)Func_0200036c,
	0x50008905,
	(s32)0xffff0016,
	(s32)Func_0200038c,
	0x50008905,
	(s32)0xffff0017,
	(s32)Func_0200041c,
	0x50008905,
	(s32)0xffff0018,
	(s32)Func_0200043c,
	0x50008905,
	(s32)0xffff0019,
	(s32)Func_020004cc,
	0,
	(s32)0xffff000b,
	MSGNO(MsgYallamDoesnT),
	0,
	(s32)0xffff000c,
	MSGNO(MsgOurMasterBoatsman),
	0,
	(s32)0xffff000d,
	MSGNO(MsgIfYeppWere),
	0,
	(s32)0xffff000e,
	MSGNO(MsgYeppFoundThis),
	0,
	(s32)0xffff000f,
	MSGNO(MsgThereUsedTo),
	0,
	(s32)0xffff0010,
	MSGNO(MsgWhenWeLost),
	0,
	(s32)0xffff0011,
	MSGNO(MsgYeppWasThe),
	0,
	(s32)0xffff0012,
	MSGNO(MsgYeppToldMe),
	0x8d15,
	(s32)0xffff000b,
	MSGNO(MsgYeppWasTheOne),
	0x8d15,
	(s32)0xffff000c,
	MSGNO(MsgSingingWithYepp),
	0x8d15,
	(s32)0xffff000d,
	MSGNO(MsgThosePearlsYepp),
	0x8d15,
	(s32)0xffff000e,
	MSGNO(MsgYeppNeverTold),
	0x8d15,
	(s32)0xffff000f,
	MSGNO(MsgYeppWasAmazing),
	0x8d15,
	(s32)0xffff0010,
	MSGNO(MsgSunshineIsA),
	0x8d15,
	(s32)0xffff0011,
	MSGNO(MsgIVeSearched),
	0x8d15,
	(s32)0xffff0012,
	MSGNO(MsgYeppAlwaysSang),
	0x10008e15,
	0x3000008,
	(s32)(Func_020022f4),
	0x8e15,
	0x3000008,
	(s32)Func_020022f8,
	-1,
	0,
	0,
};

s32 Data_02002eec[] = {
	1, (s32)0xffff0006, 6, 0xc401, (s32)0xffff0007, 7,
	-1, 0, 0,
};

s32 Data_02002f10[] = {
	1,
	(s32)0xffff000f,
	15,
	0x400,
	(s32)0xffff0008,
	(s32)Func_02000544,
	0x2400,
	(s32)0xffff0008,
	(s32)Func_02000544,
	0xe400,
	(s32)0xffff0008,
	(s32)Func_02000544,
	0xc400,
	(s32)0xffff0008,
	(s32)Func_02000544,
	0x4400,
	(s32)0xffff0008,
	(s32)Func_02000544,
	0x8d15,
	(s32)0xffff0008,
	MSGNO(MsgYeppSSongs),
	0,
	(s32)0xffff0009,
	(s32)Func_02000584,
	0x8d15,
	(s32)0xffff0009,
	(s32)Func_02000584,
	0,
	(s32)0xffff000a,
	(s32)Func_02000614,
	0x8d15,
	(s32)0xffff000a,
	(s32)Func_02000614,
	0,
	(s32)0xffff000b,
	MSGNO(MsgYeppWroteAll),
	0x8d15,
	(s32)0xffff000b,
	MSGNO(MsgTheStoryIs),
	-1,
	0,
	0,
};
