#include "TYPES.H"
#include "FIELD_SCENE.H"

struct SceneEntrance SoruMeiro_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 424, 0, 615, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 376, 0, 53, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SoruMeiro_SceneTable1[] = {
	SCENE_EXITS(14),
	SCENE_EXIT(1, 16, 7),
	SCENE_EXIT(2, 16, 14),
	SCENE_EXITS_END,
};

struct ScenePlacement SoruMeiro_SceneTable2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent SoruMeiro_SceneTable3[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf02, FIND_ITEM(0xe1) },
	{ SCENE_EVENTS_END },
};
