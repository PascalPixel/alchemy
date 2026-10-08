#include "types.h"
#include "field_event.h"
#include "field_scene.h"

struct SceneEntrance gTitleEntrances[] = {
	{ 0, CONDITION_ALWAYS, 0, 0, 0, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gTitleExits[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement gTitlePlacements[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent gTitleEvents[] = {
	{ SCENE_EVENTS_END },
};

u16 gTitleVramBlock[] = {
	MAP_CELL_STEPS_END,
};
