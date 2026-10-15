#include "types.h"
#include "field_scene.h"

void Func_020010f0(void);
void Func_02001148(void);
void Func_02001184(void);
void Func_020011b8(void);
void Func_020011d4(void);
void Func_02001228(void);
void Func_02001270(void);

extern u32 gData_020015a0[];
extern u32 gData_020015dc[];
extern u32 gData_02001618[];

u32 gData_02001540[] = {
	0, 33, 22, 38, 0, 2,
	0, 0, 0, 0, 1, 38,
};

u32 gData_02001570[] = {
	0, 44, 22, 38, 0, 2,
	0, 0, 0, 0, 1, 38,
};

u32 gData_020015a0[] = {
	0, 1, 13, 22, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_020015dc[] = {
	0, 1, 13, 44, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_02001618[] = {
	0, 1, 13, 126, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

s16 gData_02001654[] = {
	13, -1,
};

s16 gData_02001658[] = {
	10, 11, 12, 13, 14, 15, -1, 0,
};

s32 gData_02001668[] = {
	(s32)gData_020015a0,
	(s32)gData_020015dc,
	(s32)gData_02001618,
};

s16 gData_02001674[] = {
	8, 0, 512, 9, 0, 513, 12, 0,
	514, -1,
};

s16 gData_02001688[] = {
	8, 0, 512, 9, 0, 513, -1,
};

s16 gData_02001696[] = {
	10, 515, 11, 516, -1,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gData_020016d0[] = {
	0x2c00d0, 0xe002f0, 0x300003c, 0x1ffff, 0xffff, 0,
	0, 0,
};

u32 gData_020016f0[] = {
	0x100190, 0x1a00160, 0x1700020, 0x1ffff, 0xffff, 0,
	0, 0,
};

u32 gSceneExits[] = {
	SCENE_EXITS(172),
	SCENE_EXIT_IF(1, 171, 10, 0xffffffff),
	SCENE_EXIT_IF(2, 172, 3, 0xffffffff),
	SCENE_EXIT_IF(3, 172, 2, 0xffffffff),
	SCENE_EXIT_IF(4, 173, 1, 0xffffffff),
	SCENE_EXITS(173),
	SCENE_EXIT_IF(1, 172, 4, 0xffffffff),
	SCENE_EXIT_IF(2, 173, 3, 0xffffffff),
	SCENE_EXIT_IF(3, 173, 2, 0xffffffff),
	SCENE_EXITS_END,
};

u32 gData_02001754[] = {
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_0200176c[] = {
	0xffff0196, 1, 0xa80000, 0, 0x3380000, 0x2c000,
	0xffff0196, 1, 0xc80000, 0, 0x3380000, 0x2c000,
	0xffff0122, 1, 0x880000, 0, 0x3380000, 0x2c000,
	0xffff0122, 1, 0xe80000, 0, 0x3380000, 0x2c000,
	0xffff0196, 1, 0x2b80000, 0, 0xf80000, 0x2c000,
	0xffff0110, 1, 0x2080000, 0, 0x580000, 0x2c000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001814[] = {
	0xffff0196, 1, 0x1b80000, 0, 0x2280000, 0x2c000,
	0xffff0196, 1, 0x1980000, 0, 0x2280000, 0x2c000,
	0xffff0110, 1, 0x3180000, 0, 0x980000, 0x2c000,
	0xffff0110, 1, 0x3380000, 0, 0x980000, 0x2c000,
	0xffff0110, 1, 0x2f80000, 0, 0xb80000, 0x2c000,
	0xffff0110, 1, 0x3180000, 0, 0xd80000, 0x2c000,
	0xffff0110, 1, 0x3380000, 0, 0xd80000, 0x2c000,
	0xffff0110, 1, 0x3580000, 0, 0xb80000, 0x2c000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_020018ec[] = {
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
	(s32)Func_020010f0,
	0x50008615,
	0x2010009,
	(s32)Func_020010f0,
	0x50008615,
	0x202000c,
	(s32)Func_020010f0,
	0x1815,
	0x203000a,
	(s32)Func_02001148,
	0x1815,
	0x204000b,
	(s32)Func_02001148,
	0x8b85,
	-0x10000,
	(s32)Func_02001184,
	0x40008b85,
	-0x10000,
	(s32)Func_020011b8,
	0x10008c15,
	(s32)0xffff000d,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000d,
	(s32)Func_02001228,
	8,
	-0x10000,
	(s32)Func_020011d4,
	9,
	-0x10000,
	(s32)Func_02001228,
	-1,
	0,
	0,
};

s32 gData_020019ac[] = {
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
	(s32)Func_020010f0,
	0x50008615,
	0x2010009,
	(s32)Func_020010f0,
	0x50008a05,
	(s32)0xffff0032,
	(s32)Func_02001270,
	0x10008c15,
	(s32)0xffff000a,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000a,
	(s32)Func_02001228,
	0x10008c15,
	(s32)0xffff000b,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000b,
	(s32)Func_02001228,
	0x10008c15,
	(s32)0xffff000c,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000c,
	(s32)Func_02001228,
	0x10008c15,
	(s32)0xffff000d,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000d,
	(s32)Func_02001228,
	0x10008c15,
	(s32)0xffff000e,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000e,
	(s32)Func_02001228,
	0x10008c15,
	(s32)0xffff000f,
	(s32)Func_020011d4,
	0x8c15,
	(s32)0xffff000f,
	(s32)Func_02001228,
	-1,
	0,
	0,
};
