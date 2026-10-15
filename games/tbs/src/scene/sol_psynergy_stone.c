
#include "unit/scene/sol_psynergy_stone.h"
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

#include "resource_37c_runtime.h"
#include "field_scene.h"

#include "field_event.h"

u8 *State_Run(void)
{
	return (u8 *)gSolPsynergyStone_SceneTable0;
}

s32 SolPsynergyStone_GetView(void)
{
	return 0;
}

u8 *SceneData_GetPrimaryTable37c(void)
{
	return (u8 *)gSolPsynergyStone_SceneTable1;
}

u8 *SceneData_GetSecondaryTable37c(void)
{
	return (u8 *)gSolPsynergyStone_SceneTable2;
}

u8 *SceneData_GetTertiaryTable37c(void)
{
	return (u8 *)gSolPsynergyStone_SceneTable3;
}

s32 Resource37c_Run(void)
{

	s32 scaleX = 0x10000;
	s32 scaleY = 0x10000;
	s32 scaleZ = 0x10000;

	RESOURCE37C_RUNTIME->setupRequest1c0 = 0x204;
	Engine_GameFlagSet(0x144);
	if (Engine_GameFlagIsSet(0x814) != 0)
	{
		Resource37c_QueueSoundCue(0x8D);
		Engine_WorkSetValuesIfNonNegative(scaleX, scaleY, scaleZ);
		Resource37c_FinalizeSetup();
	}
	return 0;
}

struct SceneEntrance gSolPsynergyStone_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 87, 0, 142, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gSolPsynergyStone_SceneTable1[] = {
	SCENE_EXITS(13),
	SCENE_EXIT(1, 12, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement gSolPsynergyStone_SceneTable2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent gSolPsynergyStone_SceneTable3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ SEARCH(SEARCH_UNNAMED), 100, 0x350, FIND_PSYNERGY_STONE },
	{ SCENE_EVENTS_END },
};
