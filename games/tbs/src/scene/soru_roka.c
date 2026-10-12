

#include "unit/scene/soru_roka.h"
#include "unit/scene/common/soru/initzoom.h"
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
#include "fieldobj.h"

s32 Engine_GameFlagIsSet(s32);

void Engine_WorkSetValuesIfNonNegative(s32, s32, s32);

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)SoruRoka_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)SoruRoka_SceneTable1;
}

u8 *SceneData_GetActorTable(void)
{
	return (u8 *)SoruRoka_SceneTable2;
}

u8 *SceneData_GetEffectTable(void)
{
	return (u8 *)SoruRoka_SceneTable3;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/soru/initzoom.c"

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
