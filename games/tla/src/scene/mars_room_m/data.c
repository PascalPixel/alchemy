#include "types.h"
#include "field_scene.h"

void Func_0200004c(void);
void Func_020002c8(void);
void Func_020002e8(void);
void Func_020002fc(void);
void Func_0200030c(void);
void Func_020006c8(void);
void Func_02000abc(void);
void Func_020010e4(void);
void Func_02001154(void);
void Func_020011c4(void);
void Func_02001234(void);
void Func_020013a0(void);

extern s32 gData_020032c8[];

s32 gData_020032c8[] = {
	46,
	(s32)Func_0200004c,
	17,
};

u32 gData_020032d4[] = {
	0, 30, 22, 38, 0, 2,
	0, 0, 0, 0, 1, 38,
};

u32 gData_02003304[] = {
	0, 20, 0x80010000, 22, 25, 0,
	0, 1, 22, 25, 1, 0,
	1, 13, 10, 0xc0010000, 38,
};

u32 gData_02003348[] = {
	0, 20, 0x80010000, 22, 25, 0,
	0, 1, 22, 25, 1, 0,
	1, 13, 10, 0xc0010000, 38,
};

s16 gData_0200338c[] = {
	201, 202, 203, 204, 205, 206, 207, -1,
};

s16 gData_0200339c[] = {
	23, 25, 22, 23, 25, 22, 23, 25,
	22, 23, 25, 22, 23, 25, 22, 23,
	25, 22, 23, 25, 22, -1,
};

s16 gData_020033c8[] = {
	30, 27, 31, 22, 21, 31, 20, 18,
	31, 13, 11, 27, 11, 8, 24, 8,
	5, 21, 31, 31, 31, -1,
};

u32 gData_020033f4[] = {
	0xc900ce, 0xcb00ca, 0xcd00cc, 0xce00cd, 0xca00c9, 0xcc00cb,
	0xcd00cc, 0xc900ce, 0xcb00ca, 0xcc00cb, 0xce00cd, 0xca00c9,
	0xcb00ca, 0xcd00cc, 0xc900ce, 0xca00c9, 0xcc00cb, 0xce00cd,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(296),
	SCENE_EXIT(1, 295, 5),
	SCENE_EXIT(2, 293, 8),
	SCENE_EXIT(3, 292, 4),
	SCENE_EXIT(4, 294, 15),
	SCENE_EXITS(287),
	SCENE_EXIT(1, 297, 1),
	SCENE_EXIT(2, 280, 2),
	SCENE_EXIT(3, 291, 1),
	SCENE_EXIT(4, 288, 1),
	SCENE_EXIT(5, 290, 1),
	SCENE_EXIT(6, 289, 1),
	SCENE_EXIT(7, 287, 8),
	SCENE_EXIT(8, 287, 7),
	SCENE_EXIT(9, 287, 10),
	SCENE_EXIT(10, 287, 9),
	SCENE_EXIT(11, 287, 12),
	SCENE_EXIT(12, 287, 11),
	SCENE_EXIT(13, 287, 14),
	SCENE_EXIT(14, 287, 13),
	SCENE_EXITS(288),
	SCENE_EXIT(1, 287, 4),
	SCENE_EXIT(2, 293, 1),
	SCENE_EXITS(289),
	SCENE_EXIT(1, 287, 6),
	SCENE_EXIT(2, 294, 1),
	SCENE_EXITS(290),
	SCENE_EXIT(1, 287, 5),
	SCENE_EXIT(2, 292, 1),
	SCENE_EXITS(291),
	SCENE_EXIT(1, 287, 3),
	SCENE_EXIT(2, 295, 1),
	SCENE_EXITS_END,
};

u32 gData_020034f0[] = {
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02003508[] = {
	(s32)0xffff01a2,
	(s32)gData_020032c8,
	0x1f80000,
	0,
	0xb80000,
	0x20000,
	(s32)0xffff016d,
	7,
	0x1f80000,
	0,
	0xb80000,
	0x24000,
	(s32)0xffff01e9,
	(s32)gData_020032c8,
	0x1f80000,
	0,
	0x700000,
	0x20000,
	(s32)0xffff019d,
	1,
	0x1d80000,
	0,
	0xe80000,
	0x20000,
	(s32)0xffff019d,
	1,
	0x2180000,
	0,
	0xe80000,
	0x28000,
	0xffff,
	0,
	0,
	0,
	0,
	0,
};

u32 gData_02003598[] = {
	0xffff01a0, 1, 0xc80000, 0, 0xb80000, 0x20000,
	0xffff01a0, 1, 0xf80000, 0, 0xb80000, 0x1028000,
	0xffff01a0, 1, 0xa80000, 0, 0xd80000, 0x1020000,
	0xffff01a0, 1, 0x1180000, 0, 0xd80000, 0x1028000,
	0xffff016d, 1, 0xf80000, 0, 0xb80000, 0x24000,
	0xffff016d, 1, 0xa80000, 0, 0xd80000, 0x24000,
	0xffff016d, 1, 0x1180000, 0, 0xd80000, 0x24000,
	0xffff016d, 1, 0xc80000, 0, 0xb80000, 0x24000,
	0xffff01e9, 7, 0xe00000, 0, 0x900000, 0x24000,
	0xffff02a1, 7, 0, 0, 0, 0x24000,
	0xffff019c, 7, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_020036b8[] = {
	-1, 0, 0,
};

s32 gData_020036c4[] = {
	-1,
};

s32 gData_020036c8[] = {
	2,
	(s32)0xffff0001,
	(s32)Func_020002c8,
	0x10009a15,
	(s32)0xffff0008,
	(s32)Func_020002e8,
	0x60009a15,
	(s32)0xffff000a,
	(s32)Func_020002fc,
	0x20009a15,
	(s32)0xffff000a,
	(s32)Func_0200030c,
	-1,
	0,
	0,
};

s32 gData_02003704[] = {
	-1,
};

s32 gData_02003708[] = {
	49,
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
	1,
	(s32)0xffff000b,
	11,
	1,
	(s32)0xffff000c,
	12,
	1,
	(s32)0xffff000d,
	13,
	1,
	(s32)0xffff000e,
	14,
	2,
	0x1a300014,
	(s32)Func_020006c8,
	2,
	0x1a300015,
	(s32)Func_020006c8,
	2,
	0x1a300016,
	(s32)Func_020006c8,
	2,
	0x1a300017,
	(s32)Func_020006c8,
	2,
	0x1a300018,
	(s32)Func_020006c8,
	2,
	0x1a300019,
	(s32)Func_020006c8,
	2,
	0x1a30001a,
	(s32)Func_020006c8,
	2,
	0x1a30001b,
	(s32)Func_020006c8,
	0x9c05,
	(s32)0xffff0001,
	(s32)Func_02000abc,
	3,
	(s32)0xffff001e,
	(s32)Func_020010e4,
	3,
	(s32)0xffff001f,
	(s32)Func_02001154,
	3,
	(s32)0xffff0020,
	(s32)Func_020011c4,
	3,
	(s32)0xffff0021,
	(s32)Func_02001234,
	2,
	0xa2e0022,
	(s32)Func_020013a0,
	-1,
	0,
	0,
};

s32 gData_02003858[] = {
	1, (s32)0xffff0001, 1, 1, (s32)0xffff0002, 2,
	-1, 0, 0,
};

s32 gData_0200387c[] = {
	23, 9, 0x3000, 23, 10, 0x3000,
	23, 6, -0x1800, 0, 1, 13,
	8, 0, (s32)0x80010000, 23, 9, -0x3000,
	23, 10, -0x3000, 23, 6, 0x1800,
	0, 1, 13, 12, (s32)0xc0010000, 3,
	0, 1, 38,
};

u32 gData_02003900[] = {
	0, 20, 3, 0, 1, 38,
};
