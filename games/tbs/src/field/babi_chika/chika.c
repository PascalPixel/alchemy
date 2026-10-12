#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/babi_chika/chika.h"
#include "unit/field/common/effect/move.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"
#include "unit/object/palbuf.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/math.h"
#include "object_effect.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "fieldobj.h"
#include "types.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "field_effect.h"
#include "staged_actor.h"
#include "spawn_configured_effect.h"
#include "edition.h"
#include "scene_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "call.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static s32	OverlayObject_SetYAboveLinkedActor(struct FieldActor *owner);
static void FieldScene_PlaceAndPinSlots8And9(void);
static void FieldScene_PlaceAndPinSlots10And11(void);
static void BabiChika_RestorePlatformCells(void);
static void BabiChika_MarkActorCells(void);

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/create.c"
#include "../common/effect/move.c"
#include "../common/effect/spawn.c"

static s32 SceneState_ApplyArgMode0AndReturnZero(s32 no)
{
	Engine_ActorSetSpriteFlags(no, 0);
	return 0;
}

static s32 SceneActor_MoveActorZeroToTarget(const MoveTarget *target)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
	u8				   saved = actor->motion_flags;
	s32				   probe[3];

	probe[0] = (actor->x.fixed & 0xfff00000) + 0x00080000;
	probe[1] = actor->y.fixed;
	probe[2] = (actor->z.fixed & 0xfff00000) + 0x00080000;

	Call3(Vector_AddPolarOffset, 0x00100000, (actor->facing + 0x2000) & 0xc000, (s32)probe);

	if (Object_CheckMovementCollision(actor, probe) == 1)
	{
		goto refuse;
	}
	if (Object_CheckMovementCollision(actor, target) != 0)
	{
		goto refuse;
	}

	Engine_EventBegin();
	Object_SetMode(actor, 6);
	Engine_TaskWait(6);
	Engine_AudioPlayCue(152);
	Object_SetMode(actor, 7);

	actor->speed = 0x00030000;
	actor->acceleration = 0x00020000;
	actor->velocity_y = 0x00040000;
	actor->motion_flags &= 0x7e;

	Engine_ActorSetSpriteFlags(actor, 0);
#if EDITION_INTERNATIONAL
	Engine_ActorMoveToAndWait(
		ACTOR_PARTY_LEADER, ((target->x.fixed >> 20) << 4) + 8, ((target->z.fixed >> 20) << 4) + 8);
#else
	Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, target->x.part.pixel, target->z.part.pixel);
#endif
	Object_SetMode(actor, 6);
	Engine_ActorSetSpriteFlags(actor, 1);
	Engine_TaskWait(6);

	actor->motion_flags = saved;
	Engine_EventEnd();
	return 0;

refuse:
	return 1;
}

static void SceneActor_PassRaisedPointOfActorZero(void)
{
	s32				   pos[3];
	struct FieldActor *p = Object_GetById(ACTOR_PARTY_LEADER);

	pos[0] = p->x.fixed;
	pos[1] = p->y.fixed;
	pos[2] = p->z.fixed + 0x200000;
	SceneActor_MoveActorZeroToTarget(pos);
}

static void SceneActor_PassActorZeroOffsetPoint(void)
{
	s32				   pos[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	pos[0] = actor->x.fixed;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed + 0xFFE00000;
	SceneActor_MoveActorZeroToTarget(pos);
}

static void BabiChika_UpdateTrackedActor(void)
{
	struct FieldActor *actor;
	u8				  *work;
	s32				   limit;

	actor = Object_GetById(0);
	work = gActorEffectWork;
	limit = 0;
	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		switch (gGameState.entrance)
		{
		case 3:
		case 4:
			limit = 94;
			break;
		case 8:
		case 9:
			limit = 74;
			break;
		case 12:
		case 13:
			limit = 118;
			break;
		}
	}
	else if (gGameState.entrance == 12)
	{
		limit = 93;
	}
	if ((actor->z.fixed >> 19) <= limit)
	{
		*(s32 *)(work + 24) = 0;
	}
	else
	{
		*(s32 *)(work + 24) = (s32)actor;
	}
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/depth.c"

static s32 BabiChika_UpdateFlickerEffect(struct FieldActor *object)
{
	struct EffectOptions options;
	s32					 x;
	s32					 z;

	if (gFrameCount & 2)
		Object_SetMode(object, 1);
	else
		Object_SetMode(object, 2);
	if (gFrameCount & 3)
		return 0;
	options.start_scale_x = 0x4ccc;
	options.start_scale_y = 0x4ccc;
	options.palette = 5;
	x = object->x.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
	z = object->z.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
	Effect_Spawn(x, object->y.fixed + 0x100000, z, 0, 0, 0, 0x90001, &options);
	return 0;
}

static s32 SceneActor_CopyActor8PositionWhenAtRow10(Record *record)
{
	Record *ref = Object_GetById(ACTOR_PARTY_LEADER);

	if (ref->w12 > -0x300000 && (Object_GetById(8)->z.fixed >> 20) == 10)
	{
		record->w8 = Object_GetById(8)->x.fixed;
		record->w12 = -0x200000;
		record->w16 = Object_GetById(8)->z.fixed;
	}
	else
	{
		record->w8 = 0;
		record->w12 = 0;
		record->w16 = 0;
	}
	return 0;
}

static void SceneActor_WaitValueBelowLimit(struct FieldActor *track)
{
	s32 cnt = 60;
	s32 limit;

	for (;;)
	{
		if (cnt != 0)
		{
			s32 value;

			Engine_TaskWait(1);
			value = track->y.fixed;
			limit = track->terrain_height;
			cnt--;
			if (value <= limit)
			{
				break;
			}
			continue;
		}
		limit = track->terrain_height;
		break;
	}
	track->velocity_y = 0;
	track->y.fixed = limit;
	track->target_y = ACTOR_NO_TARGET;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/advance.c"

static void BabiChika_RunLeaderSpray(void)
{
	struct FieldActor	*leader = Object_GetById(0);
	struct EffectOptions options;
	u32					 i;

	Engine_EventBegin();
	Object_SetMode(leader, 6);
	ObjectMotion_WaitForAnimationChange(0);
	Object_SetMode(leader, 1);
	Engine_ActorSetSpriteFlags(leader, 0);
	leader->motion_flags |= 2;
	Engine_AudioPlayCue(152);
	leader->velocity_y = 0x40000;
	Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed, leader->z.fixed + 0xc0000);
	Engine_TaskWait(6);
	leader->motion_flags = 0;
	options.update = (void (*)(union FieldObject *))Effect_AdvanceMotion;
	Engine_AudioPlayCue(127);
	for (i = 0; i < 8; i++)
	{
		leader->y.fixed -= 0x20000;
		leader->target_y = leader->y.fixed;
		Engine_TaskWait(1);
		if (i & 1)
		{
			s32 m = (u32)Engine_RandomNext() % 10 - 5;
			s32 vx = m * 0x3332;
			s32 vz;

			vz = (u32)Engine_RandomNext() % 10 * -0x1999 - 0x7ffd;
			Effect_Spawn(
				leader->x.fixed, leader->y.fixed, leader->z.fixed, vx, 0, vz, 0x1000001, &options);
		}
	}
	Engine_ActorSetSpriteFlags(leader, 1);
	leader->motion_flags = 3;
	Engine_EventEnd();
}

#define ACTOR_UPDATE_IDLE ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#define ACTOR_UPDATE_PANEL ((void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor)

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/radial.c"

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 v = gGameState.scene;

	if (v == (s32)&SceneId_BabiChika1)
	{
		return gBabiChikaEntrances1;
	}
	if (v == (s32)&SceneId_BabiChika2)
	{
		return gBabiChikaEntrances2;
	}
	return gBabiChikaEntrancesOther;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	if (gGameState.scene == (s32)&SceneId_BabiChika2)
	{
		return gBabiChikaRegions2;
	}
	return 0;
}

u8 *BabiChika_GetExits(void)
{
	return gBabiChikaExits;
}

u8 *SceneData_SelectAndApplyTableBySceneId(void)
{
	u8 *tbl;

	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		tbl = gBabiChikaPlacements1;
	}
	else
	{
		tbl = gBabiChikaPlacementsOther;
	}
	FieldScene_PrepareActors((struct SceneRegionEntry *)tbl);
	return tbl;
}

static void EV24_SCRIPT(void)
{
	Engine_EventBegin();
	Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
	StagedActor_AdvancePair();
	FieldScene_PlaceAndPinSlots8And9();
	Engine_EventEnd();
}

static void SceneActor_ApplyPointLeftOfActorZero(void)
{
	s32				   point[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	point[0] = actor->x.fixed + 0xFFE00000;
	point[1] = actor->y.fixed;
	point[2] = actor->z.fixed;
	SceneActor_MoveActorZeroToTarget(point);
}

static void EV26_SCRIPT(void)
{
	Engine_EventBegin();
	Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
	StagedActor_AdvancePair();
	FieldScene_PlaceAndPinSlots10And11();
	Engine_EventEnd();
}

static void FieldScene_RunStepWith6(void)
{
	BattleFx_RunRisingObjectSequence(0, 6, 0);
}

static void SceneActor_PassPointTwoRightOfActorZero(void)
{
	s32				   pos[3];
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	pos[0] = actor->x.fixed + 0x200000;
	pos[1] = actor->y.fixed;
	pos[2] = actor->z.fixed;
	SceneActor_MoveActorZeroToTarget(pos);
}

static void EV28_SCRIPT(void)
{
	Engine_EventBegin();
	Engine_MapCopyCellAttributes(89, 49, 3, 2, 25, 49);
	Engine_MapCopyCellAttributes(89, 51, 8, 5, 25, 51);
	StagedActor_AdvancePair();
	BabiChika_RestorePlatformCells();
	Engine_EventEnd();
}

static void EV28_TOUCH(void)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
	s32				   target[3];

	target[0] = actor->x.fixed;
	target[1] = actor->y.fixed;
	target[2] = actor->z.fixed + 0x00200000;
	if (SceneActor_MoveActorZeroToTarget(target) != 0)
	{
		EV28_SCRIPT();
	}
}

static void EV15_PSYNERGY(void)
{
	u8 *flags;
	u8	value;

	Engine_GameFlagSet(0x200);
	if (Engine_GameFlagIsSet(0x201) != 0)
	{
		Object_GetById(14)->rise_counter = 0;
		Object_GetById(14)->collision_flags &= 0xf7;
	}
	else
	{
		Object_GetById(14)->rise_counter = 1;
		flags = Object_GetById(14);
		flags += 89;
		value = 8;
		value |= *flags;
		*flags = value;
	}
}

static void EV16_PSYNERGY(void)
{
	u8 *p;
	u8	val;

	Engine_GameFlagSet(0x201);
	if (Engine_GameFlagIsSet(0x200) != 0)
	{
		Object_GetById(14)->rise_counter = 0;
		Object_GetById(14)->collision_flags &= 0xf7;
	}
	else
	{
		Object_GetById(14)->rise_counter = 1;
		p = Object_GetById(14);
		p += 89;
		val = 8;
		val |= *p;
		*p = val;
	}
}

static void EV17_PSYNERGY(void)
{
	Engine_GameFlagSet(0x970);
}

static void SceneState_RunUnlessActorZeroAtTile32x50(void)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	if ((actor->x.fixed >> 20) != 32 || (actor->z.fixed >> 20) != 50)
	{
		SceneActor_ApplyPointLeftOfActorZero();
	}
}

static void SceneState_RunUnlessActorZeroAt30_52(void)
{
	struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

	if ((actor->x.fixed >> 20) != 30 || (actor->z.fixed >> 20) != 52)
	{
		SceneActor_PassActorZeroOffsetPoint();
	}
}

static void EV18_PSYNERGY(void)
{
	u32					  i;
	s32					  rec2;
	s32					  rec7;
	struct FieldActor	 *actor;
	s32					  value;
	s32					  v5;
	s32					  v6;
	struct EffectOptions  slot16;
	struct EffectOptions *frame;

	Engine_EventBegin();
	actor = Object_GetById(18);
	if ((actor->x.fixed >> 20) == 46)
	{
		Engine_EventWait(30);
		rec2 = OverlayObject_PrepareObject(0x2e80000, 0, 0xb80000, 253);
		frame = &slot16;
		frame->start_scale_x = 0x9999;
		frame->start_scale_y = 0x9999;
		frame->palette = 7;
		Actor_Get(18)->motion_flags = 0;
		Engine_AudioPlayCue(185);
		for (i = 0; i < 16; i++)
		{
			Engine_TaskWait(3);
			Actor_Get(18)->y.fixed -= 0x10000;
			rec7 = Random_Next();
			rec7 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x2e00000);
			value = Engine_RandomNext();
			Effect_Spawn(rec7, 0, ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0x800000),
				0, 0, 0, 0x90000, frame);
		}
		Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
		Engine_EventWait(30);
		Actor_Get(18)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
		Engine_ActorSetAnimation(18, 3);
		Engine_ObjectDispatchRelease(rec2);
		Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
		Actor_SetPosition(20, 0x2e80000, 0x880000);
		v5 = 1;
		v6 = 3;
		Engine_AudioPlayCue(188);
		Map_CopyCellsTo(58, 8, 49, 8, v5, v6);
		Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_EventWait(20);
		Engine_AudioPlayCue(188);
		Map_CopyCellsTo(59, 8, 49, 8, v5, v6);
		Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
		Work_SetValuesIfNonNegative(-1, -1, 0xe666);
		Engine_MapRenderWaitForValues();
		Engine_EventWait(10);
		GameFlag_Set(0x971);
	}
	Engine_EventEnd();
}

static void EV29_SCRIPT(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	EV18_PSYNERGY();
}

static void EV20_PSYNERGY(void)
{
	u8 *flags;

	Engine_ActorSetAnimation(20, 1);
	Engine_ActorSetChildValue(20, 0);
	Engine_ActorSetAnimation(20, 2);
	flags = &Object_GetById(20)->priority_flags;
	*flags &= 0xFD;
	Engine_GameFlagSet(0x200);
}

static void FieldScene_RunSupplementalSequenceOne(void)
{
	u32					  i;
	s32					  rec2;
	s32					  rec7;
	struct FieldActor	 *actor;
	s32					  value;
	s32					  arg0;
	s32					  arg2;
	s32					  v5;
	s32					  v6;
	struct EffectOptions  slot16;
	struct EffectOptions *slot;

	Engine_EventBegin();
	actor = Object_GetById(19);
	if ((actor->x.fixed >> 20) == 48)
	{
		if (GameFlag_IsSet(0x202) != 0)
		{
			Engine_EventWait(30);
			rec2 = OverlayObject_PrepareObject(0x3020000, 0, 0x1120000, 223);
			slot = &slot16;
			slot->start_scale_x = 0x9999;
			slot->start_scale_y = 0x9999;
			slot->palette = 7;
			Actor_Get(19)->motion_flags = 0;
			Engine_AudioPlayCue(185);
			for (i = 0; i < 16; i++)
			{
				Engine_TaskWait(3);
				Actor_Get(19)->y.fixed -= 0x10000;
				rec7 = Engine_RandomNext();
				arg0 = ((((u32)(rec7 << 4) >> 16) << 16) + 0x3000000);
				value = Engine_RandomNext();
				arg2 = ((((u32)(((value << 3) + value) << 1) >> 16) << 16) + 0xe00000);
				Effect_Spawn(arg0, 0, arg2, 0, 0, 0, 0x90000, slot);
			}
			Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
			Engine_EventWait(30);
			Actor_Get(19)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			Engine_ActorSetAnimation(19, 3);
			Engine_ObjectDispatchRelease(rec2);
			Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
			Actor_SetPosition(21, 0x3080000, 0xe80000);
			v5 = 1;
			v6 = 3;
			Engine_AudioPlayCue(188);
			Map_CopyCellsTo(58, 8, 45, 14, v5, v6);
			Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_EventWait(20);
			Engine_AudioPlayCue(188);
			Map_CopyCellsTo(59, 8, 45, 14, v5, v6);
			Work_SetValuesIfNonNegative(0, 0x50000, 0x10000);
			Work_SetValuesIfNonNegative(-1, -1, 0xe666);
			Engine_MapRenderWaitForValues();
			Engine_EventWait(10);
			GameFlag_Set(0x972);
		}
	}
	Engine_EventEnd();
}

static void FieldScene_RunFourStepSequenceA(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	FieldScene_RunSupplementalSequenceOne();
}

static void EV19_SCRIPT(void)
{
	Engine_ActorEnableActionCallback(19, BabiChika_FlickerScript);
}

static void SceneState_SetValue202ThenCall(void)
{
	Engine_GameFlagSet(0x202);
	FieldScene_RunSupplementalSequenceOne();
}

static void EV21_PSYNERGY(void)
{
	u8 *flags;

	Engine_ActorSetAnimation(21, 1);
	Engine_ActorSetChildValue(21, 0);
	Engine_ActorSetAnimation(21, 2);
	flags = &Object_GetById(21)->priority_flags;
	*flags &= 0xFD;
	Engine_GameFlagSet(0x201);
}

static void EV40_SEARCH(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	if (Engine_GameFlagIsSet(0x982) != 0 || Engine_GameFlagIsSet(0x983) != 0)
	{
		Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSpeaksAfterAnswer), 1);
	}
	else
	{
		Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSpeaksRobinSoulYe), 1);
	}
	Engine_EventEnd();
}

static void EV08_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventEnd();
}

static void EV25_SCRIPT(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
	EV08_PSYNERGY();
}

static void EV08_SCRIPT(void)
{
	struct FieldActor *owner;

	Engine_ActorEnableActionCallback(8, BabiChika_FlickerScript);
	Engine_GameFlagSet(0x203);
	owner = Object_GetById(9);
	owner->update = (void (*)(union FieldObject *))SceneActor_CopyActor8PositionWhenAtRow10;
}

static void EV09_PSYNERGY(void)
{
	struct FieldActor *desc;
	s32				   col;
	s32				   row;

	Engine_EventBegin();
	Engine_ActorSetSpritePriority(9, 1);
	Engine_ActorSetAnimation(9, 1);
	Engine_ActorSetChildValue(9, 0);
	Engine_ActorSetAnimation(9, 2);

	{
		u8 *flag = &Object_GetById(9)->priority_flags;
		*flag &= 0xfd;
	}

	Engine_GameFlagSet(0x204);

	col = Object_GetById(9)->x.fixed;
	row = Object_GetById(9)->z.fixed >> 20;
	Engine_MapCopyCellAttributes(26, 8, 1, 1, col >> 20, row);

	desc = Object_GetById(9);
	desc->update = ACTOR_UPDATE_IDLE;

	desc = Object_GetById(8);
	desc->update = ACTOR_UPDATE_IDLE;

	((u8 * (*)()) Engine_EventEnd)(desc);
}

static s32 OverlayObject_SetYAboveLinkedActor(struct FieldActor *owner)
{
	s16				  *id = &owner->timer;
	struct FieldActor *actor = Object_GetById(*id);

	owner->y.fixed = actor->y.fixed + 0x100000;
	return 0;
}

static void BabiChika_SettleSteps(s32 wait)
{
	u32 i;

	Actor_SetSpeed(10, 0x8000, 0x4000);
	Actor_SetSpeed(11, 0x8000, 0x4000);
	if (wait != 0)
	{
		Engine_AudioPlayCue(180);
	}
	Engine_ObjectSetPosition(Object_GetById(10), Object_GetById(10)->x.fixed,
		BabiChika_SettleHeights[Object_GetById(10)->timer], Object_GetById(10)->z.fixed);
	Engine_ObjectSetPosition(Object_GetById(11), Object_GetById(11)->x.fixed,
		BabiChika_SettleHeights[Object_GetById(11)->timer], Object_GetById(11)->z.fixed);
	Engine_ActorWaitForMove(10);
	Engine_ActorWaitForMove(11);
	Object_GetById(10)->y.fixed = BabiChika_SettleHeights[Object_GetById(10)->timer];
	Object_GetById(11)->y.fixed = BabiChika_SettleHeights[Object_GetById(11)->timer];
	if (wait != 0)
	{
		Engine_AudioPlayCue(0x121);
	}
	for (i = 0; i < 5; i++)
	{
		if (Object_GetById(i + 10)->y.fixed / 0x10000 < 0
			&& Object_GetById(i + 10)->y.fixed / 0x10000 > -30)
		{
			Engine_MapCopyCellAttributes(4, 9, 1, 1, Object_GetById(i + 10)->x.fixed >> 20,
				Object_GetById(i + 10)->z.fixed >> 20);
		}
	}
	Engine_EventWait(wait);
}

static void SceneActor_LandOnHighestPlatform(s32 subject)
{
	s32 best = -0x500000;
	u32 i;

	for (i = 0; i <= 4; i++)
	{
		s32 slot = i + 10;

		if (slot == subject)
			continue;

		if ((Object_GetById(slot)->x.fixed >> 20) != (Object_GetById(subject)->x.fixed >> 20))
			continue;
		if ((Object_GetById(slot)->z.fixed >> 20) != (Object_GetById(subject)->z.fixed >> 20))
			continue;

		if (best > Object_GetById(slot)->y.fixed + 0x100000)
			continue;

		best = Object_GetById(slot)->y.fixed + 0x100000;
		Object_GetById(subject)->timer = slot;
	}

	Engine_ActorSetSpeed(subject, 0x40000, 0x20000);

	{
		struct FieldActor *target = Object_GetById(subject);
		struct FieldActor *from = Object_GetById(subject);
		s32				   z = Object_GetById(subject)->z.fixed;

		Engine_ObjectSetPosition(target, from->x.fixed, best, z);
	}

	Engine_ActorWaitForMove(subject);
	Engine_AudioPlayCue(188);
	SceneEffect_SpawnNineRadialEffects(subject);
	Engine_EventWait(30);
}

static void FieldScene_RunMiddleSequence(void)
{
	struct FieldActor saved;
	s32				  i;
	s32				  j;
	s32				  found;

	Engine_EventBegin();
	for (i = 0; i <= 2; i++)
	{
		if (Object_GetById(i + 12)->sprite->priority == 3 && GameFlag_IsSet(i + 0x200) == 0)
		{
			SceneActor_WaitValueBelowLimit(Actor_Get(i + 12));
			Actor_SetPosition(i + 12, 0, 0);
			GameFlag_Set(i + 0x200);
			break;
		}
		if ((Object_GetById(i + 12)->z.fixed >> 20) == 9 && GameFlag_IsSet(i + 0x200) == 0)
		{
			Object_GetById(i + 12)->terrain_height = 0;
			Object_GetById(i + 12)->velocity_y = 0;
			Object_GetById(i + 12)->target_y = ACTOR_NO_TARGET;
			Actor_Get(i + 12)->motion_flags = 0;
			Object_GetById(i + 12)->timer = 0;
			found = i;
			for (j = 0; j < i; j++)
			{
				if (GameFlag_IsSet(0x200 + j) == 0)
				{
					saved.x.fixed = Object_GetById(i + 12)->x.fixed;
					saved.y.fixed = Object_GetById(i + 12)->y.fixed;
					saved.z.fixed = Object_GetById(i + 12)->z.fixed;
					Actor_Get(i + 12)->x.fixed = Object_GetById(j + 12)->x.fixed;
					Object_GetById(i + 12)->y.fixed = Object_GetById(j + 12)->y.fixed;
					Object_GetById(i + 12)->z.fixed = Object_GetById(j + 12)->z.fixed;
					Actor_Get(j + 12)->x.fixed = saved.x.fixed;
					Actor_Get(j + 12)->y.fixed = saved.y.fixed;
					Actor_Get(j + 12)->z.fixed = saved.z.fixed;
					found = j;
					break;
				}
			}
			Object_GetById(found + 12)->terrain_height = 0;
			Actor_Get(found + 12)->velocity_y = 0;
			Object_GetById(found + 12)->target_y = ACTOR_NO_TARGET;
			Actor_Get(found + 12)->motion_flags = 0;
			Object_GetById(found + 12)->timer = 0;
			Engine_CameraSetSpeed(0x30000, 0x6000);
			Battle_GetWorkObject1e0()->motion_flags = 0;
			Camera_MoveTo(0xa80000, 0x80000, 0xb80000, 1);
			Engine_CameraWaitForMove();
			SceneActor_LandOnHighestPlatform(found + 12);
			if ((Object_GetById(found + 12)->x.fixed >> 20) == 8)
			{
				Object_GetById(10)->timer++;
				(Object_GetById(11)->timer)--;
			}
			else
			{
				(Object_GetById(10)->timer)--;
				Object_GetById(11)->timer++;
			}
			Actor_Get(found + 12)->update =
				(void (*)(union FieldObject *))OverlayObject_SetYAboveLinkedActor;
			BabiChika_SettleSteps(40);
			Actor_Get(found + 12)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
			GameFlag_Set(0x200 + found);
			break;
		}
	}
	Engine_EventEnd();
}

static void FieldScene_RunThreeStepSequence(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	Engine_EventEnd();
}

static void SceneState_SetSlot17And18Selectors(void)
{
	Engine_EventBegin();

	if ((Object_GetById(17)->x.fixed >> 20) == 45)
	{
		Engine_GameFlagSet(0x974);
	}
	else
	{
		Engine_GameFlagClear(0x974);
	}

	if ((Object_GetById(18)->x.fixed >> 20) == 46)
	{
		Engine_GameFlagSet(0x975);
	}
	else
	{
		Engine_GameFlagClear(0x975);
	}

	BabiChika_MarkActorCells();
	Engine_EventEnd();
}

static void EV29_TOUCH(void)
{
	Engine_EventBegin();
	StagedActor_AdvancePair();
	SceneState_SetSlot17And18Selectors();
	Engine_EventEnd();
}

static void FieldScene_RunLateSequenceHead(void)
{
	Map_CopyCellAttributes(72, 49, 1, 1, 8, 49);
	Map_CopyCellAttributes(113, 43, 1, 1, 49, 43);
	MapObject_SetPosition(100, 0, 0);
	MapObject_SetPosition(101, 0, 0);
	Actor_SetPosition(15, 8912896, 51904512);
	Actor_SetPosition(16, 51904512, 45613056);
}

static void FieldScene_RunLateSequenceSecond(void)
{
	Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
	Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
	MapObject_SetPosition(100, -1, -1);
	MapObject_SetPosition(101, -1, -1);
	Actor_SetPosition(15, 0, 0);
	Actor_SetPosition(16, 0, 0);
}

static void FieldScene_RunScene3c4SequenceA(void)
{
	struct FieldActor *record;
	s32				   v6;
	s32				   p5;
	s32				   q;

	v6 = 0;
	Engine_EventBegin();
	Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
	record = Object_GetById(19);
	p5 = record->x.fixed;
	q = Object_GetById(19)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(20);
	p5 = record->x.fixed;
	q = Object_GetById(20)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(21);
	p5 = record->x.fixed;
	q = Object_GetById(21)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(22);
	p5 = record->x.fixed;
	q = Object_GetById(22)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(23);
	p5 = record->x.fixed;
	q = Object_GetById(23)->z.fixed;
	q >>= 20;
	p5 >>= 20;
	Map_CopyCellAttributes(20, 56, 1, 1, p5, q);
	record = Object_GetById(19);
	if ((record->x.fixed >> 20) == 25)
	{
		record = Object_GetById(19);
		if ((record->z.fixed >> 20) == 49)
		{
			v6 = 1;
		}
	}
	record = Object_GetById(20);
	if ((record->x.fixed >> 20) == 23)
	{
		record = Object_GetById(20);
		if ((record->z.fixed >> 20) == 49)
		{
			v6 = (v6 + 1);
		}
	}
	record = Object_GetById(21);
	if ((record->x.fixed >> 20) == 25)
	{
		record = Object_GetById(21);
		if ((record->z.fixed >> 20) == 47)
		{
			v6 = (v6 + 1);
		}
	}
	record = Object_GetById(22);
	if ((record->x.fixed >> 20) == 23)
	{
		record = Object_GetById(22);
		if ((record->z.fixed >> 20) == 47)
		{
			v6 = (v6 + 1);
		}
	}
	record = Object_GetById(23);
	if ((record->x.fixed >> 20) == 24)
	{
		record = Object_GetById(23);
		if ((record->z.fixed >> 20) == 48)
		{
			v6 = (v6 + 1);
		}
	}
	if (v6 == 5)
	{
		if (GameFlag_IsSet(0x984) != 0)
		{
			Engine_EventEnd();
			goto done;
		}
		Engine_EventWait(20);
		Camera_SetSpeed(0xcccc, 0x1999);
		Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(30);
		GameFlag_Set(0x984);
		Engine_AudioPlayCue(158);
		Map_AnimateCells(BabiChika_CellStepsA, 32, 46);
		Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
		Engine_EventWait(40);
	}
	else
	{
		if (GameFlag_IsSet(0x984) != 0)
		{
			Engine_EventWait(20);
			Camera_SetSpeed(0xcccc, 0x1999);
			Camera_MoveTo(0x1d80000, -1, 0x30c0000, 1);
			Engine_CameraWaitForMove();
			Engine_EventWait(30);
			GameFlag_Clear(0x984);
			Engine_AudioPlayCue(159);
			Map_AnimateCells(BabiChika_CellStepsB, 32, 46);
			Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
			Engine_EventWait(40);
		}
	}
	Engine_EventEnd();
done:;
}

static void EV41_SCRIPT(void)
{
	Engine_EventBegin();
	{
		s32 width = 19;
		s32 height = 45;

		Engine_MapCopyCellAttributes(83, 45, 11, 8, width, height);
	}
	StagedActor_AdvancePair();
	FieldScene_RunScene3c4SequenceA();
	Engine_EventEnd();
}

static void EV42_SEARCH(void)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_MessageShowCentered(MSGNO(MsgBabiChikaStatueSeemsSpeakYourSoul), 1);
	Engine_EventEnd();
}

static void EV20_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
	Engine_EventEnd();
}

s32 BabiChika_SelectEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		return (s32)gBabiChikaEvents1;
	}
	return (s32)gBabiChikaEventsOther;
}

static void FieldScene_PlaceAndPinSlots8And9(void)
{
	s32 row;

	{
		s32 p5 = 9, p6 = 38;
		Engine_MapCopyCellAttributes(73, 38, 5, 5, p5, p6);
	}
	SceneActor_SwapPositionsByDepth(9, 8);

	{
		s32 col = Object_GetById(8)->x.fixed >> 20;
		row = Object_GetById(8)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col, row);
	}

	{
		s32 col = Object_GetById(9)->x.fixed >> 20;
		row = Object_GetById(9)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col, row);
	}
}

static void FieldScene_PlaceAndPinSlots10And11(void)
{
	s32 row;

	{
		s32 k5 = 29, k6 = 30;
		Engine_MapCopyCellAttributes(93, 30, 6, 5, k5, k6);
	}
	SceneActor_SwapPositionsByDepth(11, 10);

	{
		s32 col20 = Object_GetById(10)->x.fixed >> 20;
		row = Object_GetById(10)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col20, row);
	}

	{
		s32 col20 = Object_GetById(11)->x.fixed >> 20;
		row = Object_GetById(11)->z.fixed >> 20;
		Engine_MapCopyCellAttributes(2, 36, 1, 1, col20, row);
	}
}

static void BabiChika_RestorePlatformCells(void)
{
	struct FieldActor *record;
	s32				   p5;

	Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
	Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
	Object_GetById(14)->terrain_id = 1;
	record = Object_GetById(12);
	p5 = record->x.fixed;
	record = Object_GetById(12);
	p5 >>= 20;
	Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
	record = Object_GetById(13);
	p5 = record->x.fixed;
	record = Object_GetById(13);
	p5 >>= 20;
	Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
	record = Object_GetById(14);
	p5 = record->x.fixed;
	record = Object_GetById(14);
	p5 >>= 20;
	Map_CopyCellAttributes(22, 52, 1, 1, p5, (record->z.fixed >> 20));
}

static void BabiChika_MarkActorCells(void)
{
	Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
	StagedActor_FillGridAttributeRectangle(
		0, Object_GetById(17)->x.fixed >> 20, Object_GetById(17)->z.fixed >> 20, 1, 1, 0xff);
	StagedActor_FillGridAttributeRectangle(
		0, Object_GetById(18)->x.fixed >> 20, Object_GetById(18)->z.fixed >> 20, 1, 1, 0xff);
}

s32 FieldScene_InitializeActorGroups(void)
{
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
	if (gGameState.scene == (s32)&SceneId_BabiChika1
		|| gGameState.scene == (s32)&SceneId_BabiChika2)
	{
		BattleFx_StartFadeOverlay(0);
		gGameState.retreat_entrance = 1;
		gGameState.retreat_scene = (s32)&SceneId_BabiChika1;
	}
	if (gGameState.scene == (s32)&SceneId_BabiChika1)
	{
		switch (gGameState.entrance)
		{
		case 1:
		case 2:
			if (Engine_GameFlagIsSet(0x982))
			{
				Map_CopyCellsTo(121, 4, 74, 9, 5, 8);
				Map_CopyCellsTo(18, 83, 9, 73, 3, 2);
				Engine_MapCopyCellsTo(18, 81, 9, 75, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 9, 77, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 9, 79, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
			}
			else
			{
				if (!Engine_GameFlagIsSet(0x983))
					break;
				Map_CopyCellsTo(121, 13, 74, 9, 5, 8);
				Map_CopyCellsTo(18, 85, 11, 74, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 13, 75, 3, 2);
				Engine_MapCopyCellsTo(18, 85, 11, 76, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 11, 78, 3, 2);
				Engine_MapCopyCellsTo(18, 83, 13, 79, 3, 2);
			}
			break;
		case 3:
		case 4:
			FieldScene_PlaceAndPinSlots8And9();
			Object_GetById(8)->motion_flags = 0;
			Object_GetById(9)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
			Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		case 5:
		case 6:
		case 7:
			if (Engine_GameFlagIsSet(0x982))
				Map_CopyCells(23, 17, 1, 2, 30, 8);
			if (Engine_GameFlagIsSet(0x983))
				Map_CopyCells(23, 17, 1, 2, 32, 10);
			break;
		case 8:
		case 9:
			FieldScene_PlaceAndPinSlots10And11();
			Object_GetById(10)->motion_flags = 0;
			Object_GetById(11)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
			Object_GetById(10)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(11)->update = ACTOR_UPDATE_IDLE;
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		case 10:
		case 11:
			Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(19), 0);
			Engine_ActorSetAnimation(18, 2);
			Engine_ActorSetSpriteFlags(Object_GetById(20), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
			Engine_ActorSetChildValue(20, 15);
			Engine_ActorSetChildValue(21, 15);
			if (Engine_GameFlagIsSet(0x971))
			{
				Map_CopyCellsTo(59, 8, 49, 8, 1, 3);
				Engine_MapCopyCellAttributes(51, 8, 1, 1, 49, 8);
				Object_GetById(18)->priority_flags |= 2;
				Engine_ActorSetAnimation(18, 3);
				Engine_MapCopyCellAttributes(45, 4, 1, 1, 46, 8);
				Actor_SetPosition(18, 186 << 18, 136 << 16);
				Object_GetById(18)->y.fixed = -0x100000;
				Actor_SetPosition(20, 186 << 18, 136 << 16);
			}
			if (Engine_GameFlagIsSet(0x200))
			{
				Engine_ActorSetChildValue(20, 0);
				Engine_ActorSetAnimation(20, 5);
			}
			if (GameFlag_IsSet(0x202))
				Engine_ActorSetAnimation(19, 2);
			if (Engine_GameFlagIsSet(0x972))
			{
				Map_CopyCellsTo(59, 8, 45, 14, 1, 3);
				Engine_MapCopyCellAttributes(51, 8, 1, 1, 45, 14);
				Object_GetById(19)->priority_flags |= 2;
				Engine_ActorSetAnimation(19, 3);
				Engine_MapCopyCellAttributes(45, 4, 1, 1, 48, 14);
				Actor_SetPosition(19, 194 << 18, 232 << 16);
				Object_GetById(19)->y.fixed = -0x100000;
				Engine_ActorSetPosition(21, 194 << 18, 232 << 16);
				Engine_GameFlagSet(0x202);
			}
			if (Engine_GameFlagIsSet(0x201))
			{
				Engine_ActorSetChildValue(21, 0);
				Engine_ActorSetAnimation(21, 5);
			}
			break;
		case 12:
		case 13:
			BabiChika_RestorePlatformCells();
			Object_GetById(12)->motion_flags = 0;
			Object_GetById(13)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(15), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(16), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(17), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
			Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
			Object_GetById(12)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(13)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(14)->update = ACTOR_UPDATE_IDLE;
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		}
	}
	else
	{
		switch (gGameState.entrance)
		{
		case 0:
			break;
		case 1:
		case 2:
		case 3:
			gGameState.retreat_entrance = 1;
			gGameState.retreat_scene = (s32)&SceneId_BabiIriguchi3;
			Engine_GameFlagClear(0x12f);
			Engine_ActorSetChildValue(17, 6);
			Engine_ActorSetChildValue(18, 6);
			if (Engine_GameFlagIsSet(0x974))
				Actor_SetPosition(17, 182 << 18, 156 << 17);
			if (Engine_GameFlagIsSet(0x975))
				Actor_SetPosition(18, 186 << 18, 156 << 17);
			BabiChika_MarkActorCells();
			break;
		case 6:
		case 7:
			Engine_ActorSetSpritePriority(8, 1);
			Object_GetById(8)->motion_flags = 0;
			Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
			Engine_ActorSetSpritePriority(9, 1);
			Engine_ActorSetChildValue(9, 15);
			Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
			Object_GetById(9)->motion_flags = 0;
			if (!Engine_GameFlagIsSet(0x204))
				break;
			Engine_ActorSetChildValue(9, 0);
			Engine_ActorSetAnimation(9, 5);
			Engine_MapCopyCellAttributes(
				26, 8, 1, 1, Object_GetById(9)->x.fixed >> 20, Object_GetById(9)->z.fixed >> 20);
			Object_GetById(9)->update = ACTOR_UPDATE_IDLE;
			Object_GetById(8)->update = ACTOR_UPDATE_IDLE;
			break;
		case 4:
		case 5:
			if (!Engine_GameFlagIsSet(0x109))
			{
				Object_GetById(10)->motion_flags = 0;
				Object_GetById(11)->motion_flags = 0;
				Object_GetById(10)->y.fixed = -0x300000;
				Object_GetById(11)->y.fixed = -0x300000;
				Object_GetById(10)->priority_flags |= 2;
				Object_GetById(11)->priority_flags |= 2;
				Object_GetById(10)->collision_flags &= 0xfe;
				Object_GetById(11)->collision_flags &= 0xfe;
				Object_GetById(10)->timer = 3;
				Object_GetById(11)->timer = 3;
				Engine_ActorSetSpritePriority(10, 1);
				Engine_ActorSetSpritePriority(11, 1);
				Object_GetById(12)->motion_flags = 0;
				Object_GetById(13)->motion_flags = 0;
				Object_GetById(14)->motion_flags = 0;
				Engine_ActorSetSpriteFlags(Object_GetById(12), 0);
				Engine_ActorSetSpriteFlags(Object_GetById(13), 0);
				Engine_ActorSetSpriteFlags(Object_GetById(14), 0);
				Object_GetById(12)->timer = 0;
				Object_GetById(13)->timer = 0;
				Object_GetById(14)->timer = 0;
				if (gGameState.entrance != 5)
					break;
				Object_GetById(10)->y.fixed = -0x200000;
				Object_GetById(11)->y.fixed = -0x400000;
				Object_GetById(10)->timer = 2;
				Object_GetById(11)->timer = 4;
				Actor_SetPosition(12, 200 << 16, 152 << 16);
				Object_GetById(12)->timer = 11;
				Object_GetById(12)->update = ACTOR_UPDATE_PANEL;
				Object_GetById(12)->priority_flags |= 2;
				Actor_SetPosition(13, 200 << 16, 152 << 16);
				Object_GetById(13)->timer = 12;
				Object_GetById(13)->update = ACTOR_UPDATE_PANEL;
				Object_GetById(13)->priority_flags |= 2;
				Engine_ActorSetPosition(14, 136 << 16, 152 << 16);
				Object_GetById(14)->timer = 10;
				Object_GetById(14)->update = ACTOR_UPDATE_PANEL;
				Object_GetById(14)->priority_flags |= 2;
				Engine_EventWait(2);
				Engine_GameFlagSet(0x200);
				Engine_GameFlagSet(0x201);
				Engine_GameFlagSet(0x202);
			}
			BabiChika_SettleSteps(0);
			break;
		case 8:
		case 9:
		case 10:
		case 11:
			if (Engine_GameFlagIsSet(0x982))
				Map_CopyCells(10, 30, 1, 2, 16, 30);
			if (Engine_GameFlagIsSet(0x983))
				Map_CopyCells(10, 30, 1, 2, 22, 30);
			Engine_GameFlagSet(0x973);
			break;
		case 12:
			Map_CopyCellAttributes(8, 49, 1, 1, 8, 113);
			FieldScene_RunLateSequenceHead();
			Engine_TaskAddCallback(BabiChika_UpdateTrackedActor, 0xc80);
			break;
		case 13:
		case 14:
			Engine_EventWait(1);
			if (Engine_GameFlagIsSet(0x984))
			{
				Map_CopyCells(24, 59, 1, 2, 32, 46);
				Actor_SetPosition(19, 204 << 17, 198 << 18);
				Actor_SetPosition(20, 188 << 17, 198 << 18);
				Actor_SetPosition(21, 204 << 17, 190 << 18);
				Actor_SetPosition(22, 188 << 17, 190 << 18);
				Actor_SetPosition(23, 196 << 17, 194 << 18);
			}
			Object_GetById(19)->motion_flags &= 0xfe;
			Object_GetById(20)->motion_flags &= 0xfe;
			Object_GetById(21)->motion_flags &= 0xfe;
			Object_GetById(22)->motion_flags &= 0xfe;
			Object_GetById(23)->motion_flags &= 0xfe;
			Engine_ActorSetChildValue(19, 4);
			Engine_ActorSetChildValue(20, 1);
			Engine_ActorSetChildValue(21, 4);
			Engine_ActorSetChildValue(22, 10);
			Engine_ActorSetChildValue(23, 0);
			Engine_ActorSetAnimation(19, 2);
			Engine_ActorSetAnimation(23, 2);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(19)->x.fixed >> 20, Object_GetById(19)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(20)->x.fixed >> 20, Object_GetById(20)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(21)->x.fixed >> 20, Object_GetById(21)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(22)->x.fixed >> 20, Object_GetById(22)->z.fixed >> 20);
			Engine_MapCopyCellAttributes(
				20, 56, 1, 1, Object_GetById(23)->x.fixed >> 20, Object_GetById(23)->z.fixed >> 20);
			break;
		case 17:
			Map_CopyCellAttributes(49, 43, 1, 1, 49, 107);
			FieldScene_RunLateSequenceHead();
			break;
		}
	}
	return 0;
}

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
	207,
	205,
	228,
	229,
	298,
	297,
};

s32 StagedActor_FootprintBounds[] = {
	-32,
	-8,
	32,
	8,
	-8,
	-32,
	8,
	32,
	-32,
	-16,
	32,
	0,
	-8,
	-32,
	8,
	32,
	-32,
	-8,
	32,
	8,
	-8,
	-32,
	8,
	32,
};

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

u32 BabiChika_SettleHeights[] = {
	0xfff00000,
	0xfff00000,
	0xffe00000,
	0xffd00000,
	0xffc00000,
	0xffb00000,
	0xffb00000,
};

const s32 *gEffectScripts[] = {
	gEffectScript0,
	gEffectScript1,
	gEffectScript2,
};

s32 BabiChika_ShrinkScript[] = {
	SC_SET(SW_SCALE_X, 0x9999),
	SC_SET(SW_SCALE_Y, 0x9999),
	SC_SET(SW_COLLISION, 0),
	SC_CALL(SceneState_ApplyArgMode0AndReturnZero),
	SC_END,
};

s32 BabiChika_StretchScript[] = {
	SC_SET(SW_SCALE_Y, 0x19999),
	SC_END,
};

s32 BabiChika_FlickerScript[] = {
	SC_WAIT(35),
	SC_SET(SW_HOOK, (s32)BabiChika_UpdateFlickerEffect),
	SC_WAIT(130),
	SC_SET(SW_HOOK, 0),
	SC_ARG(2),
	SC_END,
};

u16 BabiChika_CellStepsA[] = {
	MAP_CELL_STEP(28, 59, 1, 2, 4),
	MAP_CELL_STEP(26, 59, 1, 2, 4),
	MAP_CELL_STEP(24, 59, 1, 2, 4),
	MAP_CELL_STEPS_END,
};

u16 BabiChika_CellStepsB[] = {
	MAP_CELL_STEP(26, 59, 1, 2, 4),
	MAP_CELL_STEP(28, 59, 1, 2, 4),
	MAP_CELL_STEP(30, 59, 1, 2, 4),
	MAP_CELL_STEPS_END,
};

struct SceneEntrance gBabiChikaEntrancesOther[] = {
	{0, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{10, CONDITION_ALWAYS, 440, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gBabiChikaEntrances1[] = {
	{0, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 104, 0, 152, FACING_SOUTH, 0, 48, 48, 288, 368, 0},
	{2, CONDITION_ALWAYS, 200, 0, 328, FACING_NORTH, 0, 48, 48, 288, 368, 0},
	{3, CONDITION_ALWAYS, 184, 0, 552, FACING_SOUTH, 0, 64, 464, 304, 816, 0},
	{4, CONDITION_ALWAYS, 168, 0, 816, FACING_NORTH, 0, 64, 464, 304, 816, 0},
	{5, CONDITION_ALWAYS, 520, 0, 104, FACING_SOUTH, 0, 336, 48, 576, 288, 0},
	{6, CONDITION_ALWAYS, 392, 0, 192, FACING_NORTH, 0, 336, 48, 576, 288, 0},
	{7, CONDITION_ALWAYS, 520, 0, 256, FACING_NORTH, 0, 336, 48, 576, 288, 0},
	{8, CONDITION_ALWAYS, 568, 0, 440, FACING_SOUTH, 0, 384, 352, 624, 640, 0},
	{9, CONDITION_ALWAYS, 440, 0, 648, FACING_NORTH, 0, 384, 352, 624, 640, 0},
	{10, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, 624, 40, 864, 288, 0},
	{11, CONDITION_ALWAYS, 680, 0, 256, FACING_NORTH, 0, 624, 40, 864, 288, 0},
	{12, CONDITION_ALWAYS, 504, 0, 792, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
	{13, CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
	{14, CONDITION_ALWAYS, 776, 0, 432, FACING_NORTH, 0, 688, 368, 928, 640, 0},
	{15, CONDITION_ALWAYS, 840, 0, 608, FACING_NORTH, 0, 688, 368, 928, 640, 0},
	{16, CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 688, 736, 928, 976, 0},
	{17, CONDITION_ALWAYS, 744, 0, 944, FACING_NORTH, 0, 688, 736, 928, 976, 0},
	{18, CONDITION_ALWAYS, 872, 0, 880, FACING_NORTH, 0, 688, 736, 928, 976, 0},
	{SCENE_TABLE_END},
};

struct SceneEntrance gBabiChikaEntrances2[] = {
	{0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{1, CONDITION_ALWAYS, 632, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
	{2, CONDITION_ALWAYS, 840, 0, 120, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
	{3, CONDITION_ALWAYS, 824, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
	{4, CONDITION_ALWAYS, 216, 0, 104, FACING_SOUTH, 0, 32, 48, 272, 320, 0},
	{5, CONDITION_ALWAYS, 216, 0, 296, FACING_NORTH, 0, 32, 48, 272, 320, 0},
	{6, CONDITION_ALWAYS, 360, 0, 104, FACING_SOUTH, 0, 304, 48, 544, 352, 0},
	{7, CONDITION_ALWAYS, 488, 0, 328, FACING_NORTH, 0, 304, 48, 544, 352, 0},
	{8, CONDITION_ALWAYS, 120, 0, 568, FACING_NORTH, 0, 80, 400, 448, 592, 0},
	{9, CONDITION_ALWAYS, 216, 0, 456, FACING_SOUTH, 0, 80, 400, 448, 592, 0},
	{10, CONDITION_ALWAYS, 312, 0, 568, FACING_NORTH, 0, 80, 400, 448, 592, 0},
	{11, CONDITION_ALWAYS, 408, 0, 456, FACING_SOUTH, 0, 80, 400, 448, 592, 0},
	{12, CONDITION_ALWAYS, 136, 0, 712, FACING_SOUTH, 0, 16, 656, 256, 880, 0},
	{13, CONDITION_ALWAYS, 392, 0, 712, FACING_SOUTH, 0, 288, 656, 560, 864, 0},
	{14, CONDITION_ALWAYS, 520, 0, 776, FACING_SOUTH, 0, 288, 656, 560, 864, 0},
	{15, CONDITION_ALWAYS, 648, 0, 952, FACING_NORTH, 0, 608, 816, 848, 976, 0},
	{16, CONDITION_ALWAYS, 776, 0, 904, FACING_SOUTH, 0, 608, 816, 848, 976, 0},
	{17, CONDITION_ALWAYS, 696, 0, 680, FACING_SOUTH, 0, 624, 592, 864, 768, 0},
	{SCENE_TABLE_END},
};

s16 gBabiChikaRegions2[] = {
	624,
	16,
	336,
	640,
	32,
	352,
	-1,
	15,
	816,
	16,
	336,
	832,
	32,
	352,
	-1,
	16,
	832,
	16,
	112,
	848,
	32,
	128,
	-1,
	17,
	-1,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
};

u32 gBabiChikaExits[] = {
	SCENE_EXITS(172),
	SCENE_EXIT(1, 174, 1),
	SCENE_EXIT(2, 172, 3),
	SCENE_EXIT(3, 172, 2),
	SCENE_EXIT(4, 172, 5),
	SCENE_EXIT(5, 172, 4),
	SCENE_EXIT(6, 172, 8),
	SCENE_EXIT(7, 172, 10),
	SCENE_EXIT(8, 172, 6),
	SCENE_EXIT(9, 172, 12),
	SCENE_EXIT(10, 172, 7),
	SCENE_EXIT(11, 172, 16),
	SCENE_EXIT(12, 172, 9),
	SCENE_EXIT(13, 172, 14),
	SCENE_EXIT(14, 172, 13),
	SCENE_EXIT(15, 173, 4),
	SCENE_EXIT(16, 172, 11),
	SCENE_EXIT(17, 173, 6),
	SCENE_EXIT(18, 173, 17),
	SCENE_EXIT(0, 173, 0),
	SCENE_EXIT(1, 172, 15),
	SCENE_EXIT(2, 173, 9),
	SCENE_EXIT(3, 172, 17),
	SCENE_EXIT(4, 173, 11),
	SCENE_EXIT(5, 173, 5),
	SCENE_EXIT(6, 173, 7),
	SCENE_EXIT(7, 173, 12),
	SCENE_EXIT(8, 173, 13),
	SCENE_EXIT(9, 173, 8),
	SCENE_EXIT(10, 173, 10),
	SCENE_EXIT(11, 173, 15),
	SCENE_EXIT(12, 172, 18),
	SCENE_EXIT(13, 173, 14),
	SCENE_EXIT(14, 183, 18),
	SCENE_EXIT(15, 176, 4),
	SCENE_EXIT(16, 176, 5),
	SCENE_EXIT(17, 176, 6),
	SCENE_EXITS_END,
};

struct ScenePlacement gBabiChikaPlacements1[] = {
	{0xfd, 0x0, ACTOR_STAND, PIXELS(152), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0xfd, 0x0, ACTOR_STAND, PIXELS(168), 0, PIXELS(616), FACING_SOUTH, TALK_KEEP_FACING,
		PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{0xfd, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0xfd, 0x0, ACTOR_STAND, PIXELS(536), 0, PIXELS(488), FACING_SOUTH, TALK_KEEP_FACING,
		PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{0xfe, 0x0, ACTOR_STAND, PIXELS(488), 0, PIXELS(840), FACING_SOUTH, TALK_KEEP_FACING, 0},
	{0xfe, 0x0, ACTOR_STAND, PIXELS(520), 0, PIXELS(840), FACING_SOUTH, TALK_KEEP_FACING,
		PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{0xfe, 0x0, ACTOR_STAND, PIXELS(424), 0, PIXELS(792), FACING_SOUTH, TALK_KEEP_FACING,
		PLACEMENT_SHARE_PREVIOUS_SPRITE},
	{0xf8, 0x0, (s32)BabiChika_StretchScript, PIXELS(416), 0, PIXELS(794), FACING_NORTH,
		TALK_FACE_PARTY, 0},
	{0xf8, 0x0, (s32)BabiChika_StretchScript, PIXELS(432), 0, PIXELS(794), FACING_NORTH,
		TALK_FACE_PARTY, 0},
	{0xf8, 0x970, (s32)BabiChika_StretchScript, PIXELS(568), 0, PIXELS(856), FACING_NORTH,
		TALK_FACE_PARTY, 0},
	{0xff, 0x0, ACTOR_STAND, PIXELS(680), 0, PIXELS(136), FACING_NORTH, TALK_KEEP_FACING, 0},
	{0xff, 0x0, ACTOR_STAND, PIXELS(808), 0, PIXELS(232), FACING_NORTH, TALK_KEEP_FACING, 0},
	{0xe3, 0x0, ACTOR_STAND, PIXELS(728), 0, PIXELS(184), FACING_NORTH, TALK_KEEP_FACING, 0},
	{0xe3, 0x0, ACTOR_STAND, PIXELS(728), 0, PIXELS(184), FACING_NORTH, TALK_KEEP_FACING, 0},
	{0x114, 0x0, (s32)BabiChika_ShrinkScript, PIXELS(232), 0, PIXELS(248), FACING_SOUTH,
		TALK_KEEP_FACING, 0},
	{SCENE_TABLE_END},
};

u32 gBabiChikaPlacementsOther[] = {
	SCENE_EXITS(255),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(24, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(10, 0, 128),
	SCENE_EXIT(0, 0, 44),
	SCENE_EXIT(0, 227, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(24, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(15, 0, 128),
	SCENE_EXIT(0, 0, 44),
	SCENE_EXIT(0, 257, 0),
	SCENE_EXIT(0, 7, 0),
	SCENE_EXIT(8, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(9, 0, 128),
	SCENE_EXIT(0, 0, 44),
	SCENE_EXIT(0, 257, 0),
	SCENE_EXIT(0, 7, 0),
	SCENE_EXIT(12, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(9, 0, 128),
	SCENE_EXIT(16, 0, 44),
	SCENE_EXIT(0, 256, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(9, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(8, 0, 128),
	SCENE_EXIT(0, 0, 44),
	SCENE_EXIT(0, 256, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(11, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(8, 0, 128),
	SCENE_EXIT(16, 0, 44),
	SCENE_EXIT(0, 256, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(13, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(8, 0, 128),
	SCENE_EXIT(16, 0, 44),
	SCENE_EXIT(0, 258, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(8, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(49, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 258, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(49, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(43, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 253, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(46, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(19, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 253, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(47, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(19, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 500, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(21, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(47, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 500, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(27, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(47, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 500, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(21, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(50, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 500, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(27, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(50, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 500, 0),
	SCENE_EXIT(0, 1, 0),
	SCENE_EXIT(24, 0, 128),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(51, 0, 128),
	SCENE_EXIT(0, 0, 36),
	SCENE_EXIT(0, 4095, 15),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(0, 0, 0),
	SCENE_EXIT(0, 0, 0),
};

struct SceneEvent gBabiChikaEvents1[] = {
	{0x21, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
	{EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
	{EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
	{EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
	{EVENT_EXIT, 10, CONDITION_ALWAYS, 10},
	{EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
	{EVENT_EXIT, 12, CONDITION_ALWAYS, 12},
	{EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
	{EVENT_EXIT, 14, CONDITION_ALWAYS, 14},
	{EVENT_EXIT, 15, CONDITION_ALWAYS, 15},
	{EVENT_EXIT, 16, CONDITION_ALWAYS, 16},
	{EVENT_EXIT, 17, CONDITION_ALWAYS, 17},
	{EVENT_EXIT, 18, CONDITION_ALWAYS, 18},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStepWith6)},
	{TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_ApplyPointLeftOfActorZero)},
	{0x202, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_SCRIPT)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS,
		EVENT_SCRIPT(BabiChika_RunLeaderSpray)},
	{0x202, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_SCRIPT)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStepWith6)},
	{TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassPointTwoRightOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneState_RunUnlessActorZeroAtTile32x50)},
	{TOUCH_WALKING_INTO(FACING_EAST), 50, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassPointTwoRightOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneState_RunUnlessActorZeroAt30_52)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassRaisedPointOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT_SCRIPT(EV28_TOUCH)},
	{0x202, 28, CONDITION_ALWAYS, EVENT_SCRIPT(EV28_SCRIPT)},
	{0x202, 29, 0x971, EVENT_SCRIPT(EV29_SCRIPT)},
	{TOUCH_WALKING_INTO(FACING_EAST), 29, 0x200,
		EVENT_SCRIPT(SceneActor_PassPointTwoRightOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 29, 0x200,
		EVENT_SCRIPT(SceneActor_PassRaisedPointOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 29, 0x200,
		EVENT_SCRIPT(SceneActor_PassActorZeroOffsetPoint)},
	{TOUCH_WALKING_INTO(FACING_WEST), 31, 0x200,
		EVENT_SCRIPT(SceneActor_ApplyPointLeftOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassPointTwoRightOfActorZero)},
	{0x202, 30, 0x972, EVENT_SCRIPT(FieldScene_RunFourStepSequenceA)},
	{TOUCH_WALKING_INTO(FACING_WEST), 30, 0x201,
		EVENT_SCRIPT(SceneActor_ApplyPointLeftOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 30, 0x201,
		EVENT_SCRIPT(SceneActor_PassRaisedPointOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 30, 0x201,
		EVENT_SCRIPT(SceneActor_PassActorZeroOffsetPoint)},
	{TOUCH_WALKING_INTO(FACING_EAST), 32, 0x202, EVENT_SCRIPT(FieldScene_RunFourStepSequenceA)},
	{TOUCH_WALKING_INTO(FACING_EAST), 32, 0x201,
		EVENT_SCRIPT(SceneActor_PassPointTwoRightOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_ApplyPointLeftOfActorZero)},
	{SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_SEARCH)},
	{PSYNERGY_ON_ACTOR(0x8c), 8, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceAndPinSlots8And9)},
	{PSYNERGY_ON_ACTOR(0x8c), 9, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceAndPinSlots8And9)},
	{PSYNERGY_ON_ACTOR(0x8c), 10, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_PlaceAndPinSlots10And11)},
	{PSYNERGY_ON_ACTOR(0x8c), 11, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_PlaceAndPinSlots10And11)},
	{PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, EVENT_SCRIPT(BabiChika_RestorePlatformCells)},
	{PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, EVENT_SCRIPT(BabiChika_RestorePlatformCells)},
	{PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(BabiChika_RestorePlatformCells)},
	{PSYNERGY_ON_ACTOR(0x4e), 15, 0x200, EVENT_SCRIPT(EV15_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(0x4e), 16, 0x201, EVENT_SCRIPT(EV16_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(0x4e), 17, 0x970, EVENT_SCRIPT(EV17_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(0x8c), 18, 0x971, EVENT_SCRIPT(EV18_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 20, 0x200, EVENT_SCRIPT(EV20_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(0x8c), 19, 0x972, EVENT_SCRIPT(FieldScene_RunSupplementalSequenceOne)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 21, 0x201, EVENT_SCRIPT(EV21_PSYNERGY)},
	{0x10002115, 19, 0x202, EVENT_SCRIPT(EV19_SCRIPT)},
	{PSYNERGY_ON_ACTOR(0x21), 19, 0x202, EVENT_SCRIPT(SceneState_SetValue202ThenCall)},
	{SCENE_EVENTS_END},
};

struct SceneEvent gBabiChikaEventsOther[] = {
	{EVENT_EXIT, 1, CONDITION_ALWAYS, 1},
	{EVENT_EXIT, 2, CONDITION_ALWAYS, 2},
	{EVENT_EXIT, 3, CONDITION_ALWAYS, 3},
	{EVENT_EXIT, 4, CONDITION_ALWAYS, 4},
	{EVENT_EXIT, 5, CONDITION_ALWAYS, 5},
	{EVENT_EXIT, 6, CONDITION_ALWAYS, 6},
	{EVENT_EXIT, 7, CONDITION_ALWAYS, 7},
	{EVENT_EXIT, 8, CONDITION_ALWAYS, 8},
	{EVENT_EXIT, 9, CONDITION_ALWAYS, 9},
	{EVENT_EXIT, 10, CONDITION_ALWAYS, 10},
	{EVENT_EXIT, 11, CONDITION_ALWAYS, 11},
	{EVENT_EXIT, 12, CONDITION_ALWAYS, 12},
	{EVENT_EXIT, 13, CONDITION_ALWAYS, 13},
	{0x21, 14, CONDITION_ALWAYS, 14},
	{0x202, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_SCRIPT)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunStepWith6)},
	{TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_ApplyPointLeftOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS,
		EVENT_SCRIPT(BabiChika_RunLeaderSpray)},
	{0x202, 28, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeStepSequence)},
	{TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassPointTwoRightOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_ApplyPointLeftOfActorZero)},
	{0x202, 30, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunThreeStepSequence)},
	{TOUCH_WALKING_INTO(FACING_EAST), 29, CONDITION_ALWAYS, EVENT_SCRIPT(EV29_TOUCH)},
	{TOUCH_WALKING_INTO(FACING_SOUTH), 29, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassRaisedPointOfActorZero)},
	{TOUCH_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneActor_PassActorZeroOffsetPoint)},
	{0x202, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EV41_SCRIPT)},
	{SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_SEARCH)},
	{SEARCH(SEARCH_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(EV42_SEARCH)},
	{SEARCH(SEARCH_CHEST), 100, 0xf32, FIND_ITEM(0x71)},
	{SEARCH(SEARCH_CHEST), 101, 0xf33, FIND_ITEM(0x54)},
	{PSYNERGY_ON_ACTOR(0x8c), 8, 0x204, EVENT_SCRIPT(EV08_PSYNERGY)},
	{0x10002115, 8, 0x203, EVENT_SCRIPT(EV08_SCRIPT)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), 9, 0x204, EVENT_SCRIPT(EV09_PSYNERGY)},
	{PSYNERGY_ON_ACTOR(0x8c), 12, CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(0x8c), 13, CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(0x93), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunMiddleSequence)},
	{PSYNERGY_ON_ACTOR(0x93), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunMiddleSequence)},
	{PSYNERGY_ON_ACTOR(0x93), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunMiddleSequence)},
	{PSYNERGY_IN_AREA(ABILITY_REVEAL, 5), 0, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunLateSequenceSecond)},
	{PSYNERGY_IN_AREA(ABILITY_REVEAL, 4), 0, CONDITION_ALWAYS,
		EVENT_SCRIPT(FieldScene_RunLateSequenceHead)},
	{PSYNERGY_ON_ACTOR(0x8c), 17, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneState_SetSlot17And18Selectors)},
	{PSYNERGY_ON_ACTOR(0x8c), 18, CONDITION_ALWAYS,
		EVENT_SCRIPT(SceneState_SetSlot17And18Selectors)},
	{PSYNERGY_ON_ACTOR(0x8c), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c4SequenceA)},
	{PSYNERGY_ON_ACTOR(0x8c), 20, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c4SequenceA)},
	{PSYNERGY_ON_ACTOR(0x8c), 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c4SequenceA)},
	{PSYNERGY_ON_ACTOR(0x8c), 22, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c4SequenceA)},
	{PSYNERGY_ON_ACTOR(0x8c), 23, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3c4SequenceA)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
		MSGNO(MsgBabiChikaRedShinesNorthwestOfMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
		MSGNO(MsgBabiChikaYellowShinesNortheastOfMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
		MSGNO(MsgBabiChikaRedShinesDueWestOfMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS,
		MSGNO(MsgBabiChikaVioletShinesSoutheastOfMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS,
		MSGNO(MsgBabiChikaVioletPointsSoutheast)},
	{SCENE_EVENTS_END},
};
