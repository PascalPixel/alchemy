

#include "unit/scene/goma_ike.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
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
#include "fieldobj.h"

s32 Engine_GameFlagIsSet(s32);

void Engine_ActorSetPosition(s32, s32, s32);

u8 Engine_ActorSetSpriteFlags(s32, s32);

u8 Engine_ActorSetAnimation(s32, s32);

u8 Map_CopyCellAttributes(s32, s32, s32, s32, s32, s32);

u8 *SceneData_GetPrimaryTable(void)
{
	return (u8 *)GomaIke_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
	return (u8 *)GomaIke_SceneTable1;
}

u8 *SceneData_GetTertiaryTable(void)
{
	return (u8 *)GomaIke_SceneTable2;
}

u8 *SceneData_GetQuaternaryTable(void)
{
	return (u8 *)GomaIke_SceneTable3;
}

s32 Scene_PlaceActor8OnGate300(void)
{
	struct Resource388Runtime *work;

	s32 arg5;
	s32 arg6;
	s32 pos_x = 0xD80000;
	s32 pos_z = 0x880000;

	work = (struct Resource388Runtime *)gEventWork;
	work->setup_request_1c0 = 0x204;
	work->setup_value_1c8 = 0x18;
	if (Engine_GameFlagIsSet(0x300) != 0)
	{
		Engine_ActorSetPosition(8, pos_x, pos_z);
		Engine_ActorSetAnimation(8, 2);
		Engine_ActorSetSpriteFlags((s32)Object_GetById(8), 0);
		((struct Resource388SlotView *)Object_GetById(8))->unknown_23 = 2;
		((struct Resource388SlotView *)Object_GetById(8))->unknown_59 = 0;
		/* FAKEMATCH: the 5th and 6th arguments go through locals; as literals in the direct call they differ 6 on tbs-en, and through Map_CopyCellAttributes they differ too */
		arg5 = 0xB;
		arg6 = 6;
		Engine_MapCopyCellAttributes(0xB, 0x24, 5, 5, arg5, arg6);
	}
	return 0;
}

struct SceneEntrance GomaIke_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 120, 0, 88, FACING_SOUTH, 0, 24, 8, 504, 424, 0 },
	{ 2, CONDITION_ALWAYS, 424, 0, 344, FACING_SOUTH, 0, 24, 8, 504, 424, 0 },
	{ 3, CONDITION_ALWAYS, 408, 0, 136, FACING_SOUTH, 0, 24, 8, 504, 424, 0 },
	{ SCENE_TABLE_END },
};

u32 GomaIke_SceneTable1[] = {
	SCENE_EXITS(26),
	SCENE_EXIT(1, 27, 4),
	SCENE_EXIT(2, 27, 5),
	SCENE_EXIT(3, 27, 3),
	SCENE_EXITS_END,
};

struct ScenePlacement GomaIke_SceneTable2[] = {
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent GomaIke_SceneTable3[] = {
	{ 0x21, 5, CONDITION_ALWAYS, 1 },
	{ 0x21, 6, CONDITION_ALWAYS, 2 },
	{ 0x21, 7, CONDITION_ALWAYS, 3 },
	{ SCENE_EVENTS_END },
};
