#include "types.h"
#include "field_scene.h"

void Func_02000054(void);
void Func_02000068(void);
void Func_0200007c(void);
void Func_02000090(void);

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(313),
	SCENE_EXITS(314),
	SCENE_EXITS(315),
	SCENE_EXITS(316),
	SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
	{SCENE_TABLE_END},
};

struct SceneEvent gSceneEvents[] = {
	{0, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
	{0, 9, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000068)},
	{0, 10, CONDITION_ALWAYS, EVENT_SCRIPT(Func_0200007c)},
	{0, 11, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000090)},
	{SCENE_EVENTS_END},
};

s32 Data_0200015c[] = {
	0x2000200, 0x3600400, -1, -1,
};
