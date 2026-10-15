#include "types.h"
#include "field_scene.h"

void Func_02000038(void);
void Func_02000044(void);
void Func_02000060(void);
void Func_02000080(void);
void Func_020000a0(void);
void Func_020000c0(void);
void Func_020000e0(void);
void Func_02000100(void);
void Func_0200018c(void);
void Func_020001b0(void);
void Func_020001d4(void);
void Func_020001e0(void);
void Func_02000260(void);
void Func_020002e0(void);
void Func_02000364(void);
void Func_020003e4(void);
void Func_02000464(void);
void Func_020004e4(void);
void Func_02000564(void);
void Func_020005e8(void);
void Func_02000608(void);
void Func_0200066c(void);
void Func_0200069c(void);
void Func_020006cc(void);
void Func_02000704(void);
void Func_02000740(void);
void Func_0200077c(void);
void Func_020007b8(void);
void Func_020007f0(void);
void Func_0200082c(void);
void Func_02000868(void);
void Func_0200094c(void);
void Func_020009d4(void);
void Func_02000d90(void);

u32 gData_0200128c[] = {
	0, 15, 3, 0, 1, 38,
};

u32 gData_020012a4[] = {
	0, 1, 13, 44, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gData_02001310[] = {
	0x2e0184, 0x18c0114, 0x11c0036, 0x1ffff, 0xffff, 0,
	0, 0,
};

u32 gSceneExits[] = {
	SCENE_EXITS(126),
	SCENE_EXIT(1, 124, 2),
	SCENE_EXIT(2, 127, 1),
	SCENE_EXIT(3, 127, 3),
	SCENE_EXIT(4, 127, 4),
	SCENE_EXIT(5, 127, 5),
	SCENE_EXITS(127),
	SCENE_EXIT(1, 126, 2),
	SCENE_EXIT(2, 128, 1),
	SCENE_EXITS(128),
	SCENE_EXIT(1, 127, 2),
	SCENE_EXIT(2, 130, 1),
	SCENE_EXITS(129),
	SCENE_EXIT(1, 130, 2),
	SCENE_EXITS(130),
	SCENE_EXIT(1, 128, 2),
	SCENE_EXIT(2, 129, 1),
	SCENE_EXITS_END,
};

u32 gData_02001378[] = {
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001390[] = {
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020013a8[] = {
	0x3a00f3, 1, 0x2d80000, 0, 0x1380000, 0x4000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_020013d8[] = {
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001498[] = {
	0xffff0177, 1, 0xa80000, 0, 0x980000, 0x24000,
	0xffff0038, 1, 0, 0, 0, 0x24000,
	0xffff0182, 1, 0xc80000, 0, 0xb80000, 0x24000,
	0xffff0182, 1, 0x880000, 0, 0xb80000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 gData_02001510[] = {
	0xffff0122, 1, 0, 0, 0, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 gData_02001540[] = {
	-1, 0, 0,
};

s32 gData_0200154c[] = {
	49,
	(s32)0xffff0002,
	2,
	0x50008905,
	(s32)0xffff0029,
	(s32)Func_02000044,
	0x50008905,
	(s32)0xffff002b,
	(s32)Func_0200018c,
	0x50008905,
	(s32)0xffff002c,
	(s32)Func_020001b0,
	0x50008905,
	(s32)0xffff002d,
	(s32)Func_020001d4,
	-1,
	0,
	0,
};

s32 gData_02001594[] = {
	33,
	(s32)0xffff0001,
	1,
	1,
	(s32)0xffff0002,
	2,
	0xc400,
	(s32)0xffff0008,
	(s32)Func_02000038,
	-1,
	0,
	0,
};

s32 gData_020015c4[] = {
	1,
	(s32)0xffff0001,
	1,
	49,
	(s32)0xffff0002,
	2,
	0x50009085,
	-0x10000,
	(s32)Func_0200066c,
	0x40009085,
	-0x10000,
	(s32)Func_0200069c,
	0x50008905,
	(s32)0xffff0028,
	(s32)Func_02000060,
	0x50008905,
	(s32)0xffff0029,
	(s32)Func_02000080,
	0x50008905,
	(s32)0xffff002a,
	(s32)Func_020001e0,
	0x50008905,
	(s32)0xffff002b,
	(s32)Func_02000260,
	0x50008905,
	(s32)0xffff002c,
	(s32)Func_020002e0,
	0x50008905,
	(s32)0xffff002d,
	(s32)Func_02000364,
	0x50008905,
	(s32)0xffff002e,
	(s32)Func_020003e4,
	0x50008905,
	(s32)0xffff002f,
	(s32)Func_02000464,
	0x50008905,
	(s32)0xffff0030,
	(s32)Func_020004e4,
	0x50008905,
	(s32)0xffff0031,
	(s32)Func_02000100,
	0x1815,
	0x2000008,
	(s32)Func_020006cc,
	0x1815,
	0x2010009,
	(s32)Func_02000704,
	0x1815,
	0x202000a,
	(s32)Func_02000740,
	0x1815,
	0x203000b,
	(s32)Func_0200077c,
	0x1815,
	0x204000c,
	(s32)Func_020007b8,
	0x1815,
	0x205000d,
	(s32)Func_020007f0,
	0x1815,
	0x206000e,
	(s32)Func_0200082c,
	-1,
	0,
	0,
};

s32 gData_020016cc[] = {
	49,
	(s32)0xffff0001,
	1,
	2,
	0x9ce0028,
	(s32)Func_020009d4,
	0,
	0xf930008,
	(s32)Func_02000d90,
	0x50008905,
	(s32)0xffff001e,
	(s32)Func_020005e8,
	0x50008905,
	(s32)0xffff001f,
	(s32)Func_02000608,
	-1,
	0,
	0,
};

s32 gData_02001714[] = {
	33,
	(s32)0xffff0001,
	1,
	33,
	(s32)0xffff0002,
	2,
	0x50008905,
	(s32)0xffff0028,
	(s32)Func_020000a0,
	0x50008905,
	(s32)0xffff0029,
	(s32)Func_020000c0,
	0x50008905,
	(s32)0xffff002b,
	(s32)Func_020000e0,
	0x50008905,
	(s32)0xffff002a,
	(s32)Func_02000564,
	2,
	(s32)0xffff0032,
	(s32)Func_0200094c,
	0x1815,
	0x2000008,
	(s32)Func_02000868,
	-1,
	0,
	0,
};
