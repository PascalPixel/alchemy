
#include "unit/scene/soru_dou.h"
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

s32 Engine_GameFlagIsSet(s32);

void Engine_WorkSetValuesIfNonNegative(s32, s32, s32);

void Engine_GameFlagSet(s32);

u8 *State_Run(void)
{
	return (u8 *)SoruDou_SceneTable0;
}

s32 SoruDou_GetView(void)
{
	return 0;
}

u8 *SceneData_GetPrimaryTable37c(void)
{
	return (u8 *)SoruDou_SceneTable1;
}

u8 *SceneData_GetSecondaryTable37c(void)
{
	return (u8 *)SoruDou_SceneTable2;
}

u8 *SceneData_GetTertiaryTable37c(void)
{
	return (u8 *)SoruDou_SceneTable3;
}

s32 Resource37c_Run(void)
{

	s32 scale_x = 0x10000;
	s32 scale_y = 0x10000;
	s32 scale_z = 0x10000;

	RESOURCE37C_RUNTIME->setup_request_1c0 = 0x204;
	Engine_GameFlagSet(0x144);
	if (Engine_GameFlagIsSet(0x814) != 0)
	{
		Resource37c_QueueSoundCue(0x8D);
		Engine_WorkSetValuesIfNonNegative(scale_x, scale_y, scale_z);
		Resource37c_FinalizeSetup();
	}
	return 0;
}

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
