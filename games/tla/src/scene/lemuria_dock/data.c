#include "types.h"
#include "field_scene.h"

void Func_0200006c(void);
void Func_02000158(void);
void Func_020002dc(void);

extern s16 gData_02000604[];

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

s32 gData_0200057c[] = {
	200, 0x101030c7, -1, 0x102020c9, -1, 0x10301028,
	-1, 511,
};

s32 gData_0200059c[] = {
	200, 0x101050c7, -1, 0x102050c9, -1, 0x10301028,
	-1, 511,
};

struct ScenePlacement gScenePlacements[] = {
	{56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_NORTH, 0, 0},
#if !defined(TLA_EDITION_JA)
	{420, CONDITION_ALWAYS, 1, PIXELS(384), 0, PIXELS(392), FACING_EAST, 2, 0},
#endif
	{SCENE_TABLE_END},
};

s16 gData_02000604[] = {
	51, 38, 1, 1, 6, -1,
};

s32 gData_02000610[] = {
	(s32)gData_02000604,
	0x160046,
};

struct SceneEvent gSceneEvents[] = {
	{1, 2, CONDITION_ALWAYS, 2},
	{0xc602, 3, CONDITION_ALWAYS, EVENT_SCRIPT(Func_020002dc)},
	{2, 21, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200006c)},
	{0, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000158)},
	{0x8d15, 1032, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000158)},
	{SCENE_EVENTS_END},
};
