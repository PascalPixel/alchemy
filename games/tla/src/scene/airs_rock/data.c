#include "types.h"
#include "field_scene.h"

void Func_0200033c(void);
void Func_02000548(void);
void Func_02000568(void);
void Func_020005ec(void);
void Func_02000620(void);
void Func_020006d4(void);
void Func_02000700(void);
void Func_02000824(void);
void Func_02000830(void);
void Func_0200095c(void);
void Func_02001010(void);
void Func_02001560(void);
void Func_02001ca4(void);
void Func_02001cc8(void);
void Func_02001da8(void);
void Func_02001f7c(void);
void Func_020026b8(void);

extern u32 gData_02003ce0[];
extern u32 gData_02003d1c[];
extern u32 gData_02003d58[];
extern s32 gData_02003dac[];

u32 gData_02003ce0[] = {
	0, 1, 13, 22, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_02003d1c[] = {
	0, 1, 13, 44, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_02003d58[] = {
	0, 1, 13, 126, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

s16 gData_02003d94[] = {
	12, 13, 15, 16, 22, -1,
};

s32 gData_02003da0[] = {
	(s32)gData_02003ce0,
	(s32)gData_02003d1c,
	(s32)gData_02003d58,
};

s32 gData_02003dac[] = {
	22,
	10,
	0x18000,
	39,
	2,
	46,
	(s32)Func_0200033c,
	17,
};

u32 gData_02003dcc[] = {
	39, 5, 0, 12, 38,
};

u32 gData_02003de0[] = {
	0, 60, 39, 5, 0, 12,
	38,
};

s32 gData_02003dfc[] = {
	0, 1, 23, 9, -1310, 23,
	10, -1310, 13, 32, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38,
};

s16 gData_02003e4c[] = {
	14, 512, -1, 0,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

s16 gData_02003e84[] = {
	256, 96, 144, 272, 112, 160, -1, 2,
	608, 96, 144, 624, 112, 160, -1, 3,
	864, 96, 144, 880, 112, 160, -1, 4,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gData_02003ec4[] = {
	256, -192, 816, 272, -176, 832, -1, 1,
	608, -192, 816, 624, -176, 832, -1, 2,
	864, -192, 816, 880, -176, 832, -1, 3,
	624, 176, 704, 640, 192, 720, -1, 5,
	784, 160, 688, 800, 176, 704, -1, 6,
	624, -200, 320, 640, -184, 336, -1, 7,
	784, -216, 304, 800, -200, 320, -1, 8,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

s16 gData_02003f44[] = {
	144, -96, 304, 160, -80, 320, -1, 6,
	208, -96, 304, 224, -80, 320, -1, 7,
	144, -80, 304, 160, -64, 320, -1, 8,
	208, -80, 304, 224, -64, 320, -1, 9,
	144, 160, 192, 160, 176, 208, -1, 10,
	208, 160, 192, 224, 176, 208, -1, 11,
	144, 160, 192, 160, 176, 208, -1, 10,
	208, 160, 192, 224, 176, 208, -1, 11,
	528, -96, 304, 544, -80, 320, -1, 12,
	592, -96, 304, 608, -80, 320, -1, 13,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
	SCENE_EXITS(82),
	SCENE_EXIT_IF(1, 2, 16, 0xffffffff),
	SCENE_EXIT_IF(2, 83, 1, 0xffffffff),
	SCENE_EXIT_IF(3, 83, 2, 0xffffffff),
	SCENE_EXIT_IF(4, 83, 3, 0xffffffff),
	SCENE_EXITS(83),
	SCENE_EXIT_IF(1, 82, 2, 0xffffffff),
	SCENE_EXIT_IF(2, 82, 3, 0xffffffff),
	SCENE_EXIT_IF(3, 82, 4, 0xffffffff),
	SCENE_EXIT_IF(4, 85, 10, 0xffffffff),
	SCENE_EXIT_IF(5, 83, 10, 0xffffffff),
	SCENE_EXIT_IF(6, 83, 11, 0xffffffff),
	SCENE_EXIT_IF(7, 83, 5, 0xffffffff),
	SCENE_EXIT_IF(8, 83, 6, 0xffffffff),
	SCENE_EXIT_IF(9, 84, 1, 0xffffffff),
	SCENE_EXITS(84),
	SCENE_EXIT_IF(1, 83, 12, 0xffffffff),
	SCENE_EXIT_IF(2, 84, 4, 0xffffffff),
	SCENE_EXIT_IF(3, 84, 5, 0xffffffff),
	SCENE_EXIT_IF(4, 84, 2, 0xffffffff),
	SCENE_EXIT_IF(5, 84, 3, 0xffffffff),
	SCENE_EXIT_IF(6, 84, 8, 0xffffffff),
	SCENE_EXIT_IF(7, 84, 9, 0xffffffff),
	SCENE_EXIT_IF(8, 84, 6, 0xffffffff),
	SCENE_EXIT_IF(9, 84, 7, 0xffffffff),
	SCENE_EXIT_IF(10, 84, 8, 0x87a),
	SCENE_EXIT_IF(11, 84, 9, 0x87a),
	SCENE_EXIT_IF(10, 84, 12, 0xffffffff),
	SCENE_EXIT_IF(11, 84, 13, 0xffffffff),
	SCENE_EXIT_IF(12, 84, 10, 0xffffffff),
	SCENE_EXIT_IF(13, 84, 11, 0xffffffff),
	SCENE_EXIT_IF(14, 86, 1, 0xffffffff),
	SCENE_EXITS_END,
};

s32 gData_020040ec[] = {
	(s32)0xffff017e,
	7,
	0x1480000,
	0,
	0xb80000,
	0x24000,
	(s32)0xffff0145,
	(s32)gData_02003dac,
	0x3980000,
	0,
	0xc80000,
	0x24000,
	(s32)0xffff0145,
	(s32)gData_02003dac,
	0x680000,
	0,
	0x2f80000,
	0x24000,
	(s32)0xffff0145,
	(s32)gData_02003dac,
	0x1780000,
	0,
	0x3280000,
	0x24000,
	(s32)0xffff0145,
	(s32)gData_02003dac,
	0x2780000,
	0,
	0x3780000,
	0x24000,
	(s32)0xffff0145,
	(s32)gData_02003dac,
	0x3680000,
	0,
	0x3580000,
	0x24000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u32 gData_02004194[] = {
	0xffff0179, 1, 1, 0, 0, 0x2020000,
	0xffff0179, 1, 2, 0, 0, 0x2020000,
	0xffff0179, 1, 3, 0, 0, 0x2028000,
	0xffff0179, 1, 4, 0, 0, 0x2028000,
	0xffff0110, 1, 6, 0, 0, 0x2024000,
	0xffff0110, 1, 7, 0, 0, 0x2024000,
	0xffff0122, 1, 8, 0, 0, 0x2024000,
	0xffff0110, 1, 9, 0, 0, 0x2024000,
	0xffff0110, 1, 10, 0, 0, 0x2024000,
	0xffff0179, 7, 0x2180000, 0, 0x1280000, 0x20000,
	0xffff0179, 7, 0x2a80000, 0, 0x1280000, 0x28000,
	0xffff0179, 7, 0x2180000, 0, 0x1280000, 0x20000,
	0xffff0179, 7, 0x2880000, 0, 0x1280000, 0x28000,
	0xffff0179, 7, 0x2c80000, 0, 0x1380000, 0x2c000,
	0xffff0110, 1, 11, 0, 0, 0x2024000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02004314[] = {
	0xffff0179, 7, 0x2180000, 0, 0x3880000, 0x2c000,
	0xffff0179, 7, 0x2c80000, 0, 0x3880000, 0x102c000,
	0xffff0110, 1, 0x2080000, 0, 0x3680000, 0x2c000,
	0xffff0110, 1, 0x2180000, 0, 0x3680000, 0x102c000,
	0xffff017e, 7, 0x980000, 0, 0xd80000, 0x24000,
	0xffff0110, 1, 0xa80000, 0, 0x1180000, 0x2c000,
	0xffff017e, 7, 0x2180000, 0, 0x780000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_020043d4[] = {
	1,
	(s32)0xffff0001,
	1,
	3,
	(s32)0xffff0014,
	(s32)Func_02000548,
	(s32)0x80004e15,
	0x8580008,
	(s32)Func_02000824,
	0x10004e15,
	0x8580008,
	(s32)Func_02000830,
	0x4e15,
	0x8580008,
	(s32)Func_02001010,
	(s32)0x80004e15,
	(s32)0xffff0009,
	(s32)Func_02000824,
	0x10004e15,
	(s32)0xffff0009,
	(s32)Func_02000830,
	0x4e15,
	(s32)0xffff0009,
	(s32)Func_0200095c,
	(s32)0x80004e15,
	(s32)0xffff000a,
	(s32)Func_02000824,
	0x10004e15,
	(s32)0xffff000a,
	(s32)Func_02000830,
	0x4e15,
	(s32)0xffff000a,
	(s32)Func_0200095c,
	(s32)0x80004e15,
	(s32)0xffff000b,
	(s32)Func_02000824,
	0x10004e15,
	(s32)0xffff000b,
	(s32)Func_02000830,
	0x4e15,
	(s32)0xffff000b,
	(s32)Func_0200095c,
	(s32)0x80004e15,
	(s32)0xffff000c,
	(s32)Func_02000824,
	0x10004e15,
	(s32)0xffff000c,
	(s32)Func_02000830,
	0x4e15,
	(s32)0xffff000c,
	(s32)Func_0200095c,
	(s32)0x80004e15,
	(s32)0xffff000d,
	(s32)Func_02000824,
	0x10004e15,
	(s32)0xffff000d,
	(s32)Func_02000830,
	0x4e15,
	(s32)0xffff000d,
	(s32)Func_0200095c,
	-1,
	0,
	0,
};

s32 gData_020044d0[] = {
	0xc602,
	(s32)0xffff0001,
	(s32)Func_02000568,
	1,
	(s32)0xffff0002,
	9,
	8,
	-0x10000,
	(s32)Func_02001ca4,
	9,
	-0x10000,
	(s32)Func_02001cc8,
	0x10008c15,
	0x85f000c,
	(s32)Func_02001ca4,
	0x8c15,
	0x85f000c,
	(s32)Func_02001cc8,
	0x10008c15,
	0x860000d,
	(s32)Func_02001ca4,
	0x8c15,
	0x860000d,
	(s32)Func_02001cc8,
	0x10008c15,
	0x861000f,
	(s32)Func_02001ca4,
	0x8c15,
	0x861000f,
	(s32)Func_02001cc8,
	0x10008c15,
	0x8620010,
	(s32)Func_02001ca4,
	0x8c15,
	0x8620010,
	(s32)Func_02001cc8,
	0x1815,
	0x200000e,
	(s32)Func_02001da8,
	0x10008c15,
	0x8870016,
	(s32)Func_02001ca4,
	0x8c15,
	0x8870016,
	(s32)Func_02001cc8,
	6,
	(s32)0xffff00c8,
	(s32)Func_02001560,
	-1,
	0,
	0,
};

s32 gData_0200459c[] = {
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
	1,
	(s32)0xffff0005,
	5,
	49,
	(s32)0xffff0006,
	14,
	0x4602,
	(s32)0xffff0014,
	(s32)Func_02000700,
	0x8c15,
	(s32)0xffff000a,
	(s32)Func_020005ec,
	0x8c15,
	(s32)0xffff000b,
	(s32)Func_02000620,
	0x8c15,
	0x879000d,
	(s32)Func_020006d4,
	(s32)0x80004e15,
	0x87a000c,
	(s32)Func_02000824,
	0x10004e15,
	0x87a000c,
	(s32)Func_02000830,
	0x4e15,
	0x87a000c,
	(s32)Func_020026b8,
	(s32)0x80004e15,
	0x87b000e,
	(s32)Func_02000824,
	0x10004e15,
	0x87b000e,
	(s32)Func_02000830,
	0x4e15,
	0x87b000e,
	(s32)Func_02001f7c,
	6,
	(s32)0xffff00c8,
	(s32)Func_02001560,
	-1,
	0,
	0,
};

u32 gData_02004674[] = {
	64, 64, 64, 64, 64, 64,
	64, 64,
};

u16 gData_02004694[] = {
	262,
};

u16 gData_02004696[] = {
	262,
};

s32 gData_02004698[] = {
	-1,
};
