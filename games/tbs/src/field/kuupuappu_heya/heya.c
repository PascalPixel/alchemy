#include "game_state.h"
#include "scene_ids.h"
#include "unit/import.h"
#include "unit/field/kuupuappu_heya/heya.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
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
#include "unit/battle/battle.h"
#include "unit/battle/battle2.h"
#include "unit/battle/battle3.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
#include "unit/field/common/script/field.h"
#include "unit/field/common/script/field2.h"
#include "unit/field/common/script/field3.h"
#include "unit/field/common/script/field4.h"
#include "unit/field/common/script/field5.h"
#include "unit/field/common/script/field6.h"
#include "unit/field/common/script/field7.h"
#include "unit/field/common/script/field8.h"
#include "unit/field/common/script/field9.h"
#include "unit/field/common/script/field10.h"
#include "unit/field/common/script/field11.h"
#include "unit/game/character/chara.h"
#include "unit/game/character/chara2.h"
#include "unit/game/character/chara3.h"
#include "unit/game/character/chara4.h"
#include "unit/game/character/chara5.h"
#include "unit/game/character/chara6.h"
#include "unit/game/character/chara7.h"
#include "unit/game/character/chara8.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime2.h"
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
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "callback_scheduler.h"
#include "item_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "fixed_point_position.h"
#include "tbs_edition.h"
#include "btlpty.h"
#include "text/msg_ids.h"
#include "field_service.h"
#include "call.h"
#include "event_runtime.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void Scene_JoinRodSearch(void);
static void KuupuappuHeya_RunBranchingDialogueScene(void);
static void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc);
static void SceneActor_FaceActors24And25TowardActorZero(void);
static void FieldScene_RunLateSequence(void);
static void KuupuappuHeya_RunVaultEvent(void);
static void RunEventScript01(void);
static void RunDialoguePromptScene(void);
static void KuupuappuHeya_OpenWindowTransition(void);
static void SceneActor_SetModeZeroAndValue(s32 a, s32 b);
static void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c);
static void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c);
static void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c);
static void SceneEffect_ApplyPairWithValue141(s32 a, s32 b);
static void SceneState_SetValue2ThenFinish(void);
static void OverlayObject_ConfigureObject22WithResource17(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z);
static void KuupuappuHeya_UpdateActorStops(void);
static void KuupuappuHeya_StartActorStops(void);

// 戦闘開始の種類 (battle/party/party.c と同じ)

u8 *SceneData_GetScriptTable(void)
{
	return KuupuappuHeya_Scripts;
}

u8 *SceneData_GetRegionTable(void)
{
	return KuupuappuHeya_Regions;
}

u8 *SceneData_GetMessageTable(void)
{
	return KuupuappuHeya_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	const struct ScenePlacement *table;

	switch (gGameState.entrance)
	{
	case 15:
	case 16:
	case 17:
		table = KuupuappuHeya_SceneTableB;
		break;
	default:
		table = KuupuappuHeya_SceneTableA;
		break;
	}
	FieldScene_PrepareActors(table);
	return table;
}

#if !EDITION_INTERNATIONAL
static s32 KuupuappuHeya_TryJumpTo(struct FixedPointPosition *pos)
{
	struct FieldActor *obj;
	u8				  *flags;
	u8				   saved;

	obj = Object_GetById(0);
	flags = &obj->motion_flags;
	saved = *flags;
	if (Object_CheckMovementCollision(obj, pos) == 0)
	{
		Engine_EventBegin();
		Object_SetMode(obj, 6);
		Engine_TaskWait(6);
		Engine_AudioPlayCue(152);
		Object_SetMode(obj, 7);
		obj->speed = 0x30000;
		obj->acceleration = 0x20000;
		obj->velocity_y = 0x40000;
		*flags &= 0x7e;
		Engine_ActorSetSpriteFlags(obj, 0);
		Engine_ActorMoveToAndWait(0, ((union FieldCoordinate *)&pos->x)->part.pixel,
			((union FieldCoordinate *)&pos->z)->part.pixel);
		Object_SetMode(obj, 6);
		Engine_ActorSetSpriteFlags(obj, 1);
		Engine_TaskWait(6);
		*flags = saved;
		Engine_EventEnd();
		return 1;
	}
	return 0;
}
#endif

static void EV40_SCRIPT(void)
{
	s32 x;
	s32 z;
#if !EDITION_INTERNATIONAL
	struct FixedPointPosition  target;
	struct FixedPointPosition *pos;
	struct FieldActor		  *obj;
#endif

	if (Object_GetById(0)->facing >= 0xa000 && Object_GetById(0)->facing <= 0xe000)
	{
#if EDITION_INTERNATIONAL
		Engine_LeaderCheckAhead();
#else
		obj = Object_GetById(0);
		pos = &target;
		pos->x = obj->x.fixed;
		pos->y = Object_GetById(0)->y.fixed;
		pos->z = Object_GetById(0)->z.fixed - 0x1e0000;
		KuupuappuHeya_TryJumpTo(pos);
#endif
		x = 42;
		z = 85;
		Engine_MapCopyCellAttributes(41, 85, 1, 1, x, z);
	}
	else if (Object_GetById(0)->facing >= 0x2000 && Object_GetById(0)->facing <= 0x6000)
	{
#if EDITION_INTERNATIONAL
		Engine_LeaderCheckAhead();
#else
		obj = Object_GetById(0);
		pos = &target;
		pos->x = obj->x.fixed;
		pos->y = Object_GetById(0)->y.fixed;
		pos->z = Object_GetById(0)->z.fixed + 0x1e0000;
		KuupuappuHeya_TryJumpTo(pos);
#endif
		x = 42;
		z = 85;
		Engine_MapCopyCellAttributes(43, 85, 1, 1, x, z);
	}
}

static void EV26_PSYNERGY(void)
{
	struct FieldActor *obj;
	s32				   x;
	s32				   z;

	Engine_EventBegin();
	obj = Object_GetById(26);
	if ((obj->x.fixed >> 20) == 42)
	{
		x = 41;
		z = 24;
		Engine_MapCopyCellAttributes(101, 24, 3, 4, x, z);
		Engine_GameFlagSet(0x859);
	}
	Engine_EventEnd();
}

static void EV19_TALK(void)
{
	s32 v = Object_GetById(0)->facing;

	Engine_EventBegin();
	if (v >= 0xa001 && v <= 0xdfff)
	{
		Engine_ShopOpen(4, 19);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x855) == 0)
		{
			Engine_EventSetMessage((s32)MsgKuupuappuMasterHammetIsntOnlyOne);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKuupuappuWeFoundOurStolenWeapons);
		}
		Engine_EventShowMessage(19, 0);
	}
	Engine_EventEnd();
}

static void EV20_TALK(void)
{
	s32 v = Object_GetById(0)->facing;

	Engine_EventBegin();
	if (v >= 0xa001 && v <= 0xdfff)
	{
		Engine_ShopOpen(5, 20);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x855) == 0)
		{
			Engine_EventSetMessage((s32)MsgKuupuappuThievesDidntHitOurHouse);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKuupuappuIfYoureGonnaHeadInto);
		}
		Engine_EventShowMessage(20, 0);
	}
	Engine_EventEnd();
}

static void EV23_TALK(void)
{
	s32 v = Object_GetById(0)->facing;

	Engine_EventBegin();
	if (v >= 0xa001 && v <= 0xdfff)
	{
		Engine_InnOpen(1, 23);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x855) == 0)
		{
			Engine_EventSetMessage((s32)MsgKuupuappuEveryoneThinksOurGuestsThieves);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKuupuappuTheyHidThoseStolenGoods);
		}
		Engine_EventShowMessage(23, 0);
	}
	Engine_EventEnd();
}

static void FieldScene_RunActorEighteenConditionalScene(void)
{
	Engine_EventBegin();
	if (PartyInventory_HasSpace() == 0)
	{
		Engine_ActorSetAnimationAndWait(18, 4);
		Engine_EventWait(20);
		Engine_EventSetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
		Engine_EventShowMessage(18, 0);
	}
	else
	{
		Engine_ItemShowFound(ITEM_BONE, 3);
		Engine_PartyGiveItem(ITEM_BONE, 0);
	}
	Engine_EventEnd();
}

static void EV250_RAISED(void)
{
#if EDITION_INTERNATIONAL
	Battle_InitializeRenderObject();
#endif
	Engine_EventSetMessage((s32)MsgKuupuappuWeDontHaveTimeFor);
	Engine_EventShowMessage(GERALD, 0);
}

static void SceneDialogue_ShowEmptyBarrel(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgKuupuappuRobinCheckedBarrel, 1);
	Engine_MessageShowCentered((s32)MsgKuupuappuButDidntFindAnything, 1);
	Engine_EventEnd();
}

static void SceneDialogue_ShowEmptyChest(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgKuupuappuRobinCheckedChest, 1);
	Engine_MessageShowCentered((s32)MsgKuupuappuButChestWasEmpty, 1);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	switch (gGameState.entrance)
	{
	case 15:
	case 16:
	case 17:
		return gKuupuappuHeyaEventsEntrances15To17;
	}
	if (Engine_GameFlagIsSet(0x855) != 0)
	{
		return gKuupuappuHeyaEventsFlag855;
	}
	return gKuupuappuHeyaEvents;
}

static void ActorPresentation_RunActorModeOneThenZeroWithStep(s32 x)
{
	Engine_ActorSetAnimation(x, 1);
	SceneActor_SetPairZeroAndValue(x, 0, 2);
	Engine_EventShowMessage(x, 0);
}

static void SceneState_RunGuardedActorStep(s32 x)
{
	u8 *flag = &Object_GetById(x)->paused;
	s32 zero = 0;

	*flag = 1;
	Engine_EventBegin();
	Engine_ActorSetAnimation(x, 1);
	Engine_EventWait(2);
	Engine_EventShowMessage(x, 0);
	Engine_EventEnd();
	*flag = zero;
}

static void SceneDialogue_PromptAndCountSkip(s32 x)
{
	SceneActor_SetPairZeroAndValue(x, 0, 2);
	Engine_EventOpenMessage(x, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	Engine_EventShowMessage(x, 0);
}

static void SceneDialogue_RunActorElevenDialogue(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuMisterFunSeeStrangeNew);
	Engine_ActorSetAnimation(11, 1);
	SceneDialogue_PromptAndCountSkip(11);
	Engine_EventEnd();
}

static void EV15_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuIvanHasGreatPowersWouldnt);
	SceneDialogue_PromptAndCountSkip(15);
	Actor_FaceDirection(15, 0x8000, 0);
	Engine_EventEnd();
}

static void EV21_TALK(void)
{
	s32 value = Object_GetById(0)->facing;

	Engine_EventBegin();
	if (value >= 0xa001 && value <= 0xdfff)
	{
		Engine_ShopOpen(6, 21);
	}
	else if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuDidJustArriveInTown);
		SceneDialogue_PromptAndCountSkip(21);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWithBridgeOutWillQuite);
		Engine_EventShowMessage(21, 0);
	}
	Engine_EventEnd();
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuIfOnlyTheseRocksWere);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuThankGoodnessThoseThievesWere);
	}
	ActorPresentation_RunActorModeOneThenZeroWithStep(9);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) != 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuThievesHidStolenTreasureIn);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuJustMeOrAmMissing);
	}
	ActorPresentation_RunActorModeOneThenZeroWithStep(12);
	Engine_EventEnd();
}

static void FieldScene_RunFlag856DialogueBranch(void)
{
	s32 g;
	g = 0x851;
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x856) != 0)
	{
		if (Engine_GameFlagIsSet(g) == 0)
		{
			Engine_EventSetMessage((s32)MsgKuupuappuYoureGoingHelpIvan);
			ActorPresentation_RunActorModeOneThenZeroWithStep(16);
			Engine_EventWait(10);
			SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
			Engine_GameFlagSet(g);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgKuupuappuPleaseLookAfterIvan);
		}
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuCouldSomeonePleaseHelpIvan);
	}
	ActorPresentation_RunActorModeOneThenZeroWithStep(16);
	Engine_EventEnd();
}

static void SceneDialogue_ShowLine128E(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuThoseThreeStrangersSureHave);
	ActorPresentation_RunActorModeOneThenZeroWithStep(18);
	Engine_EventEnd();
}

static void SceneActor_StepActor24AnimationByFacing(void)
{
	struct FieldActor *p;
	s16				  *q;
	s32				   v;
	s32				   n;

	p = Object_GetById(24);
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(24, 2);
	Engine_EventSetMessage((s32)MsgKuupuappuOwStop);
	Engine_EventShowMessage(24, 0);
	Engine_ActorSetSpeed(24, 0x40000, 0x20000);
	if ((u32)((p->facing & 0xf000) - 0x5000) <= 0x6000)
	{
		q = (s16 *)&p->timer;
		v = *q;
		if (v <= 2)
		{
			Engine_ActorEnableActionCallback(24, KuupuappuHeya_StepActions[v]);
			*(u16 *)q = *(u16 *)q + 1;
			goto clamp;
		}
	}
	else
	{
		q = (s16 *)&p->timer;
		v = *q;
		if (v > 2)
		{
			Engine_ActorEnableActionCallback(24, KuupuappuHeya_StepActions[v]);
			*(u16 *)q = *(u16 *)q + 1;
			goto clamp;
		}
	}
	Engine_ActorEnableActionCallback(24, KuupuappuHeya_IdleActions[v]);
	n = *(u16 *)q - 1;
	*(u16 *)q = n;
clamp:
	if (*q > 5)
	{
		n = 0;
		*(u16 *)q = n;
	}
	if (*q < 0)
	{
		n = 5;
		*(u16 *)q = n;
	}
	Object_RefreshSelectorById(24);
	Engine_EventEnd();
}

static inline void KuupuappuHeya_WalkOn(struct FieldActor *actor, s32 row, s32 half, u16 turn)
{
	Engine_ActorEnableActionCallback(25, KuupuappuHeya_ResponseScripts[row * 4 + actor->timer]);
	actor->timer = actor->timer - half * 2 + turn;
}

static void KuupuappuHeya_RunActor25Response(void)
{
	struct FieldActor *actor;
	u32				   facing;
	s32				   half;

	actor = Object_GetById(25);
	facing = actor->facing & 0xf000;
	half = actor->timer >> 1;
	Engine_EventBegin();
	Engine_ActorRunRepeatedMotion(25, 2);
	Engine_EventSetMessage((s32)MsgKuupuappuNoLeaveAlone);
	Engine_EventShowMessage(25, 0);
	Engine_ActorSetSpeed(25, 0x38000, 0x1c000);
	switch (actor->timer)
	{
	case 4:
		if (facing > 0x2000 && facing < 0xa000)
		{
			Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptR);
			actor->timer = 2;
		}
		else
		{
			Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptP);
			actor->timer = 3;
		}
		break;
	case 0:
	case 2:
		if (facing > 0x2000 && facing < 0xa000)
		{
			KuupuappuHeya_WalkOn(actor, half, half, 1);
		}
		else
		{
			KuupuappuHeya_WalkOn(actor, half ^ 1, half, -1);
		}
		break;
	case 1:
	case 3:
		if (facing > 0x6000 && facing < 0xe000)
		{
			KuupuappuHeya_WalkOn(actor, half, half, 1);
		}
		else
		{
			KuupuappuHeya_WalkOn(actor, half ^ 1, half, -1);
		}
		break;
	}
	actor->timer &= 3;
	Object_RefreshSelectorById(25);
	Engine_EventEnd();
}

#define RATIO_HI 52428
#define RATIO_LO 26214

struct Stop
{
	u8 id;
	u8 pos;
	u8 unknown_02[2];
};

struct StopSet
{
	u8			unknown_00[4];
	struct Stop stops[3];
};

struct StopRecord
{
	u8 unknown_00[16];
};

static void EV10_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuDidThoseThievesComeFrom);
	SceneDialogue_PromptAndCountSkip(10);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor11Line(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuMyFatherWorriedAboutThose);
	ActorPresentation_RunActorModeOneThenZeroWithStep(11);
	Engine_EventEnd();
}

static void EV14_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuHeardDefeatedThoseThieves);
	SceneDialogue_PromptAndCountSkip(14);
	Engine_EventEnd();
}

static void FieldScene_RunScene383SequenceC(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x857) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuYouWereSuchGreatHelp);
		SceneActor_SetModeZeroAndValue(16, 20);
		SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
		SceneActor_SetModeZeroAndValue(16, 30);
		Actor_FaceDirection(16, 0, 0);
		Engine_EventWait(30);
		Engine_ActorRunRepeatedMotion(16, 2);
		Engine_EventWait(30);
		SceneActor_SetPairZeroAndValue(0, 16, 20);
		SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
		gEventWork->message++;
		if (PartyInventory_HasSpace() == 0)
		{
			Engine_EventSetMessage(((s32)MsgKuupuappuYouWereSuchGreatHelp + 3));
			SceneActor_SetModeZeroAndValue(16, 20);
			Engine_EventEnd();
			goto done;
		}
		GameFlag_Set(0x857);
		Engine_PartyGiveItem(ITEM_WATER_OF_LIFE, 0);
	}
	Engine_EventSetMessage((s32)MsgKuupuappuHeadingOutBeyondGomaRange);
	Event_OpenMessage(16, 0);
	Engine_EventWait(20);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	Event_ShowMessage(16, 0);
	Engine_EventEnd();
done:;
}

static void KuupuappuHeya_OfferBone(void)
{
	Engine_EventBegin();
	SceneActor_SetPairZeroAndValue(18, 0, 2);
	if (GameFlag_IsSet(0x85b) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuHaveLotLeftoverBonesFrom);
		Event_OpenMessage(18, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWantMoreBones);
		Event_OpenMessage(18, 0);
	}
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Event_ShowMessage(18, 0);
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(18, 2);
		Engine_EventWait(20);
		if (PartyInventory_HasSpace() == 0)
		{
			Engine_ActorSetAnimationAndWait(18, 4);
			Engine_EventWait(20);
			Engine_EventSetMessage((s32)MsgKuupuappuWowHaveManyThingsArent);
			Event_ShowMessage(18, 0);
			goto L_020009ec;
		}
		Engine_ItemShowFound(ITEM_BONE, 3);
		Engine_PartyGiveItem(ITEM_BONE, 0);
		GameFlag_Set(0x85b);
	}
	else
	{
		gEventWork->message++;
		Engine_EventWait(20);
		Engine_ActorSetAnimationAndWait(18, 3);
		Engine_EventWait(20);
		Event_ShowMessage(18, 0);
	}
L_020009ec:;
	Actor_FaceDirection(18, 0x4000, 0);
	Engine_EventEnd();
}

static void EV09_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuOkMisterLetMeSee);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuIfRockWorthlessMaybeThats);
	}
	SceneState_RunGuardedActorStep(9);
	Engine_EventEnd();
}

static void EV11_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWonderOutsideWorldLike);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuFatherLooksSadWorryingLike);
	}
	SceneState_RunGuardedActorStep(11);
	Engine_EventEnd();
}

static void EV12_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuCouldTheyThievesGoodDont);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuGuessNothingInOurHouse);
	}
	SceneState_RunGuardedActorStep(12);
	Engine_EventEnd();
}

static void EV16_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuTicklesBeingTickledByBoy);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuCaveInGomaRangeDangerous);
	}
	SceneState_RunGuardedActorStep(16);
	Engine_EventEnd();
}

static void EV18_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuMasterHisWifeBlindedBy);
	}
	else if (Engine_GameFlagIsSet(0x85b) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuGeeAlwaysGetHungryWhen);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuHeReallyLikesBonesWonder);
	}
	SceneState_RunGuardedActorStep(18);
	Engine_EventEnd();
}

static void EV02_TALK(void)
{

	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	Event_PlayCue(19);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x180, 0x198);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_SetPosition(GERALD, 0x1800000, 0x1980000);
	Actor_WalkToAndWait(GERALD, 0x170, 0x198);
	Actor_FaceDirection(GERALD, 0xd000, 20);
	if (GameFlag_IsSet(0x850) != 0)
	{
	}
	else
	{
		GameFlag_Set(0x850);
		SceneEffect_ApplyPairWithValue141(2, 0);
		Engine_EventWait(40);
		SceneState_SetValue2ThenFinish();
		Engine_EventSetMessage((s32)MsgKuupuappuDoPossessStrangePowers);
		Event_PlayCue(60);
		Engine_EventWait(30);
		SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
		SceneActor_SetModeZeroAndValue(2, 30);
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		Engine_ActorRunRepeatedMotion(GERALD, 1);
		Engine_EventWait(20);
		SceneEffect_ApplyPairWithValue141(2, 0);
		Engine_EventWait(40);
		SceneState_SetValue2ThenFinish();
		SceneActor_SetModeZeroAndValue(2, 30);
		SceneActor_SetPairZeroAndValue(0, 1, 50);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
		Actor_FaceDirection(GERALD, 0xd000, 0);
		Engine_EventWait(20);
		SceneEffect_ApplyPairWithValue141(2, 0);
		Engine_EventWait(40);
		SceneState_SetValue2ThenFinish();
		Engine_ActorRunRepeatedMotion(IWAN, 1);
		SceneActor_SetModeZeroAndValue(2, 50);
		Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
		SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
		SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
		SceneActor_SetModeZeroAndValue(2, 40);
		Engine_ActorRunRepeatedMotion(IWAN, 1);
		Engine_EventWait(30);
		Actor_FaceDirection(2, 0xc000, 0);
		Engine_EventWait(30);
		Actor_WalkToAndWait(IWAN, 0x178, 0x178);
		Engine_EventWait(40);
		SceneActor_SetPairZeroAndValue(0, 1, 50);
		Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
		Actor_FaceDirection(GERALD, 0xd000, 0);
		Engine_ActorRunRepeatedMotion(IWAN, 1);
		Engine_EventWait(50);
		SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
		Actor_FaceDirection(IWAN, 0x4000, 0);
		Engine_EventWait(10);
		Event_ShowMessage(IWAN, 0);
		Event_OpenMessage(IWAN, 0);
		goto L_join_setup_paths;
	}
	Event_PlayCue(60);
	Engine_EventSetMessage((s32)MsgKuupuappuWouldReallyWouldHelpMe);
	Event_OpenMessage(IWAN, 0);
L_join_setup_paths:;
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Scene_JoinRodSearch();
		GameFlag_Set(0x856);
		Engine_ActorSetAnimation(IWAN, 2);
		record = Object_GetById(0);
		if (record != 0)
		{
			Actor_SetDestination(IWAN, record->x.part.pixel, record->z.part.pixel);
		}
		Engine_ActorWaitForMove(IWAN);
		Actor_SetPosition(IWAN, 0, 0);
	}
	else
	{
		Event_ShowMessage(IWAN, 0);
	}
	Engine_ActorSetAnimation(GERALD, 2);
	record = Object_GetById(0);
	if (record != 0)
	{
		Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Audio_PlayCueFromEventWork();
	Engine_EventEnd();
}

static void Scene_JoinRodSearch(void)
{
	s32 mes;

	Engine_EventWait(20);
	Object_GetById(2)->paused = 0;
	Engine_ActorJump(2, 4, 0);
	Engine_EventWait(40);
	mes = (s32)MsgKuupuappuThankHelpBelieve;
	Engine_EventSetMessage(mes);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(2, 0x101, 0);
	Engine_EventWait(50);
	SceneActor_SetModeZeroAndValue(2, 30);
	Actor_WalkToAndWait(2, 0x178, 0x188);
	SceneEffect_ApplyPairWithValue141(2, 0);
	Engine_EventWait(40);
	SceneState_SetValue2ThenFinish();
	Engine_ActorRunRepeatedMotion(0, 1);
	Object_GetById(0)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(0, 0x180, 0x1a8);
	Engine_EventWait(1);
	Object_GetById(0)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_EventWait(30);
	Engine_EventShowMessage(2, 0);
	Actor_SetAttachedEffect(0, 0x102);
	Actor_SetAttachedEffect(1, 0x102);
	Engine_EventWait(60);
	Engine_ActorSetAnimation(0, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
	Actor_ShowEmote(2, 0x101, 0);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(2, 1);
	Engine_EventWait(10);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	Engine_EventShowMessage(2, 0);
	Engine_ActorRunRepeatedMotion(1, 2);
	Engine_EventWait(10);
	FieldScene_RunSplitTripleSteps(1, 0, 30);
	Engine_ActorRunRepeatedMotion(0, 2);
	Engine_EventWait(10);
	FieldScene_RunSplitTripleSteps(0, 1, 40);
	Actor_ShowEmote(0, 0x102, 0);
	Actor_ShowEmote(1, 0x102, 0);
	Engine_EventWait(60);
	Actor_ShowEmote(2, 0x102, 0);
	Engine_EventWait(60);
	Engine_EventOpenMessage(2, 0);
	Engine_ActorFaceActor(0, 2, 0);
	Engine_ActorFaceActor(1, 2, 0);
	Engine_EventChooseYesNo(0, 0);
	Engine_EventWait(30);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	Actor_WalkToAndWait(2, 0x180, 0x198);
	Engine_EventWait(10);
	Engine_MessageShowCentered(mes + 5, 1);
	Engine_EventSetMessage(mes + 6);
	Engine_ActorFaceEachOther(2, 1, 0);
	FieldScene_RunSplitTripleSteps(0, 1, 20);
	Actor_ShowEmote(1, 0x102, 0);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(1, 1);
	Object_GetById(1)->steering_flags &= ~ACTOR_FACES_MOTION;
	Actor_WalkToAndWait(1, 0x160, 0x198);
	Engine_EventWait(1);
	Object_GetById(1)->steering_flags |= ACTOR_FACES_MOTION;
	Engine_EventWait(10);
	Actor_WalkTo(2, 0x170, 0x198);
	Engine_EventWait(20);
	Actor_WalkToAndWait(0, 0x170, 0x1a8);
	Engine_ActorFaceActor(0, 1, 0);
	Engine_ActorWaitForMove(2);
	SceneEffect_ApplyPairWithValue141(2, 1);
	SceneEffect_ApplyThreeValuesAndFinish(1, 4, 10);
	Engine_EventShowMessage(1, 0);
	SceneState_SetValue2ThenFinish();
	SceneActor_SetPairZeroAndValue(2, 0, 30);
	Engine_EventShowMessage(2, 0);
	Engine_ActorRunRepeatedMotion(0, 1);
	Engine_EventWait(30);
	SceneEffect_ApplyThreeValuesAndFinish(0, 3, 30);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	Engine_EventShowMessage(2, 0);
	Engine_ActorRunRepeatedMotion(1, 2);
	Engine_ActorFaceActor(1, 0, 0);
	Engine_EventShowMessage(1, 0);
	Engine_ActorFaceActor(0, 1, 0);
	SceneEffect_ApplyThreeValuesAndFinish(0, 3, 40);
	Engine_ActorFaceActor(0, 2, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 30);
	Actor_ShowEmote(0, 0x101, 0);
	Actor_ShowEmote(1, 0x101, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	Engine_EventShowMessage(2, 0);
	Engine_ActorShowEmote(1, 0x103, 0);
	Engine_EventWait(60);
	FieldScene_RunSplitTripleSteps(1, 0, 10);
	Engine_EventShowMessage(1, 0);
	Engine_ActorShowEmote(2, 0x100, 0);
	Engine_EventWait(60);
	Engine_EventShowMessage(2, 0);
	Actor_ShowEmote(1, 0x102, 0);
	Engine_EventWait(60);
	Engine_MessageShowCentered(mes + 13, 1);
	Engine_EventSetMessage(mes + 14);
	Engine_ActorFaceEachOther(2, 0, 0);
	Engine_ActorFaceActor(1, 0, 0);
	Engine_ActorShowEmote(0, 0x102, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyPairWithValue141(2, 0);
	Ui_SetBank15PaletteAndClearRenderMode();
	SceneActor_SetModeZeroAndValue(1, 30);
	SceneState_SetValue2ThenFinish();
	Engine_ActorFaceEachOther(2, 1, 0);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	SceneActor_SetModeZeroAndValue(2, 20);
	Engine_ActorFaceEachOther(2, 0, 0);
	SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
	Engine_ActorRunRepeatedMotion(1, 1);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(1, 4, 10);
	SceneActor_SetModeZeroAndValue(1, 30);
	Engine_ActorFaceActor(2, 1, 0);
	Actor_ShowEmote(2, 0x101, 0);
	Engine_EventWait(30);
	SceneActor_SetPairZeroAndValue(0, 1, 10);
	Engine_ActorSetAnimation(0, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
	Engine_ActorFaceActor(0, 2, 0);
	Engine_ActorFaceActor(1, 2, 0);
	Engine_ActorFaceActor(2, 0, 0);
	Engine_ActorShowEmote(2, 0x106, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(2, 10);
	Engine_ActorSetAnimation(0, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(0, 0x101, 0);
	Engine_ActorShowEmote(1, 0x101, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	SceneActor_SetModeZeroAndValue(2, 30);
	Engine_ActorShowEmote(0, 0x105, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 10);
	SceneActor_SetModeZeroAndValue(2, 30);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	Engine_ActorSetAnimation(0, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
	Event_PrepareObjectAndApplyValue(2, 1);
	Engine_EventSetMessage(mes + 22);
	Engine_ActorRunRepeatedMotion(2, 1);
	Engine_EventShowMessage(2, 0);
}

static void EV08_TALK(void)
{
	s32 base;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 52428, 26214);
	Actor_SetSpeed(GERALD, 52428, 26214);
	Actor_SetSpeed(IWAN, 52428, 26214);
	Engine_ActorRunRepeatedMotion(0, 3);
	Engine_EventWait(20);
	base = (s32)MsgKuupuappuVaultCutFree;
	Engine_MessageShowCentered(base, 1);
	base++;
	Engine_EventSetMessage(base);
	Engine_EventWait(30);
	Engine_ActorSetAnimation(8, 1);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(8, 3, 40);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 792, 440);
	FieldScene_RunSplitTripleSteps(0, 8, 20);
	Actor_SetPosition(GERALD, 51904512, 28835840);
	Actor_SetPosition(IWAN, 51904512, 28835840);
	Actor_WalkTo(GERALD, 808, 432);
	Actor_WalkTo(IWAN, 792, 456);
	Engine_ActorWaitForMove(1);
	Engine_ActorFaceActor(1, 8, 0);
	Engine_ActorWaitForMove(2);
	FieldScene_RunSplitTripleSteps(2, 8, 60);
	Actor_ShowEmote(8, 258, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(8, 20);
	Engine_ActorSetAnimation(2, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 30);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(8, 20);
	Actor_ShowEmote(IWAN, 258, 0);
	Actor_ShowEmote(GERALD, 258, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(8, 12288, 0);
	Engine_EventWait(10);
	SceneActor_SetModeZeroAndValue(8, 30);
	Actor_FaceDirection(8, 53248, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(8, 256, 0);
	Engine_EventWait(60);
	Actor_FaceDirection(8, 45056, 0);
	Engine_EventWait(40);
	Actor_FaceDirection(8, 53248, 0);
	Engine_EventWait(40);
	FieldScene_RunSplitTripleSteps(8, 0, 20);
	SceneEffect_ApplyThreeValuesAndFinish(8, 4, 30);
	SceneActor_SetModeZeroAndValue(8, 20);
	Engine_ActorRunRepeatedMotion(2, 1);
	SceneActor_SetModeZeroAndValue(2, 40);
	Actor_SetPosition(10, 48758784, 26738688);
	Engine_AudioPlayCue(61);
	SceneActor_SetModeZeroAndValue(10, 20);
	Engine_ActorStartRepeatedMotion(0, 1);
	Engine_ActorStartRepeatedMotion(1, 1);
	Engine_ActorStartRepeatedMotion(2, 1);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(30);
	Engine_ActorFaceActor(0, 10, 0);
	Engine_ActorFaceActor(1, 10, 0);
	Engine_ActorFaceActor(2, 10, 0);
	FieldScene_RunSplitTripleSteps(8, 10, 40);
	Actor_SetSpeed(10, 52428, 26214);
	Actor_SetSpeed(11, 98304, 49152);
	Actor_SetSpeed(12, 98304, 49152);
	Actor_SetPosition(11, 48758784, 26738688);
	Actor_SetPosition(12, 48758784, 26738688);
	Actor_WalkToAndWait(10, 792, 416);
	Actor_FaceDirection(10, 12288, 0);
	Engine_EventWait(70);
	Engine_ActorFaceActor(0, 10, 0);
	Actor_FaceActor(GERALD, 10, 0);
	Engine_ActorFaceActor(2, 10, 0);
	Engine_ActorFaceActor(8, 10, 0);
	Engine_ActorEnableActionCallback(11, KuupuappuHeya_Villager11ExitScript);
	Engine_EventWait(40);
	Engine_ActorEnableActionCallback(12, KuupuappuHeya_Villager12ExitScript);
	Object_RefreshSelectorById(12);
	Actor_FaceDirection(11, 8192, 0);
	Actor_FaceDirection(12, 8192, 0);
	Engine_EventWait(40);
	SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
	SceneActor_SetModeZeroAndValue(10, 20);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventWait(10);
	SceneActor_SetModeZeroAndValue(11, 20);
	Engine_ActorRunRepeatedMotion(12, 1);
	Engine_EventWait(10);
	SceneActor_SetModeZeroAndValue(12, 40);
	Actor_ShowEmote(IWAN, 257, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
	SceneActor_SetModeZeroAndValue(10, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 257, 0);
	Actor_ShowEmote(GERALD, 257, 0);
	Engine_EventWait(60);
	Actor_ShowEmote(8, 256, 0);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(8, 2);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(8, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
	Engine_ActorFaceActor(1, 8, 0);
	FieldScene_RunSplitTripleSteps(2, 8, 20);
	SceneEffect_ApplyThreeValuesAndFinish(8, 4, 30);
	SceneActor_SetModeZeroAndValue(8, 20);
	Engine_ActorRunRepeatedMotion(2, 2);
	Engine_EventWait(20);
	Engine_ActorFaceActor(0, 2, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 30);
	SceneActor_SetModeZeroAndValue(1, 20);
	Actor_ShowEmote(8, 258, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(8, 20);
	Engine_ActorRunRepeatedMotion(10, 1);
	Engine_EventWait(10);
	SceneActor_SetModeZeroAndValue(10, 20);
	Engine_ActorFaceActor(0, 10, 0);
	Actor_FaceActor(GERALD, 10, 0);
	FieldScene_RunSplitTripleSteps(2, 10, 20);
	SceneEffect_ApplyThreeValuesAndFinish(11, 3, 20);
	SceneActor_SetModeZeroAndValue(11, 20);
	SceneEffect_ApplyThreeValuesAndFinish(12, 4, 20);
	SceneActor_SetModeZeroAndValue(12, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 258, 0);
	Actor_ShowEmote(GERALD, 258, 0);
	Actor_ShowEmote(IWAN, 258, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(10, 3, 20);
	Engine_EventOpenMessage(10, 0);
	Engine_EventWait(50);
	Engine_ActorFaceActor(2, 0, 0);
	FieldScene_RunSplitTripleSteps(1, 0, 30);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(40);
		Engine_ActorFaceActor(1, 10, 0);
		Actor_FaceActor(IWAN, 10, 0);
		Engine_ActorRunRepeatedMotion(10, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(10, 0);
	}
	else
	{
		Engine_EventWait(40);
		Engine_ActorFaceActor(1, 10, 0);
		Engine_ActorFaceActor(2, 10, 0);
		base = (s32)MsgKuupuappuVaultThievesCaught;
		Engine_EventSetMessage(base);
		Engine_ActorRunRepeatedMotion(10, 2);
		Engine_EventWait(20);
		Engine_EventShowMessage(10, 0);
		base -= 3;
		Engine_EventSetMessage(base);
	}
	Actor_ShowEmote(11, 259, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(11, 20);
	SceneEffect_ApplyThreeValuesAndFinish(12, 4, 20);
	SceneActor_SetModeZeroAndValue(12, 30);
	SceneActor_SetPairZeroAndValue(10, 11, 30);
	Engine_ActorSetAnimation(10, 3);
	SceneEffect_ApplyThreeValuesAndFinish(11, 3, 30);
	SceneActor_SetPairZeroAndValue(10, 12, 30);
	Engine_ActorSetAnimation(10, 3);
	SceneEffect_ApplyThreeValuesAndFinish(12, 3, 40);
	Actor_FaceDirection(10, 12288, 0);
	Actor_FaceDirection(11, 12288, 0);
	Actor_FaceDirection(12, 12288, 0);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
	Engine_EventShowMessage(10, 0);
	GameFlag_Set(2132);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	base = (s32)&SceneId_KuupuappuHeya;
	Party_SetFields1ceAnd1d0(base, 17);
	Event_SetDefeatDestination(base, 16);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(12, 5);
	Engine_EventEnd();
}

static void EV07_TOUCH(void)
{
	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(11);
}

static void EV26_TOUCH(void)
{
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 728, 408);
	gEventWork->transition_frames = 16;
	Event_PlayCue(123);
	Engine_EventRequestExit(15);
}

static void EV27_TOUCH(void)
{
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 32768, 16384);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 744, 408);
	if (GameFlag_IsSet(0x854) == 0)
	{
		Engine_EventBegin();
		Engine_EventSetMessage((s32)MsgKuupuappuUhnnUhnn);
		Event_ShowMessage(8, 0);
		Engine_EventEnd();
	}
	gEventWork->transition_frames = 16;
	Engine_AudioPlayCue(123);
	Engine_EventRequestExit(14);
}

static void EV10_TOUCH(void)
{

	struct FieldActor *actor24;
	struct FieldActor *actor25;

	actor24 = Object_GetById(24);
	actor25 = Object_GetById(25);
	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, RATIO_HI, RATIO_LO);
	Actor_SetSpeed(GERALD, RATIO_HI, RATIO_LO);
	Actor_SetSpeed(IWAN, RATIO_HI, RATIO_LO);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 232, 696);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 200, 696);
	Engine_EventWait(10);
	Actor_ShowEmote(25, 256, 0);
	Actor_ShowEmote(24, 256, 0);
	Engine_EventWait(60);
	FieldScene_RunSplitTripleSteps(25, 0, 10);
	Engine_ActorRunRepeatedMotion(24, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgKuupuappuWant);
	SceneActor_SetModeZeroAndValue(24, 20);
	Actor_SetAttachedEffect(25, 258);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(25, 20);
	Engine_ActorRunRepeatedMotion(24, 1);
	SceneActor_SetModeZeroAndValue(24, 30);
	Actor_SetSpeed(24, 262144, 131072);
	Actor_SetSpeed(25, 229376, 114688);
	Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptO);
	Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptB);
	Object_RefreshSelectorById(24);
	Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
	GameFlag_Set(2130);
	GameFlag_Set(768);
	Engine_TaskAddCallback((s32)SceneActor_FaceActors24And25TowardActorZero, 3200);
	actor24->timer = 1;
	actor25->timer = 3;
	Engine_EventEnd();
}

static void EV20_TOUCH(void)
{
	struct FieldActor *actor24;
	struct FieldActor *actor25;
	s32				   base;

	actor24 = Object_GetById(24);
	actor25 = Object_GetById(25);
	Engine_EventBegin();
	Scheduler_RemoveCallback(((s32)SceneActor_FaceActors24And25TowardActorZero));
	GameFlag_Clear(0x300);
	if (actor24->timer <= 3)
	{
		Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptG);
	}
	else
	{
		Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptF);
	}
	if (actor25->timer <= 2)
	{
		Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptK);
	}
	else
	{
		Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptF);
	}
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 0x2d8);
	Actor_SetPosition(IWAN, 0xf80000, 0x2d80000);
	Actor_SetPosition(GERALD, 0xf80000, 0x2d80000);
	Actor_WalkTo(IWAN, 0x108, 0x2e8);
	Actor_WalkToAndWait(GERALD, 232, 0x2e8);
	Engine_ActorWaitForMove(IWAN);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Actor_FaceActor(GERALD, ACTOR_PARTY_LEADER, 0);
	FieldScene_RunSplitTripleSteps(2, 0, 30);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	base = (s32)MsgKuupuappuTheyreActingSuspiciousSomethingsNot;
	Engine_EventSetMessage(base);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
	SceneActor_SetPairZeroAndValue(0, 1, 10);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	SceneActor_SetModeZeroAndValue(1, 30);
	Engine_EventSetMessage((base + 4));
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 50);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Engine_EventWait(60);
	SceneActor_SetPairZeroAndValue(0, 1, 50);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 30);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(2, 30);
	Engine_EventSetMessage((s32)MsgKuupuappuRobinTakeLead);
	Event_ShowMessage(GERALD, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
	Engine_EventWait(40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
	Actor_WalkTo(IWAN, 248, 0x2d8);
	Actor_WalkToAndWait(GERALD, 248, 0x2d8);
	Actor_SetPosition(GERALD, 0, 0);
	Actor_SetPosition(IWAN, 0, 0);
	Actor_SetPosition(24, 0x680000, 0x2b80000);
	Actor_SetPosition(25, 0x780000, 0x2b80000);
	Actor_FaceDirection(24, 0, 0);
	Actor_FaceDirection(25, 0x8000, 0);
	Map_CopyCellAttributes(14, 50, 3, 1, 14, 44);
	Engine_EventEnd();
}

static void EV21_TOUCH(void)
{
	Engine_GameFlagSet(0x107);
	Engine_GameFlagSet(0x250);
	KuupuappuHeya_StartActorStops();
}

static void FieldScene_ConfigurePairedActors(void)
{
	Engine_EventWait(30);
	Engine_ActorRunRepeatedMotion(24, 1);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgKuupuappuTheyreBack);
	SceneActor_SetModeZeroAndValue(24, 20);
	Actor_FaceDirection(25, 0, 20);
	Actor_SetAttachedEffect(25, 0x102);
	Engine_ActorRunRepeatedMotion(25, 2);
	SceneActor_SetModeZeroAndValue(25, 20);
	Engine_ActorSetAnimationAndWait(24, 4);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(24, 20);
	Actor_SetSpeed(24, 0x40000, 0x20000);
	Actor_SetSpeed(25, 0x38000, 0x1c000);
	Engine_ActorEnableActionCallback(25, KuupuappuHeya_PairScriptO);
	Engine_ActorEnableActionCallback(24, KuupuappuHeya_PairScriptB);
	Object_RefreshSelectorById(24);
	{
		struct FieldActor *record = Object_GetById(24);
		u16				   shown = 1;

		record->timer = shown;
	}
	{
		struct FieldActor *record = Object_GetById(25);
		u16				   shown = 3;

		record->timer = shown;
	}
	Map_CopyCellAttributes(14, 48, 4, 1, 14, 44);
}

static void FieldScene_SelectActorPair(void)
{
	u8				 *record;
	s32				  actor;
	struct EventWork *work;

	work = gEventWork;
	Engine_EventBegin();
	Scheduler_RemoveCallback(((s32)KuupuappuHeya_UpdateActorStops));
	GameFlag_Clear(0x107);
	GameFlag_Clear(0x250);
	Engine_ActorSetAnimation(24, 1);
	Engine_ActorSetAnimation(25, 1);
	Engine_ActorSetAnimation(IWAN, 1);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetAnimation(GERALD, 1);
	Actor_FaceActor(GERALD, IWAN, 0);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(24, IWAN, 0);
	Actor_FaceActor(25, IWAN, 0);
	Engine_EventWait(10);
	actor = 24;
	switch (work->raised_trigger)
	{
	case 202:
	case 203:
		Engine_EventSetMessage((s32)MsgKuupuappuImSurrounded);
		Actor_SetAttachedEffect(25, 0x102);
		Engine_ActorRunRepeatedMotion(25, 2);
		SceneActor_SetModeZeroAndValue(25, 20);
		if (work->raised_trigger == 202)
		{
			actor = 25;
			break;
		}
	case 201:
		Engine_EventSetMessage((s32)MsgKuupuappuTheresNowhereRun);
		Actor_SetAttachedEffect(24, 0x102);
		Engine_ActorRunRepeatedMotion(24, 2);
		actor = 24;
		SceneActor_SetModeZeroAndValue(24, 20);
		break;
	}
	Engine_ActorSetAnimation(IWAN, 1);
	Actor_FaceActor(IWAN, actor, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Engine_ActorRunRepeatedMotion(GERALD, 2);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgKuupuappuNowIvan);
	SceneActor_SetModeZeroAndValue(1, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(2, 20);
	SceneEffect_ApplyPairWithValue141(2, actor);
	Ui_SetBank15PaletteAndClearRenderMode();
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(24, 2);
	SceneActor_SetModeZeroAndValue(24, 20);
	Engine_ActorRunRepeatedMotion(25, 2);
	SceneActor_SetModeZeroAndValue(25, 20);
	Actor_ShowEmote(IWAN, 0x100, 60);
	SceneState_SetValue2ThenFinish();
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(1, 20);
	Actor_FaceActor(IWAN, GERALD, 0);
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(IWAN, 3);
	Engine_EventWait(40);
	record = (u8 *)Object_GetById(0);
	((struct FieldSpritePriority *)((struct FieldActor *)record)->sprite)->priority = 1;
	record = (u8 *)Object_GetById(1);
	((struct FieldSpritePriority *)((struct FieldActor *)record)->sprite)->priority = 1;
	record = (u8 *)Object_GetById(2);

	((struct FieldSpritePriority *)((struct FieldActor *)record)->sprite)->priority = 1;
	gEventWork->transition_frames = 24;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	KuupuappuHeya_RunBranchingDialogueScene();
	Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
	GameFlag_Set(0x853);
	{
		u8 *record = (u8 *)Object_GetById(24);
		s32 shown = 5;

		((struct FieldActor *)record)->timer = shown;
	}
	{
		u8 *record = (u8 *)Object_GetById(25);
		s32 shown = 4;

		((struct FieldActor *)record)->timer = shown;
	}
	Engine_TaskAddCallback((s32)SceneActor_FaceActors24And25TowardActorZero, 0xc80);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	Engine_EventEnd();
}

static void KuupuappuHeya_RunBranchingDialogueScene(void)
{
	u8 *record;
	s32 v5;

	Audio_PlayCueFromEventWork();
	Engine_ActorSetSpritePriority(0, 1);
	{
		struct FieldActor *record = Object_GetById(0);

		record->priority_flags = ACTOR_PRIORITY_AUTOMATIC | record->priority_flags;
	}
	Map_SetViewBounds(0x200000, 0x2400000, 0x1900000, 0x3a80000);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Actor_SetSpeed(1, 0xcccc, 0x6666);
	Actor_SetSpeed(2, 0xcccc, 0x6666);
	Actor_SetPosition(0, 0xf80000, 0x2d80000);
	Actor_SetPosition(2, 0x1080000, 0x2e80000);
	Engine_ActorSetPosition(1, 0xe80000, 0x2e80000);
	record = (u8 *)Object_GetById(0);
	((struct FieldSpritePriority *)((struct FieldActor *)record)->sprite)->priority = 1;
	record = (u8 *)Object_GetById(1);
	((struct FieldSpritePriority *)((struct FieldActor *)record)->sprite)->priority = 1;
	record = (u8 *)Object_GetById(2);
	((struct FieldSpritePriority *)((struct FieldActor *)record)->sprite)->priority = 1;
	Engine_ActorFaceEachOther(0, 2, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 30);
	Actor_SetPosition(24, 0x680000, 0x2b80000);
	Actor_SetPosition(25, 0x780000, 0x2b80000);
	Engine_ActorFaceEachOther(24, 25, 0);
	Engine_CameraFollowActor(0, 0);
	Engine_CameraWaitForMove();
	Engine_MapRedraw();
	Engine_EventWait(30);
	{
		u8 *work = *(u8 **)&gEventWork;

		((struct EventWork *)work)->transition_frames = 24;
		((struct EventWork *)work)->start_transition = 0x201;
	}
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(1, 1);
	Engine_EventWait(10);
	Engine_EventSetMessage((s32)MsgKuupuappuLearn);
	SceneActor_SetModeZeroAndValue(1, 20);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	SceneActor_SetPairZeroAndValue(0, 1, 50);
	Engine_ActorFaceActor(0, 2, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 20);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
	Engine_EventShowMessage(2, 0);
	Actor_ShowEmote(0, 0x102, 0);
	Actor_ShowEmote(1, 0x102, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(2, 30);
	Engine_ActorRunRepeatedMotion(1, 1);
	Engine_EventWait(20);
	SceneActor_SetPairZeroAndValue(0, 1, 20);
	Engine_EventOpenMessage(1, 0);
	v5 = 0;
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		Engine_EventWait(20);
		Engine_ActorStartRepeatedMotion(2, 2);
		SceneEffect_ApplyThreeValuesAndFinish(1, 4, 30);
		Engine_EventOpenMessage(1, 0);
		v5 = 0;
		if (Engine_EventChooseYesNo(0, 0) != 0)
		{
			Engine_EventWait(20);
			Engine_ActorShowEmote(2, 0x102, 0);
			Engine_EventWait(60);
			Engine_ActorRunRepeatedMotion(2, 2);
			SceneActor_SetPairZeroAndValue(0, 2, 20);
			Engine_EventOpenMessage(2, 0);
			v5 = 0;
			if (Engine_EventChooseYesNo(2, 0) != 0)
			{
				Engine_EventWait(20);
				Engine_ActorShowEmote(2, 0x105, 0);
				Engine_EventWait(60);
				SceneActor_SetModeZeroAndValue(2, 20);
				FieldScene_RunSplitTripleSteps(1, 2, 10);
				Engine_ActorRunRepeatedMotion(1, 1);
				Engine_EventWait(10);
				SceneActor_SetModeZeroAndValue(1, 10);
				FieldScene_RunSplitTripleSteps(2, 1, 20);
				Engine_ActorShowEmote(2, 0x101, 0);
				Engine_EventWait(60);
				SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
				SceneActor_SetModeZeroAndValue(1, 10);
				SceneActor_SetPairZeroAndValue(1, 0, 20);
				SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
				v5 = 1;
				SceneActor_SetModeZeroAndValue(1, 20);
			}
		}
	}
	if (v5 == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuLeave);
		SceneActor_SetModeZeroAndValue(1, 20);
		SceneActor_SetPairZeroAndValue(1, 0, 20);
		SceneActor_SetModeZeroAndValue(1, 20);
	}
	Engine_ActorShowEmote(0, 0x105, 0);
	Engine_EventWait(60);
	Engine_ActorRunRepeatedMotion(1, 1);
	SceneActor_SetModeZeroAndValue(1, 10);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 10);
	Engine_ActorFaceEachOther(1, 2, 0);
	SceneActor_SetPairZeroAndValue(0, 2, 10);
	Engine_EventShowMessage(1, 0);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 10);
	Actor_WalkTo(2, 248, 0x2d8);
	Actor_WalkToAndWait(1, 248, 0x2d8);
	Engine_ActorSetPosition(1, 0, 0);
	Engine_ActorSetPosition(2, 0, 0);
}

static void KuupuappuHeya_PoseDialogueActors(void)
{
	switch (gEventWork->touched_trigger)
	{
	case 11:
		SceneActor_UpdateAnimationOnStateMatch(24, 1, 2, (s32)KuupuappuHeya_PairScriptC);
		SceneActor_UpdateAnimationOnStateMatch(25, 3, 4, (s32)KuupuappuHeya_PairScriptR);
		break;
	case 12:
		SceneActor_UpdateAnimationOnStateMatch(24, 1, 4, (s32)KuupuappuHeya_PairScriptG);
		SceneActor_UpdateAnimationOnStateMatch(24, 2, 3, (s32)KuupuappuHeya_PairScriptD);
		SceneActor_UpdateAnimationOnStateMatch(25, 1, 3, (s32)KuupuappuHeya_PairScriptO);
		break;
	case 13:
		SceneActor_UpdateAnimationOnStateMatch(24, 2, 1, (s32)KuupuappuHeya_PairScriptA);
		SceneActor_UpdateAnimationOnStateMatch(24, 3, 6, (s32)KuupuappuHeya_PairScriptJ);
		SceneActor_UpdateAnimationOnStateMatch(25, 2, 4, (s32)KuupuappuHeya_PairScriptQ);
		break;
	case 14:
		SceneActor_UpdateAnimationOnStateMatch(24, 3, 2, (s32)KuupuappuHeya_PairScriptC);
		SceneActor_UpdateAnimationOnStateMatch(25, 4, 3, (s32)KuupuappuHeya_PairScriptP);
		break;
	case 15:
		SceneActor_UpdateAnimationOnStateMatch(24, 4, 5, (s32)KuupuappuHeya_PairScriptH);
		SceneActor_UpdateAnimationOnStateMatch(25, 1, 2, (s32)KuupuappuHeya_PairScriptM);
		break;
	case 16:
		SceneActor_UpdateAnimationOnStateMatch(24, 4, 1, (s32)KuupuappuHeya_PairScriptB);
		SceneActor_UpdateAnimationOnStateMatch(24, 5, 6, (s32)KuupuappuHeya_PairScriptI);
		SceneActor_UpdateAnimationOnStateMatch(25, 3, 1, (s32)KuupuappuHeya_PairScriptL);
		break;
	case 17:
		SceneActor_UpdateAnimationOnStateMatch(24, 5, 4, (s32)KuupuappuHeya_PairScriptF);
		SceneActor_UpdateAnimationOnStateMatch(24, 6, 3, (s32)KuupuappuHeya_PairScriptE);
		SceneActor_UpdateAnimationOnStateMatch(25, 4, 2, (s32)KuupuappuHeya_PairScriptN);
		break;
	case 18:
		SceneActor_UpdateAnimationOnStateMatch(24, 6, 5, (s32)KuupuappuHeya_PairScriptH);
		SceneActor_UpdateAnimationOnStateMatch(25, 2, 1, (s32)KuupuappuHeya_PairScriptK);
		break;
	}
}

static void SceneActor_UpdateAnimationOnStateMatch(s32 actor, s32 expected, s32 next, const u8 *desc)
{
	struct FieldActor *rec = Object_GetById(actor);

	if (rec->timer == expected)
	{
		Engine_ActorEnableActionCallback(actor, desc);
		rec->timer = next;
	}
}

static void SceneState_SetFlagByActorPosition(void)
{
	struct FieldActor *p0 = Object_GetById(0);
	s32				   rx = p0->x.fixed;
	s32				   x;
	s32				   z;

	struct FieldActor *p1 = Object_GetById(0);
	x = rx >> 20;
	z = p1->z.fixed;
	x -= 34;
	z >>= 20;

	if ((u32)x <= 1 && z > 40 && z <= 42)
	{
		Engine_GameFlagSet(148 << 2);
	}
	else
	{
		Engine_GameFlagClear(148 << 2);
	}
}

s32 KuupuappuHeya_ApplyEntryState(void)
{
	gEventWork->start_transition = 0x209;
	switch (gGameState.entrance)
	{
	case 5:
	{
		s32 zero = 0;

		Object_GetById(8)->motion_flags = zero;
		Object_GetById(8)->y.fixed = zero;
		Object_GetById(8)->terrain_height = zero;
	}
	break;
	case 10:
		if (Engine_GameFlagIsSet(0x850) != 0)
		{
			Actor_SetPosition(2, 0x1780000, 0x1780000);
			Actor_FaceDirection(2, 0x4000, 0);
		}
		if (Engine_GameFlagIsSet(0x856) != 0)
		{
			Engine_ActorSetPosition(2, 0, 0);
		}
		if (Engine_GameFlagIsSet(0x855) != 0)
		{
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			Actor_SetPosition(16, 0x2180000, 0x1c30000);
#else
			Actor_SetPosition(16, 0x2180000, 0x1d00000);
#endif
			Engine_ActorEnableActionCallback(16, (const s32 *)1);
			Actor_FaceDirection(16, 0x5000, 0);
		}
		break;
	case 7:
	case 11:
		if (Engine_GameFlagIsSet(0x855) != 0)
		{
			Actor_SetPosition(18, 0x2380000, 0x2880000);
			Engine_ActorEnableActionCallback(18, (const s32 *)1);
			Actor_FaceDirection(18, 0x4000, 0);
			OverlayObject_ConfigureObject22WithResource17(231, 0x2380000, 0x100000, 0x2a00000);
			((void (*)())Engine_TaskAddCallback)((s32)SceneState_SetFlagByActorPosition, 0xc80);
		}
		else if (Engine_GameFlagIsSet(0x853) != 0)
		{
			Engine_ActorSetPosition(18, 0, 0);
		}
		break;
	case 12:
		if (Engine_GameFlagIsSet(0x109) != 0 && Engine_GameFlagIsSet(0x852) != 0
			&& Engine_GameFlagIsSet(0x853) == 0 && Engine_GameFlagIsSet(0x300) != 0)
		{
			Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
			((void (*)())Engine_TaskAddCallback)(
				(s32)SceneActor_FaceActors24And25TowardActorZero, 0xc80);
			break;
		}
		if (Engine_GameFlagIsSet(0x856) != 0)
		{
			Actor_SetPosition(25, 0x780000, 0x2b80000);
			Actor_FaceDirection(25, 0x8000, 0);
		}
		if (Engine_GameFlagIsSet(0x852) != 0)
		{
			Map_CopyCellAttributes(14, 45, 3, 1, 14, 44);
			if (Engine_GameFlagIsSet(0x853) == 0)
			{
				Engine_MapCopyCellAttributes(14, 50, 3, 1, 14, 44);
				break;
			}
			Object_GetById(24)->timer = 5;
			Object_GetById(25)->timer = 4;
			Engine_TaskAddCallback((s32)SceneActor_FaceActors24And25TowardActorZero, 0xc80);
		}
		break;
	case 13:
	case 14:
		Engine_EventWait(2);
		Map_CopyCellsTo(54, 2, 35, 20, 2, 10);
		Engine_MapCopyCellsTo(54, 2, 95, 20, 2, 10);
		Engine_MapCopyCellsTo(54, 2, 35, 80, 2, 10);
		Map_CopyCellsTo(54, 2, 46, 21, 4, 8);
		Engine_MapCopyCellsTo(54, 2, 46, 81, 4, 8);
		Object_GetById(26)->collision_flags |= 4;
		Engine_ActorSetSpriteFlags(Object_GetById(26), 0);
		if (Engine_GameFlagIsSet(0x859) != 0)
		{
			Actor_SetPosition(26, 0x2a40000, 0x19b0000);
			Map_CopyCellAttributes(101, 24, 3, 4, 41, 24);
		}
		break;
	case 15:
		Engine_EventWait(2);
		Map_CopyCellsTo(54, 2, 44, 21, 2, 8);
		Engine_MapCopyCellsTo(54, 2, 44, 81, 2, 8);
		Engine_EventBegin();
		if (Engine_GameFlagIsSet(0x855) != 0)
		{
			Engine_ActorSetAnimation(15, 2);
			Engine_ActorSetAnimation(16, 2);
			Engine_EventWait(1);
			Engine_ActorSetAnimation(17, 2);
			break;
		}
		Engine_ActorSetPosition(8, 0x3380000, 0x1c80000);
		if (Engine_GameFlagIsSet(0x854) != 0)
		{
			KuupuappuHeya_RunVaultEvent();
			Engine_EventEnd();
			break;
		}
		Engine_ActorSetAnimation(8, 7);
		KuupuappuHeya_OpenWindowTransition();
		Engine_AudioPlayCue(17);
		Engine_EventSetMessage((s32)MsgKuupuappuUhnnUhnn);
		SceneActor_SetModeZeroAndValue(8, 10);
		Engine_EventEnd();
		break;
	case 16:
		Engine_MapCopyCells(54, 2, 2, 8, 44, 21);
		Engine_MapCopyCells(54, 2, 2, 8, 44, 81);
		Engine_ActorSetPosition(8, 0x3380000, 0x1c80000);
		FieldScene_RunLateSequence();
		Engine_EventRequestExit(16);
		break;
	case 17:
		Engine_MapCopyCells(54, 2, 2, 8, 44, 21);
		Engine_MapCopyCells(54, 2, 2, 8, 44, 81);
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			Actor_SetPosition(8, 0x3380000, 0x1c80000);
			RunEventScript01();
			RunDialoguePromptScene();
			break;
		}
		Engine_ActorSetAnimation(15, 2);
		Engine_ActorSetAnimation(16, 2);
		Engine_EventWait(1);
		Engine_ActorSetAnimation(17, 2);
		break;
	}
	return 0;
}

static void SceneActor_FaceActors24And25TowardActorZero(void)
{
	struct FieldActor *origin = Object_GetById(0);
	struct FieldActor *first = Object_GetById(24);
	struct FieldActor *second = Object_GetById(25);

	first->facing = ArcTan2(origin->z.fixed - first->z.fixed, origin->x.fixed - first->x.fixed);
	second->facing = ArcTan2(origin->z.fixed - second->z.fixed, origin->x.fixed - second->x.fixed);
}

static void FieldScene_RunLateSequence(void)
{
	struct FieldActor *record;
	s32				   v5;

	Engine_EventBegin();
	Actor_SetPosition(10, 0x3180000, 0x1a00000);
	Actor_SetPosition(11, 0x3200000, 0x1900000);
	Actor_SetPosition(12, 0x3080000, 0x1a00000);
	Actor_FaceDirection(10, 0x3000, 0);
	Actor_FaceDirection(11, 0x3000, 0);
	Actor_FaceDirection(12, 0x3000, 0);
	Actor_SetSpeed(11, 0xcccc, 0x6666);
	Actor_SetSpeed(12, 0xcccc, 0x6666);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3100000, 0x1c00000);
	Actor_SetPosition(GERALD, 0x3280000, 0x1b00000);
	Actor_SetPosition(IWAN, 0x3080000, 0x1b00000);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 19);
	Engine_ActorSetAnimation(GERALD, 19);
	Engine_ActorSetAnimation(IWAN, 19);
	v5 = 2;
	Object_GetById(0)->priority_flags = v5;
	Object_GetById(1)->priority_flags = v5;
	Object_GetById(2)->priority_flags = v5;
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(2);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(1);
	Engine_ActorSetSpriteFlags(record, 0);
	Actor_FaceDirection(8, 0xb000, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
	Engine_CameraWaitForMove();
	Engine_MapRedraw();
	KuupuappuHeya_OpenWindowTransition();
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(10, 3, 20);
	Engine_EventSetMessage((s32)MsgKuupuappuTheseKidsNothingWorryAbout);
	SceneActor_SetModeZeroAndValue(10, 30);
	SceneActor_SetModeZeroAndValue(8, 30);
	Actor_WalkTo(11, 0x328, 0x1c8);
	Actor_WalkTo(12, 0x318, 0x1c8);
	Engine_ActorWaitForMove(12);
	Actor_FaceDirection(12, 0, 0);
	Engine_ActorWaitForMove(11);
	Actor_FaceDirection(11, 0, 0);
	Engine_EventWait(30);
	SceneEffect_ApplyThreeValuesAndFinish(11, 3, 20);
	SceneActor_SetModeZeroAndValue(11, 20);
	FieldScene_RunSplitTripleSteps(12, 0, 30);
	SceneActor_SetModeZeroAndValue(12, 60);
	Engine_EventEnd();
}

static void KuupuappuHeya_RunVaultEvent(void)
{

	Actor_SetPosition(10, 0x3180000, 0x1a00000);
	Actor_SetPosition(11, 0x3200000, 0x1900000);
	Actor_SetPosition(12, 0x3080000, 0x1a00000);
	Actor_FaceDirection(10, 0x3000, 0);
	Actor_FaceDirection(11, 0x3000, 0);
	Actor_FaceDirection(12, 0x3000, 0);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
	Actor_SetPosition(1, 0x3280000, 0x1b00000);
	Actor_SetPosition(2, 0x3180000, 0x1c80000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(1, 0xb000, 0);
	Actor_FaceDirection(2, 0xb000, 0);
	Actor_FaceActor(8, 10, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	Engine_CameraFollowActor(0, 0);
	Engine_CameraWaitForMove();
	Engine_MapRedraw();
	Engine_TaskWait(1);
	gEventWork->transition_frames = 32;
	KuupuappuHeya_OpenWindowTransition();
	Engine_EventWait(60);
	Engine_EventSetMessage((s32)MsgKuupuappuWontGoAway);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(11, 30);
	Engine_ActorRunRepeatedMotion(12, 1);
	Engine_EventWait(20);
	Event_ShowMessage(12, 0);
	SceneActor_SetPairZeroAndValue(10, 11, 30);
	Engine_ActorSetAnimation(10, 3);
	SceneEffect_ApplyThreeValuesAndFinish(11, 3, 30);
	SceneActor_SetPairZeroAndValue(10, 12, 30);
	Engine_ActorSetAnimation(10, 3);
	SceneEffect_ApplyThreeValuesAndFinish(12, 3, 40);
	Actor_FaceDirection(10, 0x3000, 0);
	Actor_FaceDirection(11, 0x3000, 0);
	Actor_FaceDirection(12, 0x3000, 0);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(10, 4, 20);
	Event_ShowMessage(10, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	Party_SetFields1ceAnd1d0((s32)&SceneId_KuupuappuHeya, 17);
	Event_SetDefeatDestination((s32)&SceneId_KuupuappuHeya, 16);
	Battle_SetStart(3);
	BattleFx_SetWeightedResult(12, 5);
}

static s32 KuupuappuHeya_GetIwanVenusDjinnCount(void)
{
	return UnitGet(2)->djinn_owned_counts[0];
}

static s32 OverlayObject_RunObjectTwoWhenFlagged(void)
{
	u8 *state;

	BattlePlacement_UpdateTimedEntriesTwentyTimes();
	state = UnitGet(2);
	if (*(s32 *)(state + 0xf8) & 1)
	{
		Djinn_Transfer(2, 0, 0, 0);
		Engine_AudioPlayCue(126);
		UnitCalc(0);
		UnitCalc(2);
	}
}

static void RunEventScript01(void)
{
	struct FieldActor *record;
	struct EventWork  *work;
	s32				   base5_200d17c;
	u8				  *actor12_record;

	record = Object_GetById(12);
	actor12_record = (u8 *)record->sprite;
	Engine_EventBegin();
	Actor_SetPosition(10, 0x3180000, 0x1a00000);
	Actor_SetPosition(11, 0x3200000, 0x1900000);
	Actor_SetPosition(12, 0x3080000, 0x1880000);
	record = Object_GetById(10);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(11);
	Engine_ActorSetSpriteFlags(record, 0);
	record = Object_GetById(12);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_ActorSetAnimation(10, 9);
	Engine_ActorSetAnimation(11, 9);
	Engine_ActorSetAnimation(12, 9);
	Object_GetById(12)->priority_flags &= 254;
	actor12_record[9] |= 12;
	base5_200d17c = (s32)KuupuappuHeya_ActionTable;
	Engine_ActorEnableActionCallback(10, base5_200d17c);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
	Actor_SetPosition(GERALD, 0x3280000, 0x1b00000);
	Actor_SetPosition(IWAN, 0x3080000, 0x1b80000);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xc000, 0);
	Actor_FaceDirection(GERALD, 0xb000, 0);
	Actor_FaceDirection(IWAN, 0xb000, 0);
	Actor_SetSpeed(8, 0xcccc, 0x6666);
	Actor_FaceDirection(8, 0xb000, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
	Engine_CameraWaitForMove();
	Engine_MapRedraw();
	KuupuappuHeya_OpenWindowTransition();
	Engine_ActorEnableActionCallback(11, base5_200d17c);
	Engine_EventWait(30);
	Engine_ActorEnableActionCallback(12, base5_200d17c);
	Engine_EventWait(30);
	Engine_EventSetMessage((s32)MsgKuupuappuTheyTheyGotUs);
	SceneActor_SetModeZeroAndValue(10, 20);
	Actor_ShowEmote(8, 0x102, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(8, 20);
	Actor_WalkToAndWait(8, 0x328, 0x1c8);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_WalkToAndWait(GERALD, 0x318, 0x1b0);
	Actor_FaceDirection(GERALD, 0, 0);
	Actor_WalkToAndWait(8, 0x328, 0x198);
	Actor_FaceDirection(8, 0x8000, 0);
	Actor_WalkToAndWait(GERALD, 0x328, 0x1b0);
	Actor_FaceDirection(GERALD, 0xc000, 0);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(8, 3, 20);
	SceneActor_SetModeZeroAndValue(8, 20);
	Actor_WalkToAndWait(8, 0x300, 0x198);
	Engine_EventWait(20);
	Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
	Actor_FaceActor(GERALD, 8, 0);
	FieldScene_RunSplitTripleSteps(2, 8, 40);
	SceneActor_SetModeZeroAndValue(8, 30);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	Actor_WalkToAndWait(8, 0x2e8, 0x198);
	Engine_EventWait(50);
	Engine_ActorRunRepeatedMotion(11, 2);
	SceneActor_SetModeZeroAndValue(11, 20);
	Engine_ActorEnableActionCallback(11, base5_200d17c);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 11, 0);
	Actor_FaceActor(GERALD, 11, 0);
	FieldScene_RunSplitTripleSteps(2, 11, 20);
	SceneActor_SetModeZeroAndValue(1, 20);
	Engine_ActorRunRepeatedMotion(12, 2);
	SceneActor_SetModeZeroAndValue(12, 30);
	Engine_ActorEnableActionCallback(12, base5_200d17c);
	Actor_ShowEmote(GERALD, 0x103, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(1, 30);
	FieldScene_RunSplitTripleSteps(2, 0, 30);
	FieldScene_RunSplitTripleSteps(0, 2, 30);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	FieldScene_RunSplitTripleSteps(1, 2, 30);
	SceneEffect_ApplyThreeValuesAndFinish(0, 3, 30);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(1, 30);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Engine_EventWait(60);
	SceneActor_SetPairZeroAndValue(0, 1, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
	SceneEffect_ApplyPairWithValue141(2, 0);
	Ui_SetBank15PaletteAndClearRenderMode();
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 10);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Engine_EventWait(60);
	FieldScene_RunSplitTripleSteps(0, 1, 10);
	Event_OpenMessage(GERALD, 0);
	if (Engine_EventChooseYesNo(0, 0) == 0)
	{
		Engine_EventWait(20);
		Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
		FieldScene_RunSplitTripleSteps(1, 2, 20);
		SceneState_SetValue2ThenFinish();
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		Engine_ActorRunRepeatedMotion(GERALD, 1);
		Engine_EventWait(20);
		SceneActor_SetModeZeroAndValue(1, 20);
	}
	else
	{
		gEventWork->message++;
		Actor_ShowEmote(GERALD, 0x102, 0);
		Engine_EventWait(60);
		SceneActor_SetModeZeroAndValue(1, 20);
		Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
		FieldScene_RunSplitTripleSteps(1, 2, 20);
		SceneState_SetValue2ThenFinish();
		Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
		Engine_ActorRunRepeatedMotion(GERALD, 1);
		Engine_EventWait(20);
	}
	Actor_ShowEmote(IWAN, 0x102, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
	Engine_EventSetMessage((s32)MsgKuupuappuSeeThatsHappened);
	SceneActor_SetModeZeroAndValue(2, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 40);
	Actor_WalkToAndWait(8, 0x328, 0x198);
	Actor_FaceDirection(8, 0x8000, 0);
	Engine_EventWait(30);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 8, 0);
	Actor_FaceActor(GERALD, 8, 0);
	FieldScene_RunSplitTripleSteps(2, 8, 20);
	Engine_ActorRunRepeatedMotion(8, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(8, 20);
	Actor_SetSpeed(9, 0xcccc, 0x6666);
	Actor_SetSpeed(13, 0xcccc, 0x6666);
	Actor_SetSpeed(14, 0xcccc, 0x6666);
	Actor_SetPosition(9, 0x2e80000, 0x1980000);
	Actor_WalkToAndWait(9, 0x300, 0x198);
	FieldScene_RunSplitTripleSteps(9, 10, 30);
	Actor_SetPosition(13, 0x2e80000, 0x1980000);
	Actor_WalkToAndWait(13, 0x300, 0x198);
	Actor_SetPosition(14, 0x2e80000, 0x1980000);
	Actor_WalkTo(14, 0x310, 0x190);
	Actor_WalkToAndWait(13, 0x308, 0x1a8);
	Engine_ActorWaitForMove(14);
	Actor_FaceActor(13, 10, 0);
	FieldScene_RunSplitTripleSteps(14, 10, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
	Actor_FaceActor(GERALD, 9, 0);
	FieldScene_RunSplitTripleSteps(2, 9, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	SceneActor_SetModeZeroAndValue(11, 20);
	SceneActor_SetModeZeroAndValue(12, 30);
	SceneActor_SetPairZeroAndValue(9, 13, 20);
	Engine_ActorRunRepeatedMotion(13, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(13, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 30);
	SceneActor_SetPairZeroAndValue(9, 14, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 30);
	FieldScene_RunSplitTripleSteps(9, 10, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorSetAnimation(13, 3);
	SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
	SceneActor_SetPairZeroAndValue(13, 14, 20);
	Engine_ActorSetAnimation(13, 3);
	SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
	Actor_WalkTo(14, 0x318, 0x188);
	Actor_WalkToAndWait(13, 0x310, 0x190);
	Actor_FaceActor(13, 12, 0);
	Engine_ActorWaitForMove(14);
	FieldScene_RunSplitTripleSteps(14, 11, 20);
	Engine_ActorRunRepeatedMotion(13, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(13, 20);
	SceneEffect_ApplyThreeValuesAndFinish(14, 4, 20);
	SceneActor_SetModeZeroAndValue(14, 30);
	SceneActor_SetPairZeroAndValue(13, 0, 20);
	SceneActor_SetModeZeroAndValue(13, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
	work = gEventWork;
	work->transition_frames = 30;
	work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(60);
	Engine_EventEnd();
}

static void RunDialoguePromptScene(void)
{
	struct FieldActor *obj;
	struct FieldActor *actor;
	s32				   aftermath;
	s32				   rod;
	struct EventWork  *event;

	event = gEventWork;
	GameFlag_Set(0x855);
	Engine_EventBegin();
	{
		struct FieldActor *actor = Object_GetById(12);

		actor->priority_flags = ACTOR_PRIORITY_AUTOMATIC | actor->priority_flags;
	}
	Actor_MoveToAndWait(15, 0x368, 0x1a9);
	Actor_MoveToAndWait(16, 0x368, 0x199);
	Actor_MoveToAndWait(17, 0x368, 0x179);
	Actor_SetPosition(11, 0x3080000, 0x1880000);
	Actor_SetPosition(10, 0x3180000, 0x1880000);
	Actor_SetPosition(12, 0x3280000, 0x1880000);
	Engine_ActorSetAnimation(10, 5);
	Engine_ActorSetAnimation(11, 5);
	Engine_ActorSetAnimation(12, 5);
	Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(10, ACTOR_PARTY_LEADER, 0);
	Actor_FaceActor(12, ACTOR_PARTY_LEADER, 0);
	actor = Object_GetById(10);
	Engine_ActorSetSpriteFlags(actor, 1);
	actor = Object_GetById(11);
	Engine_ActorSetSpriteFlags(actor, 1);
	actor = Object_GetById(12);
	Engine_ActorSetSpriteFlags(actor, 1);
	Actor_SetPosition(13, 0x3000000, 0x1980000);
	Actor_SetPosition(14, 0x3000000, 0x1a80000);
	Actor_MoveToAndWait(9, 0x310, 0x1a8);
	Actor_SetPosition(8, 0x3280000, 0x1980000);
	Actor_FaceActor(13, 9, 0);
	Actor_FaceActor(8, 9, 0);
	Actor_FaceActor(14, 10, 0);
	Actor_FaceActor(9, 10, 0);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0x3180000, 0x1b80000);
	Actor_SetPosition(GERALD, 0x3280000, 0x1b80000);
	Actor_SetPosition(IWAN, 0x3080000, 0x1b80000);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 10, 0);
	Actor_FaceActor(GERALD, 10, 0);
	Actor_FaceActor(IWAN, 10, 0);
	gEventWork->transition_frames = 30;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(10, 2);
	aftermath = (s32)MsgKuupuappuYouRobinRightWontForget;
	Engine_EventSetMessage(aftermath);
	SceneActor_SetModeZeroAndValue(10, 20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorSetAnimation(13, 3);
	SceneEffect_ApplyThreeValuesAndFinish(8, 3, 20);
	Engine_ActorStartRepeatedMotion(11, 2);
	Engine_ActorStartRepeatedMotion(12, 2);
	Engine_EventWait(60);
	Actor_SetSpeed(13, 0xcccc, 0x6666);
	Actor_WalkTo(13, 0x2ea, 0x198);
	Actor_FaceDirection(9, 0xb000, 0);
	Actor_FaceDirection(14, 0xb000, 0);
	Engine_ActorEnableActionCallback(11, KuupuappuHeya_VaultScriptB);
	Engine_EventWait(20);
	Engine_ActorEnableActionCallback(10, KuupuappuHeya_VaultScriptB);
	Engine_EventWait(15);
	Engine_ActorEnableActionCallback(12, KuupuappuHeya_VaultScriptB);
	Engine_EventWait(35);
	Engine_ActorEnableActionCallback(8, KuupuappuHeya_VaultScriptA);
	Engine_EventWait(20);
	Engine_ActorWaitForMove(13);
	Actor_SetPosition(13, 0, 0);
	Engine_EventWait(40);
	Actor_SetSpeed(9, 0xcccc, 0x6666);
	Actor_SetSpeed(14, 0xcccc, 0x6666);
	Actor_WalkToAndWait(9, 0x310, 0x198);
	Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
	Actor_WalkToAndWait(14, 0x300, 0x198);
	FieldScene_RunSplitTripleSteps(14, 0, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	SceneEffect_ApplyThreeValuesAndFinish(14, 3, 20);
	Actor_FaceDirection(14, 0x2000, 10);
	SceneActor_SetModeZeroAndValue(14, 20);
	SceneActor_SetPairZeroAndValue(0, 1, 50);
	SceneActor_SetPairZeroAndValue(0, 2, 50);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
	Actor_FaceActor(GERALD, 9, 0);
	FieldScene_RunSplitTripleSteps(2, 9, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimation(GERALD, 3);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 50);
	Engine_ActorFaceEachOther(9, 14, 0);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorEnableActionCallback(14, KuupuappuHeya_VaultScriptC);
	Engine_EventWait(50);
	Engine_ActorEnableActionCallback(9, KuupuappuHeya_VaultScriptD);
	Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	Actor_WalkToAndWait(GERALD, 0x318, 0x1c8);
	Actor_FaceDirection(GERALD, 0xd000, 0);
	Engine_EventWait(30);
	Actor_WalkToAndWait(IWAN, 0x318, 0x198);
	Actor_FaceDirection(IWAN, 0, 0);
	Actor_WalkToAndWait(GERALD, 0x328, 0x1c8);
	Actor_FaceDirection(GERALD, 0xd000, 0);
	Engine_EventWait(100);
	FieldScene_RunSplitTripleSteps(14, 9, 60);
	FieldScene_RunSplitTripleSteps(9, 14, 40);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 40);
	Actor_FaceDirection(9, 0, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(9, 2);
	Event_PlayCue(124);
	Engine_ActorSetAnimation(15, 4);
	Actor_SetPosition(18, 0x3680000, 0x1a80000);
	Engine_ActorSetSpritePriority(18, 1);
	Actor_SetSpeed(18, 0xcccc, 0x6666);
	Actor_SetDestinationOffset(18, 0, -8);
	Engine_ActorWaitForMove(18);
	Engine_ActorRunRepeatedMotion(18, 2);
	Engine_EventWait(60);
	Engine_MessageShowCentered((aftermath + 5), 1);
	Engine_ActorSetAnimation(15, 2);
	Actor_SetPosition(18, 0, 0);
	gEventWork->message++;
	Engine_ActorRunRepeatedMotion(14, 1);
	SceneActor_SetModeZeroAndValue(14, 20);
	SceneActor_SetPairZeroAndValue(0, 1, 40);
	FieldScene_RunSplitTripleSteps(9, 14, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	SceneActor_SetModeZeroAndValue(9, 30);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 14, 0);
	FieldScene_RunSplitTripleSteps(1, 14, 40);
	Actor_FaceDirection(14, 0, 0);
	Engine_EventWait(40);
	Engine_ActorRunRepeatedMotion(14, 2);
	Event_PlayCue(124);
	Engine_ActorSetAnimation(16, 4);
	Object_GetById(19)->motion_flags = 0;
	Engine_ActorSetSpritePriority(19, 1);
	Actor_SetPosition(19, 0x3680000, 0x1980000);
	Actor_SetSpeed(19, 0xcccc, 0x6666);
	Actor_SetDestinationOffset(19, 0, -8);
	Engine_ActorWaitForMove(19);
	Engine_ActorRunRepeatedMotion(19, 2);
	Engine_EventWait(60);
	Engine_MessageShowCentered((aftermath + 8), 1);
	Engine_ActorSetAnimation(16, 2);
	Actor_SetPosition(19, 0, 0);
	gEventWork->message++;
	Actor_ShowEmote(9, 0x102, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	Engine_ActorStartRepeatedMotion(GERALD, 2);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(14, 3, 50);
	FieldScene_RunSplitTripleSteps(9, 0, 20);
	SceneActor_SetModeZeroAndValue(9, 30);
	Actor_FaceDirection(14, 0xd000, 0);
	Engine_EventWait(30);
	Actor_ShowEmote(14, 0x100, 0);
	Engine_EventWait(60);
	Actor_WalkToAndWait(14, 0x358, 0x178);
	Engine_EventWait(20);
	FieldScene_RunSplitTripleSteps(14, 9, 20);
	SceneActor_SetModeZeroAndValue(14, 20);
	Actor_FaceActor(9, 14, 0);
	Actor_ShowEmote(IWAN, 0x100, 0);
	Engine_EventWait(60);
	FieldScene_RunSplitTripleSteps(2, 14, 30);
	FieldScene_RunSplitTripleSteps(9, 2, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	Event_ShowMessage(9, 0);
	Actor_FaceDirection(14, 0x5000, 0);
	Engine_EventWait(30);
	FieldScene_RunSplitTripleSteps(2, 9, 20);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 40);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
	Actor_FaceActor(GERALD, 9, 0);
	FieldScene_RunSplitTripleSteps(2, 9, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Actor_ShowEmote(IWAN, 0x102, 0);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(2, 40);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
	Engine_ActorSetAnimationAndWait(GERALD, 4);
	Engine_ActorRunRepeatedMotion(IWAN, 3);
	Engine_EventWait(30);
	SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	SceneActor_SetPairZeroAndValue(0, 1, 20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 30);
	Actor_SetSpeed(IWAN, 0x18000, 0xc000);
	Actor_WalkToAndWait(IWAN, 0x320, 0x198);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Event_ShowMessage(IWAN, 0);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
	FieldScene_RunSplitTripleSteps(1, 9, 30);
	SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GERALD, 0x102);
	Actor_SetAttachedEffect(IWAN, 0x102);
	Engine_EventWait(60);
	SceneActor_SetModeZeroAndValue(2, 20);
	Engine_ActorRunRepeatedMotion(9, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Actor_ShowEmote(IWAN, 0x101, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(9, 4, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
	Engine_ActorStartRepeatedMotion(GERALD, 1);
	Engine_ActorRunRepeatedMotion(IWAN, 1);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	SceneActor_SetModeZeroAndValue(9, 40);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(60);
	Actor_WalkToAndWait(9, 0x348, 0x1a8);
	SceneActor_SetPairZeroAndValue(9, 0, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
	Actor_FaceDirection(9, 0x5000, 0);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(1, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	SceneActor_SetModeZeroAndValue(9, 30);
	SceneActor_SetPairZeroAndValue(9, 14, 20);
	SceneActor_SetModeZeroAndValue(9, 20);
	Actor_WalkToAndWait(14, 0x358, 0x198);
	Engine_ActorEnableActionCallback(9, KuupuappuHeya_VaultScriptE);
	Engine_ActorEnableActionCallback(14, KuupuappuHeya_VaultScriptE);
	Actor_WalkTo(GERALD, 0x318, 0x1c8);
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	Actor_WalkTo(IWAN, 0x308, 0x1b0);
	Engine_ActorWaitForMove(GERALD);
	Actor_FaceDirection(GERALD, 0xd000, 0);
	Engine_ActorWaitForMove(IWAN);
	Actor_FaceDirection(IWAN, 0xd000, 0);
	Object_RefreshSelectorById(9);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
	obj = Object_GetById(14);
	{
		u8 *target = &obj->paused;
		u8	shown = 1;

		*target = shown;
	}
	obj->target_x = ACTOR_NO_TARGET;
	obj->target_y = ACTOR_NO_TARGET;
	obj->target_z = ACTOR_NO_TARGET;
	Actor_ShowEmote(9, 0x100, 0);
	Engine_ActorSetAnimation(14, 1);
	Engine_EventWait(50);
	Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
	Actor_WalkToAndWait(GERALD, 0x328, 0x1b8);
	Actor_FaceDirection(GERALD, 0xb000, 0);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(9, 20);
	SceneActor_SetPairZeroAndValue(0, 1, 50);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 9, 0);
	FieldScene_RunSplitTripleSteps(1, 9, 20);
	SceneEffect_ApplyThreeValuesAndFinish(9, 3, 20);
	Actor_WalkTo(9, 0x2e8, 0x198);
	Actor_WalkTo(14, 0x2e8, 0x198);
	Engine_ActorWaitForMove(9);
	Engine_ActorWaitForMove(14);
	Engine_EventWait(30);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x318, 0x198);
	SceneActor_SetPairZeroAndValue(0, 1, 30);
	Actor_ShowEmote(IWAN, 0x105, 0);
	Engine_EventWait(50);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(1, 40);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
	SceneActor_SetPairZeroAndValue(0, 1, 40);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 4);
	SceneEffect_ApplyThreeValuesAndFinish(1, 4, 30);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(1, 40);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(2, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
	Actor_ShowEmote(GERALD, 0x101, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
	Actor_ShowEmote(GERALD, 0x100, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
	Actor_ShowEmote(GERALD, 0x102, 0);
	Engine_EventWait(60);
	SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
	SceneActor_SetModeZeroAndValue(2, 20);
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 20);
	SceneActor_SetModeZeroAndValue(1, 30);
	Event_OpenMessage(IWAN, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		Engine_EventWait(20);
		Engine_ActorRunRepeatedMotion(GERALD, 1);
		Engine_EventWait(20);
		SceneActor_SetModeZeroAndValue(1, 20);
		SceneEffect_ApplyThreeValuesAndFinish(2, 4, 20);
		SceneActor_SetModeZeroAndValue(2, 20);
	}
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 30);
	Actor_WalkToAndWait(IWAN, 0x320, 0x1c8);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_WalkToAndWait(IWAN, 0x358, 0x1c8);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_WalkToAndWait(IWAN, 0x358, 0x178);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_FaceDirection(IWAN, 0, 0);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(IWAN, 2);
	obj = (struct FieldActor *)BattleFx_PlayCueAndStartEmitterOnTarget(2, 17, 65);
	Engine_EventWait(60);
	rod = (s32)MsgKuupuappuIvanGotShamansRod;
	Engine_MessageShowCentered(rod, 1);
	Engine_ObjectDispatchRelease(obj);
	Engine_ActorSetAnimation(17, 2);
	Engine_EventWait(20);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	Actor_WalkToAndWait(IWAN, 0x358, 0x1c8);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_WalkToAndWait(IWAN, 0x320, 0x1c8);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_WalkToAndWait(IWAN, 0x308, 0x1a8);
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IWAN, 0);
	FieldScene_RunSplitTripleSteps(1, 2, 30);
	SceneEffect_ApplyThreeValuesAndFinish(2, 3, 20);
	Engine_EventSetMessage((rod + 1));
	SceneActor_SetModeZeroAndValue(2, 20);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
	{
		u16 *slot = &event->message;
		s32	 saved = (s16)*slot;

		if (KuupuappuHeya_GetIwanVenusDjinnCount() != 0)
		{
			Engine_EventSetMessage((s32)MsgKuupuappuWaitDontWantTakeYour);
			Event_ShowMessage(IWAN, 0);
			OverlayObject_RunObjectTwoWhenFlagged();
		}
		Party_RemoveOwnerRestored(2);
		*slot = saved;
	}
	SceneEffect_ApplyThreeValuesAndFinish(1, 3, 50);
	Actor_WalkToAndWait(IWAN, 0x308, 0x198);
	Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
	Actor_FaceActor(GERALD, IWAN, 0);
	Actor_WalkToAndWait(IWAN, 0x2e8, 0x198);
	Engine_EventWait(40);
	SceneActor_SetPairZeroAndValue(0, 1, 20);
	Engine_ActorRunRepeatedMotion(GERALD, 1);
	Engine_EventWait(20);
	SceneActor_SetModeZeroAndValue(1, 20);
	SceneEffect_ApplyThreeValuesAndFinish(0, 3, 20);
	Engine_ActorSetAnimation(GERALD, 2);
	actor = Object_GetById(0);
	if (actor != NULL)
	{
		Actor_SetDestination(GERALD, actor->x.part.pixel, actor->z.part.pixel);
	}
	Engine_ActorWaitForMove(GERALD);
	Actor_SetPosition(GERALD, 0, 0);
	Engine_EventWait(30);
	Actor_SetPosition(8, 0, 0);
	Actor_SetPosition(9, 0, 0);
	Actor_SetPosition(13, 0, 0);
	Actor_SetPosition(14, 0, 0);
	Actor_SetPosition(10, 0, 0);
	Actor_SetPosition(11, 0, 0);
	Actor_SetPosition(12, 0, 0);
	Actor_SetPosition(IWAN, 0, 0);
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	Engine_EventEnd();
}

static void KuupuappuHeya_OpenWindowTransition(void)
{
	struct EventWork *state;

	state = gEventWork;
	state->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(1);
}

static void SceneActor_SetModeZeroAndValue(s32 a, s32 b)
{
	Engine_EventShowMessage(a, 0);
	Engine_EventWait(b);
}

static void FieldScene_RunSplitTripleSteps(s32 a, s32 b, s32 c)
{
	Engine_ActorFaceActor(a, b, 0);
	Engine_EventWait(c);
}

static void SceneActor_SetPairZeroAndValue(s32 a, s32 b, s32 c)
{
	Engine_ActorFaceEachOther(a, b, 0);
	Engine_EventWait(c);
}

static void SceneEffect_ApplyThreeValuesAndFinish(s32 a, s32 b, s32 c)
{
	((void (*)(s32, s32, s32))Engine_ActorSetAnimation)(a, b, c);
	ObjectMotion_WaitForAnimationChange(a);
	Engine_EventWait(c);
}

static void SceneEffect_ApplyPairWithValue141(s32 a, s32 b)
{
	Engine_PsynergyBegin(141, 1);
	Engine_PsynergySetTarget(a, b);
	Engine_PsynergyRaiseHands();
	Engine_PsynergyPlayEffect(1);
	Engine_TaskWait(1);
}

static void SceneState_SetValue2ThenFinish(void)
{
	Engine_PsynergyPlayEffect(2);
	Engine_PsynergyLowerHands();
	BattleEffect_CleanupSceneObjects();
}

static void OverlayObject_ConfigureObject22WithResource17(s32 a, s32 fixed_x, s32 fixed_y, s32 fixed_z)
{
	u8 *o;
	u8 *q;
	u8 *p;
	u8 *v;
	s32 z;
	s32 m;

	z = 0;
	o = (u8 *)Engine_ObjectCreate(22, fixed_x, fixed_y, fixed_z);
	if (o != 0)
	{
		q = (u8 *)((struct FieldActor *)o)->sprite;
		p = q + 38;
		*p = z;
		p += 1;
		*p = z;
		m = 33;
		m = -m;
		q[5] &= m;
		q[9] &= 15;
		o[0x55] = z;
		o[0x5c] = 1;
		v = Runtime_AllocateHeapBlock(17, 0x608);
		Engine_ItemLoadIcon(a);
		v += 0x400;
		Engine_VramLoad(q[28], 0x80, v);
		Engine_HeapRelease(17);
	}
}

static u8 *SceneData_FindEntryAtPosition(s32 *o)
{
	s32 x = (o[0] - 0x400000) >> 19;
	s32 y = (o[2] - 0x2700000) >> 19;
	u8 *e = KuupuappuHeya_Stops;
	u8 *ret = 0;
	u32 i;

	for (i = 0; i <= 36; i++, e += 16)
	{
		s32 a = e[0];

		if (a == x || a + 1 == x)
		{
			s32 b = e[1];

			if (b == y || b + 1 == y)
			{
				ret = e;
				break;
			}
		}
	}
	return ret;
}

static struct StopRecord *KuupuappuHeya_SnapToNearestStop(struct StopSet *set, s16 *pos)
{
	struct Stop *stop;
	s32			 id;
	s32			 best;
	s32			 at;
	s32			 d;
	u32			 i;
	u16			 p;

	id = -1;
	at = *pos;
	stop = set->stops;
	best = 0x8000;
	for (i = 0; i <= 2; i++, stop++)
	{
		p = stop->pos << 8;
		d = (s16)(p - *pos);
		if (d < 0)
			d = -d;
		/* FAKEMATCH: the used-stop test reads the id through a volatile lvalue, so the id is loaded again for the result. */
		if (*(volatile u8 *)&stop->id != 0xff && d < best)
		{
			best = d;
			id = stop->id;
			at = (s16)p;
		}
	}
	if (id == -1)
		return 0;
	*pos = at;
	return &((struct StopRecord *)KuupuappuHeya_Stops)[id];
}

static s32 KuupuappuHeya_IsRecordNearPoint(s32 x, s32 y, s32 id)
{
	struct FieldActor *rec = Engine_ActorLookup(id);
	s32				   target_x = rec->target_x;
	s32				   target_y;

	if (target_x == 0x80000000)
	{
		target_x = rec->x.fixed;
	}
	target_y = rec->target_z;
	if (target_y == 0x80000000)
	{
		target_y = rec->z.fixed;
	}

	target_x = (target_x - x) >> 16;
	target_y = (target_y - y) >> 16;
	if (target_x * target_x + target_y * target_y <= 64)
		return 1;
	return 0;
}

static s32 KuupuappuHeya_IsActorNearPoint(s32 x, s32 z, s32 id)
{
	struct FieldActor *actor = Engine_ActorLookup(id);
	s32				   ax;
	s32				   az;

	ax = actor->target_x;
	if (ax == ACTOR_NO_TARGET)
		ax = actor->x.fixed;
	az = actor->target_z;
	if (az == ACTOR_NO_TARGET)
		az = actor->z.fixed;
	ax = (ax - x) >> 16;
	az = (az - z) >> 16;
	if (ax * ax + az * az <= 0x100)
		return 1;
	return 0;
}

static s32 SceneActor_CheckTileFreeOfKinds(u8 *p)
{
	s32 x;
	s32 y;

	if (p == 0)
	{
		return 1;
	}
	x = (p[0] << 19) + 0x480000;
	y = (p[1] << 19) + 0x2780000;
	if (KuupuappuHeya_IsActorNearPoint(x, y, 0) != 0
		|| KuupuappuHeya_IsRecordNearPoint(x, y, 2) != 0
		|| KuupuappuHeya_IsRecordNearPoint(x, y, 24) != 0
		|| KuupuappuHeya_IsRecordNearPoint(x, y, 25) != 0)
	{
		return -1;
	}
	return 0;
}

static void SceneActor_ApplyScaledBytePairPosition(s32 a, u8 *p)
{
	Engine_ObjectSetPosition(a, (p[0] << 19) + 0x480000, 0, (p[1] << 19) + 0x2780000);
}

static void KuupuappuHeya_UpdateActorStops(void)
{
	struct FieldActor *leader;
	struct FieldActor *actor;
	struct EventWork  *work;
	u8				  *entry;
	u8				  *dest;
	s32				   dx;
	s32				   dz;
	s32				   blocked;
	u16				   angle;
	s16				   facing;
	s16				   d;

	leader = Engine_ActorLookup(0);
	blocked = 0;
	work = gEventWork;
	actor = Engine_ActorLookup(2);
	entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
	if (entry != NULL && actor->target_x == ACTOR_NO_TARGET)
	{
		dx = actor->x.fixed - leader->x.fixed;
		dz = actor->z.fixed - leader->z.fixed;
		facing = leader->facing;
		angle = ArcTan2(dz, dx);
		dx >>= 16;
		dz >>= 16;
		if (work->blocked_steps > 0 && dx * dx + dz * dz <= 400
			&& (d = (u16)facing - angle) > -0x1000 && d < 0x1000)
		{
			/* FAKEMATCH: a dead store keeps this empty branch through the first jump optimization, as the native lays out its jumps. */
			d = 0;
#if EDITION_INTERNATIONAL
		}
		else if (dx * dx + dz * dz > 64)
		{
#else
		}
		else
		{
#endif
			facing = actor->facing;
		}
		dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
		if (SceneActor_CheckTileFreeOfKinds(dest) == 0)
		{
			SceneActor_ApplyScaledBytePairPosition((s32)actor, dest);
			Object_SetMode(actor, 2);
		}
		else
		{
			Object_SetMode(actor, 1);
		}
	}

	actor = Engine_ActorLookup(24);
	entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
	if (entry != NULL && actor->target_x == ACTOR_NO_TARGET)
	{
		u32 turn = (u32)Engine_RandomNext() * 2 >> 16;
		s32 f;

		turn *= 0x60000000;
		f = actor->facing;
		facing = (turn - 0x30000000 >> 16) + f;
		dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
		if (SceneActor_CheckTileFreeOfKinds(dest) != 0)
		{
			facing = actor->facing + 0x8000;
			dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
			if (SceneActor_CheckTileFreeOfKinds(dest) == 0)
			{
				Engine_ActorSetAttachedEffect(24, 2);
				goto move_first;
			}
			Object_SetMode(actor, 4);
			blocked = 1;
		}
		else
		{
		move_first:
			SceneActor_ApplyScaledBytePairPosition((s32)actor, dest);
			Object_SetMode(actor, 2);
		}
	}

	actor = Engine_ActorLookup(25);
	entry = SceneData_FindEntryAtPosition(&actor->x.fixed);
	if (entry != NULL && actor->target_x == ACTOR_NO_TARGET)
	{
		u32 turn = (u32)Engine_RandomNext() * 3 >> 16;
		s32 f;

		turn *= 0x30000000;
		f = actor->facing;
		facing = (turn - 0x30000000 >> 16) + f;
		dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
		if (SceneActor_CheckTileFreeOfKinds(dest) != 0)
		{
			facing = actor->facing + 0x8000;
			dest = (u8 *)KuupuappuHeya_SnapToNearestStop((struct StopSet *)entry, &facing);
			if (SceneActor_CheckTileFreeOfKinds(dest) == 0)
			{
				Engine_ActorSetAttachedEffect(25, 2);
				goto move_second;
			}
			Object_SetMode(actor, 4);
			blocked += 2;
		}
		else
		{
		move_second:
			SceneActor_ApplyScaledBytePairPosition((s32)actor, dest);
			Object_SetMode(actor, 2);
		}
	}
	if (blocked != 0)
	{
		if (++KuupuappuHeya_StopTimer > 29)
			work->raised_trigger = blocked + 200;
	}
	else
	{
		KuupuappuHeya_StopTimer = blocked;
	}
}

static void KuupuappuHeya_StartActorStops(void)
{
	Engine_EventBegin();
	Camera_MoveTo(0xa80000, -1, 0x2900000, 1);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Actor_SetSpeed(1, 0xcccc, 0x6666);
	Actor_SetSpeed(2, 0xcccc, 0x6666);
	Actor_WalkToAndWait(0, 248, 0x2b8);
	Actor_SetPosition(1, 0xf80000, 0x2b80000);
	Actor_SetPosition(2, 0xf80000, 0x2b80000);
	Actor_WalkTo(0, 200, 0x2b8);
	Actor_WalkTo(1, 248, 0x2c8);
	Actor_WalkToAndWait(2, 232, 0x2b8);
	Engine_ActorWaitForMove(1);
	Actor_FaceDirection(1, 0xc000, 0);
	Actor_FaceDirection(2, 0x8000, 0);
	Engine_ActorWaitForMove(0);
	Engine_ActorSetAnimation(1, 12);
	FieldScene_ConfigurePairedActors();
	Call4(Map_SetViewBounds, 0x300000, 0x2400000, 0x1200000, 0x2e00000);
	Actor_SetSpeed(1, 0x10000, 0x8000);
	Actor_SetSpeed(2, 0xc000, 0x6000);
	Actor_SetSpeed(24, 0x10000, 0x13333);
	Actor_SetSpeed(25, 0x18000, 0x18000);
	KuupuappuHeya_StopTimer = 0;
	FieldScheduler_AddOrUpdateCallback((s32)KuupuappuHeya_UpdateActorStops, 0xc94);
	GameFlag_Clear(0x1ff);
	Engine_EventEnd();
	Engine_AudioPlayCue(9);
}

u32 KuupuappuHeya_Stops[] = {
	4, 1, 0x801e, 0xffff, 6, 2, 0x8000, 0xffff,
	8, 3, 0x8001, 0x401f, 10, 4, 0x8002, 0xffff,
	12, 5, 0x8003, 0xffff, 14, 6, 0x8004, 0xffff,
	16, 7, 0x8005, 0x4022, 18, 8, 0x8006, 0xffff,
	20, 9, 0x8007, 0xffff, 22, 10, 0x8008, 0xffff,
	24, 0x400b, 0x8009, 0xffff, 536, 0x400c, 0xc00a, 0xffff,
	1048, 0x400d, 0xc00b, 0xffff, 1560, 0x400e, 0xc00c, 0xffff,
	2072, 0x800f, 0xc00d, 0xffff, 2070, 0x8010, 14, 0xffff,
	2068, 0x8011, 15, 0xffff, 2066, 0x8012, 16, 0xffff,
	2064, 0x8013, 17, 0xc024, 2062, 0x8014, 18, 0xffff,
	2060, 0x8015, 19, 0xffff, 2058, 0x8016, 20, 0xffff,
	2056, 0x8017, 21, 0xc021, 2054, 0x8018, 22, 0xffff,
	2052, 0x8019, 23, 0xffff, 2050, 0x801a, 24, 0xffff,
	2048, 0xc01b, 25, 0xffff, 1536, 0xc01c, 0x401a, 0xffff,
	1024, 0xc01d, 0x401b, 0xffff, 512, 30, 0x401c, 0xffff,
	258, 0, 0x801d, 0xffff, 520, 0x4020, 0xc002, 0xffff,
	1032, 0x4021, 0xc01f, 0xffff, 1544, 0x4016, 0xc020, 0xffff,
	528, 0x4023, 0xc006, 0xffff, 1040, 0x4024, 0xc022, 0xffff,
	1552, 0x4012, 0xc023, 0xffff,
};

s32 KuupuappuHeya_ActionTable[] = {
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, -256),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_Y, -0x1000),
	SC_ADD(SW_SCALE_X, 256),
	SC_WAIT(2),
	SC_SET(SW_SCALE_Y, 0x10000),
	SC_SET(SW_SCALE_X, 0x10000),
	SC_WAIT(180),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 KuupuappuHeya_Villager11ExitScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(400)),
	SC_SYNC,
	SC_MOVE(PIXELS(800), 0, PIXELS(400)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(20),
	SC_END,
};

s32 KuupuappuHeya_Villager12ExitScript[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(416)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(20),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptA[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(744), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptB[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(6),
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(744), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptC[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(792), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(440)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(840), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(856), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptD[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(808), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(840), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(856), 0, PIXELS(424)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(10),
	SC_END,
};

s32 KuupuappuHeya_VaultScriptE[] = {
	SC_SET(SW_SPEED, 0xcccc),
	SC_ARG(2),
	SC_MOVE(PIXELS(840), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(456)),
	SC_SYNC,
	SC_MOVE(PIXELS(808), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(776), 0, PIXELS(408)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(104), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptB[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(80), 0, PIXELS(640)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptC[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptD[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(232), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptE[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(632)),
	SC_SYNC,
	SC_MOVE(PIXELS(232), 0, PIXELS(632)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptF[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(104), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptG[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(80), 0, PIXELS(640)),
	SC_SYNC,
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptH[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptI[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(232), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptJ[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(632)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(232), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptK[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(648)),
	SC_SYNC,
	SC_MOVE(PIXELS(72), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptL[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptM[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(648)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptN[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(648)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptO[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptP[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(120), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(72), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptQ[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

s32 KuupuappuHeya_PairScriptR[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(696)),
	SC_SYNC,
	SC_MOVE(PIXELS(264), 0, PIXELS(696)),
	SC_SYNC,
	SC_ARG(1),
	SC_END,
};

struct SceneEntrance KuupuappuHeya_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 192, 0, 220, FACING_NORTH, 0, 32, 0, 272, 240, 0 },
	{ 6, CONDITION_ALWAYS, 432, 0, 254, FACING_NORTH, 0, 288, 0, 528, 272, 0 },
	{ 7, CONDITION_ALWAYS, 720, 0, 782, FACING_NORTH, 0, 432, 576, 800, 800, 0 },
	{ 8, CONDITION_ALWAYS, 624, 0, 236, FACING_NORTH, 0, 560, 32, 848, 256, 0 },
	{ 9, CONDITION_ALWAYS, 144, 0, 492, FACING_NORTH, 0, 16, 288, 256, 512, 0 },
	{ 10, CONDITION_ALWAYS, 416, 0, 526, FACING_NORTH, 0, 320, 288, 592, 544, 0 },
	{ 11, CONDITION_ALWAYS, 744, 0, 640, FACING_SOUTH, 0, 432, 576, 800, 800, 0 },
	{ 12, CONDITION_ALWAYS, 358, 0, 632, FACING_SOUTH, 0, 32, 576, 400, 776, 0 },
	{ 13, CONDITION_ALWAYS, 648, 0, 424, FACING_SOUTH, 0, 560, 320, 800, 480, 0 },
	{ 14, CONDITION_ALWAYS, 704, 0, 408, FACING_WEST, 0, 560, 320, 800, 480, 0 },
	{ 15, CONDITION_ALWAYS, 768, 0, 408, FACING_EAST, 0, 712, 288, 952, 496, 0 },
	{ 16, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
	{ 17, CONDITION_ALWAYS, 792, 0, 432, FACING_NORTH, 0, 712, 288, 952, 496, 0 },
	{ SCENE_TABLE_END },
};

s16 KuupuappuHeya_Regions[] = {
	639, 33, 407, 657, 51, 425, -1, 13,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

u32 KuupuappuHeya_Messages[] = {
	SCENE_EXITS(21),
	SCENE_EXIT(5, 20, 5),
	SCENE_EXIT(6, 20, 6),
	SCENE_EXIT(7, 20, 7),
	SCENE_EXIT(8, 20, 8),
	SCENE_EXIT(9, 20, 9),
	SCENE_EXIT(10, 20, 10),
	SCENE_EXIT(11, 21, 11),
	SCENE_EXIT(12, 21, 12),
	SCENE_EXIT(13, 20, 13),
	SCENE_EXIT(14, 21, 14),
	SCENE_EXIT(15, 21, 15),
	SCENE_EXIT(16, 9, 12),
	SCENE_EXITS_END,
};

struct ScenePlacement KuupuappuHeya_SceneTableA[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(160), 0, PIXELS(136), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, 0x0, 3,
		PIXELS(152), 0, PIXELS(72), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_WANDER,
		PIXELS(368), 0, PIXELS(120), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_WANDER,
		PIXELS(744), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(752), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(416), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, 0x0, ACTOR_STAND,
		PIXELS(512), 0, PIXELS(416), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x75, 0x0, ACTOR_STAND,
		PIXELS(536), 0, PIXELS(451), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(416), 0, PIXELS(464), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_WANDER,
		PIXELS(536), 0, PIXELS(672), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(392), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(632), 0, PIXELS(136), FACING_SOUTH, 3, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(664), 0, PIXELS(632), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_STAND,
		PIXELS(696), 0, PIXELS(696), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x3f, 0x854, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
	{ 0x40, 0x854, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdf, 0x0, ACTOR_STAND,
		PIXELS(692), 0, PIXELS(411), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement KuupuappuHeya_SceneTableB[] = {
	{ GERALD, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(160), 0, PIXELS(136), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x75, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(504), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x3f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(696), FACING_EAST, 3, 0 },
	{ 0x40, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(680), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x40, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x14, 0x0, ACTOR_STAND,
		PIXELS(872), 0, PIXELS(376), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xfb, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xdd, CONDITION_ALWAYS, 7,
		PIXELS(8), 0, PIXELS(8), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKuupuappuHeyaEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
	{ EVENT_TOUCH, 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
	{ EVENT_TOUCH, 10, IF_FLAG_SET(0x856), EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 11, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 12, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 13, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 14, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 15, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 16, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 17, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 18, IF_FLAG_SET(0x300), EVENT_SCRIPT(KuupuappuHeya_PoseDialogueActors) },
	{ EVENT_TOUCH, 20, IF_FLAG_SET(0x300), EVENT_SCRIPT(EV20_TOUCH) },
	{ EVENT_TOUCH, 21, IF_FLAG_SET(0x852), EVENT_SCRIPT(EV21_TOUCH) },
	{ EVENT_RAISED, 201, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SelectActorPair) },
	{ EVENT_RAISED, 202, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SelectActorPair) },
	{ EVENT_RAISED, 203, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_SelectActorPair) },
	{ EVENT_RAISED, 250, CONDITION_ALWAYS, EVENT_SCRIPT(EV250_RAISED) },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_TOUCH) },
	{ 0x202, 40, CONDITION_ALWAYS, EVENT_SCRIPT(EV40_SCRIPT) },
	{ EVENT_TALK, 2, IF_FLAG_SET(0x250), 0 },
	{ EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TALK) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThisRockWasBlownHereAll) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMyNeighborCreepsMeOutHe) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActorElevenDialogue) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuEverSinceTheEruptionGrandpaKeeps) },
	{ EVENT_TALK, 14, 0x856, MSGNO(MsgKuupuappuICouldHelpIvanIfI) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYoureHelpingIvanYouDontLook) },
	{ EVENT_TALK, 15, 0x856, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuSinceYoureHelpingIvanIllTell) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunFlag856DialogueBranch) },
	{ EVENT_TALK, 17, 0x856, MSGNO(MsgKuupuappuMyHusbandTheMayorCantBelieve) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuReallyYoureGoingToHelpIvan) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowLine128E) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheGuestsUpstairsWerentInTheir) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ EVENT_TALK, 24, IF_FLAG_SET(0x250), 0 },
	{ EVENT_TALK, 25, IF_FLAG_SET(0x250), 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, IF_FLAG_SET(0x250), 0 },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, IF_FLAG_SET(0x250), 0 },
	{ EVENT_TALK, 24, 0x853, MSGNO(MsgKuupuappuThatKidIvanHeCreepsMe) },
	{ EVENT_TALK, 25, 0x853, MSGNO(MsgKuupuappuWeHaventDoneAnythingYoureA) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_StepActor24AnimationByFacing) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuHeya_RunActor25Response) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheOtherSparklingRocksAllCrumbled) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuHeyMaybeMyNeighborIsThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYouNeverBoughtMeAnythingWorth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheseGuysLookReliableIGuess) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWonderWhatSortOfThing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMaybeTheyllFindOurStolenFamily) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAllOurBestWeaponsWereStolen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWonderIfTheyLeftUs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKuupuappuHahhWereSureToSeeA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuItWouldBeAwfulIfOur) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThatVolcanoCostUsALot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x418, CONDITION_ALWAYS, EVENT_SCRIPT(SceneActor_StepActor24AnimationByFacing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x419, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuHeya_RunActor25Response) },
	{ PSYNERGY_ON_ACTOR(0x8c), 26, 0x859, EVENT_SCRIPT(EV26_PSYNERGY) },
	{ SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyBarrel) },
	{ SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
	{ 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledHistoryOf)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledGemsOf)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuHeyaEventsEntrances15To17[] = {
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TOUCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyChest) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyChest) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_ShowEmptyChest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAhhHelpMe) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKuupuappuHeyaEventsFlag855[] = {
	{ SEARCH(SEARCH_UNNAMED), 41, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActorEighteenConditionalScene) },
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 12 },
	{ EVENT_TOUCH, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EV26_TOUCH) },
	{ EVENT_TOUCH, 27, CONDITION_ALWAYS, EVENT_SCRIPT(EV27_TOUCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuICanSleepWellNowThat) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11Line) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuJustAsIThoughtNothingWas) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIvanLearnedALotByTalking) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene383SequenceC) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYoureThePersonWhoGotOur) },
	{ EVENT_TALK, 18, 0x250, EVENT_SCRIPT(KuupuappuHeya_OfferBone) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuImShockedThoseGuestsOfOurs) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuEverybodysAfterMyRockTheyWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuTheReasonLunpaTurnedBadIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWishIHadSomethingWorth) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhenIGrowUpIWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWishIHadWingsLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuImSoRelievedNowThatWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMyWeaponsWereFoundInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuItMustBeSnowingUpIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhyAreThesePeopleSoAnxious) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhoCouldHaveKnownTheyWere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgKuupuappuOfCourseIAlwaysSuspectedThey) },
	{ SEARCH(SEARCH_JAR), 100, 0xf4b, FIND_COINS(7) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf4c, FIND_COINS(4) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf4d, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_WOODEN_BOX), 103, 0xf4e, FIND_ITEM(0xc3) },
	{ 0xc4f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledHistoryOf)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKuupuappuTheresABookEntitledGemsOf)) },
	{ SCENE_EVENTS_END },
};

s32 *KuupuappuHeya_StepActions[] = {
	KuupuappuHeya_PairScriptC,
	KuupuappuHeya_PairScriptD,
	KuupuappuHeya_PairScriptJ,
	KuupuappuHeya_PairScriptH,
	KuupuappuHeya_PairScriptF,
	KuupuappuHeya_PairScriptB,
};

s32 *KuupuappuHeya_IdleActions[] = {
	KuupuappuHeya_PairScriptG,
	KuupuappuHeya_PairScriptA,
	KuupuappuHeya_PairScriptC,
	KuupuappuHeya_PairScriptE,
	KuupuappuHeya_PairScriptI,
	KuupuappuHeya_PairScriptH,
};

s32 *KuupuappuHeya_ResponseScripts[] = {
	KuupuappuHeya_PairScriptM,
	KuupuappuHeya_PairScriptQ,
	KuupuappuHeya_PairScriptP,
	KuupuappuHeya_PairScriptL,
	KuupuappuHeya_PairScriptO,
	KuupuappuHeya_PairScriptK,
	KuupuappuHeya_PairScriptN,
	KuupuappuHeya_PairScriptR,
};

u16 KuupuappuHeya_StopTimer;
