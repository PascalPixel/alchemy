#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000084(void);
void Func_020000a0(void);
void Func_020000ec(void);
void Func_020000fc(void);
void Func_02000168(void);
void Func_020001d4(void);
void Func_02000818(void);

void Func_02000098(void);
void Func_0200009c(void);

s16 Data_02001608[] = {
	16, 515, -1, 0,
};

s32 Data_02001610[] = {
	39, 2, 5, 0x100000, 0, 0,
	1, 5, 0, 0, -0x3c0000, 1,
	5, -0x40000, 0, 0, 1, 39,
	1, 22, 3, 0x8000, 0, 1,
	17,
};

u32 Data_02001674[] = {
	39, 2, 4, 0x30c0000, 0, 0x1200000,
	1, 5, 0, 0, 0xc0000, 1,
	5, 0x400000, 0, 0, 1, 5,
	0x200000, 0, 0x200000, 1, 5, 0,
	0, 0x360000, 1, 17,
};

s16 Data_020016e4[] = {
	9, 1, 513, -1,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(100),
	SCENE_EXIT_IF(1, 95, 12, 0xffffffff),
	SCENE_EXIT_IF(2, 100, 3, 0xffffffff),
	SCENE_EXIT_IF(3, 100, 2, 0xffffffff),
	SCENE_EXIT_IF(4, 101, 6, 0xffffffff),
	SCENE_EXIT_IF(5, 101, 7, 0xffffffff),
	SCENE_EXITS(101),
	SCENE_EXIT_IF(6, 100, 4, 0xffffffff),
	SCENE_EXIT_IF(7, 100, 5, 0xffffffff),
	SCENE_EXIT_IF(8, 95, 8, 0xffffffff),
	SCENE_EXIT_IF(9, 101, 10, 0xffffffff),
	SCENE_EXIT_IF(10, 101, 9, 0xffffffff),
	SCENE_EXITS_END,
};

s16 Data_02001778[] = {
	369, -1, 7, 0, 0, 120, 0, 0,
	0, 296, 0, 2, 369, -1, 7, 0,
	0, 472, 0, 0, 0, 200, 0, 2,
	369, -1, 7, 0, 0, 424, 0, 0,
	0, 312, 0, 2, 371, -1, 7, 0,
	0, 296, 0, 0, 0, 392, 0, 2,
	371, -1, 7, 0, 0, 344, 0, 0,
	0, 392, 0x8000, 2, 369, -1, 7, 0,
	0, 324, 0, 0, 0, 416, 0x4000, 2,
	272, -1, 1, 0, 0, 216, 0, 0,
	0, 136, 0x4000, 2, 272, -1, 1, 0,
	0, 408, 0, 0, 0, 360, 0x4000, 2,
	290, -1, 1, 0, 0, 312, 0, 0,
	0, 392, 0x4000, 2, -1, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
};

u32 Data_02001868[] = {
	0xffff011f, 1, 0x2d80000, 0, 0xb80000, 0x24000,
	0xffff0196, 1, 0x580000, 0, 0xd80000, 0x24000,
	0xffff0197, 1, 0xe80000, 0, 0x1480000, 0x24000,
	0xffff0197, 1, 0x1000000, 0, 0x1380000, 0x1024000,
	0xffff0173, 7, 0xb80000, 0, 0x11c0000, 0x20000,
	0xffff0173, 7, 0xe80000, 0, 0x11c0000, 0x28000,
	0xffff0171, 7, 0xc40000, 0, 0x1300000, 0x24000,
	0xffff02ab, 7, 0x2d00000, 0, 0x1500000, 0x24000,
	0xffff0027, 1, 0x2b80000, 0, 0xe80000, 0x3000,
	0xffff0027, 1, 0x2c80000, 0, 0xe80000, 0x3000,
	0xffff0010, 1, 0x2d80000, 0, 0xe80000, 0x3000,
	0xffff0027, 1, 0x2e80000, 0, 0xe80000, 0x5000,
	0xffff0027, 1, 0x2f80000, 0, 0xe80000, 0x5000,
	0xffff0026, 1, 0x2c80000, 0, 0x1300000, 0x2c000,
	0xffff0025, 1, 0x2d80000, 0, 0x1280000, 0x2a000,
	0xffff00e7, 2, 0x2e80000, 0, 0x1400000, 0x4000,
	0xffff00e7, 1, 0x2e00000, 0, 0x1680000, 0xb000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_02001a18[] = {
	1,
	(s32)0xffff0001,
	1,
	49,
	(s32)0xffff0002,
	2,
	33,
	(s32)0xffff0003,
	3,
	1,
	(s32)0xffff0004,
	4,
	1,
	(s32)0xffff0005,
	5,
	2,
	(s32)0xffff0014,
	(s32)Func_02000168,
	2,
	(s32)0xffff0015,
	(s32)Func_020000fc,
	0x1815,
	0x2030010,
	(s32)Func_020000ec,
	0x8b85,
	-0x10000,
	(s32)Func_020001d4,
	-1,
	0,
	0,
};

s32 Data_02001a90[] = {
	1,
	(s32)0xffff0006,
	6,
	1,
	(s32)0xffff0007,
	7,
	1,
	(s32)0xffff0008,
	8,
	1,
	(s32)0xffff0009,
	9,
	1,
	(s32)0xffff000a,
	10,
	2,
	0x8bd0014,
	(s32)Func_02000818,
	0,
	0x8a70017,
	MSGNO(MsgEverythingSSo),
	0x8d15,
	0x8a70017,
	MSGNO(MsgIDonTKnow),
	0,
	(s32)0xffff0017,
	MSGNO(MsgBeforeIKnew),
	0x8d15,
	(s32)0xffff0017,
	MSGNO(MsgNoMatterHow),
	0,
	(s32)0xffff0018,
	MSGNO(MsgIVeBeen),
	0,
	(s32)0xffff0012,
	(s32)Func_020000a0,
	0,
	(s32)0xffff0010,
	MSGNO(MsgIHatePrison),
	0,
	(s32)0xffff0011,
	MSGNO(MsgItSoundsLike),
	0,
	(s32)0xffff0013,
	MSGNO(MsgEverythingWouldHave),
	0,
	(s32)0xffff0014,
	MSGNO(MsgISawYou),
	0x8d15,
	(s32)0xffff0018,
	MSGNO(MsgItSSo),
	0x8d15,
	(s32)0xffff0012,
	MSGNO(MsgHasStrangePowers),
	0x8d15,
	(s32)0xffff0010,
	MSGNO(MsgIMNotStaying),
	0x8d15,
	(s32)0xffff0011,
	MSGNO(MsgIfTheyDon),
	0x8d15,
	(s32)0xffff0013,
	MSGNO(MsgOhWhatS),
	0x8d15,
	(s32)0xffff0014,
	MSGNO(MsgIHearSome),
	0x8515,
	0x200000a,
	0,
	0x50008615,
	0x2010009,
	(s32)Func_02000084,
	0x1815,
	0x2030010,
	(s32)Func_020000ec,
	0x10008c15,
	(s32)0xffff0008,
	(s32)Func_02000098,
	0x8c15,
	(s32)0xffff0008,
	(s32)Func_0200009c,
	-1,
	0,
	0,
};
