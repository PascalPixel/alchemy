#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/field/korima_hashi/hashi.h"
#include "unit/field/common/object/staged_actor.h"
#include "unit/field/common/object/distance.h"
#include "unit/field/common/object/fillgrid.h"
#include "unit/field/korima_hashi/falling_effect.h"
#include "unit/battle/effect/slots.h"
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
#include "field_event.h"
#include "field_scene.h"
#include "korima_mura.h"
#include "staged_actor.h"
#include "staged_actor_effect.h"
#include "text/msg_ids.h"
#include "objcmd.h"

struct Struct3848
{
	u8	pad00[8];
	u32 field08;
	s32 field0c;
	u32 field10;
};

struct Struct2798
{
	u8	pad00[0x18];
	s32 field18;
	u8	pad1c[0x38 - 0x1c];
	s32 field38;
	s32 field3c;
	s32 field40;
};

struct Sub
{
	u8 pad00[9];
	u8 f09;
	u8 pad0a[28];
	u8 f26;
};

struct Obj
{
	u8			pad00[0x18];
	s32			f18;
	u8			pad1c[7];
	u8			f23;
	u8			pad24[12];
	s32			f30;
	s32			f34;
	u8			pad38[24];
	struct Sub *f50;
	u8			pad54[1];
	u8			f55;
};

struct Struct288c
{
	u8	pad00[8];
	s32 field08;
	u8	pad0c[4];
	s32 field10;
};

extern s32 KorimaHashi_TriggerPending;
extern s32 KorimaHashi_PartyFlag;
extern s32 KorimaHashi_SparkleSound;

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/object/staged_actor.c"

s32 OverlayObject_ClearPendingAndRestoreMode(u8 *object)
{
	s32 *pending = &KorimaHashi_TriggerPending;
	if (*pending)
	{
		Object_SetMode(object, 2);
		*pending = 0;
	}
	return 1;
}

s32 SceneActor_UpdateRandomCounterMode(u8 *object)
{
	u16 *counter = (u16 *)(object + 100);
	*counter = (*counter + (((u32)Engine_RandomNext() * 100) >> 16));
	if ((s16)*counter > 1000)
		ObjectGroup_SetChildValue(object, 7);
	else
		ObjectGroup_SetChildValue(object, 10);
	if (*(s16 *)counter > 1200)
	{
		u16 z = 0;
		*counter = z;
	}
	return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	return KorimaHashi_Entrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return KorimaHashi_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	return KorimaHashi_Placements;
}

void FieldScene_RunTile10x20Transition(void)
{
	struct StagedActorProbe res;
	Engine_EventBegin();

	if (StagedActor_FindClearPosition(&res))
	{
		SceneActor_MoveAndRedraw(res);
		if (res.actor_slot == 10 && (res.position_x >> 20) == 20)
		{
			u8 *actor;
			s32 zero;

			Engine_ActorSetAnimation(10, 3);
			Actor_SetDestinationOffset(10, -18, 6);
			Engine_EventWait(30);
			Engine_AudioPlayCue(240);
			Engine_ActorSetAnimation(10, 8);
			Object_GetById(10)->priority_flags = 2;
			zero = 0;
			Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
			StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
			Engine_GameFlagSet(0x200);
			actor = (void *)Object_GetById(10);
			Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
		}
	}

	Engine_EventEnd();
}

s32 StagedActor_RunStepEffect(struct StagedActorEffectRequest *request)
{
	struct StagedActorEffect *actor = (void *)Object_GetById(0);
	u8						 *flags = &actor->motion_flags;
	u8						  saved = *flags;
	s32						  ret = Object_CheckMovementCollision(actor, request);

	if (ret == 0)
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
		Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, request->cell_x, request->cell_z);
		Object_SetMode((struct FieldActor *)actor, 6);
		Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 1);
		*flags = ret;
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

void SceneActor_PassSubjectOffsetPosition(void)
{
	u32 buf[3];
#if !EDITION_INTERNATIONAL
	buf[0] = ((struct Struct3848 *)Object_GetById(0))->field08 + 0x1e0000;
	buf[1] = ((struct Struct3848 *)Object_GetById(0))->field0c;
	buf[2] = ((struct Struct3848 *)Object_GetById(0))->field10;
#else
	s32				   off = 500;
	struct Struct3848 *p = (void *)Object_GetById(*(s32 *)((u8 *)&gGameState + off));
	u32				   base = p->field08 & 0xfff00000;

	buf[0] = base + 0x80000;
	buf[1] = p->field0c;
	buf[2] = (p->field10 & 0xfff00000) + 0x80000;
	buf[0] = base + 0x280000;
#endif
	StagedActor_RunStepEffect((struct StagedActorEffectRequest *)buf);
}

const struct SceneEvent *Scene_GetEvents(void)
{
	return KorimaHashi_Events;
}

s32 Scene_Initialize(void)
{
	s32 record;
	s32 zero;

	FieldScene_RedrawActorFootprint(10);
	if (Engine_GameFlagIsSet(0x200) != 0)
	{
		zero = 0;
		Object_GetById(10)->priority_flags = 2;
		Map_CopyCellAttributes(0, 17, 2, 4, 19, 17);
		record = StagedActor_FillGridAttributeRectangle(2, 20, 17, 1, 4, zero);
		record = (s32)Object_GetById(10);
		Engine_ActorSetSpriteFlags((struct FieldActor *)record, 0);
	}
	FieldScene_RedrawActorFootprint(8);
	FieldScene_RedrawActorFootprint(9);
	if (gGameState.entrance == 4)
	{
		if (Engine_GameFlagIsSet(0x843) == 0)
		{
			FieldScene_RunBranchingFormationPresentation();
		}
	}
	if (Engine_GameFlagIsSet(0x845) != 0)
	{
		Engine_ActorSetPosition(17, 0, 0);
		Engine_ActorSetPosition(18, 0, 0);
		Engine_ActorSetPosition(19, 0, 0);
		Engine_ActorSetPosition(20, 0, 0);
		Engine_ActorSetPosition(21, 0, 0);
	}
	return 0;
}

void FieldScene_RunBranchingFormationPresentation(void)
{
	struct FieldActor *record;
	s32				  *party;
	const u8		  *entry_action;
	const u8		  *motion_action;
	s32				  *party_flag;
	const u8		  *reset_action;
	const u8		  *formation_action;
	const u8		  *finish_action;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Camera_MoveTo(0xf60000, -1, 0x25c0000, 0);
	party = &KorimaHashi_PartyFlag;
	*party = GameFlag_IsSet(3);
	record = Object_GetById(13);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(14);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(15);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(16);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(17);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(18);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(19);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(20);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(21);
	Engine_ActorSetSpriteFlags(record, 0);
	entry_action = KorimaHashi_EntryAction;
	Engine_ActorEnableActionCallback(17, entry_action);
	Engine_ActorEnableActionCallback(18, entry_action);
	Engine_ActorEnableActionCallback(19, entry_action);
	Engine_ActorEnableActionCallback(20, entry_action);
	Engine_ActorEnableActionCallback(21, entry_action);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x740000, 0x25a0000);
	Engine_TaskWait(1);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 254, 0x251);
	Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
	Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
	{
		struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

		if (leader != 0)
		{
			Actor_SetPosition(GERALD, leader->x.fixed, leader->z.fixed);
		}
	}
	{
		struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

		if (leader != 0)
		{
			Actor_SetPosition(IWAN, leader->x.fixed, leader->z.fixed);
		}
	}
	Engine_ActorEnableActionCallback(GERALD, KorimaHashi_GeraldAction);
	Engine_ActorEnableActionCallback(IWAN, KorimaHashi_IvanAction);
	if (*party != 0)
	{
		Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
		{
			struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

			if (leader != 0)
			{
				Actor_SetPosition(MEARI, leader->x.fixed, leader->z.fixed);
			}
		}
		Engine_ActorEnableActionCallback(MEARI, KorimaHashi_MiaAction);
	}
	Object_RefreshSelectorById(2);
	SceneActor_SetPairZeroAndValue(2, 0x2000, 40);
	SceneActor_SetPairZeroAndValue(2, 0x8000, 20);
	SceneActor_SetPairZeroAndValue(2, 0x4000, 40);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
	SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventSetMessage((s32)MsgKorimaMatterIvan);
	Event_SayThenWait(GERALD, 10);
	Engine_EventSetMessage((s32)MsgKorimaSparklyStuffOnGround);
	SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(IWAN, 20);
	SceneActor_SetPairZeroAndValue(1, 0, 20);
	SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
	SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 30);
	SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
	SceneActor_SetPairZeroAndValue(0, 0xe000, 30);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
	SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
	Event_PlayCue(17);
	Event_PlayCue(206);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(1);
	KorimaHashi_SparkleSound = 1;
	Engine_TaskAddCallback(SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
	Engine_TaskWait(20);
	ColorBuffer_ApplyTarget(0x405210, 1);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(120);
	Engine_TaskWait(60);
	motion_action = KorimaHashi_PartyAction;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, motion_action);
	Engine_ActorEnableActionCallback(GERALD, motion_action);
	Engine_ActorEnableActionCallback(IWAN, motion_action);
	Engine_ActorEnableActionCallback(MEARI, motion_action);
	Engine_EventWait(100);
	Event_SayThenWait(GERALD, 20);
	Event_SayThenWait(IWAN, 40);
	if (KorimaHashi_PartyFlag != 0)
	{
		Engine_EventWait(40);
		Actor_ShowEmote(MEARI, 0x102, 0);
		Engine_EventWait(40);
		Event_SayThenWait(MEARI, 40);
	}
	else
	{
		gEventWork->message += 1;
	}
	Engine_EventWait(20);
	if (KorimaHashi_PartyFlag != 0)
	{
		record = Object_GetById(3);
		record->velocity_y = 0x20000;
		Engine_EventWait(10);
		Actor_SetSpeed(MEARI, 0x20000, 0x20000);
		Actor_SetDestinationOffset(MEARI, -2, 0);
		Engine_ActorEnableActionCallback(MEARI, KorimaHashi_ResetAction);
		record = Object_GetById(3);
		Engine_ActorSetSpriteFlags(record, 0);
		Engine_ActorSetAnimation(MEARI, 19);
		Engine_EventWait(10);
	}
	record = Object_GetById(0);
	record->velocity_y = 0x20000;
	Engine_EventWait(10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x20000);
	reset_action = KorimaHashi_ResetAction;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, reset_action);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_EventWait(20);
	record = Object_GetById(1);
	record->velocity_y = 0x20000;
	Engine_EventWait(10);
	Actor_SetSpeed(GERALD, 0x20000, 0x20000);
	Engine_ActorEnableActionCallback(GERALD, reset_action);
	record = Object_GetById(1);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_ActorSetAnimation(GERALD, 19);
	Engine_EventWait(40);
	record = Object_GetById(2);
	record->velocity_y = 0x20000;
	Engine_EventWait(10);
	Engine_ActorEnableActionCallback(IWAN, reset_action);
	record = Object_GetById(2);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_ActorSetAnimation(IWAN, 19);
	KorimaHashi_SparkleSound = 0;
	Engine_EventWait(160);
	Engine_TaskRemoveCallback(SceneEffect_SpawnObject26EveryEightFrames);
	Engine_EventWait(120);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	gFallingEffectWidth = 0;
	gFallingEffectOffset = 0x800000;
	gFallingEffectState = 1;
	Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
	Engine_EventWait(180);
	Event_PlayCue(21);
	Event_SayThenWait(GERALD, 80);
	Event_SayThenWait(IWAN, 40);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(IWAN, 0x102, 0);
	Actor_ShowEmote(MEARI, 0x102, 0);
	Engine_EventWait(60);
	Event_SayThenWait(IWAN, 20);
	gFallingEffectState = 2;
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 1);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(IWAN, 3);
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Event_SayThenWait(GERALD, 20);
	if (KorimaHashi_PartyFlag != 0)
	{
		Actor_ShowEmote(MEARI, 0x102, 0);
		Event_SayThenWait(MEARI, 10);
	}
	else
	{
		gEventWork->message += 1;
	}
	gFallingEffectState = 3;
	Object_GetById(0)->priority_flags &= 254;
	Object_GetById(1)->priority_flags &= 254;
	Object_GetById(2)->priority_flags &= 254;
	Object_GetById(3)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetSpritePriority(GERALD, 3);
	Engine_ActorSetSpritePriority(IWAN, 3);
	Engine_ActorSetSpritePriority(MEARI, 3);
	KorimaHashi_TriggerPending = 0;
	Engine_TaskAddCallback(KorimaHashi_AlternateBridgePostHeights, 0xc80);
	Event_PlayCue(220);
	Object_GetById(13)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(13, 2);
	Actor_SetPosition(13, 0xfd0000, 0x25b0000);
	formation_action = KorimaHashi_FormationAction;
	Engine_ActorEnableActionCallback(13, formation_action);
	Object_GetById(14)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(14, 2);
	Actor_SetPosition(14, 0xe90000, 0x2750000);
	Engine_ActorEnableActionCallback(14, formation_action);
	if (KorimaHashi_PartyFlag != 0)
	{
		Object_GetById(15)->priority_flags &= 254;
		Engine_ActorSetSpritePriority(15, 2);
		Actor_SetPosition(15, 0xcf0000, 0x2610000);
		Engine_ActorEnableActionCallback(15, formation_action);
	}
	Object_GetById(16)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(16, 2);
	Actor_SetPosition(16, 0xe30000, 0x2440000);
	Engine_ActorEnableActionCallback(16, formation_action);
	if (gFallingEffectState != 0)
	{
		do
		{
			Engine_TaskWait(1);
		} while (gFallingEffectState != 0);
	}
	Engine_EventWait(0x12c);
	Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
	Engine_EventWait(120);
	Event_PlayCue(17);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	Engine_ActorStop(13);
	Engine_ActorStop(14);
	if (KorimaHashi_PartyFlag != 0)
	{
		Engine_ActorStop(15);
	}
	Engine_ActorStop(16);
	Engine_TaskWait(1);
	finish_action = KorimaHashi_FinishAction;
	Engine_ActorEnableActionCallback(13, finish_action);
	Engine_ActorEnableActionCallback(14, finish_action);
	if (KorimaHashi_PartyFlag != 0)
	{
		Engine_ActorEnableActionCallback(15, finish_action);
	}
	Object_SetActionCallbackAndRefreshById(16, finish_action);
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(40);
	Event_OpenMessage(GERALD, 0);
	Actor_SetPosition(11, 0xdc0000, 0x1ee0000);
	Actor_SetPosition(12, 0xdc0000, 0x1ee0000);
	Engine_TaskWait(1);
	if (Engine_EventChooseYesNo(11, 0) == 1)
	{
		gEventWork->message += 1;
	}
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Event_SayThenWait(IWAN, 20);
	if (KorimaHashi_PartyFlag != 0)
	{
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		Engine_EventWait(10);
		Engine_EventSetMessage((s32)MsgKorimaKnowThoseFieldsWere);
		Event_SayThenWait(MEARI, 40);
	}
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventSetMessage((s32)MsgKorimaWasOurPsynergy);
	Event_SayThenWait(IWAN, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(40);
	Engine_ActorSetSpritePriority(GERALD, 2);
	Object_GetById(1)->priority_flags |= 1;
	record = Object_GetById(1);
	Engine_ActorSetSpriteFlags(record, 1);
	Engine_ActorJump(GERALD, 6, 0);
	Actor_SetDestinationOffset(GERALD, -3, 0);
	Engine_ActorSetAnimation(GERALD, 1);
	SceneActor_SetPairZeroAndValue(1, 0x4000, 60);
	Event_SayThenWait(GERALD, 20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Event_SayThenWait(GERALD, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Engine_EventWait(40);
	SceneActor_SetPairZeroAndValue(1, 0x6000, 40);
	SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
	SceneActor_SetPairZeroAndValue(1, 0x6000, 20);
	SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
	Engine_ActorJump(GERALD, 2, 0);
	Engine_EventWait(40);
	Engine_ActorJump(GERALD, 2, 0);
	Engine_EventWait(10);
	Engine_ActorJump(GERALD, 4, 0);
	Engine_EventWait(20);
	Event_SayThenWait(GERALD, 20);
	if (KorimaHashi_PartyFlag != 0)
	{
		Actor_ShowEmote(MEARI, 0x100, 0);
		Engine_EventWait(60);
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		Engine_EventWait(80);
		Engine_ActorSetSpritePriority(MEARI, 2);
		Object_GetById(3)->priority_flags |= 1;
		record = Object_GetById(3);
		Engine_ActorSetSpriteFlags(record, 1);
		Engine_ActorJump(MEARI, 4, 0);
		Actor_SetDestinationOffset(MEARI, -2, 0);
		Engine_ActorSetAnimation(MEARI, 1);
		SceneActor_SetPairZeroAndValue(3, 0xe000, 60);
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		Engine_EventWait(20);
		Event_SayThenWait(MEARI, 20);
	}
	else
	{
		gEventWork->message += 1;
	}
	Engine_ActorJump(GERALD, 2, 0);
	SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
	Event_SayThenWait(GERALD, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Engine_ActorSetSpritePriority(IWAN, 2);
	Object_GetById(2)->priority_flags |= 1;
	record = Object_GetById(2);
	Engine_ActorSetSpriteFlags(record, 1);
	Engine_ActorJump(IWAN, 4, 0);
	Engine_ActorSetAnimation(IWAN, 1);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(10);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Object_GetById(0)->priority_flags |= 1;
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 60);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(60);
	SceneActor_SetPairZeroAndValue(0, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
	SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(IWAN, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	Event_OpenMessage(IWAN, 0);
	Engine_ActorSetAnimation(IWAN, 3);
	Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimation(IWAN, 3);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		gEventWork->message += 1;
	}
	else
	{
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
		Engine_ActorStartRepeatedMotion(GERALD, 2);
		Event_ShowMessage(GERALD, 0);
	}
	SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Event_SayThenWait(GERALD, 20);
	SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(IWAN, 10);
	if (KorimaHashi_PartyFlag != 0)
	{
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		SceneActor_SetPairZeroAndValue(3, 0, 20);
		SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
		Engine_ActorSetAnimation(MEARI, 4);
		Event_SayThenWait(MEARI, 10);
	}
	else
	{
		gEventWork->message += 1;
	}
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	SceneActor_SetPairZeroAndValue(0, 0xa000, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	SceneActor_SetPairZeroAndValue(1, 0x4000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Engine_EventWait(80);
	SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	Event_SayThenWait(IWAN, 20);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	SceneActor_SetPairZeroAndValue(0, 0xa000, 40);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 10);
	SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(IWAN, 10);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(40);
	Event_SayThenWait(GERALD, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Engine_EventWait(40);
	Event_SayThenWait(GERALD, 20);
	Engine_ActorSetAnimation(IWAN, 3);
	Event_SayThenWait(IWAN, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	SceneActor_SetPairZeroAndValue(1, 0x2000, 10);
	Event_OpenMessage(GERALD, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorSetAnimationAndWait(GERALD, 3);
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Engine_EventWait(40);
		gEventWork->message += 1;
	}
	Event_ShowMessage(GERALD, 0);
	Event_PlayCue(21);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	gFallingEffectWidth = 0;
	gFallingEffectOffset = 0x800000;
	gFallingEffectState = 1;
	Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
	Engine_EventWait(80);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(60);
	SceneActor_SetPairZeroAndValue(2, 0xc000, 10);
	Engine_EventSetMessage((s32)MsgKorimaWatchOutItsHappeningAgain);
	Event_SayThenWait(IWAN, 10);
	SceneActor_SetPairZeroAndValue(1, 0xc000, 10);
	SceneActor_SetPairZeroAndValue(0, 0xc000, 10);
	if (KorimaHashi_PartyFlag != 0)
	{
		SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
	}
	Object_GetById(0)->priority_flags &= 254;
	Object_GetById(1)->priority_flags &= 254;
	Object_GetById(2)->priority_flags &= 254;
	Object_GetById(3)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetSpritePriority(GERALD, 3);
	Engine_ActorSetSpritePriority(IWAN, 3);
	Engine_ActorSetSpritePriority(MEARI, 3);
	gFallingEffectState = 2;
	Event_PlayCue(220);
	Actor_SetPosition(13, 0xfd0000, 0x25b0000);
	formation_action = KorimaHashi_FormationAction;
	Engine_ActorEnableActionCallback(13, formation_action);
	Actor_SetPosition(14, 0xe90000, 0x2750000);
	Engine_ActorEnableActionCallback(14, formation_action);
	if (KorimaHashi_PartyFlag != 0)
	{
		Actor_SetPosition(15, 0xcf0000, 0x2610000);
		Engine_ActorEnableActionCallback(15, formation_action);
	}
	Actor_SetPosition(16, 0xe30000, 0x2440000);
	Engine_ActorEnableActionCallback(16, formation_action);
	Engine_EventWait(120);
	gFallingEffectState = 3;
	do
	{
		Engine_TaskWait(1);
	} while (gFallingEffectState != 0);
	Event_SayThenWait(11, 80);
	Event_SayThenWait(12, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 0);
	Engine_EventWait(60);
	Event_SayThenWait(12, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(12, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(12, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Engine_EventWait(40);
	Event_SayThenWait(11, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(MEARI, FACING_EAST, 0);
	SceneActor_SetPairZeroAndValue(2, 0xc000, 40);
	Event_SayThenWait(12, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(IWAN, 0x102, 0);
	Actor_ShowEmote(MEARI, 0x102, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	SceneActor_SetPairZeroAndValue(3, 0xc000, 80);
	Event_SayThenWait(12, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	SceneActor_SetPairZeroAndValue(3, 0, 40);
	Event_SayThenWait(11, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
	Engine_ActorSetAnimation(GERALD, 4);
	Engine_ActorSetAnimation(MEARI, 4);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(60);
	Event_SayThenWait(12, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Event_SayThenWait(12, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	SceneActor_SetPairZeroAndValue(3, 0, 20);
	Event_SayThenWait(12, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	Event_SayThenWait(12, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	SceneActor_SetPairZeroAndValue(3, 0, 20);
	Event_SayThenWait(11, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(MEARI, 0x102, 0);
	Actor_ShowEmote(IWAN, 0x102, 0);
	Engine_EventWait(40);
	Event_SayThenWait(12, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	SceneActor_SetPairZeroAndValue(3, 0xc000, 10);
	Event_SayThenWait(12, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(60);
	Event_ShowMessage(12, 0);
	Event_ShowMessage(11, 0);
	Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
	Engine_EventWait(80);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(80);
	Engine_ActorStop(13);
	Engine_ActorStop(14);
	party_flag = &KorimaHashi_PartyFlag;
	Engine_ActorStop(15);
	Engine_ActorStop(16);
	Engine_TaskWait(1);
	finish_action = KorimaHashi_FinishAction;
	Engine_ActorEnableActionCallback(13, finish_action);
	Engine_ActorEnableActionCallback(14, finish_action);
	if (*party_flag != 0)
	{
		Engine_ActorEnableActionCallback(15, finish_action);
	}
	Object_SetActionCallbackAndRefreshById(16, finish_action);
	Engine_EventWait(20);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetSpritePriority(GERALD, 2);
	Engine_ActorSetSpritePriority(IWAN, 2);
	Engine_ActorSetSpritePriority(MEARI, 2);
	Object_GetById(0)->priority_flags |= 1;
	Object_GetById(1)->priority_flags |= 1;
	Object_GetById(2)->priority_flags |= 1;
	Object_GetById(3)->priority_flags |= 1;
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
	Event_OpenMessage(IWAN, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	Actor_FaceDirection(MEARI, FACING_EAST, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Engine_EventWait(10);
		Event_OpenMessage(GERALD, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			SceneActor_SetPairZeroAndValue(3, 0, 20);
			Actor_ShowEmote(GERALD, 0x101, 0);
			Actor_ShowEmote(IWAN, 0x101, 0);
			Actor_ShowEmote(MEARI, 0x101, 0);
			Engine_EventWait(40);
			SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
			Event_SayThenWait(GERALD, 10);
			SceneActor_SetPairZeroAndValue(2, 0xc000, 20);
			SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
			Engine_ActorSetAnimationAndWait(IWAN, 3);
			Event_SayThenWait(IWAN, 20);
			SceneActor_SetPairZeroAndValue(1, 0x2000, 20);
		}
		else
		{
			SceneActor_SetPairZeroAndValue(3, 0, 20);
			Actor_ShowEmote(GERALD, 0x102, 0);
			Actor_ShowEmote(IWAN, 0x102, 0);
			Actor_ShowEmote(MEARI, 0x102, 0);
			Engine_EventWait(40);
			SceneActor_SetPairZeroAndValue(1, 0x4000, 20);
			Engine_EventSetMessage((s32)MsgKorimaHesAsStumpedAsWe);
			Event_SayThenWait(GERALD, 20);
			Engine_ActorSetAnimationAndWait(IWAN, 3);
			Event_SayThenWait(IWAN, 20);
		}
		Engine_ActorSetAnimation(MEARI, 3);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		goto ivan_bows;
	}
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgKorimaYoureRobinThereIsntMuch);
	Event_SayThenWait(GERALD, 10);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	SceneActor_SetPairZeroAndValue(0, 0x6000, 20);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(10);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Event_OpenMessage(IWAN, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Actor_ShowEmote(IWAN, 0x103, 0);
		Engine_EventWait(40);
		SceneActor_SetPairZeroAndValue(2, 0xe000, 10);
		Event_SayThenWait(IWAN, 10);
		if (*party_flag != 0)
		{
			SceneActor_SetPairZeroAndValue(3, 0, 10);
			Engine_ActorStartRepeatedMotion(MEARI, 3);
			Event_SayThenWait(MEARI, 20);
		}
		else
		{
			gEventWork->message += 1;
		}
		Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
		Actor_ShowEmote(GERALD, 0x102, 0);
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Event_SayThenWait(GERALD, 20);
		Actor_ShowEmote(GERALD, 0x105, 0);
		Engine_EventWait(120);
		Event_SayThenWait(IWAN, 40);
		if (KorimaHashi_PartyFlag != 0)
		{
			SceneActor_SetPairZeroAndValue(3, 0x2000, 10);
			Engine_ActorSetAnimationAndWait(MEARI, 4);
			Event_SayThenWait(MEARI, 10);
		}
		else
		{
			gEventWork->message += 1;
		}
		Engine_EventWait(60);
		Engine_ActorRunRepeatedMotion(IWAN, 2);
		if (KorimaHashi_PartyFlag != 0)
		{
			SceneActor_SetPairZeroAndValue(2, 0xa000, 40);
			SceneActor_SetPairZeroAndValue(2, 0xe000, 20);
		}
		Event_SayThenWait(IWAN, 10);
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Engine_EventWait(40);
		Event_SayThenWait(IWAN, 20);
		Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		Engine_EventWait(20);
		Engine_ActorSetAnimation(MEARI, 3);
	ivan_bows:;
		Engine_ActorSetAnimationAndWait(IWAN, 3);
		goto leave;
	}
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventSetMessage((s32)MsgKorimaThatsReliefRobinThoughtYoud);
	Event_SayThenWait(IWAN, 20);
	if (*party_flag != 0)
	{
		SceneActor_SetPairZeroAndValue(3, 0, 10);
		Engine_ActorStartRepeatedMotion(MEARI, 1);
		Event_SayThenWait(MEARI, 20);
	}
	else
	{
		gEventWork->message += 1;
	}
	Actor_ShowEmote(GERALD, 0x102, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Event_SayThenWait(GERALD, 20);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(80);
	Event_SayThenWait(IWAN, 40);
	if (KorimaHashi_PartyFlag != 0)
	{
		SceneActor_SetPairZeroAndValue(3, 0x2000, 20);
		Engine_ActorSetAnimation(MEARI, 4);
		Event_SayThenWait(MEARI, 40);
	}
	else
	{
		gEventWork->message += 1;
	}
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	Event_SayThenWait(IWAN, 20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(40);
	Event_SayThenWait(IWAN, 20);
leave:;
	Event_PlayCue(17);
	Actor_SetSpeed(GERALD, 0x13333, 0x9999);
	Actor_SetSpeed(IWAN, 0x13333, 0x9999);
	Actor_SetSpeed(MEARI, 0x13333, 0x9999);
	Engine_ActorSetAnimation(GERALD, 2);
	{
		struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

		if (leader != 0)
		{
			Actor_SetDestination(GERALD, leader->x.part.pixel, leader->z.part.pixel);
		}
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_ActorSetAnimation(IWAN, 2);
	{
		struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

		if (leader != 0)
		{
			Actor_SetDestination(IWAN, leader->x.part.pixel, leader->z.part.pixel);
		}
	}
	Engine_ActorWaitForMove(IWAN);
	Actor_SetPosition(IWAN, 0, 0);
	if (KorimaHashi_PartyFlag != 0)
	{
		Engine_ActorSetAnimation(MEARI, 2);
		{
			struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

			if (leader != 0)
			{
				Actor_SetDestination(MEARI, leader->x.part.pixel, leader->z.part.pixel);
			}
		}
		Engine_ActorWaitForMove(MEARI);
		Actor_SetPosition(MEARI, 0, 0);
	}
	GameFlag_Set(0x843);
	Actor_SetPosition(11, 0, 0);
	Actor_SetPosition(12, 0, 0);
	Audio_PlayCueFromEventWork();
	Engine_EventEnd();
}

void Event_SayThenWait(s32 speaker, s32 frames)
{
	Engine_EventShowMessage(speaker, 0);
	Engine_EventWait(frames);
}

void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
	Engine_ActorFaceDirection(a, b, 0);
	Engine_EventWait(c);
}

s32 SceneEffect_AdvanceAngleAndFinishWhenParked(struct Struct2798 *p)
{
	p->field18 += 0x1eb8;
	if (p->field38 == 0x80000000 && p->field3c == 0x80000000 && p->field40 == 0x80000000)
	{
		Engine_ObjectDispatchRelease(p);
	}
	return 1;
}

void SceneEffect_SpawnObject26EveryEightFrames(void)
{

	struct Obj *obj;
	struct Sub *sprite;
	s32			phase;
	s32			v;
	s32			w;
	s32			c1 = 0x00e70000;
	s32			c2 = 0x01cc0000;
	s32			c3 = 0x00e70000;
	s32			c4 = 0x02700000;

	phase = gFrameCount & 7;
	if (phase != 0)
		return;
	if (KorimaHashi_SparkleSound != 0)
		Engine_AudioPlayCue(200);
	obj = (struct Obj *)Engine_ObjectCreate(26, c1, 0, c2);
	if (obj == 0)
		return;
	sprite = obj->f50;
	sprite->f26 = phase;
	v = 0xfe;
	v &= obj->f23;
	obj->f23 = v;
	w = ~12;
	w &= sprite->f09;
	w |= 4;
	sprite->f09 = w;
	obj->f18 = 0x1999;
	obj->f30 = 0x80000;
	obj->f34 = 0x80000;
	obj->f55 = phase;
	Object_SetMode((struct FieldActor *)obj, 2);
	Engine_ObjectSetPosition((struct FieldActor *)obj, c3, 0, c4);
	Engine_ObjectSetScript((struct FieldActor *)obj, (const s32 *)KorimaHashi_Object26Script);
}

s32 OverlayObject_SelectValueByFrameBit1(s32 obj)
{

	if (((gFrameCount >> 1) & 1) != 0)
	{
		ObjectGroup_SetChildValue(obj, 10);
	}
	else
	{
		ObjectGroup_SetChildValue(obj, 7);
	}
	return 0;
}

s32 SceneActor_CheckRegionTrigger(struct Struct288c *arg0)
{
	s32 x;

	if (KorimaHashi_PartyFlag != 0)
	{
		x = arg0->field08;
		if (x > 0xc00000 && x < 0x1120000 && arg0->field10 > 0x2360000 && arg0->field10 < 0x2640000)
		{
			goto hit;
		}
		if (x > 0xca0000 && x < 0xff0000 && arg0->field10 > 0x2250000 && arg0->field10 < 0x2780000)
		{
			goto hit;
		}
	}
	else
	{
		x = arg0->field08;
		if (x > 0xc00000 && x < 0xf40000 && arg0->field10 > 0x2250000 && arg0->field10 <= 0x248ffff)
		{
			goto hit;
		}
		if (x > 0xf40000 && x < 0x1120000 && arg0->field10 > 0x23b0000
			&& arg0->field10 <= 0x25cffff)
		{
			goto hit;
		}
		if (x > 0xd30000 && x < 0xff0000 && arg0->field10 > 0x2540000 && arg0->field10 < 0x2780000)
		{
			goto hit;
		}
	}
	return 0;
hit:
	Engine_AudioPlayCue(106);
	((s32 (*)())Engine_ObjectSetScript)((s32)arg0, (s32)KorimaHashi_TriggerScript);
	KorimaHashi_TriggerPending = 1;
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "falling_effect.c"
s32 KorimaHashi_TriggerPending __attribute__((section(".bss")));
s32 KorimaHashi_PartyFlag __attribute__((section(".bss")));
s32 KorimaHashi_SparkleSound __attribute__((section(".bss")));

void KorimaHashi_AlternateBridgePostHeights(void)
{
	struct FieldActor *p;

	p = Object_GetById(13);
	if (p != 0)
	{
		p->motion_flags = 0;
		if ((gFrameCount & 1) == 0)
		{
			p->y.fixed = 0;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
	p = Object_GetById(14);
	if (p != 0)
	{
		p->motion_flags = 0;
		if ((gFrameCount & 1) != 0)
		{
			p->y.fixed = 0;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
	p = Object_GetById(15);
	if (p != 0)
	{
		p->motion_flags = 0;
		if ((gFrameCount & 1) == 0)
		{
			p->y.fixed = 0;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
	p = Object_GetById(16);
	if (p != 0)
	{
		p->motion_flags = 0;
		if ((gFrameCount & 1) != 0)
		{
			p->y.fixed = 0;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
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

s32 KorimaHashi_GeraldAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(234), 0, PIXELS(576)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_ARG(1),
	SC_END,
};

s32 KorimaHashi_IvanAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(234), 0, PIXELS(623)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_ARG(1),
	SC_END,
};

s32 KorimaHashi_MiaAction[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(211), 0, PIXELS(602)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH),
	SC_ARG(1),
	SC_END,
};

s32 KorimaHashi_PartyAction[] = {
	SC_ADD(SW_SCALE_X, -0x4000),
	SC_ADD(SW_SCALE_Y, 0x2000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x2000),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, 0x1000),
	SC_ADD(SW_SCALE_Y, -2048),
	SC_WAIT(2),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaHashi_ResetAction[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(1),
	SC_END,
};

s32 KorimaHashi_FormationAction[] = {
	SC_SET(SW_SCALE_X, 0x1999),
	SC_SET(SW_SCALE_Y, 0x1999),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 1310),
	SC_ADD(SW_SCALE_Y, 1310),
	SC_WAIT(1),
	SC_LOOP(70, SC_TO(1)),
	SC_CALL(OverlayObject_ClearPendingAndRestoreMode),
	SC_END,
};

s32 KorimaHashi_FinishAction[] = {
	SC_ARG(1),
	SC_ADD(SW_Z, -0x1999),
	SC_ADD(SW_SCALE_X, -983),
	SC_ADD(SW_SCALE_Y, -983),
	SC_WAIT(1),
	SC_LOOP(90, SC_TOP),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

s32 KorimaHashi_EntryAction[] = {
	SC_SET(SW_SCALE_X, 0xc000),
	SC_SET(SW_SCALE_Y, 0xc000),
	SC_SET(SW_COLLISION, 0),
	SC_SET(0x23, 0),
	SC_LABEL(1),
	SC_CALL(SceneActor_UpdateRandomCounterMode),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance KorimaHashi_Entrances[] = {
	{ 0, CONDITION_ALWAYS, 384, 0, 312, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 24, 0, 128, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 172, 0, 602, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance KorimaHashi_Exits[] = {
	{ 40, 0x0, 4137, 16, 4136, 0x20, 4136, 48, -32766, 64, 511, 0 },
};

struct SceneEntrance KorimaHashi_Placements[] = {
	{ 1, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0 },
	{ 2, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0 },
	{ 3, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 0, 0 },
	{ 205, CONDITION_ALWAYS, 7, 0, 0, 0xe8, 0, 0, 0, 128, 16384, 2 },
	{ 207, CONDITION_ALWAYS, 7, 0, 0, 0xd0, 0, 0, 0, 296, 16384, 2 },
	{ 205, CONDITION_ALWAYS, 7, 0, 0, 0x168, 0, 0, 0, 304, 16384, 2 },
	{ 55, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0 },
	{ 56, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 0 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256 },
	{ 220, CONDITION_ALWAYS, 1, 0, 0, FACING_EAST, 0, 0, 0, 0, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0xe8, 0, 0, 0, 655, 16384, 0 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0xa6, 0, 0, 0, 576, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0xbd, 0, 0, 0, 532, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0x120, 0, 0, 0, 570, 16384, 256 },
	{ 285, CONDITION_ALWAYS, 7, 0, 0, 0x14b, 0, 0, 0, 659, 16384, 256 },
	{ SCENE_TABLE_END },
};

struct SceneEvent KorimaHashi_Events[] = {
	{ 0x202, 10, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunTile10x20Transition) },
	{ TOUCH_WALKING_INTO(FACING_EAST), 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_PassSubjectOffsetPosition) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ SCENE_EVENTS_END },
};

s32 KorimaHashi_Object26Script[] = {
	SC_CALL(SceneEffect_AdvanceAngleAndFinishWhenParked),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaHashi_TriggerScript[] = {
	SC_SET(SW_SCALE_X, 0x20000),
	SC_SET(SW_SCALE_Y, 0x20000),
	SC_WAIT(10),
	SC_FREE,
};

s32 gFallingEffectScript[] = {
	SC_ADD(SW_Y, PIXELS(-2)),
	SC_CALL(OverlayObject_SelectValueByFrameBit1),
	SC_WAIT(1),
	SC_ADD(SW_Y, PIXELS(-2)),
	SC_CALL(OverlayObject_SelectValueByFrameBit1),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_Y, PIXELS(-1)),
	SC_CALL(OverlayObject_SelectValueByFrameBit1),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(1)),
	SC_CALL(SceneActor_CheckRegionTrigger),
	SC_FREE,
};
