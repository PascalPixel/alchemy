#include "game_state.h"
#include "window.h"
#include "unit/scene/kuupuappu_mura.h"
#include "unit/scene/common/kuupuappu/range.h"
#include "unit/scene/common/effect/move.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/sparkle.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/battle/battle.h"
#include "unit/battle/unit.h"
#include "unit/battle/turnend.h"
#include "unit/game/chara.h"
#include "unit/game/item.h"
#include "unit/game/djinn.h"
#include "unit/runtime/math.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "tbs_edition.h"
#include "btlpty.h"
#include "text/msg_ids.h"
#include "call.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "inventory.h"
#include "b5_context.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void SceneActor_RunActorStep(s32 actor);
static void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay);
static void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/effect/create.c"
#include "common/effect/move.c"
#include "common/effect/spawn.c"

union DriftingObject
{
	struct FieldActor		   actor;
	struct OverlayEffectMotion motion;
};

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/kuupuappu/nearby.c"

static s32 ActorPresentation_UpdateEntityFromLeader(struct FieldActor *entity)
{
	u8				  *base = gWindowWork[0];
	u8				  *workspace = gWindowWork[12];
	s32				   flag = 0;
	s32				   selector = 18;
	struct FieldActor *leader;

	if (entity->target_x == 0x80000000)
		return 0;

	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (*(s16 *)(workspace + 376) != 0 || base[RENDER_MODE_OFS] != 0)
	{
		selector = 26;
		flag = 1;
	}
	SceneActor_CheckFacingAndRange(entity, leader, selector, flag);
	return 0;
}

void *SceneData_GetScriptTable(void)
{
	return KuupuappuMura_Scripts;
}

int SceneData_ReturnZero(void)
{
	return 0;
}

void *SceneData_GetMessageTable(void)
{
	return KuupuappuMura_Messages;
}

void *SceneData_SelectActorTableByFlag855(void)
{
	if (Engine_GameFlagIsSet(0x855) != 0)
		return KuupuappuMura_ActorsFlag855;
	return KuupuappuMura_Actors;
}

static int KuupuappuMura_GetIwanVenusDjinnCount(void)
{
	return UnitGet(2)->djinn_owned_counts[0];
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/kuupuappu/djinn.c"

static void KuupuappuMura_SayFarewellToIvan(void)
{
	struct FieldActor *record;
	struct EventWork  *p5;

	p5 = gEventWork;
	if (GameFlag_IsSet(0x855) != 0 || GameFlag_IsSet(0x856) == 0)
	{
		Engine_EventRequestExit(p5->touched_trigger - 19);
		return;
	}
	Engine_EventBegin();
	record = Actor_Get(ACTOR_PARTY_LEADER);
	if (record != 0)
	{
		Actor_SetPosition(IWAN, record->x.fixed, record->z.fixed);
	}
	Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
	if (p5->touched_trigger == 20)
	{
		Actor_WalkToAndWait(IWAN, 0x190, 0x1c0);
	}
	else
	{
		Camera_SetSpeed(0xcccc, 0x1999);
		Camera_MoveTo(PIXELS(0xE0), -1, PIXELS(0xA2), 1);
		Actor_WalkToAndWait(IWAN, 224, 162);
		Engine_CameraWaitForMove();
	}
	Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, IWAN, 0);
	Engine_EventWait(20);
	Engine_EventSetMessage((s32)MsgKuupuappuLeavingImStillWorriedAbout);
	Event_ShowMessageAndWait(0x9002, 0, 20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	if (KuupuappuMura_GetIwanVenusDjinnCount() != 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWaitDontWantTakeYour);
		Event_ShowMessage(IWAN, 0);
		OverlayObject_RunObject2WhenFlagged();
		Engine_TaskWait(20);
	}
	Party_RemoveOwnerRestored(2);
	Engine_EventRequestExit(p5->touched_trigger - 19);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

static void EV90_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
	Engine_MessageShowCentered((s32)MsgKuupuappuCanHearWaterRumblingDown, 1);
	Engine_EventEnd();
}

void *KuupuappuMura_SelectEvents(void)
{
	if (Engine_GameFlagIsSet(0x855) != 0)
		return KuupuappuMura_ExtrasFlag855;
	return KuupuappuMura_Extras;
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuWasntEruptionMtAlephIncredible);
	SceneActor_ApplyActorCueThenWait(9, 0, 2);
	Event_OpenMessage(9, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	Event_ShowMessage(9, 0);
	Actor_FaceDirection(9, 0x5000, 0);
	Engine_EventEnd();
}

static void ActorPresentation_RunActorThirteenSceneSetup(void)
{
	struct EventWork *workspace;

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuThoseTravelersLeftInBig);
	Engine_ActorSetAnimation(13, 1);
	SceneActor_ApplyActorCueThenWait(13, 0, 2);
	Engine_EventOpenMessage(13, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	Engine_EventShowMessage(13, 0);
	Engine_EventEnd();
}

static void EV17_TALK(void)
{
	struct EventWork *workspace;

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuAccusingUsStealingHammetsTreasured);
	SceneActor_ApplyActorCueThenWait(17, 0, 2);
	Engine_EventOpenMessage(17, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	Engine_EventShowMessage(17, 0);
	Engine_EventEnd();
}

static void ActorPresentation_RunActorEighteenSceneSetup(void)
{
	struct EventWork *workspace;

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuOffOnAdventure);
	SceneActor_ApplyActorCueThenWait(18, 0, 2);
	Engine_EventOpenMessage(18, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	Engine_EventShowMessage(18, 0);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor11Line(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuGuessFolksGot);
	SceneActor_RunActorStep(11);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor16Line(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuNobodysStealingAnything);
	SceneActor_RunActorStep(16);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor19Line(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff);
	Engine_ActorSetAnimation(19, 0);
	SceneActor_ApplyActorCueThenWait(19, 0, 2);
	Engine_EventShowMessage(19, 0);
	Engine_EventEnd();
}

static void ActorPresentation_RunActorFourteenDialogue(void)
{
	struct FieldActor *actor = Object_GetById(14);
	s16				   saved = actor->facing;

	actor->timer |= 2;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuPoorGuyLeft);
	Engine_ActorSetAnimation(14, 0);
	SceneActor_ApplyActorCueThenWait(14, 0, 2);
	SceneActor_ApplyActorZeroThenWait(14, 10);
	actor->facing = saved;
	Engine_TaskWait(1);
	Engine_EventEnd();
	actor->timer &= 1;
}

static void ActorPresentation_RunActorFifteenDialogue(void)
{
	struct FieldActor *actor = Object_GetById(15);
	s16				   saved = actor->facing;

	actor->timer |= 2;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuMasterHammetsCaravan);
	Engine_ActorSetAnimation(15, 0);
	SceneActor_ApplyActorCueThenWait(15, 0, 2);
	SceneActor_ApplyActorZeroThenWait(15, 10);
	actor->facing = saved;
	Engine_TaskWait(1);
	Engine_EventEnd();
	actor->timer &= 1;
}

static void EV21_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuFixingRoofCant);
	SceneActor_ApplyActorCueThenWait(21, 0, 2);
	Engine_ActorShowEmote(21, 0x103, 0);
	Engine_EventWait(30);
	Engine_EventOpenMessage(21, 0);
	Engine_EventEnd();
}

static void SceneActor_RunActorStep(s32 actor)
{
	Engine_EventBegin();
	Engine_ActorSetAnimation(actor, 1);
	SceneActor_ApplyActorCueThenWait(actor, 0, 2);
	Engine_EventShowMessage(actor, 0);
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/kuupuappu/talk.c"

static void EV08_TALK(void)
{
	struct EventWork *workspace;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuYoureHittingRoadAgain);
	SceneActor_ApplyActorCueThenWait(8, 0, 2);
	Engine_EventOpenMessage(8, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	Engine_EventShowMessage(8, 0);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor11SecondLine(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuGreatCaughtThieves);
	SceneActor_RunActorStep(11);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{
	struct EventWork *workspace;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuTalkingAboutHammetsServantIvan);
	if (Engine_GameFlagIsSet(2) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	SceneActor_RunActorStep(12);
	Engine_EventEnd();
}

static void SceneDialogue_RunActor13Line(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuEruptionDevastatedRoads);
	SceneActor_RunActorStep(13);
	Engine_EventEnd();
}

static void ActorPresentation_RunActorFourteenDialogueAndAdvanceStory(void)
{
	struct FieldActor *actor = Object_GetById(14);
	u16				  *flags = (u16 *)&actor->timer;
	s16				   saved = actor->facing;

	s32 tmp;

	*flags = (tmp = *flags | 2);
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuDidntGoLunpa);
	if (Engine_GameFlagIsSet(2) != 0)
		++gEventWork->message;
	Engine_ActorSetAnimation(14, 0);
	SceneActor_ApplyActorCueThenWait(14, 0, 2);
	SceneActor_ApplyActorZeroThenWait(14, 10);
	actor->facing = saved;
	Engine_TaskWait(1);
	Engine_EventEnd();
	*flags &= 1;
}

static void ActorPresentation_RunActorFifteenFollowupDialogue(void)
{
	struct FieldActor *actor = Object_GetById(15);
	s16				   saved = actor->facing;

	actor->timer |= 2;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuEveryoneGratefulCaptured);
	Engine_ActorSetAnimation(15, 0);
	SceneActor_ApplyActorCueThenWait(15, 0, 2);
	SceneActor_ApplyActorZeroThenWait(15, 10);
	actor->facing = saved;
	Engine_TaskWait(1);
	Engine_EventEnd();
	actor->timer &= 1;
}

static void ActorPresentation_RunActorSixteenSceneSetup(void)
{
	struct EventWork *workspace;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuThoseMenCapturedTheyreIn);
	Engine_ActorSetAnimation(16, 1);
	SceneActor_ApplyActorCueThenWait(16, 0, 2);
	Engine_EventOpenMessage(16, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	Engine_EventShowMessage(16, 0);
	Engine_EventEnd();
}

static void ActorPresentation_RunActorEighteenFollowupSceneSetup(void)
{
	struct EventWork *workspace;
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuOnesWhoCapturedThieves);
	SceneActor_ApplyActorCueThenWait(18, 0, 2);
	Engine_EventOpenMessage(18, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		workspace = gEventWork;
		++workspace->message;
	}
	Engine_EventShowMessage(18, 0);
	Engine_EventEnd();
}

static void ActorPresentation_RunActorNineteenDialogueAndSetSceneState(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuRuffRrruff2);
	Engine_ActorSetAnimation(19, 0);
	SceneActor_ApplyActorCueThenWait(19, 0, 2);
	Engine_EventShowMessage(19, 0);
	Engine_ActorSetAnimation(19, 1);
	if (PartyInventory_FindOwner(231) != -1 && Engine_GameFlagIsSet(0x858) == 0)
	{
		u16 *p = &gEventWork->unknown_172;
		u16	 value = 1;

		*p = value;
	}
	Engine_EventEnd();
}

static void EV20_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuThankForOtherDayLeaving);
	SceneActor_ApplyActorCueThenWait(20, 0, 2);
	Engine_ActorSetAnimationAndWait(20, 3);
	Engine_EventWait(20);
	Engine_EventShowMessage(20, 0);
	Engine_EventEnd();
}

static void EV11_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
		Engine_EventSetMessage((s32)MsgKuupuappuManShouldStealFromAnother);
	else
		Engine_EventSetMessage((s32)MsgKuupuappuHowPunishPrisoners);
	SceneActor_RunActorCommandWithFlag91(11);
	Engine_EventEnd();
}

static void EV13_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuGroupTravelersWasStrangeBunch);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWithRoadOutOnlyWay);
	}
	SceneActor_RunActorCommandWithFlag91(13);
	Engine_EventEnd();
}

static void EV14_PSYNERGY(void)
{
	Object_GetById(14)->timer |= 2;
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuBetWasThoseThreeCreeps);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuWhereDidIvanGoBy);
		if (Engine_GameFlagIsSet(2) != 0)
			++gEventWork->message;
	}
	SceneActor_RunActorCommandWithFlag91(14);
	Engine_EventEnd();
	Object_GetById(14)->timer &= 1;
}

static void EV15_PSYNERGY(void)
{
	Object_GetById(15)->timer |= 2;
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
		Engine_EventSetMessage((s32)MsgKuupuappuSupposeDoesntMatterHowRich);
	else
		Engine_EventSetMessage((s32)MsgKuupuappuMustStrongerThanLookHave);
	SceneActor_RunActorCommandWithFlag91(15);
	Engine_EventEnd();
	Object_GetById(15)->timer &= 1;
}

static void EV16_PSYNERGY(void)
{
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
		Engine_EventSetMessage((s32)MsgKuupuappuEveryoneKnowsThoseThreeAt);
	else
		Engine_EventSetMessage((s32)MsgKuupuappuFeelMuchBetter);
	SceneActor_RunActorCommandWithFlag91(16);
	Engine_EventEnd();
}

static void EV19_PSYNERGY(void)
{
	struct FieldActor *actor = Object_GetById(19);
	actor->paused = 1;
	Engine_EventBegin();
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuStealingInMidstVolcanicEruption);
		Engine_ActorSetAnimation(19, 0);
		Engine_EventWait(2);
	}
	else if (Engine_GameFlagIsSet(0x858) != 0)
	{
		Engine_EventSetMessage((s32)MsgKuupuappuCarefulSearchWillRevealPassage);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgKuupuappuIfBringMeBoneIll);
	}
	Engine_EventShowMessage(19, 0);
	Engine_EventEnd();
	actor->paused = 0;
}

static void EV21_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgKuupuappuSneakWastingTime);
	SceneActor_RunActorCommandWithFlag91(21);
	Engine_EventEnd();
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/kuupuappu/toggle.c"

static void SceneActor_PlaceAndSetSceneDelay(s32 x, s32 y, s32 continuation)
{
	Actor_SetSpeed(0, 0x8000, 0x4000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, x, y);
	gEventWork->transition_frames = 16;
	Engine_EventRequestExit(continuation);
}

static void EV05_TOUCH(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene5Cells, 56, 19);
	SceneActor_PlaceAndSetSceneDelay(408, 320, 5);
}

static void EV06_TOUCH(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene6Cells, 50, 18);
	SceneActor_PlaceAndSetSceneDelay(312, 304, 6);
}

static void EV07_TOUCH(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene7Cells, 44, 17);
	SceneActor_PlaceAndSetSceneDelay(216, 288, 7);
}

static void EV08_TOUCH(void)
{
	struct FieldActor		   *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)actor->sprite;

	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene8Cells, 54, 13);
	{
		s32 cell = 23;
		s32 row = 12;

		Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
	}
	actor->priority_flags &= ~1;
	sprite->priority = 3;
	SceneActor_PlaceAndSetSceneDelay(376, 224, 8);
}

static void EV09_TOUCH(void)
{
	struct FieldActor		   *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)actor->sprite;

	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene9Cells, 49, 10);
	{
		s32 cell = 18;
		s32 row = 10;

		Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
	}
	actor->priority_flags &= ~1;
	sprite->priority = 3;
	SceneActor_PlaceAndSetSceneDelay(296, 176, 9);
}

static void EV10_TOUCH(void)
{
	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene10Cells, 38, 6);
	SceneActor_PlaceAndSetSceneDelay(120, 144, 10);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "kuupuappu_mura_scene_cells.c"

static void EV12_TOUCH(void)
{
	struct FieldActor		   *actor = Object_GetById(ACTOR_PARTY_LEADER);
	struct FieldSpritePriority *sprite = (struct FieldSpritePriority *)actor->sprite;

	Engine_AudioPlayCue(158);
	Engine_MapAnimateCells(KuupuappuMura_Scene12Cells, 35, 9);
	{
		s32 cell = 4;
		s32 row = 10;

		Engine_MapCopyCellAttributes(33, 20, 1, 3, cell, row);
	}
	actor->priority_flags &= ~1;
	sprite->priority = 3;
	SceneActor_PlaceAndSetSceneDelay(72, 160, 12);
}

static void EV13_TOUCH(void)
{
	Engine_AudioPlayCue(123);
	SceneActor_PlaceAndSetSceneDelay(152, 264, 13);
}

static void ActorPresentation_MoveActorToPositionAndWait(int actor, int x, int z, int field40)
{
	struct FieldActor *record = Object_GetById(actor);
	int				   frames;
	Engine_ActorSetSpeed(actor, 0x30000, 0x18000);
	record->gravity = 0x8000;
	record->bounce = 0;
	record->velocity_y = field40;
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_ActorMoveToAndWait(actor, x, z);
	Engine_ActorSetPosition(actor, x << 16, z << 16);
	for (frames = 60; frames != 0; --frames)
	{
		Engine_TaskWait(1);
		if (*(s16 *)((u8 *)record + 42) == 0)
			break;
	}
	Engine_ActorSetSpriteFlags(record, 1);
	record->gravity = 0x10000;
}

static void EV30_SEARCH(void)
{
	Engine_EventBegin();
	Event_PlayCue(100);
	Engine_EventWait(40);
	if (GameFlag_IsSet(0x867) == 0)
	{
		Actor_SetAttachedEffect(23, 0x102);
		Engine_ActorJump(23, 4, 0);
		Engine_EventWait(12);
		Engine_ActorJump(23, 4, 0);
		Engine_EventWait(20);
		ActorPresentation_MoveActorToPositionAndWait(23, 0x188, 104, 0x70000);
		Engine_EventWait(20);
		Actor_WalkToAndWait(23, 0x198, 104);
		Actor_WalkToAndWait(23, 0x198, 120);
		GameFlag_Set(0x867);
	}
	Engine_EventEnd();
}

static void EV19_SCRIPT(void)
{
	PartyInventory_Discard(231);
	Engine_EventBegin();
	Engine_EventWait(10);
	Engine_ActorRunRepeatedMotion(19, 2);
	Actor_SetSpeed(19, 0xcccc, 0x6666);
	Actor_WalkToAndWait(19, 216, 0x198);
	Engine_EventWait(10);
	Actor_FaceDirection(19, 0x4000, 20);
	Engine_ActorJump(19, 6, 0);
	Engine_EventWait(30);
	Engine_ActorJump(19, 6, 0);
	Engine_EventWait(30);
	Engine_ActorJump(19, 6, 0);
	Engine_EventWait(30);
	Actor_WalkToAndWait(19, 216, 0x188);
	Engine_EventWait(10);
	Actor_FaceDirection(19, 0x4000, 20);
	GameFlag_Set(0x858);
	Engine_EventEnd();
}

s32 KuupuappuMura_RestoreEntryState(void)
{
	s32				   rec7;
	struct FieldActor *record;
	s32				   v7;
	s32				   zero;

	if (Engine_GameFlagIsSet(0x87a) != 0)
	{
		Engine_EventRequestExit(14);
	}
	if (Engine_GameFlagIsSet(0x200) != 0)
	{
		Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
	}
	OverlayObject_CreateConfiguredObject(0x800000, 0, 0x1a40000, 223);
	v7 = 1;
	zero = 0;
	record = Object_GetById(14);
	record->timer = v7;
	record->update = (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
	record = Object_GetById(15);
	record->timer = zero;
	record->update = (void (*)(union FieldObject *))SceneActor_UpdateProximityToLeader;
	if (Engine_GameFlagIsSet(0x858) != 0)
	{
		Actor_SetPosition(19, 0xd80000, 0x1880000);
	}
	rec7 = Engine_GameFlagIsSet(0x853);
	if (Engine_GameFlagIsSet(0x855) == 0)
	{
		if ((v7 & rec7) != 0)
		{
			record = Object_GetById(21);
			record->update =
				(void (*)(union FieldObject *))ActorPresentation_UpdateEntityFromLeader;
		}
	}
	if (gGameState.entrance <= 2)
	{
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			Engine_GameFlagClear(0x867);
			if (Engine_GameFlagIsSet(0x855) == 0)
			{
				if (Engine_GameFlagIsSet(0x856) != 0)
				{
					Engine_EventBegin();
					record = Object_GetById(0);
					if (record != 0)
					{
						Engine_ActorSetPosition(2, record->x.fixed, record->z.fixed);
					}
					if (gGameState.entrance == 1)
					{
						Actor_SetPosition(2, 0x1900000, 0x1c00000);
					}
					else
					{
						Actor_SetPosition(2, 0xe00000, 0xa20000);
					}
					Engine_ActorFaceEachOther(2, 0, 0);
					Engine_EventOpenScreen();
					Engine_EventWaitForScreen();
					Engine_EventWait(30);
					Engine_ActorRunRepeatedMotion(2, 2);
					Engine_EventSetMessage((s32)MsgKuupuappuWelcomeBackTwo);
					Engine_EventShowMessageAndWait(2, 0, 20);
					Engine_ActorSetAnimationAndWait(0, 3);
					Actor_SetSpeed(2, 0xcccc, 0x6666);
					Engine_ActorSetAnimation(2, 2);
					record = Object_GetById(0);
					if (record != 0)
					{
						Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
					}
					Engine_ActorWaitForMove(2);
					Engine_ActorSetPosition(2, 0, 0);
					Event_PrepareObjectAndApplyValue(2, 0);
					Engine_EventEnd();
				}
			}
		}
	}
	if (Engine_GameFlagIsSet(0x867) != 0)
	{
		Actor_SetPosition(23, 0x1980000, 0x780000);
	}
	if (gGameState.entrance == 11)
	{
		if (Engine_GameFlagIsSet(0x855) == 0)
		{
			if (Engine_GameFlagIsSet(0x856) != 0)
			{
				if (Engine_GameFlagIsSet(2) == 0)
				{
					Engine_EventBegin();
					Actor_SetPosition(2, 0x280000, 0x1360000);
					Engine_ActorFaceEachOther(2, 0, 0);
					Engine_EventOpenScreen();
					Engine_EventWaitForScreen();
					Engine_EventWait(30);
					Engine_ActorRunRepeatedMotion(2, 2);
					Engine_EventSetMessage((s32)MsgKuupuappuWelcomeBackTwo);
					Engine_EventShowMessageAndWait(2, 0, 20);
					Engine_ActorSetAnimationAndWait(0, 3);
					Actor_SetSpeed(2, 0xcccc, 0x6666);
					Engine_ActorSetAnimation(2, 2);
					record = Object_GetById(0);
					if (record != 0)
					{
						Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
					}
					Engine_ActorWaitForMove(2);
					Engine_ActorSetPosition(2, 0, 0);
					Event_PrepareObjectAndApplyValue(2, 0);
					Engine_EventEnd();
				}
			}
		}
		Engine_GameFlagClear(0x12f);
	}
	else
	{
		if (gGameState.entrance == 13)
		{
			if (Engine_GameFlagIsSet(0x855) != 0)
			{
				Engine_ActorSetPosition(20, 0, 0);
			}
		}
	}
	return 0;
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object/distance.c"
#include "common/kuupuappu/range.c"

static void SceneActor_ApplyActorZeroThenWait(s32 actor, s32 delay)
{
	Engine_EventShowMessage(actor, 0);
	Engine_EventWait(delay);
}

static void SceneActor_ApplyActorCueThenWait(s32 actor, s32 cue, s32 delay)
{
	Engine_ActorFaceEachOther(actor, cue, 0);
	Engine_EventWait(delay);
}

static void SceneEffect_UpdateMotionWithDamping(struct OverlayEffectMotion *effect)
{
	s32 horizontal_rate;
	s32 vertical_rate;

	effect->x += effect->horizontal_rate;
	effect->shadow_x = effect->x;

	if (effect->mode != 0)
	{
		effect->y += effect->vertical_rate;
		effect->shadow_y = effect->y;
	}
	else
	{
		effect->z += effect->vertical_rate;
		effect->shadow_z = effect->z;
		effect->y += 1024;
		effect->shadow_y = effect->y;
	}

	horizontal_rate = effect->horizontal_rate;
	effect->horizontal_rate = horizontal_rate - Math_Divide(horizontal_rate, 28);
	vertical_rate = effect->vertical_rate;
	effect->vertical_rate = vertical_rate - Math_Divide(vertical_rate, 28);
}

static void KuupuappuMura_SpawnDriftingEffect(s32 flags)
{
	union DriftingObject *leader;
	union DriftingObject *leaf;
	struct FieldSprite	 *sprite;
	s32					  x;
	s32					  z;
	u8					  zero;

	leader = (union DriftingObject *)Object_GetById(19);
	if (leader == NULL)
		return;
	x = Engine_RandomNext();
	z = Engine_RandomNext();
	x <<= 3;
	x = (u32)x >> 16;
	z <<= 3;
	x -= 4;
	z = (u32)z >> 16;
	z -= 4;
	x <<= 16;
	z <<= 16;
	{
		s32 base_x = leader->motion.x;
		s32 base_z = leader->motion.z;
		s32 base_y;

		x += base_x;
		base_y = leader->motion.y;
		z += base_z;
		leaf = (union DriftingObject *)Engine_ObjectCreate(0xac, x, base_y, z);
	}
	if (leaf == NULL)
		return;
	sprite = leaf->actor.sprite;
	switch (Engine_RandomNext() & 1)
	{
	case 1:
		Object_SetMode(&leaf->actor, 3);
		Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptA);
		break;
	default:
		Object_SetMode(&leaf->actor, 2);
		Engine_ObjectSetScript(&leaf->actor, KuupuappuMura_DriftScriptB);
		break;
	}
	{
		u8 *motion_flags = &leaf->actor.motion_flags;

		zero = 0;
		*motion_flags = zero;
	}
	if (flags & 2)
	{
		s32 cnt;
		s32 bias;

		cnt = (u32)Engine_RandomNext() % 10 + 5;
		/* FAKEMATCH: reuse the coordinate local for the direction mask. */
		x = 1;
		flags &= x;
		bias = flags ^ x;
		bias <<= 2;
		cnt += bias;
		leaf->motion.vertical_rate = (0x3332 * flags - 0x1999) * cnt;
		cnt = (u32)Engine_RandomNext() % 15 - 7;
		leaf->motion.horizontal_rate = 0x1999 * cnt;
		leaf->motion.mode = zero;
	}
	else
	{
		s32 cnt;

		cnt = (u32)Engine_RandomNext() % 10 + 8;
		leaf->motion.horizontal_rate = (0x3332 * flags - 0x1999) * cnt;
		cnt = (u32)Engine_RandomNext() % 14 + 1;
		leaf->motion.vertical_rate = 0x1999 * cnt;
		leaf->motion.mode = 1;
	}
	{
		void (*update)(union FieldObject *) =
			(void (*)(union FieldObject *))SceneEffect_UpdateMotionWithDamping;
		u8 *sprite_flags;

		/* FAKEMATCH: preserve the sprite-flags address scheduling boundary. */
		do
		{
			sprite_flags = &sprite->flags;
		} while (0);
		leaf->actor.update = update;
		*sprite_flags = 0;
	}
	sprite->priority = leader->actor.sprite->priority;
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

s32 KuupuappuMura_Actor2[] = {
	SC_SET(SW_SPEED, 0x6666),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(312)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(10),
	SC_ARG(4),
	SC_WAIT(240),
	SC_SET(SW_FACING, FACING_EAST),
	SC_ARG(2),
	SC_MOVE(PIXELS(200), 0, PIXELS(312)),
	SC_SYNC,
	SC_ARG(1),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(10),
	SC_ARG(4),
	SC_WAIT(240),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
	SC_SET(SW_SPEED, 0x6666),
	SC_SET(SW_ACCEL, 0x3333),
	SC_SET(SW_COLLISION, 129),
	SC_SET(SW_RADIUS, 0x280000),
	SC_LABEL(1),
	SC_ARG(2),
	SC_WANDER_HOME(0x80000, 0x100000, 0x180000),
	SC_SYNC,
	SC_ARG(0),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 KuupuappuMura_Actor0[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(136), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(392)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

s32 KuupuappuMura_Actor1[] = {
	SC_ARG(2),
	SC_SET(SW_SPEED, 0xcccc),
	SC_SET(SW_ACCEL, 0x3333),
	SC_LABEL(1),
	SC_MOVE(PIXELS(120), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(424)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(408)),
	SC_SYNC,
	SC_MOVE(PIXELS(152), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(136), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(120), 0, PIXELS(376)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(392)),
	SC_SYNC,
	SC_MOVE(PIXELS(104), 0, PIXELS(408)),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

struct SceneEntrance KuupuappuMura_Scripts[] = {
	{ 0, CONDITION_ALWAYS, 223, 0, 135, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 400, 0, 472, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 224, 0, 138, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 408, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 312, 0, 328, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 7, CONDITION_ALWAYS, 216, 0, 312, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 8, CONDITION_ALWAYS, 376, 0, 248, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 9, CONDITION_ALWAYS, 296, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 120, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 64, 0, 294, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 70, 0, 183, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 152, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 KuupuappuMura_Messages[] = {
	SCENE_EXITS(20),
	SCENE_EXIT(1, 2, 3),
	SCENE_EXIT(2, 2, 44),
	SCENE_EXIT(5, 21, 5),
	SCENE_EXIT(6, 21, 6),
	SCENE_EXIT(7, 21, 7),
	SCENE_EXIT(8, 21, 8),
	SCENE_EXIT(9, 21, 9),
	SCENE_EXIT(10, 21, 10),
	SCENE_EXIT(11, 9, 10),
	SCENE_EXIT(12, 22, 1),
	SCENE_EXIT(13, 21, 13),
	SCENE_EXIT(14, 23, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement KuupuappuMura_Actors[] = {
	{ IWAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, (s32)KuupuappuMura_Actor0,
		PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x66, CONDITION_ALWAYS, (s32)KuupuappuMura_Actor1,
		PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x40, 0x854, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(284), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(392), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, 0x853, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, IF_FLAG_SET(0x853), (s32)KuupuappuMura_Actor2,
		PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xf8, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x5a, 0x34, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement KuupuappuMura_ActorsFlag855[] = {
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(381), 0, PIXELS(342), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(396), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x65, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(264), 0, PIXELS(150), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(204), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(124), 0, PIXELS(196), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(244), 0, PIXELS(300), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6b, CONDITION_ALWAYS, (s32)KuupuappuMura_Actor0,
		PIXELS(132), 0, PIXELS(380), FACING_WEST, TALK_KEEP_FACING, 0 },
	{ 0x66, CONDITION_ALWAYS, (s32)KuupuappuMura_Actor1,
		PIXELS(132), 0, PIXELS(420), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(332), 0, PIXELS(412), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x16, 0x854, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(92), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x68, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(440), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(296), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x855, (s32)KuupuappuMura_Actor2,
		PIXELS(168), 0, PIXELS(312), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xf8, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_NORTH, TALK_FACE_PARTY, 0 },
	{ 0x5a, 0x34, ACTOR_STAND,
		PIXELS(344), 0, PIXELS(96), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent KuupuappuMura_Extras[] = {
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMura_SayFarewellToIvan) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMura_SayFarewellToIvan) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorDraw_SetupActorSceneCells) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWelcomeToVaultTheTownAt) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMasterHammetAWealthyLandownerAnd) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11Line) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMasterHammetsServantCantReturnTo) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorThirteenSceneSetup) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorFourteenDialogue) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorFifteenDialogue) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor16Line) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorEighteenSceneSetup) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor19Line) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuAFieryRockBurntAHole) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuISupposeTheseChildrenCantGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMtAlephsEruptionWasBeyondDescription) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuWhyDoesAWealthyManLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKuupuappuPoorIvanButTheRodMay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKuupuappuHmphItLooksLikeTheyreOnto) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKuupuappuIWasJustAboutToLeave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuFixTheRoofWhyPayFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_PSYNERGY) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
	{ SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent KuupuappuMura_ExtrasFlag855[] = {
	{ EVENT_TOUCH, 20, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMura_SayFarewellToIvan) },
	{ EVENT_TOUCH, 21, CONDITION_ALWAYS, EVENT_SCRIPT(KuupuappuMura_SayFarewellToIvan) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 5, CONDITION_ALWAYS, EVENT_SCRIPT(EV05_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 6, CONDITION_ALWAYS, EVENT_SCRIPT(EV06_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 7, CONDITION_ALWAYS, EVENT_SCRIPT(EV07_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 11, CONDITION_ALWAYS, EVENT_SCRIPT(ActorDraw_SetupActorSceneCells) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TOUCH) },
	{ EVENT_TOUCH, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TOUCH) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuSmokeIsStillRisingFromMt) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuMasterHammetTheWealthyMerchantI) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor11SecondLine) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor13Line) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorFourteenDialogueAndAdvanceStory) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorFifteenFollowupDialogue) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorSixteenSceneSetup) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorEighteenFollowupSceneSetup) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(ActorPresentation_RunActorNineteenDialogueAndSetSceneState) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKuupuappuYouveMadeOurTownPeacefulAt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKuupuappuNowMtAlephHasCalmedDown) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKuupuappuOfAllThePlacesWhyWould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKuupuappuNobodyGetsIntoLunpaExceptFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKuupuappuThereIsAlwaysAFightWhen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKuupuappuSniffAhTheSmellOfCooking) },
	{ SEARCH(SEARCH_UNNAMED), 30, CONDITION_ALWAYS, EVENT_SCRIPT(EV30_SEARCH) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
	{ 0xe714, 19, 0x858, EVENT_SCRIPT(EV19_SCRIPT) },
	{ SEARCH(SEARCH_WOODEN_BOX), 101, 0xf4a, FIND_ITEM(ITEM_NUT) },
	{ SCENE_EVENTS_END },
};

u16 KuupuappuMura_Scene5Cells[] = {
	MAP_CELL_STEP(35, 52, 2, 2, 4), MAP_CELL_STEP(33, 52, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMura_Scene6Cells[] = {
	MAP_CELL_STEP(35, 50, 2, 2, 4), MAP_CELL_STEP(33, 50, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMura_Scene7Cells[] = {
	MAP_CELL_STEP(35, 48, 2, 2, 4), MAP_CELL_STEP(33, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMura_Scene8Cells[] = {
	MAP_CELL_STEP(42, 48, 2, 2, 4), MAP_CELL_STEP(40, 48, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMura_Scene9Cells[] = {
	MAP_CELL_STEP(42, 46, 2, 2, 4), MAP_CELL_STEP(40, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMura_Scene10Cells[] = {
	MAP_CELL_STEP(35, 46, 2, 2, 4), MAP_CELL_STEP(33, 46, 2, 2, 4), MAP_CELL_STEPS_END,
};

u16 KuupuappuMura_Scene12Cells[] = {
	MAP_CELL_STEP(35, 33, 2, 2, 4), MAP_CELL_STEP(33, 33, 2, 2, 4), MAP_CELL_STEPS_END,
};

s32 KuupuappuMura_DriftScriptA[] = {
	SC_WAIT(1),
	SC_LOOP(118, SC_TOP),
	SC_FREE,
};

s32 KuupuappuMura_DriftScriptB[] = {
	SC_WAIT(1),
	SC_LOOP(38, SC_TOP),
	SC_FREE,
};
