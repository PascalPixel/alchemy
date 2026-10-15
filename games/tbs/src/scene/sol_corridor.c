

#include "unit/scene/sol_corridor.h"
#include "unit/scene/common/sol_initzoom.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"

#include "resource_37de_scene.h"
#include "field_scene.h"
#include "field_obj.h"

#include "field_event.h"

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)SolCorridor_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)SolCorridor_SceneTable1;
}

u8 *SceneData_GetActorTable(void)
{
	return (u8 *)SolCorridor_SceneTable2;
}

u8 *SceneData_GetEffectTable(void)
{
	return (u8 *)SolCorridor_SceneTable3;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/sol_initzoom.h"

struct SceneEntrance SolCorridor_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 727, 0, 92, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 56, 0, 616, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SolCorridor_SceneTable1[] = {
	SCENE_EXITS(15),
	SCENE_EXIT(1, 16, 5),
	SCENE_EXIT(2, 16, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement SolCorridor_SceneTable2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent SolCorridor_SceneTable3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};
