#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/scene/haidia_dou.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/objtable.h"
#include "unit/battle/effect/marker.h"
#include "unit/battle/effect/sparkle.h"
#include "unit/battle/effect/evbasic.h"
#include "unit/battle/effect/evactor.h"
#include "unit/battle/effect/evcamera.h"
#include "unit/battle/effect/djinnev.h"
#include "unit/battle/effect/psydisp.h"
#include "unit/battle/effect/psyfx.h"
#include "unit/system/scheduler/math.h"

#include "fieldobj.h"
#include "mapcopy.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "staged_actor.h"
#include "select_overlay_data_by_runtime_selector.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "field_effect.h"
#include "scene_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y);
static void HaidiaDou_ApplyEntryState(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../field/common/effect/create.c"
#include "../field/common/effect/move.c"
#include "../field/common/effect/spawn.c"
#include "../field/common/object/staged_actor.c"

static void EV08_SCRIPT(void)
{
	BattleFx_RunPageEffectForSlot(8, 3, 4);
}

static void SceneActor_WaitActorDescent(struct FieldActor *actor)
{
	s32 frames = 60;

	while (frames != 0)
	{
		Engine_TaskWait(1);
		frames--;
		if (actor->y.fixed <= actor->terrain_height)
			break;
	}
	actor->y.fixed = actor->terrain_height;
}

static s32 SceneActor_FaceActorZero(struct FieldActor *obj)
{
	struct FieldActor *target = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   dz = target->z.fixed - obj->z.fixed;
	s32				   dx = target->x.fixed - obj->x.fixed;

	obj->facing = ArcTan2(dz, dx);
	return 0;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_HaidiaDou1)
	{
		return gHaidiaDouEntrances1;
	}
	if (scene == (s32)&SceneId_HaidiaDou2)
	{
		return gHaidiaDouEntrances2;
	}
	if (scene == (s32)&SceneId_HaidiaDou3)
	{
		return gHaidiaDouEntrances3;
	}
	return gHaidiaDouEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return HaidiaDou_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_HaidiaDou1)
	{
		return gHaidiaDouPlacements1;
	}
	if (scene == (s32)&SceneId_HaidiaDou2)
	{
		return gHaidiaDouPlacements2;
	}
	if (scene == (s32)&SceneId_HaidiaDou3)
	{
		return gHaidiaDouPlacements3;
	}
	return gHaidiaDouPlacements1;
}

static void FieldScene_ConfigureRegionAtRow15(void)
{
	Engine_MapCopyCells(16, 15, 1, 1, 15, 15);
}

static void FieldScene_ConfigureRegionAtRow17(void)
{
	Engine_MapCopyCells(16, 17, 1, 1, 15, 15);
}

static void EV09_PSYNERGY(void)
{
	struct EffectOptions  options;
	struct EffectOptions *o;
	struct FieldActor	 *left;
	struct FieldActor	 *right;
	u32					  i;

	Engine_EventBegin();
	if (Object_GetById(9)->x.fixed >> 20 == 23)
	{
		Actor_MoveToAndWait(0, 360, 664);
		Engine_ActorFaceDirection(0, 0xe000, 10);
		Object_GetById(9)->x.fixed += 0x20000;
		left = OverlayObject_PrepareObject(
			Object_GetById(9)->x.fixed, 0, Object_GetById(9)->z.fixed + 0x340000, 241);
		right = OverlayObject_PrepareObject(
			Object_GetById(9)->x.fixed + 0x100000, 0, Object_GetById(9)->z.fixed + 0x340000, 241);
		Object_GetById(9)->motion_flags = 0;
		o = &options;
		o->start_scale_x = 0x9999;
		o->start_scale_y = 0x9999;
		o->palette = 7;
		Engine_AudioPlayCue(216);
		for (i = 0; i < 68; i++)
		{
			s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1700000;
			s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2700000;

			Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
			Object_GetById(9)->y.fixed -= 0x8000;
			Engine_EventWait(1);
		}
		Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
		Object_GetById(9)->priority_flags |= 2;
		Engine_GameFlagSet(0x200);
		Object_GetById(9)->y.fixed = -0x80000;
		Engine_ActorSetAnimation(9, 2);
		Engine_ObjectDispatchRelease(left);
		Engine_ObjectDispatchRelease(right);
		Engine_EventWait(30);
	}
	Engine_EventEnd();
}

static void EV10_PSYNERGY(void)
{
	struct EffectOptions  options;
	struct EffectOptions *o;
	struct FieldActor	 *left;
	struct FieldActor	 *right;
	u32					  i;

	Engine_EventBegin();
	if (Object_GetById(10)->x.fixed >> 20 == 27)
	{
		left = OverlayObject_PrepareObject(
			Object_GetById(10)->x.fixed - 0x80000, 0, Object_GetById(10)->z.fixed + 0x340000, 241);
		right = OverlayObject_PrepareObject(
			Object_GetById(10)->x.fixed + 0x80000, 0, Object_GetById(10)->z.fixed + 0x340000, 241);
		Object_GetById(9)->motion_flags = 0;
		o = &options;
		o->start_scale_x = 0x9999;
		o->start_scale_y = 0x9999;
		o->palette = 7;
		Engine_AudioPlayCue(216);
		for (i = 0; i < 68; i++)
		{
			s32 x = (((u32)(Engine_RandomNext() * 17) >> 16) << 16) + 0x1b00000;
			s32 z = (((u32)(Engine_RandomNext() * 14) >> 16) << 16) + 0x2900000;

			Effect_Spawn(x, 0, z, 0, 0, 0, 0x90000, o);
			Object_GetById(10)->y.fixed -= 0x8000;
			Engine_EventWait(1);
		}
		Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
		Object_GetById(10)->priority_flags |= 2;
		Engine_GameFlagSet(0x201);
		Object_GetById(10)->y.fixed = -0x80000;
		Engine_ActorSetAnimation(10, 2);
		Engine_ObjectDispatchRelease(left);
		Engine_ObjectDispatchRelease(right);
		Engine_EventWait(30);
	}
	Engine_EventEnd();
}

static void EV24_SCRIPT(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	EV10_PSYNERGY();
}

static s32 FieldScene_RunPrimarySequence(s32 a0)
{

	union FieldCoordinate to[3];
	struct FieldActor	 *leader;
	u8					 *flags;
	s32					  saved;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	flags = &leader->motion_flags;
	saved = *flags;
#if EDITION_INTERNATIONAL
	to[0].fixed = (leader->x.fixed & -0x100000) + 0x80000;
	to[1].fixed = leader->y.fixed;
	to[2].fixed = (leader->z.fixed & -0x100000) + 0x280000;
#else
	to[0].fixed = leader->x.fixed;
	to[1].fixed = leader->y.fixed;
	to[2].fixed = leader->z.fixed + 0x200000;
#endif
	if (Object_CheckMovementCollision(leader, to) == 0)
	{
		Engine_EventBegin();
		Object_SetMode(leader, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode(leader, 7);
		leader->speed = 0x30000;
		leader->acceleration = 0x20000;
		leader->velocity_y = 0x40000;
		*flags &= 126;
		Engine_ActorSetSpriteFlags(leader, 0);
		Engine_ActorMoveToAndWait(0, to[0].part.pixel, to[2].part.pixel);
		Object_SetMode(leader, 6);
		Engine_ActorSetSpriteFlags(leader, 1);
		*flags = saved;
		Engine_EventEnd();
		return 1;
	}
	return 0;
}

static void EV40_TOUCH(void)
{

	struct FieldActor *record;

	if (GameFlag_IsSet(0x200) == 0)
	{
		GameFlag_Set(0x200);
		Engine_EventBegin();
		Camera_SetSpeed(0x10000, 0x2000);
		Engine_CameraFollowActor(8, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(60);
		Actor_FaceDirection(8, 0xc000, 20);
		Actor_SetAttachedEffect(8, 0x102);
		Engine_ActorRunRepeatedMotion(8, 2);
		Engine_EventWait(20);
		Actor_SetSpeed(8, 0x10000, 0x8000);
		Actor_WalkToAndWait(8, 0x318, 248);
		Engine_AudioPlayCue(152);
		record = Actor_Get(8);
		record->velocity_y = 0x80000;
		Actor_WalkToAndWait(8, 0x318, 0x118);
		Engine_EventWait(20);
		Actor_FaceDirection(8, 0xc000, 20);
		Engine_EventWait(30);
		Engine_EventEnd();
	}
}

static void EV41_TOUCH(void)
{

	struct FieldActor *record;

	if (GameFlag_IsSet(0x200) != 0)
	{
		if (GameFlag_IsSet(0x201) == 0)
		{
			GameFlag_Set(0x201);
			GameFlag_Set(0x302);
			Engine_EventBegin();
			Actor_SetAttachedEffect(8, 0x102);
			Engine_ActorRunRepeatedMotion(8, 2);
			Engine_EventWait(20);
			Actor_SetSpeed(8, 0x20000, 0x10000);
			Actor_WalkToAndWait(8, 0x2f8, 0x118);
			Actor_WalkToAndWait(8, 0x2f8, 0x138);
			Actor_WalkToAndWait(8, 0x318, 0x138);
			Engine_EventWait(10);
			Actor_FaceDirection(8, 0xc000, 20);
			record = Actor_Get(8);
			record->update = (void (*)(union FieldObject *))SceneActor_FaceActorZero;
			Engine_EventEnd();
		}
	}
}

static void FieldScene_RunScene3a6SequenceC(void)
{

	struct FieldActor *rec8;
	struct FieldActor *record;
	s32				   idx;
	s32				   tbl;
	s32				   idx4;
	s32				   off24a;
	struct EventWork  *p5;

	p5 = gEventWork;
	if (Engine_GameFlagIsSet(0x302) != 0)
	{
		off24a = 0x24a;
		if (*(s16 *)((s32)&gGameState + off24a) != 8)
		{
			idx = p5->touched_trigger;
			rec8 = Object_GetById(8);
			record = Object_GetById(ACTOR_PARTY_LEADER);
			rec8->speed = record->speed;
			rec8 = Object_GetById(8);
			record = Object_GetById(ACTOR_PARTY_LEADER);
			rec8->acceleration = record->acceleration;
			idx -= 45;
			tbl = (s32)HaidiaDou_WalkTargets;
			idx <<= 3;
			idx4 = idx + 4;
			Engine_ActorWalkTo(8, *(s32 *)(tbl + idx), *(s32 *)(tbl + idx4));
		}
	}
}

static void EV08_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(8, 0);
	Engine_EventEnd();
}

static void EV20_SCRIPT(void)
{
	struct FieldActor	   *record;
	s32						v6;
	s32						v5;
	s32						two;
	struct StagedActorProbe probe;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		SceneActor_MoveAndRedraw(probe);
		if ((probe.position_x >> 20) == 17)
		{
			Engine_ActorSetAnimation(probe.actor_slot, 3);
			v6 = 0;
			Object_GetById(probe.actor_slot)->motion_flags = v6;
			record = Object_GetById(probe.actor_slot);
			record->bounce = v6;
			Engine_ActorSetDestinationOffset(probe.actor_slot, -12, 0);
			Engine_ActorWaitForMove(probe.actor_slot);
			Engine_ActorSetAnimation(probe.actor_slot, 3);
			Engine_ActorSetSpritePriority(10, 3);
			Object_GetById(probe.actor_slot)->motion_flags = 3;
			Engine_ActorSetDestinationOffset(probe.actor_slot, -6, 0);
			SceneActor_WaitActorDescent(Object_GetById(probe.actor_slot));
			Engine_ActorSetAnimation(probe.actor_slot, 8);
			{
				struct FieldActor *obj = Object_GetById(probe.actor_slot);

				two = 2;
				obj->priority_flags = two;
			}
			v5 = 4;
			StagedActor_FillGridAttributeRectangle(
				0, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
			StagedActor_FillGridAttributeRectangle(
				2, (probe.position_x >> 20), ((probe.position_z >> 20) - 2), 1, v5, v6);
			StagedActor_FillGridAttributeRectangle(2, 16, 18, 1, two, v6);
			StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, v5, v6);
			Engine_GameFlagSet(0x203);
			Engine_AudioPlayCue(240);
		}
	}
	Engine_EventEnd();
}

static void EV21_TOUCH_3(void)
{

	struct FieldActor *record;
	s32				   zero;

	Engine_EventBegin();
	Engine_EventWait(10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x1999);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
	Engine_EventWait(15);
	Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 8, 0);
	Engine_EventWait(4);
	Event_PlayCue(0x120);
	Event_PlayCue(239);
	Actor_SetSpeed(9, 0x8000, 0x1999);
	Engine_ActorSetAnimation(9, 2);
	zero = 0;
	Object_GetById(9)->motion_flags = zero;
	record = Actor_Get(9);
	record->bounce = zero;
	Engine_ActorSetDestinationOffset(9, 12, 0);
	Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_ActorWaitForMove(9);
	Event_PlayCue(0x120);
	Event_PlayCue(213);
	Engine_ActorSetAnimation(9, 3);
	Object_GetById(9)->motion_flags = 3;
	Engine_ActorSetDestinationOffset(9, 6, 0);
	SceneActor_WaitActorDescent(Actor_Get(9));
	Engine_ActorSetAnimation(9, 8);
	Engine_ActorSetSpritePriority(9, 3);
	Object_GetById(9)->priority_flags = 2;
	StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
	StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
	GameFlag_Set(0x202);
	Event_PlayCue(240);
	Engine_EventEnd();
}

static void EV22_SCRIPT(void)
{
	struct StagedActorProbe probe;

	Engine_EventBegin();
	if (StagedActor_FindClearPosition(&probe) != 0)
	{
		SceneActor_MoveAndRedraw(probe);
		Engine_ActorSetAnimation(11, 3);
		Actor_SetSpeed(11, 0x4000, 0x8000);
		Engine_ActorSetDestinationOffset(11, 0, -16);
		Engine_EventWait(45);
		Engine_AudioPlayCue(240);
		Engine_ActorSetAnimation(11, 8);
		{
			struct FieldActor *obj = Object_GetById(11);
			s32				   two = 2;
			s32				   zero = 0;

			obj->priority_flags = two;
			StagedActor_FillGridAttributeRectangle(
				0, 13, (probe.position_z >> 20) - 1, 4, two, zero);
		}
		if ((probe.position_z >> 20) == 20)
		{
			Engine_GameFlagSet(0x205);
		}
		else
		{
			Engine_GameFlagSet(0x204);
			{
				s32 column = 14;

				Engine_MapCopyCellAttributes(14, 17, 2, 1, column, 16);
				Engine_MapCopyCellAttributes(14, 13, 1, 1, column, 15);
			}
		}
	}
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_HaidiaDou1)
	{
		return gHaidiaDouEvents1;
	}
	if (scene == (s32)&SceneId_HaidiaDou2)
	{
		return gHaidiaDouEvents2;
	}
	if (scene == (s32)&SceneId_HaidiaDou3)
	{
		return gHaidiaDouEvents3;
	}
	return gHaidiaDouEventsOther;
}

s32 HaidiaDou_RunSceneScript(void)
{
	s32 *request = &gEventWork->start_transition;

	*request = 0x204;
	if (gGameState.scene == (s32)&SceneId_HaidiaDou1)
	{
		*request = 0x100;
		Engine_TaskWait(1);
		Engine_ActorSetSpritePriority(11, 3);
		Engine_ActorSetSpritePriority(12, 3);
		Engine_GameFlagClear(0x12f);
	}
	HaidiaDou_ApplyEntryState();
	return 0;
}

static void HaidiaDou_PlayCueAndRequestTouchedExit(void)
{
	s32 val = gEventWork->touched_trigger;

	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(val);
}

static void DialogueLayout_ConfigureRowsByFlag301(void)
{
	Engine_MapCopyCellAttributes(0, 34, 13, 3, 23, 34);

	if (Engine_GameFlagIsSet(0x301) != 0)
	{
		SceneActor_PlaceAtTile(11, 35, 35);
		Engine_MapCopyCellAttributes(24, 34, 1, 3, 23, 34);
	}
	else
	{
		SceneActor_PlaceAtTile(11, 23, 35);
		Engine_MapCopyCellAttributes(24, 34, 1, 3, 35, 34);
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../field/common/object/pairmove.c"

static void EV20_TOUCH(void)
{

	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(11, 112, 0);
	SceneActor_PlacePairAtOffset(11, 80, 0);
	Engine_GameFlagSet(0x301);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureRowsByFlag301();
	Engine_AudioPlayCue(0x121);
}

static void FieldScene_RunActor11Transition301(void)
{

	Engine_AudioPlayCue(241);
	SceneActor_PlacePairAtOffset(11, -112, 0);
	SceneActor_PlacePairAtOffset(11, -80, 0);
	Engine_GameFlagClear(0x301);
	Engine_TaskWait(2);
	DialogueLayout_ConfigureRowsByFlag301();
	Engine_AudioPlayCue(0x121);
}

static void SceneActor_PlaceAtTile(s32 id, s32 x, s32 y)
{
	struct FieldActor *rec = Object_GetById(id);

	if (rec != 0)
	{
		Engine_ActorSetSpritePriority(id, 3);
		rec->terrain_id = 2;
		rec->priority_flags |= 2;
		rec->x.fixed = (x << 20) + 0x80000;
		rec->z.fixed = (y << 20) + 0x80000;
	}
}

static void HaidiaDou_ApplyEntryState(void)
{
	s32 flag;

	if (gGameState.scene == (s32)&SceneId_HaidiaDou2)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
		case 3:
		case 4:
			Engine_ActorSetSpritePriority(15, 3);
			Engine_ActorSetSpritePriority(13, 3);
			OverlayObject_CreateConfiguredObject(0x780000, 0, 0xe80000, 223);
			break;
		case 5:
		case 6:
		case 7:
			if (Engine_GameFlagIsSet(0x70) != 0)
			{
				break;
			}
			if (Engine_GameFlagIsSet(0x302) == 0)
			{
				break;
			}
			Engine_GameFlagSet(0x200);
			if (gGameState.entrance == 5)
			{
				Engine_GameFlagSet(0x201);
			}
			Engine_TaskWait(1);
			if (Engine_GameFlagIsSet(0x109) != 0)
			{
				break;
			}
			Actor_SetPosition(8, 0x3180000, 0x1180000);
			Object_GetById(8)->update = SceneActor_FaceActorZero;
			break;
		case 8:
		case 9:
		case 10:
			OverlayObject_CreateConfiguredObject(0x2820000, 0, 0x2280000, 20);
			Map_CopyCellAttributes(23, 34, 13, 3, 0, 34);
			DialogueLayout_ConfigureRowsByFlag301();
			if (Engine_GameFlagIsSet(0x200) != 0)
			{
				Map_CopyCellAttributes(23, 41, 1, 1, 23, 39);
			}
			if (Engine_GameFlagIsSet(0x201) != 0)
			{
				Map_CopyCellAttributes(31, 39, 2, 1, 27, 41);
			}
			break;
		}
	}
	else if (gGameState.scene == (s32)&SceneId_HaidiaDou3)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
		case 3:
			if (Engine_GameFlagIsSet(0x202) != 0)
			{
				StagedActor_FillGridAttributeRectangle(0, 12, 16, 1, 4, 0);
				StagedActor_FillGridAttributeRectangle(0, 13, 16, 1, 4, 0);
			}
			else
			{
				FieldScene_RedrawActorFootprint(9);
			}
			if (Engine_GameFlagIsSet(0x203) != 0)
			{
				StagedActor_FillGridAttributeRectangle(2, 16, 16, 1, 4, 0);
				StagedActor_FillGridAttributeRectangle(0, 16, 16, 1, 4, 0);
			}
			else
			{
				FieldScene_RedrawActorFootprint(10);
			}
			flag = Engine_GameFlagIsSet(0x205);
			if (flag != 0)
			{
				StagedActor_FillGridAttributeRectangle(0, 13, 19, 4, 2, 0);
			}
			else if (Engine_GameFlagIsSet(0x204) != 0)
			{
				StagedActor_FillGridAttributeRectangle(0, 13, 15, 4, 2, flag);
				Engine_MapCopyCellAttributes(14, 17, 2, 1, 14, 16);
				Engine_MapCopyCellAttributes(14, 13, 1, 1, 14, 15);
			}
			else
			{
				FieldScene_RedrawActorFootprint(11);
				Engine_ActorSetSpritePriority(11, 3);
			}
			break;
		}
	}
}

static void ActorPresentation_AdvanceActorElevenStates(void)
{
	Engine_ActorSetAnimation(11, 1);
	Engine_ActorSetAnimation(11, 2);
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

s32 HaidiaDou_WalkTargets[] = {
	0x2f8, 0x128,
	0x2f8, 0x118,
	0x308, 0x118,
	0x318, 0x118,
	0x328, 0x118,
	0x338, 0x118,
	0x338, 0x128,
	0x338, 0x138,
	0x328, 0x138,
	0x318, 0x138,
	0x308, 0x138,
	0x2f8, 0x138,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

struct SceneEntrance gHaidiaDouEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gHaidiaDouEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
	{ 1, CONDITION_ALWAYS, 104, 0, 344, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
	{ 2, CONDITION_ALWAYS, 136, 0, 184, FACING_SOUTH, 0, 16, 0, 336, 368, 0 },
	{ 3, CONDITION_ALWAYS, 136, 0, 376, FACING_NORTH, 0, 16, 0, 336, 368, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gHaidiaDouEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 120, 0, 152, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 328, 0, 392, FACING_NORTH, 0, 48, 32, 544, 416, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 160, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 120, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ 4, CONDITION_ALWAYS, 504, 0, 136, FACING_SOUTH, 0, 48, 32, 544, 416, 0 },
	{ 5, CONDITION_ALWAYS, 792, 0, 360, FACING_NORTH, 0, 624, 16, 944, 384, 0 },
	{ 6, CONDITION_ALWAYS, 680, 0, 248, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
	{ 7, CONDITION_ALWAYS, 904, 0, 232, FACING_SOUTH, 0, 624, 16, 944, 384, 0 },
	{ 8, CONDITION_ALWAYS, 408, 0, 872, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
	{ 9, CONDITION_ALWAYS, 488, 0, 744, FACING_NORTH, 0, 312, 472, 704, 896, 0 },
	{ 10, CONDITION_ALWAYS, 632, 0, 552, FACING_SOUTH, 0, 312, 472, 704, 896, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gHaidiaDouEntrances3[] = {
	{ 0, CONDITION_ALWAYS, 200, 0, 432, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 200, 0, 432, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
	{ 2, CONDITION_ALWAYS, 344, 0, 120, FACING_SOUTH, 0, 64, 24, 416, 464, 0 },
	{ 3, CONDITION_ALWAYS, 120, 0, 264, FACING_NORTH, 0, 64, 24, 416, 464, 0 },
	{ 4, CONDITION_ALWAYS, 536, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
	{ 5, CONDITION_ALWAYS, 840, 0, 776, FACING_NORTH, 0, 400, 440, 896, 800, 0 },
	{ SCENE_TABLE_END },
};

u32 HaidiaDou_Exits[] = {
	SCENE_EXITS(93),
	SCENE_EXIT(1, 6, 4),
	SCENE_EXIT(2, 94, 1),
	SCENE_EXIT(3, 6, 6),
	SCENE_EXIT(0, 94, 0),
	SCENE_EXIT(1, 93, 2),
	SCENE_EXIT(2, 94, 5),
	SCENE_EXIT(3, 94, 8),
	SCENE_EXIT(4, 95, 1),
	SCENE_EXIT(5, 94, 2),
	SCENE_EXIT(6, 94, 9),
	SCENE_EXIT(7, 95, 3),
	SCENE_EXIT(8, 94, 3),
	SCENE_EXIT(9, 94, 6),
	SCENE_EXIT(10, 95, 4),
	SCENE_EXIT(0, 95, 0),
	SCENE_EXIT(1, 94, 4),
	SCENE_EXIT(2, 95, 5),
	SCENE_EXIT(3, 94, 7),
	SCENE_EXIT(4, 94, 10),
	SCENE_EXIT(5, 95, 2),
	SCENE_EXITS_END,
};

struct ScenePlacement gHaidiaDouPlacements1[] = {
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(168), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(232), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(192), 0, PIXELS(184), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(208), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaDouPlacements2[] = {
	{ 0x5d, 0x70, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(344), 0, PIXELS(632), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(456), 0, PIXELS(664), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xed, CONDITION_ALWAYS, 7,
		PIXELS(376), 0, PIXELS(576), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(422), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xf1, CONDITION_ALWAYS, 7,
		PIXELS(440), 0, PIXELS(206), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(384), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(400), 0, PIXELS(216), FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gHaidiaDouPlacements3[] = {
	{ 0x5d, 0x70, ACTOR_STAND,
		PIXELS(792), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(200), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcd, CONDITION_ALWAYS, 7,
		PIXELS(312), 0, PIXELS(288), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xcf, CONDITION_ALWAYS, 7,
		PIXELS(240), 0, PIXELS(360), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gHaidiaDouEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaDouEvents1[] = {
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_PlayCueAndRequestTouchedExit) },
	{ EVENT_TOUCH, 3, CONDITION_ALWAYS, EVENT_SCRIPT(HaidiaDou_PlayCueAndRequestTouchedExit) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaDouEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ TOUCH_WALKING_INTO(FACING_EAST), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor11Transition301) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunPrimarySequence) },
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorElevenStates) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_AdvanceActorElevenStates) },
	{ 0x202, 24, 0x201, EVENT_SCRIPT(EV24_SCRIPT) },
	{ EVENT_TOUCH, 40, 0x70, EVENT_SCRIPT(EV40_TOUCH) },
	{ EVENT_TOUCH, 41, 0x70, EVENT_SCRIPT(EV41_TOUCH) },
	{ EVENT_TOUCH, 45, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 46, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 47, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 48, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 49, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 50, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 51, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 52, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 53, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 54, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 55, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ EVENT_TOUCH, 56, 0x70, EVENT_SCRIPT(FieldScene_RunScene3a6SequenceC) },
	{ 0x7, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_SCRIPT) },
	{ SEARCH(SEARCH_CHEST), 100, 0xfc2, FIND_ITEM(0xcc) },
	{ SEARCH(SEARCH_CHEST), 101, 0xfc3, FIND_ITEM(ITEM_NUT) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ConfigureRegionAtRow17) },
	{ PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_ConfigureRegionAtRow15) },
	{ PSYNERGY_ON_ACTOR(0x8c), 9, 0x200, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x8c), 10, 0x201, EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(0x91), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_PSYNERGY) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gHaidiaDouEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ 0x202, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SCRIPT) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TOUCH_3) },
	{ 0x202, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_SCRIPT) },
	{ SCENE_EVENTS_END },
};
