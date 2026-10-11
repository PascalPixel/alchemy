#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000054(void);
void Func_020000c8(void);
void Func_02000224(void);

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 152, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(53),
	SCENE_EXIT_IF(1, 2, 10, 0xffffffff),
	SCENE_EXIT_IF(2, 2, 11, 0xffffffff),
	SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
	{73, CONDITION_ALWAYS, 1, PIXELS(188), 0, PIXELS(260), FACING_SOUTH, 1, 0},
	{88, CONDITION_ALWAYS, 1, PIXELS(132), 0, PIXELS(260), FACING_SOUTH, 1, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent gSceneEvents[] = {
	{1, 1, CONDITION_ALWAYS, 1},
	{1, 2, CONDITION_ALWAYS, 2},
	{2, 10, 0x913, EVENT_SCRIPT(Func_020000c8)},
	{2, 10, 0x916, EVENT_SCRIPT(Func_02000224)},
	{0, 8, 0x913, EVENT_SCRIPT(Func_02000054)},
	{0, 8, CONDITION_ALWAYS, MSGNO(MsgMadoraMikasaraVillage)},
	{0, 9, 0x913, MSGNO(MsgMadoraCanCrossWithPermission)},
	{0, 9, CONDITION_ALWAYS, MSGNO(MsgMadoraAvoidYanpiSabaku)},
	{0x8d15, 8, 0x913, MSGNO(MsgMadoraForTheirSafety)},
	{0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgMadoraWouldNeedBoat)},
	{0x8d15, 9, 0x913, MSGNO(MsgMadoraElderReturn)},
	{0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgMadoraRatherStayHome)},
	{SCENE_EVENTS_END},
};
