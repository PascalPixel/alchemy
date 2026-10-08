#include "TYPES.H"
#include "FIELD_SCENE.H"

struct SceneEntrance SoruDou_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SoruDou_SceneTable1[] = {
	SCENE_EXITS(13),
	SCENE_EXIT(1, 12, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement SoruDou_SceneTable2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent SoruDou_SceneTable3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE },
	{ SCENE_EVENTS_END },
};
