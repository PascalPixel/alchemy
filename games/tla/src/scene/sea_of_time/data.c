#include "types.h"
#include "field_scene.h"

void Func_0200033c(void);
void Func_0200043c(void);
void Func_0200054c(void);
void Func_02000818(void);
void Func_02000a80(void);
void Func_02000c94(void);
void Func_02000da0(void);
void Func_02000ef0(void);
void Func_02001068(void);
void Func_02001108(void);
void Func_02001480(void);
void Func_020014b4(void);
void Func_020014ec(void);
void Func_02001524(void);
void Func_0200155c(void);
void Func_02001594(void);
void Func_020015cc(void);
void Func_020015ec(void);

extern u32 Data_0200205c[];
extern u32 Data_02002098[];
extern u32 Data_020020d4[];
extern s32 Data_02002154[];

u32 Data_0200205c[] = {
	0, 1, 13, 22, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 Data_02002098[] = {
	0, 1, 13, 44, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 Data_020020d4[] = {
	0, 1, 13, 126, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 Data_02002110[] = {
	0x18101810, 0x1810,
};

u16 Data_02002118[] = {
	0, 672, 0, 704, 0, 896, 0, 320,
	0, 512, 0, 192, 0, 320, 0, 416,
	0, 320, 0, 832, 0, 96, 0, 160,
};

s32 Data_02002148[] = {
	(s32)Data_0200205c,
	(s32)Data_02002098,
	(s32)Data_020020d4,
};

s32 Data_02002154[] = {
	46,
	(s32)Func_0200033c,
	17,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(197),
	SCENE_EXIT_IF(1, 2, 42, 0xffffffff),
	SCENE_EXIT_IF(2, 198, 1, 0xffffffff),
	SCENE_EXIT_IF(3, 197, 1, 0xffffffff),
	SCENE_EXIT_IF(4, 42, 20, 0xffffffff),
	SCENE_EXITS(198),
	SCENE_EXIT_IF(1, 197, 2, 0xffffffff),
	SCENE_EXIT_IF(2, 198, 6, 0xffffffff),
	SCENE_EXIT_IF(3, 2, 44, 0xffffffff),
	SCENE_EXIT_IF(4, 2, 43, 0xffffffff),
	SCENE_EXIT_IF(5, 198, 2, 0xffffffff),
	SCENE_EXITS_END,
};

s32 Data_020021e4[] = {
	(s32)0xffff0008,
	(s32)Data_02002154,
	0x780000,
	0,
	0x1180000,
	0x2c000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

s32 Data_02002214[] = {
	(s32)0xffff0008,
	(s32)Data_02002154,
	0x780000,
	0,
	0x1180000,
	0x2c000,
	(s32)0xffff01a8,
	7,
	0,
	0,
	0,
	0x2c000,
	(s32)0xffff01a8,
	7,
	0,
	0,
	0,
	0x2c000,
	(s32)0xffff01a8,
	7,
	0,
	0,
	0,
	0x2c000,
	(s32)0xffff01a8,
	7,
	0,
	0,
	0,
	0x2c000,
	(s32)0xffff01a8,
	7,
	0,
	0,
	0,
	0x2c000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u16 Data_020022bc[] = {
	0,
};

u16 Data_020022be[] = {
	0,
};

u32 Data_020022c0[] = {
	0,
};

u32 Data_020022c4[] = {
	0,
};

u32 Data_020022c8[] = {
	0,
};

u32 Data_020022cc[] = {
	0,
};

s32 Data_020022d0[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	2,
	(s32)0xffff0031,
	(s32)Func_02001068,
	2,
	(s32)0xffff001e,
	(s32)Func_0200054c,
	2,
	(s32)0xffff001f,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0020,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0021,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0022,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0023,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0024,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0025,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0028,
	(s32)Func_0200043c,
	2,
	(s32)0xffff0029,
	(s32)Func_0200043c,
	2,
	(s32)0xffff002c,
	(s32)Func_02000ef0,
	2,
	0x200002d,
	(s32)Func_02000818,
	2,
	0x200002e,
	(s32)Func_02000818,
	2,
	0x200002f,
	(s32)Func_02000818,
	2,
	0x2000030,
	(s32)Func_02000818,
	2,
	0x2000032,
	(s32)Func_02000a80,
	2,
	0x2000033,
	(s32)Func_02000a80,
	2,
	0x2000034,
	(s32)Func_02000a80,
	2,
	0x2000035,
	(s32)Func_02000a80,
	2,
	0x2000036,
	(s32)Func_02000a80,
	2,
	0x2000037,
	(s32)Func_02000a80,
	2,
	0x2000038,
	(s32)Func_02000a80,
	2,
	0x2000039,
	(s32)Func_02000a80,
	6,
	(s32)0xffff00c8,
	(s32)Func_02000c94,
	6,
	(s32)0xffff00c9,
	(s32)Func_02000da0,
	-1,
	0,
	0,
};

s32 Data_0200242c[] = {
	1,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	1,
	(s32)0xffff0003,
	3,
	2,
	0x9400005,
	(s32)Func_02001108,
	2,
	(s32)0xffff001e,
	(s32)Func_0200054c,
	2,
	(s32)0xffff001f,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0020,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0021,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0022,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0023,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0024,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0025,
	(s32)Func_0200054c,
	2,
	(s32)0xffff0030,
	(s32)Func_02000818,
	2,
	(s32)0xffff0029,
	(s32)Func_0200043c,
	0x9815,
	(s32)0xffff0009,
	(s32)Func_02001480,
	0x9815,
	(s32)0xffff000a,
	(s32)Func_020014b4,
	0x9815,
	(s32)0xffff000b,
	(s32)Func_020014ec,
	0x9815,
	(s32)0xffff000c,
	(s32)Func_02001524,
	0x9815,
	(s32)0xffff000d,
	(s32)Func_0200155c,
	-1,
	0,
	0,
};

s32 Data_0200251c[] = {
	2,
	(s32)0xffff0001,
	(s32)Func_020015cc,
	2,
	(s32)0xffff0002,
	(s32)Func_020015ec,
	2,
	(s32)0xffff002d,
	(s32)Func_02000818,
	2,
	(s32)0xffff002e,
	(s32)Func_02000818,
	2,
	(s32)0xffff002f,
	(s32)Func_02000818,
	2,
	(s32)0xffff0030,
	(s32)Func_02000818,
	0x9815,
	(s32)0xffff0009,
	(s32)Func_02001594,
	-1,
	0,
	0,
};

u16 Data_0200257c[] = {
	1,
};
