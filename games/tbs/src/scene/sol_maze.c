

#include "unit/scene/sol_maze.h"
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
#include "fieldobj.h"

#include "field_event.h"

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)SolMaze_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)SolMaze_SceneTable1;
}

u8 *SceneData_GetActorTable(void)
{
	return (u8 *)SolMaze_SceneTable2;
}

u8 *SceneData_GetEffectTable(void)
{
	return (u8 *)SolMaze_SceneTable3;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/sol_initzoom.h"

struct SceneEntrance SolMaze_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 424, 0, 615, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 376, 0, 53, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 SolMaze_SceneTable1[] = {
	SCENE_EXITS(14),
	SCENE_EXIT(1, 16, 7),
	SCENE_EXIT(2, 16, 14),
	SCENE_EXITS_END,
};

struct ScenePlacement SolMaze_SceneTable2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent SolMaze_SceneTable3[] = {
	{ 0x31, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf02, FIND_ITEM(0xe1) },
	{ SCENE_EVENTS_END },
};
