#include "types.h"
#include "field_scene.h"
#include "text/msg_ids.h"

void Func_02000054(void);

u32 gData_020001b0[] = {
	0,
};

struct SceneEntrance gSceneEntrances[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

u32 gSceneExits[] = {
	SCENE_EXITS(54),
	SCENE_EXIT_IF(1, 2, 12, 0xffffffff),
	SCENE_EXIT_IF(2, 2, 13, 0xffffffff),
	SCENE_EXITS_END,
};

struct ScenePlacement gScenePlacements[] = {
	{192, 0x8ff, 1, PIXELS(200), 0, PIXELS(226), FACING_EAST, 1, 0},
	{187, 0x8ff, 1, PIXELS(259), 0, PIXELS(312), FACING_NORTH, 1, 0},
	{190, 0x8ff, 1, PIXELS(242), 0, PIXELS(313), FACING_NORTH, 1, 0},
	{407, CONDITION_ALWAYS, 1, PIXELS(240), 0, PIXELS(184), FACING_NORTH, 2, 0},
	{407, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_NORTH, 2, 0},
	{407, CONDITION_ALWAYS, 1, PIXELS(424), 0, PIXELS(104), FACING_NORTH, 2, 0},
	{407, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(104), FACING_NORTH, 2, 0},
	{SCENE_TABLE_END},
};

struct SceneEvent gSceneEvents[] = {
	{1, 1, CONDITION_ALWAYS, 1},
	{1, 2, CONDITION_ALWAYS, 2},
	{0, 8, CONDITION_ALWAYS, MSGNO(MsgMadraChampaBoat)},
	{0, 9, CONDITION_ALWAYS, MSGNO(MsgMadraSailorNotChampa)},
	{0, 10, CONDITION_ALWAYS, MSGNO(MsgMadraWaveDamage)},
	{0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgMadraCannotEscapeBySea)},
	{0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgMadraBriggsNewShip)},
	{0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgMadraPirateAttackParties)},
	{0x8515, 13, 0x203, EVENT_SCRIPT(Func_02000054)},
	{SCENE_EVENTS_END},
};
