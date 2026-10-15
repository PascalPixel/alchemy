

#include "unit/scene/goma_pond.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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

#include "resource_388_runtime.h"
#include "field_scene.h"
#include "field_obj.h"

#include "field_event.h"

u8 *SceneData_GetPrimaryTable(void)
{
	return (u8 *)GomaPond_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
	return (u8 *)GomaPond_SceneTable1;
}

u8 *SceneData_GetTertiaryTable(void)
{
	return (u8 *)GomaPond_SceneTable2;
}

u8 *SceneData_GetQuaternaryTable(void)
{
	return (u8 *)GomaPond_SceneTable3;
}

s32 Scene_PlaceActor8OnGate300(void)
{
	struct Resource388Runtime *work;

	work = (struct Resource388Runtime *)gEventWork;
	work->setupRequest1c0 = 0x204;
	work->setupValue1c8 = 0x18;
	if (Engine_GameFlagIsSet(0x300) != 0)
	{
		Actor_SetPosition(8, 0xD80000, 0x880000);
		Engine_ActorSetAnimation(8, 2);
		Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
		((struct Resource388SlotView *)Object_GetById(8))->unk23 = 2;
		((struct Resource388SlotView *)Object_GetById(8))->unk59 = 0;
		Map_CopyCellAttributes(0xB, 0x24, 5, 5, 0xB, 6);
	}
	return 0;
}

struct SceneEntrance GomaPond_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 120, 0, 88, FACING_SOUTH, 0, 24, 8, 504, 424, 0 },
	{ 2, CONDITION_ALWAYS, 424, 0, 344, FACING_SOUTH, 0, 24, 8, 504, 424, 0 },
	{ 3, CONDITION_ALWAYS, 408, 0, 136, FACING_SOUTH, 0, 24, 8, 504, 424, 0 },
	{ SCENE_TABLE_END },
};

u32 GomaPond_SceneTable1[] = {
	SCENE_EXITS(26),
	SCENE_EXIT(1, 27, 4),
	SCENE_EXIT(2, 27, 5),
	SCENE_EXIT(3, 27, 3),
	SCENE_EXITS_END,
};

struct ScenePlacement GomaPond_SceneTable2[] = {
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent GomaPond_SceneTable3[] = {
	{ 0x21, 5, CONDITION_ALWAYS, 1 },
	{ 0x21, 6, CONDITION_ALWAYS, 2 },
	{ 0x21, 7, CONDITION_ALWAYS, 3 },
	{ SCENE_EVENTS_END },
};
