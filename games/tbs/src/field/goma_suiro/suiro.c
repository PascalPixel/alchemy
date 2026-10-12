#include "game_state.h"
#include "unit/field/goma_suiro/suiro.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/object.h"
#include "unit/field/common/script/field.h"
#include "fieldobj.h"
#include "types.h"
#include "configured_effect_spawn.h"
#include "field_event.h"
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
#include "field_scene.h"
#include "scene_ids.h"
#include "call.h"
#include "fixed_math.h"
#include "objcmd.h"

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
	work->sprite->priority = mode;
}

void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 0;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 14);
		Engine_ObjectSetBlendMode(object, 1);
		return object;
	}
	return 0;
}

void *OverlayObject_CreateConfiguredObject(s32 x, s32 y, s32 z, s32 type)
{
	struct FieldActor *object = Engine_ObjectCreate(type, x, y, z);

	if (object != 0)
	{
		object->sprite->priority = 1;
		object->motion_flags = 0;
		object->collision_flags = 8;
		Engine_ActorSetSpriteFlags(object, 0);
		ObjectGroup_SetChildValue(object, 15);
		object->priority_flags = (object->priority_flags & 0xfe) | 2;
		return object;
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/move.c"
#include "../common/effect/spawn.c"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	if (gGameState.scene == (s32)&SceneId_GomaSuiro2)
	{
		return gGomaSuiroEntrances2;
	}
	return gGomaSuiroEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return GomaSuiro_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_GomaSuiro2)
	{
		return gGomaSuiroPlacements2;
	}
	return gGomaSuiroPlacementsOther;
}

void FieldScene_RunActor8AtCell24Sequence(void)
{
	struct FieldActor *record;
	struct FieldActor *target;
	s32				   value;

	record = Object_GetById(8);
	value = record->x.fixed / 0x100000;
	if (value == 24)
	{
		SceneEffect_RunActorBurst(8);
		Object_GetById(8)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
		target = Object_GetById(8);
		Engine_ActorSetSpriteFlags((s32)target, 0);
		Engine_GameFlagSet(0x864);
	}
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_GomaSuiro2)
	{
		return gGomaSuiroEvents2;
	}
	return gGomaSuiroEventsOther;
}

s32 FieldScene_PlaceActor8OnEntry(void)
{
	struct FieldActor *record;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_GomaSuiro1)
	{
		if (gGameState.entrance == 5)
		{
			Engine_GameFlagClear(0x12f);
		}
		else
		{
			Object_GetById(8)->collision_flags |= 16;
			if (Engine_GameFlagIsSet(0x864) != 0)
			{
				Engine_ActorSetPosition(8, 0x15a0000, 0x1240000);
				record = Object_GetById(8);
				Engine_ActorSetSpriteFlags(record, 0);
				Object_GetById(8)->priority_flags |= 2;
				Engine_ActorSetAnimation(8, 2);
				Map_CopyCellAttributes(19, 74, 9, 3, 19, 17);
			}
		}
	}
	return 0;
}

void SceneEffect_RotateRecord(EffectWork_020004c4 *work)
{
	EffectRecord_020004c4 *record = work->record;

	record->angle -= 0x800;
}

void SceneActor_WaitObjectBelowHeight(struct FieldActor *object, s32 height)
{
	s32 frames = 60;

	while (frames != 0)
	{
		Engine_TaskWait(1);
		frames--;
		if (object->y.fixed <= height)
			break;
	}
}

union SceneActor
{
	struct
	{
		u32			   unk_00[2];
		s32			   x, y, z;
		u32			   unk_14[3];
		u8			   unk_20[2];
		u8			   field_22;
		u8			   unk_23[5];
		s32			   field_28;
		u32			   unk_2c[3];
		u32			   field_38;
		u32			   unk_3c[3];
		s32			   velocity_y;
		u32			   unk_4c;
		struct Sprite *sprite;
		u8			   unk_54;
		u8			   mode;
		u8			   unk_56[22];
		u32			   callback;
	} fields;
	u8 bytes[112];
};
struct Vector
{
	s32 x, y, z;
};

void Effect_Spawn();

void SceneEffect_RunActorBurst(s32 no)
{
	union SceneActor *work;
	u32				  cnt;
	struct Vector	  vec;
	union
	{
		struct ConfiguredEffectOptions fields;
		u8							   bytes[sizeof(struct ConfiguredEffectOptions)];
	} opt;

	work = (union SceneActor *)Object_GetById(no);
	work->fields.mode = 0;
	for (cnt = 0; cnt < 18; cnt++)
	{
		Engine_TaskWait(1);
		work->fields.sprite->angle -= 256;
		work->fields.x -= Trig_Cos(work->fields.sprite->angle) / 2;
		work->fields.field_38 = 0x80000000;
	}
	work->fields.callback = (u32)SceneEffect_RotateRecord;
	Actor_SetSpeed(no, 0x30000, 0x18000);
	Actor_SetDestination(no, 376, 288);
	work->fields.velocity_y = 0xcccc;
	work->fields.mode = 3;
	work->fields.field_22 = 0;
	Engine_ActorWaitForMove(no);
	SceneActor_WaitObjectBelowHeight(work, 0);
	Engine_AudioPlayCue(188);
	Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
	Engine_AudioPlayCue(141);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	for (cnt = 0; cnt < 17; cnt++)
	{
		vec.x = Trig_Cos(cnt << 12);
		vec.y = 0;
		vec.z = Trig_Sin(cnt << 12);
		vec.x -= vec.x / 4;
		vec.z -= vec.z / 2;
		Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, vec.x, vec.y, vec.z, 0, NULL);
	}
	work->fields.field_28 = 0x50000;
	Actor_SetDestination(no, 346, 292);
	Engine_ActorWaitForMove(no);
	SceneActor_WaitObjectBelowHeight(work, 0);
	work->fields.callback = 0;
	work->fields.sprite->angle = 0x1000;
	opt.fields.kind = 214;
	opt.fields.accum18 = 0x8000;
	opt.fields.accum1c = 0xcccc;
	opt.fields.target30 = 0x18000;
	opt.fields.target34 = 0x13333;
	Effect_Spawn(work->fields.x, work->fields.y, work->fields.z, 0, 0, 0, 0x1c0000, &opt.fields);
	Engine_AudioPlayCue(154);
	Engine_ActorSetAnimation(no, 3);
	Map_WaitWorkValuesBelow256();
}

s32 gEffectScript0[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript1[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

s32 gEffectScript2[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_WAIT(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_FREE,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

struct SceneEntrance gGomaSuiroEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 376, 0, 776, FACING_NORTH, 0, 88, 8, 664, 832, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 552, FACING_SOUTH, 0, 88, 8, 664, 832, 0 },
	{ 3, CONDITION_ALWAYS, 600, 0, 520, FACING_SOUTH, 0, 88, 8, 664, 832, 0 },
	{ 4, CONDITION_ALWAYS, 376, 0, 168, FACING_SOUTH, 0, 88, 8, 664, 832, 0 },
	{ 5, CONDITION_ALWAYS, 614, 0, 848, FACING_NORTH, 0, 480, 605, 720, 900, 0 },
	{ 99, CONDITION_ALWAYS, 424, 0, 280, FACING_WEST, 0, 88, 8, 664, 832, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gGomaSuiroEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 154, 0, 350, FACING_NORTH, 0, 16, 48, 632, 360, 0 },
	{ 2, CONDITION_ALWAYS, 552, 0, 312, FACING_NORTH, 0, 16, 48, 632, 360, 0 },
	{ SCENE_TABLE_END },
};

u32 GomaSuiro_Exits[] = {
	SCENE_EXITS(28),
	SCENE_EXIT(1, 25, 2),
	SCENE_EXIT(2, 27, 1),
	SCENE_EXIT(3, 27, 2),
	SCENE_EXIT(4, 29, 1),
	SCENE_EXIT(5, 5, 11),
	SCENE_EXIT(0, 29, 0),
	SCENE_EXIT(1, 28, 4),
	SCENE_EXIT(2, 2, 40),
	SCENE_EXITS_END,
};

struct ScenePlacement gGomaSuiroPlacementsOther[] = {
	{ 0xd4, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(400), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gGomaSuiroPlacements2[] = {
	{ SCENE_TABLE_END },
};

struct SceneEvent gGomaSuiroEventsOther[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 1 },
	{ 0x31, 6, CONDITION_ALWAYS, 2 },
	{ 0x31, 7, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 4 },
	{ SEARCH(SEARCH_CHEST), 100, 0xf28, FIND_ITEM(0xbf) },
	{ PSYNERGY_ON_ACTOR(0x8c), 8, 0x864, EVENT_SCRIPT(FieldScene_RunActor8AtCell24Sequence) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gGomaSuiroEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ SCENE_EVENTS_END },
};
