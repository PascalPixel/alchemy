#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000054(void);

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(184),
	SCENE_EXIT_IF(1, 182, 7, 0xffffffff),
	SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
	{202, CONDITION_ALWAYS, 1, PIXELS(720), 0, PIXELS(147), FACING_SOUTH, 1, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent gSceneEvents[] = {
	{1, 1, CONDITION_ALWAYS, 1},
	{0xc400, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Func_02000054)},
	{0, 8, 0x197f, MSGNO(MsgChanpaTreasureEncouragesLaziness)},
	{0x8d15, 8, 0x197f, MSGNO(MsgChanpaVillagersStoppedWorking)},
	{0, 8, CONDITION_ALWAYS, MSGNO(MsgChanpaCaptainLastHope)},
	{0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgChanpaNeedsCaptainTreasure)},
	{SCENE_EVENTS_END},
};
