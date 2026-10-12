#include "battle_effect_runtime.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/korima_kawa/kawa.h"
#include "unit/field/common/korima/palette.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mapcopy.h"
#include "types.h"
#include "tbs_edition.h"

#include "staged_actor.h"
#include "staged_actor_effect.h"
#include "resource_393.h"
#include "fieldobj.h"
#include "game_state.h"
#include "field_event.h"
#include "field_scene.h"

#ifdef MODERN
// called before its definition below, which modern GCC refuses for an
// implicit int declaration of a u16 function
#endif

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"

u8 *MapStagedScene_SelectPrimaryData(void)
{
	return Data_02008fc8;
}

s32 MapStagedScene_GetEmptyData(void)
{
	return 0;
}

u8 *MapStagedScene_SelectSecondaryData(void)
{
	return Data_02009028;
}

u8 *MapStagedScene_SelectTertiaryData(void)
{
	return Data_02009038;
}

static void EV10_SCRIPT(void)
{
	struct StagedActorProbe result;
	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&result))
	{
		SceneActor_MoveAndRedraw(result);
		if (result.actor_slot == 10 && (result.position_x >> 20) == 12)
		{
			struct FieldActor *actor;
			Engine_ActorSetAnimation(10, 3);
			Engine_ActorSetDestinationOffset(10, -18, 6);
			Engine_EventWait(30);
			Engine_AudioPlayCue(240);
			Engine_ActorSetAnimation(10, 8);
			Object_GetById(10)->priority_flags = 2;
			Map_CopyCellAttributes(32, 20, 2, 4, 11, 16);
			StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
			Engine_GameFlagSet(0x201);
			actor = Object_GetById(10);
			Engine_ActorSetSpriteFlags(actor, 0);
		}
	}
	Engine_EventEnd();
}

static s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{

	struct StagedActorEffect *actor = (void *)Object_GetById(0);
	u8						 *flags = &actor->motion_flags;
	u8						  saved = *flags;
	s32						  result = Object_CheckMovementCollision(actor, request);

	if (result == 0)
	{
		Engine_EventBegin();
		Object_SetMode((struct FieldActor *)actor, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode((struct FieldActor *)actor, 7);
		actor->move_rate_x = 0x30000;
		actor->move_rate_z = 0x20000;
		actor->elevation_rate = 0x40000;
		*flags &= 0x7e;
		Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
		ObjectMotion_SetPositionAndCommit(0, request->cell_x, request->cell_z);
		Object_SetMode((struct FieldActor *)actor, 6);
		Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 1);
		*flags = result;
		Engine_ActorSetAnimation(10, 7);
		actor->position_x += 0xffff0000;
		actor->position_z += 0xffff0000;
		Engine_TaskWait(2);
		actor->position_x += 0xffff0000;
		actor->position_z += 0xffff0000;
		Engine_TaskWait(10);
		actor->position_x += 0x10000;
		actor->position_z += 0x10000;
		Engine_TaskWait(4);
		actor->position_x += 0x10000;
		actor->position_z += 0x10000;
		*flags = saved;
		Engine_EventEnd();
		return 1;
	}
	return 0;
}

static void EV11_TOUCH(void)
{
	struct Resource393Position pos;
#if !EDITION_INTERNATIONAL
	pos.x = ((struct Resource393Object *)Object_GetById(0))->position_x + 0x1e0000;
	pos.y = ((struct Resource393Object *)Object_GetById(0))->position_y;
	pos.z = ((struct Resource393Object *)Object_GetById(0))->position_z;
#else
	struct Resource393Object *obj = (void *)Object_GetById(gGameState.selected_actor);
	u32						  xb = obj->position_x & 0xfff00000;

	pos.x = xb + 0x80000;
	pos.y = obj->position_y;
	pos.z = (obj->position_z & 0xfff00000) + 0x80000;
	pos.x = xb + 0x280000;
#endif
	StagedActor_RunStepEffect(&pos);
}

u8 *KorimaKawa_GetEvents(void)
{
	return Data_02009098;
}

s32 KorimaKawa_PrepareWindowTransition(void)
{
	struct EventWork *work = gEventWork;

	work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	FieldScene_RedrawActorFootprint(10);

	if (Engine_GameFlagIsSet(0x201) != 0)
	{
		Object_GetById(10)->priority_flags = 2;
		Map_CopyCellAttributes(32, 20, 2, 4, 11, 16);
		StagedActor_FillGridAttributeRectangle(2, 12, 16, 1, 4, 0);
		Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
	}

	FieldScene_RedrawActorFootprint(8);
	FieldScene_RedrawActorFootprint(9);

	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		SceneEffect_AdjustPaletteColors(6);
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/korima/palette.c"

// 向きごとの一歩: 上位16ビットがX、下位16ビットがZ (ピクセル)
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 StagedActor_DirectionSteps[] = {
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(16, 0),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(0, 16),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(-16, 0),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(0, -16),
	DIRECTION_STEP(16, 0),
};

u32 StagedActor_FootprintKinds[] = {
	207, 205, 228, 229, 298, 297,
};

s32 StagedActor_FootprintBounds[] = {
	-32, -8, 32, 8,
	-8, -32, 8, 32,
	-32, -16, 32, 0,
	-8, -32, 8, 32,
	-32, -8, 32, 8,
	-8, -32, 8, 32,
};

struct SceneEntrance Data_02008fc8[] = {
	{ 0, CONDITION_ALWAYS, 209, 0, 279, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 320, 0, 488, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 480, 0, 104, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Data_02009028[] = {
	SCENE_EXITS(42),
	SCENE_EXIT(1, 43, 1),
	SCENE_EXIT(2, 41, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement Data_02009038[] = {
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(256), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(224), 0, PIXELS(280), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent Data_02009098[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_SCRIPT) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TOUCH) },
	{ SCENE_EVENTS_END },
};

// .bss 内の並びは最初に宣言した順 (field.h の extern)
u8 KorimaPalette_First[1792] __attribute__((section(".bss")));
u8 KorimaPalette_Second[896] __attribute__((section(".bss")));
