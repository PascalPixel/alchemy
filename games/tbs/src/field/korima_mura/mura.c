#include "game_state.h"
#include "global_cells.h"
#include "scene_ids.h"
#include "unit/field/korima_mura/mura.h"
#include "unit/field/korima_hashi/falling_effect.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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
#include "field_event.h"
#include "field_service.h"
#include "field_scene.h"
#include "korima_mura.h"
#include "object_runtime.h"
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "facing_object.h"

// 前方宣言
static void FieldScene_RunExtendedActorSequence(void);
static void FieldScene_RunPairedStepA(s32 arg0, s32 arg1);
static void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2);
static void SceneEffect_SpawnObject26EveryEightFrames(void);
static void FieldScene_RunFlag845And847Branches(void);

struct Obj
{
	s32 f00, f04, f08, f0c, f10, f14, f18, f1c;
	s32 f20, f24, f28, f2c, f30, f34, f38, f3c;
	s32 f40, f44, f48, f4c, f50, f54, f58, f5c;
	s32 f60;
	u16 f64;
};

struct Sub
{
	u8 pad00[9];
	u8 f09;
	u8 pad0a[28];
	u8 f26;
};

static s32 KorimaMura_ApplyPendingPose(s32 arg0)
{
	if (KorimaMura_PendingPose != 0)
	{
		Object_SetMode(arg0, 2);
		KorimaMura_PendingPose = 0;
	}
	return 1;
}

static s32 SceneEffect_AdvanceCounterAndSwitchMode(struct Obj *p)
{
	s32 v = Engine_RandomNext();
	s32 t = v * 100;
	s32 h = p->f64 + ((u32)t >> 16);
	p->f64 = h;
	if ((s16)h > 1000)
	{
		ObjectGroup_SetChildValue(p, 7);
	}
	else
	{
		ObjectGroup_SetChildValue(p, 10);
	}
	if ((s16)p->f64 > 1200)
	{
		p->f64 = 0;
	}
	return 1;
}

static void SceneData_InitRecordTable(u8 *o)
{
	u8 *p = o + 72;
	u32 i;
	s32 normal;
	s32 special;

	i = 0;
	normal = 105;
	special = 110;
	for (; i <= 8; i++)
	{
		*(u16 *)p = normal;
		if ((i - 6) <= 1)
		{
			*(u16 *)p = special;
		}
		p[22] = 2;
		*(s32 *)(p + 4) = 1;
		p += 24;
	}
}

static s32 SceneEffect_UpdateFallingObject(struct Obj *p)
{
	p->f08 += p->f24;
	p->f10 += p->f2c;
	p->f2c -= 2621;
	p->f18 += 0x600;
	p->f1c += 0x600;
	{
		s32 t = p->f64 - 1;
		p->f64 = t;
		if ((u16)t == 0)
		{
			Engine_ObjectDispatchRelease(p);
		}
	}
	return 1;
}

static s32 OverlayObject_UpdateFacingTowardTarget(struct FacingObject *obj)
{
	s32					 delta;
	u16					 old;
	s32					 angle;
	struct FacingObject *target;

	target = obj->facing_target;
	if (target != 0)
	{
		obj->facing_flags = (0xFE & obj->facing_flags);
		angle = (u16)ArcTan2(
			target->position_z - obj->position_z, target->position_x - obj->position_x);
		old = obj->facing;
		delta = (s16)(angle - old);
		if (delta != 0)
		{
			if (delta > 0x1000)
			{
				delta = 0x1000;
			}

			if (delta < -0x1000)
			{
				delta = -0x1000;
			}
			obj->facing = (old + delta);
		}
	}
	return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KorimaMura3)
	{
		return gKorimaMuraEntrances3;
	}
	if (scene == (s32)&SceneId_KorimaMura2)
	{
		return gKorimaMuraEntrances2;
	}
	return gKorimaMuraEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
	if (gGameState.scene == (s32)&SceneId_KorimaMura2)
	{
		return gKorimaMuraRegions2;
	}
	return 0;
}

const u32 *Scene_GetExits(void)
{
	return gKorimaMuraExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s32 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KorimaMura1)
	{
		if (Engine_GameFlagIsSet(0x845) == 0)
		{
			SceneData_InitRecordTable(gKorimaMuraPlacements1);
		}
		return gKorimaMuraPlacements1;
	}
	if (scene == (s32)&SceneId_KorimaMura3)
	{
		return gKorimaMuraPlacements3;
	}
	return gKorimaMuraPlacements;
}

static void EV16_TALK(void)
{
	struct FieldActor *q = Object_GetById(0);
	s32				   v = q->facing;
	Engine_EventBegin();
	if (v >= 0xa001 && v <= 0xdfff)
	{
		Engine_SanctumOpen(16);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKorimaToldHolyTrees);
		Engine_EventAskYesNo(16, 0);
	}
	Engine_EventEnd();
}

static void EV27_TALK(void)
{
	BattleFx_RunPageEffectForSlot(27, 0, 1);
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_KorimaMura3)
	{
		return gKorimaMuraEvents3;
	}
	return gKorimaMuraEvents;
}

static void FieldScene_ConfigureActor0ThenRun(s32 a0)
{
	Object_GetById(0)->motion_flags = 0;
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, -8);
	gEventWork->transition_frames = 16;
	Engine_EventRequestExit(a0);
}

static void KorimaMura_AnimateSwitchOrExit(void)
{
	struct EventWork *work = gEventWork;
	s32				  x = 0;
	s32				  y = 0;

	Engine_EventBegin();
	Engine_AudioPlayCue(158);
	switch (work->touched_trigger)
	{
	case 5:
		x = 71;
		y = 9;
		break;
	case 6:
		x = 73;
		y = 17;
		break;
	case 7:
		x = 80;
		y = 21;
		break;
	case 8:
		x = 84;
		y = 12;
		break;
	case 9:
		Object_GetById(0)->motion_flags = 0;
		Actor_SetSpeed(0, 0x8000, 0x4000);
		Engine_ActorSetDestinationOffset(0, 0, 8);
		gEventWork->transition_frames = 16;
		Engine_EventRequestExit(9);
		Engine_EventEnd();
		return;
	}
	Engine_MapAnimateCells(KorimaMura_SwitchCells, x, y);
	FieldScene_ConfigureActor0ThenRun(work->touched_trigger);
	Engine_EventEnd();
}

static void EV10_TOUCH(void)
{
	u32 i;
	s32 v5;
	s32 v6;

	Engine_EventBegin();
	Camera_SetSpeed(0x20000, 0x4000);
	Camera_MoveTo(0xa80000, 0, 0xf60000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	Actor_ShowEmote(8, 0x100, 40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventSetMessage((s32)MsgKorimaGoingCrossBridge);
	Engine_EventShowMessage(8, 0);
	Engine_CameraSetSpeed(0x6666, 0xccc);
	Camera_MoveTo(0xa80000, 0, 0xea0000, 1);
	Actor_SetSpeed(0, 0x9999, 0x4ccc);
	Actor_WalkToAndWait(0, 174, 0x116);
	Actor_FaceDirection(0, FACING_NORTHEAST, 20);
	Engine_ActorSetAnimationAndWait(0, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimation(8, 3);
	Engine_EventShowMessage(8, 0);
	Actor_FaceDirection(8, 0x9000, 20);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(20);
	Object_GetById(8)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_SetSpeed(8, 0x20000, 0x10000);
	Engine_ActorJump(8, 2, 0);
	Engine_ObjectMotionSetPositionAndCommit(8, 224, 197);
	Engine_AudioPlayCue(176);
	Engine_EventWait(10);
	Engine_ObjectMotionSetPositionAndCommit(8, 234, 200);
	Engine_EventWait(10);
	Engine_AudioPlayCue(198);
	v5 = 5;
	v6 = 4;
	Engine_EventWait(30);
	Engine_MapCopyCellsTo(91, 0, 72, 9, v5, v6);
	Engine_EventWait(12);
	Engine_MapCopyCellsTo(91, 4, 72, 9, v5, v6);
	Engine_EventWait(9);
	Actor_FaceDirection(0, FACING_NORTH, 0);
	Engine_MapCopyCellsTo(91, 8, 72, 9, v5, v5);
	Engine_EventWait(6);
	Engine_MapCopyCellsTo(91, 13, 72, 9, v5, 6);
	Engine_EventWait(3);
	Engine_AudioPlayCue(188);
	for (i = 0; i < 10; i++)
	{
		struct FieldActor *obj = Engine_ObjectCreate(222, (148 + i * 4) << 16, 0, 0x1020000);

		if (obj != 0)
		{
			obj->motion_flags = 0;
			Engine_ActorSetSpriteFlags(obj, 0);
			((struct Flags9 *)obj->sprite)->mode = 0;
			{
				u16 delay = (((u32)(Engine_RandomNext() * 40)) >> 16) + 40;

				obj->timer = delay;
			}
			obj->velocity_x = (s32)(((i & 3) << 16) + 0x10000) >> 1;
			obj->velocity_z = 0x10000;
			if (i & 1)
			{
				obj->velocity_x = -((s32)(((i & 3) << 16) + 0x10000) >> 1);
			}
			Object_SetMode(obj, 1);
			Engine_ObjectSetScript(obj, (s32)KorimaMura_DebrisScript);
		}
	}
	Map_CopyCellsTo(91, 19, 72, 9, 5, 7);
	Map_CopyCellAttributes(23, 11, 5, 7, 8, 11);
	Work_SetValuesIfNonNegative(0, 0x40000, 0x10000);
	Engine_EventWait(10);
	Engine_ActorJump(0, 6, 0);
	Engine_EventWait(20);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	FieldObject_SetTargetAndCallback(8, 0x10000, KorimaMura_Actor8Path);
	Engine_EventWait(60);
	Engine_ActorShowEmote(0, 0x102, 60);
	Engine_GameFlagSet(0x847);
	Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
	s16 scene;
	u32 actor;
	u8 *tbl;
	s32 x;
	s32 y;

	scene = gGameState.scene;

	if (scene == (s32)&SceneId_KorimaMura3)
	{
		FieldScene_RunFlag845And847Branches();
		return 0;
	}

	if (scene == (s32)&SceneId_KorimaMura2)
	{
		gEventWork->start_transition = 0x204;
		return 0;
	}

	Engine_ActorSetSpriteFlags(Object_GetById(23), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(24), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(25), 0);
	Engine_ActorSetSpriteFlags(Object_GetById(26), 0);

	tbl = KorimaMura_EntryActions;
	Engine_ActorEnableActionCallback(23, tbl);
	Engine_ActorEnableActionCallback(24, tbl);
	Engine_ActorEnableActionCallback(25, tbl);
	Engine_ActorEnableActionCallback(26, tbl);

	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		for (actor = 8; actor <= 16; actor++)
		{
			Engine_ActorSetSpriteFlags(Object_GetById(actor), 0);
		}
		Engine_MapCopyCellAttributes(13, 9, 1, 1, 13, 8);
		Engine_MapCopyCellAttributes(13, 9, 1, 1, 15, 8);
		x = 14;
		y = 9;
		Engine_MapCopyCellAttributes(13, 9, 1, 1, x, y);
	}

	if (Engine_GameFlagIsSet(0x843) == 0)
	{
		if (gGameState.entrance == 1)
		{
			FieldScene_RunExtendedActorSequence();
		}
	}

	if (Engine_GameFlagIsSet(0x843) != 0)
	{
		Engine_ActorDestroy(GERALD);
		Engine_ActorDestroy(IWAN);
		Engine_ActorDestroy(MEARI);
		Engine_ActorDestroy(17);
		Engine_ActorDestroy(18);
		Engine_ActorDestroy(19);
		Engine_ActorDestroy(20);
		Engine_ActorDestroy(21);
		Engine_ActorDestroy(22);
		Event_CallWithLastActiveObjectId(KorimaMura_EntryScript);
	}

	return 0;
}

static void SceneActor_SetActors19To22HeightByFrameParity(void)
{
	struct FieldActor *p;

	p = Object_GetById(19);
	if (p != 0)
	{
		s32 m;
		p->motion_flags = 0;
		m = gFrameCount & 1;
		if (m == 0)
		{
			p->y.fixed = m;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
	p = Object_GetById(20);
	if (p != 0)
	{
		s32 z = 0;
		p->motion_flags = z;
		if (gFrameCount & 1)
		{
			p->y.fixed = z;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
	p = Object_GetById(21);
	if (p != 0)
	{
		s32 m;
		p->motion_flags = 0;
		m = gFrameCount & 1;
		if (m == 0)
		{
			p->y.fixed = m;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
	p = Object_GetById(22);
	if (p != 0)
	{
		s32 z = 0;
		p->motion_flags = z;
		if (gFrameCount & 1)
		{
			p->y.fixed = z;
		}
		else
		{
			p->y.fixed = 0x1f40000;
		}
	}
}

static void FieldScene_StartEffect141Sequence(s32 arg0, s32 arg1)
{
	Engine_PsynergyBegin(141, 1);
	Engine_PsynergySetTarget(arg0, arg1);
	Engine_PsynergyRaiseHands();
	Engine_PsynergyPlayEffect(1);
	Engine_TaskWait(1);
}

static void FieldScene_RunSequenceA(void)
{
	Engine_PsynergyPlayEffect(2);
	Engine_PsynergyLowerHands();
	BattleEffect_CleanupSceneObjects();
}

static void FieldScene_RunExtendedActorSequence(void)
{
	struct FieldActor *actor;
	s32				  *layout;
	s32				   mask;
	s32				   value;
	const u8		  *actions_a;
	const u8		  *actions_b;
	s32				  *falling;
	s32				  *work;
	const u8		  *actions_c;
	const u8		  *actions_d;
	s32				  *again;

	Engine_EventBegin();
	layout = &KorimaMura_LayoutFlag;
	*layout = GameFlag_IsSet(3);
	actor = Object_GetById(19);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor = Object_GetById(20);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor = Object_GetById(21);
	Engine_ActorSetSpriteFlags(actor, 0);
	actor = Object_GetById(22);
	Engine_ActorSetSpriteFlags(actor, 0);
	Camera_MoveTo(0x680000, -1, 0x1000000, 0);
	Engine_MapRedraw();
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x170000, 0xf70000);
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 121, 238);
	Actor_SetSpeed(GERALD, 0x9999, 0x4ccc);
	Actor_SetSpeed(IWAN, 0x9999, 0x4ccc);
	actor = Object_GetById(0);
	if (actor != NULL)
	{
		Actor_SetPosition(GERALD, actor->x.fixed, actor->z.fixed);
	}
	actor = Object_GetById(0);
	if (actor != NULL)
	{
		Actor_SetPosition(IWAN, actor->x.fixed, actor->z.fixed);
	}
	Engine_ActorEnableActionCallback(GERALD, KorimaMura_ActionTable1);
	Engine_ActorEnableActionCallback(IWAN, KorimaMura_ActionTable2);
	if (*layout != 0)
	{
		Actor_SetSpeed(MEARI, 0x9999, 0x4ccc);
		actor = Object_GetById(0);
		if (actor != NULL)
		{
			Actor_SetPosition(MEARI, actor->x.fixed, actor->z.fixed);
		}
		Engine_ActorEnableActionCallback(MEARI, KorimaMura_ActionTable3);
	}
	Object_RefreshSelectorById(2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 20);
	Actor_FaceDirection(IWAN, FACING_NORTHEAST, 20);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 20);
	layout = &KorimaMura_LayoutFlag;
	if (*layout != 0)
	{
		Actor_FaceDirection(MEARI, FACING_SOUTHEAST, 0);
	}
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 20);
	Actor_FaceDirection(IWAN, FACING_SOUTHEAST, 20);
	Actor_FaceDirection(GERALD, FACING_NORTHEAST, 60);
	if (*layout != 0)
	{
		Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
	}
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
	if (*layout != 0)
	{
		Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
	}
	Engine_EventWait(40);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_EventSetMessage((s32)MsgKorimaQuiet);
	FieldScene_RunPairedStepA(1, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, GERALD, 0);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(30);
	Actor_WalkToAndWait(IWAN, 72, 0x11e);
	Actor_WalkToAndWait(IWAN, 72, 0x12e);
	Actor_WalkToAndWait(IWAN, 88, 0x136);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	if (*layout != 0)
	{
		Actor_FaceActor(MEARI, IWAN, 0);
	}
	Engine_EventWait(30);
	Engine_ActorSetAnimation(GERALD, 3);
	if (*layout != 0)
	{
		Engine_ActorSetAnimation(MEARI, 3);
	}
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(30);
	Actor_FaceDirection(IWAN, FACING_EAST, 0);
	Engine_EventWait(20);
	FieldScene_StartEffect141Sequence(2, 9);
	Engine_EventWait(40);
	FieldScene_RunSequenceA();
	Actor_ShowEmote(IWAN, 0x100, 40);
	Actor_SetSpeed(IWAN, 0x10000, 0x8000);
	((struct ObjectRuntime *)Object_GetById(2))->action_flags &= 254;
	Actor_WalkToAndWait(IWAN, 80, 0x136);
	mask = 1;
	Engine_EventWait(1);
	((struct ObjectRuntime *)Object_GetById(2))->action_flags |= mask;
	Actor_ShowEmote(GERALD, 0x102, 40);
	FieldScene_RunPairedStepA(1, 20);
	Actor_ShowEmote(IWAN, 0x102, 40);
	FieldScene_RunPairedStepA(2, 20);
	if (*layout != 0)
	{
		Actor_FaceActor(MEARI, ACTOR_PARTY_LEADER, 0);
	}
	Engine_ActorFaceEachOther(IWAN, GERALD, 60);
	if (*layout != 0)
	{
		Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	}
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	FieldScene_RunPairedStepA(1, 20);
	Actor_SetSpeed(IWAN, 0x8000, 0x4000);
	((struct ObjectRuntime *)Object_GetById(2))->action_flags &= 254;
	Actor_WalkToAndWait(IWAN, 72, 0x11e);
	Engine_EventWait(1);
	((struct ObjectRuntime *)Object_GetById(2))->action_flags |= mask;
	Engine_ActorEnableActionCallback(IWAN, KorimaMura_ActionTable2);
	if (*layout != 0)
	{
		Actor_ShowEmote(MEARI, 0x105, 0);
		Engine_EventWait(60);
		FieldScene_RunPairedStepA(3, 20);
		Engine_ActorSetAnimation(MEARI, 3);
	}
	else
	{
		gEventWork->message++;
	}
	Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(10);
	FieldScene_RunPairedStepB(1, 0x2000, 10);
	FieldScene_RunPairedStepB(0, 0xa000, 10);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimation(GERALD, 3);
	}
	else
	{
		Engine_ActorSetAnimationAndWait(GERALD, 4);
		gEventWork->message++;
	}
	FieldScene_RunPairedStepA(1, 40);
	FieldScene_RunPairedStepB(2, 0x2000, 40);
	FieldScene_RunPairedStepB(2, 0x8000, 20);
	FieldScene_RunPairedStepB(2, 0x4000, 40);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	FieldScene_RunPairedStepB(0, 0x6000, 60);
	value = 160;
	FieldScene_RunPairedStepB(3, 0x2000, 10);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	FieldScene_RunPairedStepB(0, (value << 8), 10);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	FieldScene_RunPairedStepB(0, 0x6000, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventSetMessage((s32)MsgKorimaWhatIsIt);
	FieldScene_RunPairedStepA(1, 10);
	FieldScene_RunPairedStepB(2, 0xc000, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(2, 20);
	FieldScene_RunPairedStepB(1, 0, 20);
	FieldScene_RunPairedStepB(0, (value << 8), 40);
	FieldScene_RunPairedStepB(1, 0x4000, 20);
	FieldScene_RunPairedStepB(0, 0x6000, 30);
	FieldScene_RunPairedStepB(1, 0x6000, 20);
	FieldScene_RunPairedStepB(0, 0xe000, 30);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	FieldScene_RunPairedStepB(0, 0x6000, 20);
	FieldScene_RunPairedStepB(2, 0xc000, 10);
	Event_PlayCue(17);
	Event_PlayCue(206);
	ColorBuffer_ApplyTarget(0x7fff, 0);
	Engine_ColorBufferInterpolate(1);
	Engine_TaskWait(1);
	KorimaMura_EffectActive = 1;
	Engine_TaskAddCallback(SceneEffect_SpawnObject26EveryEightFrames, 0xc80);
	Engine_TaskWait(20);
	ColorBuffer_ApplyTarget(0x405210, 1);
	ColorBuffer_ApplyTarget(0x10000, 2);
	Engine_ColorBufferInterpolate(120);
	Engine_TaskWait(60);
	actions_a = KorimaMura_ActionTable4;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, actions_a);
	Engine_ActorEnableActionCallback(GERALD, actions_a);
	Engine_ActorEnableActionCallback(IWAN, actions_a);
	Engine_ActorEnableActionCallback(MEARI, actions_a);
	Engine_EventWait(100);
	FieldScene_RunPairedStepA(1, 20);
	FieldScene_RunPairedStepA(2, 40);
	if (KorimaMura_LayoutFlag != 0)
	{
		Engine_EventWait(40);
		Actor_SetAttachedEffect(MEARI, 0x102);
		Engine_EventWait(40);
		FieldScene_RunPairedStepA(3, 40);
	}
	else
	{
		gEventWork->message++;
	}
	Engine_EventWait(20);
	if (KorimaMura_LayoutFlag != 0)
	{
		value = 128;
		actor = Object_GetById(3);
		actor->velocity_y = (value << 10);
		Engine_EventWait(10);
		Actor_SetSpeed(MEARI, (value << 10), (value << 10));
		Actor_SetDestinationOffset(MEARI, -2, 0);
		Engine_ActorEnableActionCallback(MEARI, KorimaMura_ActionTable5);
		actor = Object_GetById(3);
		Engine_ActorSetSpriteFlags(actor, 0);
		Engine_ActorSetAnimation(MEARI, 19);
		Engine_EventWait(10);
	}
	value = 128;
	actor = Object_GetById(0);
	actor->velocity_y = (value << 10);
	Engine_EventWait(10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, (value << 10), (value << 10));
	actions_b = KorimaMura_ActionTable5;
	Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, actions_b);
	actor = Object_GetById(0);
	Engine_ActorSetSpriteFlags(actor, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_EventWait(20);
	actor = Object_GetById(1);
	actor->velocity_y = (value << 10);
	Engine_EventWait(10);
	Actor_SetSpeed(GERALD, (value << 10), (value << 10));
	Engine_ActorEnableActionCallback(GERALD, actions_b);
	actor = Object_GetById(1);
	Engine_ActorSetSpriteFlags(actor, 0);
	Engine_ActorSetAnimation(GERALD, 19);
	Engine_EventWait(40);
	actor = Object_GetById(2);
	actor->velocity_y = (value << 10);
	Engine_EventWait(10);
	Engine_ActorEnableActionCallback(IWAN, actions_b);
	actor = Object_GetById(2);
	Engine_ActorSetSpriteFlags(actor, 0);
	Engine_ActorSetAnimation(IWAN, 19);
	KorimaMura_EffectActive = 0;
	Engine_EventWait(160);
	Engine_TaskRemoveCallback(SceneEffect_SpawnObject26EveryEightFrames);
	Engine_EventWait(120);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	gFallingEffectWidth = 0;
	falling = &gFallingEffectState;
	gFallingEffectOffset = 0x800000;
	*falling = 1;
	Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
	Engine_EventWait(180);
	Event_PlayCue(21);
	FieldScene_RunPairedStepA(1, 80);
	FieldScene_RunPairedStepA(2, 40);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Engine_EventWait(60);
	FieldScene_RunPairedStepA(2, 20);
	*falling = 2;
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
	Actor_SetAttachedEffect(GERALD, 0x102);
	FieldScene_RunPairedStepA(1, 20);
	if (KorimaMura_LayoutFlag != 0)
	{
		Actor_SetAttachedEffect(MEARI, 0x102);
		FieldScene_RunPairedStepA(3, 10);
	}
	else
	{
		gEventWork->message++;
	}
	work = &gFallingEffectState;
	*work = 3;
	Object_GetById(0)->priority_flags &= 254;
	Object_GetById(1)->priority_flags &= 254;
	Object_GetById(2)->priority_flags &= 254;
	Object_GetById(3)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetSpritePriority(GERALD, 3);
	Engine_ActorSetSpritePriority(IWAN, 3);
	value = 0;
	Engine_ActorSetSpritePriority(MEARI, 3);
	KorimaMura_PendingPose = value;
	Engine_TaskAddCallback(SceneActor_SetActors19To22HeightByFrameParity, 0xc80);
	Event_PlayCue(220);
	Object_GetById(19)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(19, 2);
	Actor_SetPosition(19, 0x780000, 0xf80000);
	actions_c = KorimaMura_ActionTable6;
	Engine_ActorEnableActionCallback(19, actions_c);
	Object_GetById(20)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(20, 2);
	Actor_SetPosition(20, 0x640000, 0x1120000);
	Engine_ActorEnableActionCallback(20, actions_c);
	if (KorimaMura_LayoutFlag != 0)
	{
		Object_GetById(21)->priority_flags &= 254;
		Engine_ActorSetSpritePriority(21, 2);
		Actor_SetPosition(21, 0x4a0000, 0xfe0000);
		Engine_ActorEnableActionCallback(21, actions_c);
	}
	Object_GetById(22)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(22, 2);
	Actor_SetPosition(22, 0x5e0000, 0xe10000);
	Engine_ActorEnableActionCallback(22, actions_c);
	if (*work != 0)
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
	Engine_ActorStop(19);
	Engine_ActorStop(20);
	if (KorimaMura_LayoutFlag != 0)
	{
		Engine_ActorStop(21);
	}
	Engine_ActorStop(22);
	Engine_TaskWait(1);
	actions_d = KorimaMura_ActionTable7;
	Engine_ActorEnableActionCallback(19, actions_d);
	Engine_ActorEnableActionCallback(20, actions_d);
	if (KorimaMura_LayoutFlag != 0)
	{
		Engine_ActorEnableActionCallback(21, actions_d);
	}
	Object_SetActionCallbackAndRefreshById(22, actions_d);
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(40);
	Event_OpenMessage(GERALD, 0);
	Actor_SetPosition(17, 0x570000, 0x8b0000);
	Actor_SetPosition(18, 0x570000, 0x8b0000);
	Engine_TaskWait(1);
	if (Engine_EventChooseYesNo(17, 0) == 1)
	{
		gEventWork->message++;
	}
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	FieldScene_RunPairedStepA(2, 20);
	if (KorimaMura_LayoutFlag != 0)
	{
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		Engine_EventWait(10);
		Engine_EventSetMessage((s32)MsgKorimaKnowThoseFieldsWere);
		FieldScene_RunPairedStepA(3, 40);
	}
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Engine_EventWait(80);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventSetMessage((s32)MsgKorimaWasOurPsynergy);
	FieldScene_RunPairedStepA(2, 40);
	Engine_ActorRunRepeatedMotion(GERALD, 3);
	Engine_EventWait(40);
	Engine_ActorSetSpritePriority(GERALD, 2);
	Object_GetById(1)->priority_flags |= 1;
	actor = Object_GetById(1);
	Engine_ActorSetSpriteFlags(actor, 1);
	Engine_ActorJump(GERALD, 6, 0);
	Engine_ActorSetAnimation(GERALD, 1);
	FieldScene_RunPairedStepB(1, 0x4000, 60);
	FieldScene_RunPairedStepA(1, 20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	FieldScene_RunPairedStepA(1, 10);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 3);
	FieldScene_RunPairedStepB(1, 0x2000, 20);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Engine_EventWait(40);
	FieldScene_RunPairedStepB(1, 0x6000, 40);
	FieldScene_RunPairedStepB(1, 0x2000, 20);
	FieldScene_RunPairedStepB(1, 0x6000, 20);
	FieldScene_RunPairedStepB(1, 0x2000, 10);
	Engine_ActorJump(GERALD, 2, 0);
	Engine_EventWait(40);
	Engine_ActorJump(GERALD, 2, 0);
	Engine_EventWait(10);
	Engine_ActorJump(GERALD, 4, 0);
	Engine_EventWait(20);
	FieldScene_RunPairedStepA(1, 20);
	if (KorimaMura_LayoutFlag != 0)
	{
		Actor_ShowEmote(MEARI, 0x100, 0);
		Engine_EventWait(60);
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		Engine_EventWait(80);
		Engine_ActorSetSpritePriority(MEARI, 2);
		Object_GetById(3)->priority_flags |= 1;
		actor = Object_GetById(3);
		Engine_ActorSetSpriteFlags(actor, 1);
		Engine_ActorJump(MEARI, 4, 0);
		Actor_SetDestinationOffset(MEARI, -2, 0);
		Engine_ActorSetAnimation(MEARI, 1);
		FieldScene_RunPairedStepB(3, 0xe000, 60);
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		Engine_EventWait(20);
		FieldScene_RunPairedStepA(3, 20);
	}
	else
	{
		gEventWork->message++;
	}
	Engine_ActorJump(GERALD, 2, 0);
	FieldScene_RunPairedStepB(1, 0x4000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	FieldScene_RunPairedStepB(1, 0x2000, 10);
	FieldScene_RunPairedStepA(1, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	value = 1;
	Engine_ActorSetSpritePriority(IWAN, 2);
	Object_GetById(2)->priority_flags |= value;
	actor = Object_GetById(2);
	Engine_ActorSetSpriteFlags(actor, 1);
	Engine_ActorJump(IWAN, 4, 0);
	Engine_ActorSetAnimation(IWAN, 1);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(10);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Object_GetById(0)->priority_flags |= value;
	actor = Object_GetById(0);
	Engine_ActorSetSpriteFlags(actor, 1);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 4, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	FieldScene_RunPairedStepB(0, 0x6000, 60);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(60);
	FieldScene_RunPairedStepB(0, 0xa000, 20);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	FieldScene_RunPairedStepB(0, 0x6000, 10);
	FieldScene_RunPairedStepB(1, 0x4000, 10);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(2, 20);
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
		gEventWork->message++;
	}
	else
	{
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		FieldScene_RunPairedStepB(1, 0x2000, 10);
		Engine_ActorStartRepeatedMotion(GERALD, 2);
		Event_ShowMessage(GERALD, 0);
	}
	FieldScene_RunPairedStepB(1, 0x4000, 10);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	FieldScene_RunPairedStepA(1, 20);
	FieldScene_RunPairedStepB(2, 0xc000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(2, 10);
	if (KorimaMura_LayoutFlag != 0)
	{
		Engine_ActorRunRepeatedMotion(MEARI, 2);
		FieldScene_RunPairedStepB(3, 0, 20);
		FieldScene_RunPairedStepB(3, 0x2000, 10);
		Engine_ActorSetAnimation(MEARI, 4);
		FieldScene_RunPairedStepA(3, 10);
	}
	else
	{
		gEventWork->message++;
	}
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	FieldScene_RunPairedStepB(0, 0xa000, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(GERALD, 3);
	value = 128;
	Engine_EventWait(20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	FieldScene_RunPairedStepB(1, (value << 7), 10);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 80);
	FieldScene_RunPairedStepB(2, 0xe000, 10);
	Engine_ActorStartRepeatedMotion(IWAN, 2);
	FieldScene_RunPairedStepA(2, 20);
	Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
	FieldScene_RunPairedStepB(0, 0xa000, 40);
	Actor_FaceDirection(GERALD, (value << 7), 0);
	FieldScene_RunPairedStepB(0, 0x6000, 10);
	FieldScene_RunPairedStepB(2, 0xc000, 10);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(2, 10);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(40);
	FieldScene_RunPairedStepA(1, 20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(40);
	FieldScene_RunPairedStepA(1, 20);
	Engine_ActorSetAnimation(IWAN, 3);
	FieldScene_RunPairedStepA(2, 10);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	FieldScene_RunPairedStepB(1, 0x2000, 10);
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
		gEventWork->message++;
	}
	Event_ShowMessage(GERALD, 0);
	Event_PlayCue(21);
	ColorBuffer_ApplyTarget(0x406218, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(60);
	gFallingEffectWidth = 0;
	gFallingEffectOffset = 0x800000;
	again = &gFallingEffectState;
	*again = 1;
	Engine_TaskAddCallback(FieldScene_UpdateFallingEffect, 0xc80);
	Engine_EventWait(80);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(60);
	FieldScene_RunPairedStepB(2, 0xc000, 10);
	Engine_EventSetMessage((s32)MsgKorimaWatchOutItsHappeningAgain);
	FieldScene_RunPairedStepA(2, 10);
	FieldScene_RunPairedStepB(1, 0xc000, 10);
	FieldScene_RunPairedStepB(0, 0xc000, 10);
	if (KorimaMura_LayoutFlag != 0)
	{
		FieldScene_RunPairedStepB(3, 0xc000, 10);
	}
	Object_GetById(0)->priority_flags &= 254;
	Object_GetById(1)->priority_flags &= 254;
	Object_GetById(2)->priority_flags &= 254;
	Object_GetById(3)->priority_flags &= 254;
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetSpritePriority(GERALD, 3);
	Engine_ActorSetSpritePriority(IWAN, 3);
	Engine_ActorSetSpritePriority(MEARI, 3);
	*again = 2;
	Event_PlayCue(220);
	Actor_SetPosition(19, 0x780000, 0xf80000);
	actions_c = KorimaMura_ActionTable6;
	Engine_ActorEnableActionCallback(19, actions_c);
	Actor_SetPosition(20, 0x640000, 0x1120000);
	Engine_ActorEnableActionCallback(20, actions_c);
	if (KorimaMura_LayoutFlag != 0)
	{
		Actor_SetPosition(21, 0x4a0000, 0xfe0000);
		Engine_ActorEnableActionCallback(21, actions_c);
	}
	Actor_SetPosition(22, 0x5e0000, 0xe10000);
	Engine_ActorEnableActionCallback(22, actions_c);
	Engine_EventWait(120);
	*again = 3;
	do
	{
		Engine_TaskWait(1);
	} while (gFallingEffectState != 0);
	FieldScene_RunPairedStepA(17, 80);
	FieldScene_RunPairedStepA(18, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Actor_ShowEmote(MEARI, 0x101, 0);
	Engine_EventWait(60);
	FieldScene_RunPairedStepA(18, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(18, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(18, 10);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Actor_ShowEmote(MEARI, 0x100, 0);
	Engine_EventWait(40);
	FieldScene_RunPairedStepA(17, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(MEARI, FACING_EAST, 0);
	FieldScene_RunPairedStepB(2, 0xc000, 40);
	FieldScene_RunPairedStepA(18, 10);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	FieldScene_RunPairedStepB(3, 0xc000, 80);
	FieldScene_RunPairedStepA(18, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	FieldScene_RunPairedStepB(3, 0, 40);
	FieldScene_RunPairedStepA(17, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	FieldScene_RunPairedStepB(3, 0xc000, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
	Engine_ActorSetAnimation(GERALD, 4);
	Engine_ActorSetAnimation(MEARI, 4);
	Engine_ActorSetAnimationAndWait(IWAN, 4);
	Engine_EventWait(60);
	FieldScene_RunPairedStepA(18, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	FieldScene_RunPairedStepA(18, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	FieldScene_RunPairedStepB(3, 0, 20);
	FieldScene_RunPairedStepA(18, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorStartRepeatedMotion(MEARI, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	Actor_FaceDirection(MEARI, FACING_NORTH, 0);
	FieldScene_RunPairedStepA(18, 20);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	FieldScene_RunPairedStepB(3, 0, 20);
	FieldScene_RunPairedStepA(17, 20);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(MEARI, 0x102);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Engine_EventWait(40);
	FieldScene_RunPairedStepA(18, 10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GERALD, FACING_NORTH, 0);
	Actor_FaceDirection(IWAN, FACING_NORTH, 0);
	FieldScene_RunPairedStepB(3, 0xc000, 10);
	FieldScene_RunPairedStepA(18, 10);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	Engine_ActorSetAnimation(MEARI, 3);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(60);
	Event_ShowMessage(18, 0);
	Event_ShowMessage(17, 0);
	Engine_TaskRemoveCallback(FieldScene_UpdateFallingEffect);
	Engine_EventWait(80);
	ColorBuffer_ApplyTarget(0x10000, 1);
	Engine_ColorBufferInterpolate(60);
	Engine_TaskWait(80);
	Engine_ActorStop(19);
	Engine_ActorStop(20);
	work = &KorimaMura_LayoutFlag;
	Engine_ActorStop(21);
	Engine_ActorStop(22);
	Engine_TaskWait(1);
	actions_d = KorimaMura_ActionTable7;
	Engine_ActorEnableActionCallback(19, actions_d);
	Engine_ActorEnableActionCallback(20, actions_d);
	if (*work != 0)
	{
		Engine_ActorEnableActionCallback(21, actions_d);
	}
	Object_SetActionCallbackAndRefreshById(22, actions_d);
	Engine_EventWait(20);
	Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
	Engine_ActorSetSpritePriority(GERALD, 2);
	Engine_ActorSetSpritePriority(IWAN, 2);
	value = 1;
	Engine_ActorSetSpritePriority(MEARI, 2);
	Object_GetById(0)->priority_flags |= value;
	Object_GetById(1)->priority_flags |= value;
	Object_GetById(2)->priority_flags |= value;
	Object_GetById(3)->priority_flags |= value;
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	FieldScene_RunPairedStepB(2, 0xe000, 10);
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
			FieldScene_RunPairedStepB(3, 0, 20);
			Actor_ShowEmote(GERALD, 0x101, 0);
			Actor_ShowEmote(IWAN, 0x101, 0);
			Actor_ShowEmote(MEARI, 0x101, 0);
			Engine_EventWait(40);
			FieldScene_RunPairedStepB(1, 0x4000, 20);
			FieldScene_RunPairedStepA(1, 10);
			FieldScene_RunPairedStepB(2, 0xc000, 20);
			FieldScene_RunPairedStepB(2, 0xe000, 20);
			Engine_ActorSetAnimationAndWait(IWAN, 3);
			FieldScene_RunPairedStepA(2, 20);
			FieldScene_RunPairedStepB(1, 0x2000, 20);
		}
		else
		{
			FieldScene_RunPairedStepB(3, 0, 20);
			Actor_ShowEmote(GERALD, 0x102, 0);
			Actor_ShowEmote(IWAN, 0x102, 0);
			Actor_ShowEmote(MEARI, 0x102, 40);
			FieldScene_RunPairedStepB(1, 0x4000, 20);
			Engine_EventSetMessage((s32)MsgKorimaHesAsStumpedAsWe);
			FieldScene_RunPairedStepA(1, 20);
			Engine_ActorSetAnimationAndWait(IWAN, 3);
			FieldScene_RunPairedStepA(2, 20);
		}
		Engine_ActorSetAnimation(MEARI, 3);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
	}
	else
	{
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		Engine_EventWait(10);
		Engine_EventSetMessage((s32)MsgKorimaYoureRobinThereIsntMuch);
		FieldScene_RunPairedStepA(1, 10);
		Actor_FaceDirection(GERALD, FACING_SOUTH, 0);
		FieldScene_RunPairedStepB(0, 0x6000, 20);
		Engine_ActorSetAnimation(GERALD, 3);
		Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
		Engine_EventWait(10);
		Engine_ActorSetAnimationAndWait(IWAN, 4);
		Event_OpenMessage(IWAN, 0);
		if (Engine_EventChooseYesNo(0, 0) != 0)
		{
			goto relieved;
		}
		Engine_EventWait(20);
		Actor_ShowEmote(IWAN, 0x103, 0);
		Engine_EventWait(40);
		FieldScene_RunPairedStepB(2, 0xe000, 10);
		FieldScene_RunPairedStepA(2, 10);
		if (KorimaMura_LayoutFlag != 0)
		{
			FieldScene_RunPairedStepB(3, 0, 10);
			Engine_ActorStartRepeatedMotion(MEARI, 3);
			FieldScene_RunPairedStepA(3, 20);
		}
		else
		{
			gEventWork->message++;
		}
		Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
		Actor_SetAttachedEffect(GERALD, 0x102);
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		FieldScene_RunPairedStepA(1, 20);
		Actor_ShowEmote(GERALD, 0x105, 0);
		Engine_EventWait(120);
		FieldScene_RunPairedStepA(2, 40);
		if (KorimaMura_LayoutFlag != 0)
		{
			FieldScene_RunPairedStepB(3, 0x2000, 10);
			Engine_ActorSetAnimationAndWait(MEARI, 4);
			FieldScene_RunPairedStepA(3, 10);
		}
		else
		{
			gEventWork->message++;
		}
		Engine_EventWait(60);
		Engine_ActorRunRepeatedMotion(IWAN, 2);
		if (KorimaMura_LayoutFlag != 0)
		{
			FieldScene_RunPairedStepB(2, 0xa000, 40);
			FieldScene_RunPairedStepB(2, 0xe000, 20);
		}
		FieldScene_RunPairedStepA(2, 10);
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
		Engine_ActorRunRepeatedMotion(GERALD, 2);
		Engine_EventWait(40);
		FieldScene_RunPairedStepA(2, 20);
		Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
		Engine_ActorSetAnimationAndWait(GERALD, 3);
		Engine_EventWait(20);
		Engine_ActorSetAnimation(MEARI, 3);
	}
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	goto leave;
relieved:
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventSetMessage((s32)MsgKorimaThatsReliefRobinThoughtYoud);
	FieldScene_RunPairedStepA(2, 20);
	if (KorimaMura_LayoutFlag != 0)
	{
		FieldScene_RunPairedStepB(3, 0, 10);
		Engine_ActorStartRepeatedMotion(MEARI, 1);
		FieldScene_RunPairedStepA(3, 20);
	}
	else
	{
		gEventWork->message++;
	}
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	FieldScene_RunPairedStepA(1, 20);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(80);
	FieldScene_RunPairedStepA(2, 40);
	if (KorimaMura_LayoutFlag != 0)
	{
		FieldScene_RunPairedStepB(3, 0x2000, 20);
		Engine_ActorSetAnimation(MEARI, 4);
		FieldScene_RunPairedStepA(3, 40);
	}
	else
	{
		gEventWork->message++;
	}
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	FieldScene_RunPairedStepA(2, 20);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(40);
	FieldScene_RunPairedStepA(2, 20);
leave:
	Event_PlayCue(17);
	Actor_SetSpeed(GERALD, 0x13333, 0x9999);
	Actor_SetSpeed(IWAN, 0x13333, 0x9999);
	Actor_SetSpeed(MEARI, 0x13333, 0x9999);
	Engine_ActorSetAnimation(GERALD, 2);
	actor = Object_GetById(0);
	if (actor != NULL)
	{
		Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_ActorSetAnimation(IWAN, 2);
	actor = Object_GetById(0);
	if (actor != NULL)
	{
		Actor_SetDestination(IWAN, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorWaitForMove(IWAN);
	Actor_SetPosition(IWAN, 0, 0);
	Actor_SetPosition(17, 0, 0);
	Actor_SetPosition(18, 0, 0);
	if (KorimaMura_LayoutFlag != 0)
	{
		Engine_ActorSetAnimation(MEARI, 2);
		actor = Object_GetById(0);
		if (actor != NULL)
		{
			Actor_SetDestination(MEARI, actor->x.part.pixel, actor->z.part.pixel);
		}
		Engine_ActorWaitForMove(MEARI);
		Actor_SetPosition(MEARI, 0, 0);
	}
	GameFlag_Set(0x843);
	Audio_PlayCueFromEventWork();
	Engine_EventEnd();
}

static void FieldScene_RunPairedStepA(s32 arg0, s32 arg1)
{
	Engine_EventShowMessage(arg0, 0);
	Engine_EventWait(arg1);
}

static void FieldScene_RunPairedStepB(s32 arg0, s32 arg1, s32 arg2)
{
	Engine_ActorFaceDirection(arg0, arg1, 0);
	Engine_EventWait(arg2);
}

static s32 SceneEffect_AdvanceAngleUntilIdle(struct FieldActor *p)
{
	p->scale_x += 0x1eb8;
	if (p->target_x == 0x80000000 && p->target_y == p->target_x && p->target_z == p->target_y)
	{
		Engine_ObjectDispatchRelease(p);
	}
	return 1;
}

static void SceneEffect_SpawnObject26EveryEightFrames(void)
{
	struct FieldActor *p;
	struct Sub		  *q;
	s32				   f;
	s32				   v;
	s32				   w;
	s32				   c1 = 0x620000;
	s32				   c2 = 0x690000;
	s32				   c3 = 0x620000;
	s32				   c4 = 0x010d0000;

	f = gFrameCount & 7;
	if (f != 0)
	{
		return;
	}
	if (KorimaMura_EffectActive != 0)
	{
		Engine_AudioPlayCue(200);
	}
	p = Engine_ObjectCreate(26, c1, 0, c2);
	if (p == 0)
	{
		return;
	}
	q = (struct Sub *)p->sprite;
	q->f26 = f;
	v = 0xfe;
	v &= p->priority_flags;
	p->priority_flags = v;
	w = ~12;
	w &= q->f09;
	w |= 4;
	q->f09 = w;
	p->scale_x = 0x1999;
	p->speed = 0x80000;
	p->acceleration = 0x80000;
	p->motion_flags = f;
	Object_SetMode(p, 2);
	Engine_ObjectSetPosition(p, c3, 0, c4);
	Engine_ObjectSetScript(p, KorimaMura_Object26Script);
}

static s32 SceneEffect_SetModeByFrameBit1(s32 arg0)
{
	if ((gFrameCount >> 1) & 1)
	{
		ObjectGroup_SetChildValue(arg0, 10);
	}
	else
	{
		ObjectGroup_SetChildValue(arg0, 7);
	}
	return 0;
}

static s32 KorimaMura_TriggerRegionScript(struct FieldActor *actor)
{
	if (KorimaMura_LayoutFlag != 0)
	{
		if (actor->x.fixed > 0x3b0000 && actor->x.fixed < 0x8d0000 && actor->z.fixed > 0xd30000
			&& actor->z.fixed < 0x1010000)
			goto hit;
		if (actor->x.fixed > 0x450000 && actor->x.fixed < 0x7a0000 && actor->z.fixed > 0xc20000
			&& actor->z.fixed < 0x1150000)
			goto hit;
	}
	else
	{
		if (actor->x.fixed > 0x3b0000 && actor->x.fixed < 0x6f0000 && actor->z.fixed > 0xc20000
			&& actor->z.fixed < 0xe60000)
			goto hit;
		if (actor->x.fixed > 0x6f0000 && actor->x.fixed < 0x8d0000 && actor->z.fixed > 0xd80000
			&& actor->z.fixed < 0xfa0000)
			goto hit;
		if (actor->x.fixed > 0x4e0000 && actor->x.fixed < 0x7a0000 && actor->z.fixed > 0xf10000
			&& actor->z.fixed < 0x1150000)
			goto hit;
	}
	goto done;
hit:
	Engine_AudioPlayCue(106);
	Engine_ObjectSetScript(actor, KorimaMura_RegionScript);
	KorimaMura_PendingPose = 1;
done:
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../korima_hashi/falling_effect.c"
s32 KorimaMura_PendingPose __attribute__((section(".bss")));
s32 KorimaMura_LayoutFlag __attribute__((section(".bss")));
s32 KorimaMura_EffectActive __attribute__((section(".bss")));

static void FieldScene_RunFlag845And847Branches(void)
{
	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		Engine_ActorSetPosition(8, 0, 0);
		{
			s32 k5 = 9, k6 = 18;
			Engine_MapCopyCellAttributes(9, 17, 5, 1, k5, k6);
		}
		Engine_MapRedraw();
		Engine_TaskWait(1);
	}
	else
	{
		Engine_ActorSetPosition(9, 0, 0);
	}
	if (Engine_GameFlagIsSet(0x847) != 0)
	{
		{
			s32 k5 = 5, k6 = 7;
			Engine_MapCopyCellsTo(91, 19, 72, 9, k5, k6);
		}
		{
			s32 k5 = 8, k6 = 11;
			Engine_MapCopyCellAttributes(23, 11, 5, 7, k5, k6);
		}
		Engine_MapRedraw();
		Engine_TaskWait(1);
	}
}

s32 KorimaMura_ActionTable1[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(97), 0, PIXELS(221)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_ARG(1),
	SC_END,
};

s32 KorimaMura_ActionTable2[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(101), 0, PIXELS(268)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_EAST),
	SC_ARG(1),
	SC_END,
};

s32 KorimaMura_ActionTable3[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(78), 0, PIXELS(247)),
	SC_SYNC,
	SC_SET(SW_FACING, FACING_NORTH),
	SC_ARG(1),
	SC_END,
};

s32 KorimaMura_ActionTable4[] = {
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

s32 KorimaMura_ActionTable5[] = {
	SC_SET(SW_SCALE_X, 0x10000),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_WAIT(1),
	SC_END,
};

s32 KorimaMura_ActionTable6[] = {
	SC_SET(SW_SCALE_X, 0x1999),
	SC_SET(SW_SCALE_Y, 0x1999),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 1310),
	SC_ADD(SW_SCALE_Y, 1310),
	SC_WAIT(1),
	SC_LOOP(70, SC_TO(1)),
	SC_CALL(KorimaMura_ApplyPendingPose),
	SC_END,
};

s32 KorimaMura_ActionTable7[] = {
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

s32 KorimaMura_EntryActions[] = {
	SC_SET(SW_SCALE_X, 0xc000),
	SC_SET(SW_SCALE_Y, 0xc000),
	SC_SET(SW_COLLISION, 0),
	SC_SET(0x23, 0),
	SC_LABEL(1),
	SC_CALL(SceneEffect_AdvanceCounterAndSwitchMode),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaMura_DebrisScript[] = {
	SC_CALL(SceneEffect_UpdateFallingObject),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaMura_Actor8Path[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

u16 KorimaMura_SwitchCells[] = {
	MAP_CELL_STEP(66, 32, 1, 2, 4), MAP_CELL_STEP(67, 32, 1, 2, 4), MAP_CELL_STEPS_END,
};

struct SceneEntrance gKorimaMuraEntrances[] = {
	{ 0, CONDITION_ALWAYS, 232, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 418, 0, 285, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 119, 0, 182, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 151, 0, 310, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 264, 0, 373, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 329, 0, 230, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 263, 0, 326, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 360, 0, 346, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 44, 0, 267, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 232, 0, 176, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKorimaMuraEntrances3[] = {
	{ 1, CONDITION_ALWAYS, 170, 0, 386, FACING_NORTH, 0, 0, 0, 288, 416, 0 },
	{ 2, CONDITION_ALWAYS, 168, 0, 50, FACING_SOUTH, 0, 48, 0, 288, 416, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKorimaMuraEntrances2[] = {
	{ 1, CONDITION_ALWAYS, 216, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 312, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

s16 gKorimaMuraRegions2[] = {
	207, 31, 311, 225, 49, 329, -1, 1,
	303, 31, 303, 321, 49, 321, -1, 2,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gKorimaMuraExits[] = {
	SCENE_EXITS(36),
	SCENE_EXIT(1, 2, 7),
	SCENE_EXIT(2, 2, 38),
	SCENE_EXIT(5, 37, 4),
	SCENE_EXIT(6, 37, 3),
	SCENE_EXIT(7, 37, 7),
	SCENE_EXIT(8, 37, 10),
	SCENE_EXIT(9, 37, 8),
	SCENE_EXIT(10, 38, 2),
	SCENE_EXIT(0, 39, 0),
	SCENE_EXIT(1, 2, 11),
	SCENE_EXIT(2, 2, 46),
	SCENE_EXIT(0, 38, 0),
	SCENE_EXIT(1, 37, 9),
	SCENE_EXIT(2, 36, 8),
	SCENE_EXITS_END,
};

struct ScenePlacement gKorimaMuraPlacements[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKorimaMuraPlacements1[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ MEARI, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(312), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(168), 0, PIXELS(360), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(344), 0, PIXELS(280), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(392), FACING_SOUTH, 3, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(248), 0, PIXELS(184), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(264), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(264), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x37, 0x843, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x38, 0x843, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdc, 0x843, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdc, 0x843, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xdc, 0x843, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xdc, 0x843, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x11d, 0x845, ACTOR_STAND,
		PIXELS(24), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x11d, 0x845, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(296), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x11d, 0x845, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x11d, 0x845, ACTOR_STAND,
		PIXELS(40), 0, PIXELS(328), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x5a, 0x31, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(360), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement KorimaMura_EntryScript[] = {
	{ 0x11d, 0x845, 7,
		PIXELS(120), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x11d, 0x845, 7,
		PIXELS(296), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x11d, 0x845, 7,
		PIXELS(440), 0, PIXELS(344), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x11d, 0x845, 7,
		PIXELS(296), 0, PIXELS(232), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x11d, 0x845, 7,
		PIXELS(152), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKorimaMuraPlacements3[] = {
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(234), 0, PIXELS(200), FACING_SOUTH + FACING_STEP, 3, 0 },
	{ 0x69, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(234), 0, PIXELS(200), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKorimaMuraEvents[] = {
	{ EVENT_EXIT, 99, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMura_AnimateSwitchOrExit) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMura_AnimateSwitchOrExit) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMura_AnimateSwitchOrExit) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMura_AnimateSwitchOrExit) },
	{ TOUCH_WALKING_INTO(FACING_SOUTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(KorimaMura_AnimateSwitchOrExit) },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraThisIsKolimaVillageOfLumberjacks) },
	{ EVENT_TALK, 9, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraWeRelyOnTheForestsSo) },
	{ EVENT_TALK, 10, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraYouKnowBeingATreeGives) },
	{ EVENT_TALK, 11, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraITurnedIntoATreeRight) },
	{ EVENT_TALK, 12, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraDontBlameTheVillagersForBeing) },
	{ EVENT_TALK, 13, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraWhoWouldTryToUseThe) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraIDidntMindBeingATree) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraYawnTreesAreSoSleepy) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x845), EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraForAWhileIThoughtId) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraTheForestTreesAreAliveJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraIWonderIfIdLiveLonger) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraIfThoseSparklesStartFallingAgain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraIveBeenPoorAllMyLife) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraItsNotCalledTheHolyTree) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraIfWeWereAllTreesThered) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraNobodyYellsAtYouWhenYoure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x845), MSGNO(MsgKorimaMuraIfTheHolyTreeAloneCaused) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraIfICouldSpeakIdTell) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraHowCouldAnyoneTurnAGirl) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraThisIsADreamItHas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraBeingUnableToMoveIsSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraIAmOldIveLivedA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraItsTheSacredTreesCurseWhy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraTheWaterAtMyFeetHuh) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraIfICantMoveThenMom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraMaybeIfIMeditateHarderIll) },
	{ SEARCH(SEARCH_JAR), 100, 0xf57, FIND_ITEM(0xc1) },
	{ SEARCH(SEARCH_JAR), 101, 0xf58, FIND_ITEM(0xb4) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKorimaMuraEvents3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_TOUCH, 10, 0x847, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraForAWhileIThoughtNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKorimaMuraYouWouldntUnderstandButItsTruly) },
	{ SCENE_EVENTS_END },
};

s32 KorimaMura_Object26Script[] = {
	SC_CALL(SceneEffect_AdvanceAngleUntilIdle),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KorimaMura_RegionScript[] = {
	SC_SET(SW_SCALE_X, 0x20000),
	SC_SET(SW_SCALE_Y, 0x20000),
	SC_WAIT(10),
	SC_FREE,
};

s32 gFallingEffectScript[] = {
	SC_ADD(SW_Y, PIXELS(-2)),
	SC_CALL(SceneEffect_SetModeByFrameBit1),
	SC_WAIT(1),
	SC_ADD(SW_Y, PIXELS(-2)),
	SC_CALL(SceneEffect_SetModeByFrameBit1),
	SC_WAIT(1),
	SC_LOOP(30, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_Y, PIXELS(-1)),
	SC_CALL(SceneEffect_SetModeByFrameBit1),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(1)),
	SC_CALL(KorimaMura_TriggerRegionScript),
	SC_FREE,
};
