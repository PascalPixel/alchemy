#include "types.h"
#include "field_scene.h"

void Func_02000078(void);
void Func_02000168(void);
void Func_02000ce0(void);
void Func_02001064(void);
void Func_02001074(void);
void Func_02001178(void);
void Func_0200128c(void);
void Func_020014c0(void);
void Func_02001da0(void);

s16 Data_020020dc[] = {
	8, 512, -1, 0,
};

u32 Data_020020e4[] = {
	39, 2, 4, 0x1070000, 0, 0xb00000,
	1, 39, 1, 0, 1, 3,
	0, 1, 17,
};

u32 Data_02002120[] = {
	39, 2, 4, 0x1700000, 0, 0x1c00000,
	1, 39, 1, 0, 10, 22,
	3, 0, 0, 10, 22, 3,
	0xe000, 0, 10, 22, 3, 0xc000,
	0, 10, 17,
};

u32 Data_0200218c[] = {
	22, 3, 0x4000, 0, 30, 22,
	3, 0x6000, 0, 50, 22, 3,
	0x4000, 0, 10, 22, 3, 0x2000,
	0, 70, 13, 0xffff, 0,
};

u32 Data_020021e8[] = {
	22, 3, 0, 0, 50, 22,
	3, 0x2000, 0, 70, 13, 0xffff,
	0,
};

u32 Data_0200221c[] = {
	22, 3, 0xd000, 0, 70, 22,
	3, 0xb000, 0, 50, 22, 3,
	0xd000, 0, 10, 22, 3, 0,
	0, 30, 13, 0xffff, 0,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(43),
	SCENE_EXIT(1, 40, 1),
	SCENE_EXIT(1, 2, 68),
	SCENE_EXIT(2, 2, 8),
	SCENE_EXITS(42),
	SCENE_EXIT(1, 2, 42),
	SCENE_EXIT(2, 2, 8),
	SCENE_EXIT(10, 2, 71),
	SCENE_EXITS_END,
};

u32 Data_020022cc[] = {
	0xffff0122, 1, 0x880000, 0, 0x1380000, 0x24000,
	0xffff0038, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0005, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0006, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0007, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

u32 Data_0200235c[] = {
	0xffff0122, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0038, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0005, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0006, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff0007, 1, 0x80000, 0, 0x80000, 0x24000,
	0xffff, 0, 0, 0, 0, 0,
};

s32 Data_020023ec[] = {
	-1, 0, 0,
};

s32 Data_020023f8[] = {
	0xc602,
	(s32)0xffff0001,
	(s32)Func_02000078,
	1,
	(s32)0xffff0002,
	2,
	2,
	0x980000a,
	(s32)Func_02000ce0,
	2,
	0x981000b,
	(s32)Func_020014c0,
	2,
	0x989000c,
	(s32)Func_02001da0,
	2,
	0x9850001,
	(s32)Func_0200128c,
	3,
	0x985000e,
	(s32)Func_02000168,
	2,
	0x985000f,
	(s32)Func_02001074,
	2,
	0x9850010,
	(s32)Func_02001178,
	0x1815,
	0x2000008,
	(s32)Func_02001064,
	-1,
	0,
	0,
};

u32 Data_0200247c[] = {
	0, 10, 1, 3, 0, 1,
	38,
};

u32 Data_02002498[] = {
	0, 15, 3, 0, 1, 38,
};
