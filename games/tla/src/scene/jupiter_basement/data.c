#include "types.h"
#include "field_scene.h"

void Func_02000054(void);
void Func_02000074(void);
void Func_02000094(void);
void Func_020000f8(void);
void Func_02000118(void);
void Func_02000124(void);

extern u32 gData_020021c8[];
extern u32 gData_02002204[];
extern u32 gData_02002240[];

u32 gData_020021c8[] = {
	0, 1, 13, 22, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_02002204[] = {
	0, 1, 13, 44, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

u32 gData_02002240[] = {
	0, 1, 13, 126, 0, 0,
	1, 2, 0, 0, 0, 0,
	1, 38, 17,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(252),
	SCENE_EXIT(1, 253, 1),
	SCENE_EXIT(2, 252, 3),
	SCENE_EXIT(3, 252, 2),
	SCENE_EXIT(4, 251, 4),
	SCENE_EXIT(6, 254, 6),
	SCENE_EXITS(253),
	SCENE_EXIT(1, 252, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
	{SCENE_TABLE_END},
};

s32 gData_020022e8[] = {
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
	0x50008905,
	(s32)0xffff001e,
	(s32)Func_02000054,
	0x50008905,
	(s32)0xffff001f,
	(s32)Func_02000074,
	0x50008905,
	(s32)0xffff0020,
	(s32)Func_020000f8,
	0x50008905,
	(s32)0xffff0021,
	(s32)Func_02000118,
	0x50008905,
	(s32)0xffff0022,
	(s32)Func_02000124,
	0x50008905,
	(s32)0xffff0026,
	(s32)Func_02000094,
	-1,
	0,
	0,
};

s32 gData_0200236c[] = {
	1, (s32)0xffff0001, 1, -1, 0, 0,
};

s32 gData_02002384[] = {
	-1,
};

u32 gData_02002388[] = {
	1,
};

u32 gData_0200238c[] = {
	39, 5, 0, 12, 38,
};

s32 gData_020023a0[] = {
	(s32)gData_020021c8,
	(s32)gData_02002204,
	(s32)gData_02002240,
};
