#include "types.h"
#include "field_scene.h"

struct SceneEntrance SoruRoka_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 727, 0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 56, 0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SoruRoka_SceneTable1[] = {
	SCENE_EXITS(15),
	SCENE_EXIT(1, 16, 5),
	SCENE_EXIT(2, 16, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement SoruRoka_SceneTable2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent SoruRoka_SceneTable3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};
