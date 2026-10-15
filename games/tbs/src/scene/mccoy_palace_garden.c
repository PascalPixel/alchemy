#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/mccoy_palace_garden.h"
#include "unit/scene/common/object_orbit.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mccoy_palace_garden.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"
#include "facing_object.h"

// 前方宣言
static void McCoyPalaceGarden_ApplyEntryState(void);
static void McCoyPalaceGarden_RunGardenScene(void);
static void FieldScene_OpenGate(void);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_facing.h"

u8 *SceneData_GetScriptTable(void)
{
	return (u8 *)Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetMessageTable(void)
{
	return (u8 *)Placement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden)
	{
		if (Engine_GameFlagIsSet(0x84f) != 0)
			gMcCoyPalaceGardenPlacements[4].talkFacing = TALK_FACE_PARTY_AND_BACK;
		if (Engine_GameFlagIsSet(0x845) != 0)
			gMcCoyPalaceGardenPlacements[2].talkFacing = TALK_FACE_PARTY;
		return gMcCoyPalaceGardenPlacements;
	}
	return gMcCoyPalaceGardenPlacementsOther;
}

static void EV13_PSYNERGY(void)
{
	Engine_EventBegin();
	Engine_ActorSetPosition(0xD, 0, 0);
	Engine_GameFlagSet(0xFD2);
	Engine_ItemShowFound(ITEM_NUT, 3);
	Engine_PartyGiveItem(ITEM_NUT, 0);
	Engine_EventEnd();
}

static void EV90_SEARCH(void)
{
	Engine_EventBegin();
	Engine_MessageShowCentered(MsgFieldPeeredWell, 1);
	Engine_MessageShowCentered(MsgBilibinUponCloserInspectionSeemsDried, 1);
	Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
	if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden)
	{
		return gMcCoyPalaceGardenEvents;
	}
	return gMcCoyPalaceGardenEventsOther;
}

static void EV09_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgBilibinHaveYouSeenBarricadeWe);
	Engine_EventAskYesNo(9, 0);
	Engine_EventEnd();
}

static void EV10_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgBilibinTellingMeImResponsibleFor);
	Engine_EventAskYesNo(10, 0);
	Engine_EventEnd();
}

static void EV11_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgBilibinDoThinkCanBecomeAs);
	Engine_EventAskYesNo(11, 0);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x84a) != 0)
	{
		if (Engine_GameFlagIsSet(0x304) != 0)
		{
			if (GameFlag_IsSet(0x201) == 0)
			{
				Engine_EventSetMessage(MsgBilibinWhere);
				Engine_EventShowMessageAndWait(12, 0, 10);
				Actor_ShowEmote(12, 0x107, 40);
				Engine_EventShowMessageAndWait(12, 0, 10);
				Engine_ActorRunRepeatedMotion(12, 2);
				Engine_GameFlagSet(0x201);
			}
			Engine_EventSetMessage(MsgBilibinMasterQuiteCranky);
			Engine_EventShowMessage(12, 0);
		}
		else
		{
			Engine_EventSetMessage(MsgBilibinLordMccoyDifficult);
			Engine_EventShowMessage(12, 0);
			goto guard;
		}
	}
	else
	{
		Engine_EventSetMessage(MsgBilibinLordOnlyMeet);
		Engine_EventOpenMessage(12, 0);
		if (Engine_EventChooseYesNo(0, 0) == 0)
		{
			gEventWork->message++;
			Engine_EventShowMessageAndWait(12, 0, 10);
			if (Object_GetById(0)->z.fixed < 0x10e0000)
			{
				Actor_SetSpeed(12, 0xcccc, 0x6666);
				Actor_WalkToAndWait(0, 0x15a, 0x112);
				Actor_WalkToAndWait(0, 0x148, 0x11a);
				Actor_FaceDirection(0, 0xc000, 0);
			}
			Actor_FaceDirection(11, 0x1000, 0);
			Actor_FaceDirection(12, 0x7000, 20);
			Actor_ShowEmote(11, 0x102, 20);
			Engine_ActorStartRepeatedMotion(11, 1);
			Engine_EventShowMessageAndWait(11, 0, 10);
			Actor_ShowEmote(12, 0x108, 60);
			Engine_ActorStartRepeatedMotion(12, 1);
			Engine_EventShowMessageAndWait(12, 0, 20);
			Engine_ActorSetAnimation(11, 3);
			Engine_ActorSetAnimationAndWait(12, 3);
			Actor_FaceDirection(11, 0x3000, 0);
			Actor_FaceDirection(12, 0x5000, 10);
			Engine_ActorStartRepeatedMotion(11, 1);
			Engine_EventShowMessageAndWait(11, 0, 20);
			Actor_FaceDirection(11, 0xf000, 0);
			Engine_ActorSetSpeed(12, 0x10000, 0x8000);
			Object_GetById(12)->steeringFlags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(12, 0x15a, 0x107);
			Engine_EventWait(1);
			Object_GetById(12)->steeringFlags |= ACTOR_FACES_MOTION;
			Actor_SetSpeed(11, 0x9999, 0x4ccc);
			Actor_WalkToAndWait(11, 0x148, 0x107);
			Actor_WalkToAndWait(11, 0x148, 252);
			Actor_FaceDirection(11, 0xc000, 10);
			FieldScene_OpenGate();
			Actor_WalkToAndWait(11, 0x148, 246);
			/* FAKEMATCH: instruction order only: called through a cast with no
			   prototype that returns a value, the call sets r0, so sched2 loads
			   the actor id after the two zeros, as native. Plain forms measured
			   (2026-10-13): Engine_ActorSetPosition or Actor_SetPosition by
			   their prototype load r0 first (differ 2). */
			((s32 (*)())Engine_ActorSetPosition)(11, 0, 0);
			/* FAKEMATCH: instruction order only: the one-pass block's loop note
			   after the call above is a scheduling barrier; with the cast it
			   keeps r0 last among that call's arguments. A plain call or
			   GameFlag_Set here loads r0 first (differ 2). */
			do
			{
				Engine_GameFlagSet(0x84a);
			} while (0);
		guard:
			FieldObject_SetTargetAndCallback(12, 0x10000, McCoyPalaceGarden_GuardScript);
		}
		else
		{
			Engine_EventShowMessage(12, 0);
			Actor_FaceDirection(12, 0x3000, 10);
		}
	}
	Engine_EventEnd();
}

static void EV02_TOUCH(void)
{
	Engine_EventBegin();
	if (GameFlag_IsSet(0x200) == 0)
	{
		FieldScene_OpenGate();
	}
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
	Engine_EventWait(16);
	Engine_EventRequestExit(2);
	Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
	struct EventWork  *work = gEventWork;
	struct FieldActor *actor;

	work->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

	actor = Object_GetById(8);
	actor->priorityFlags = 0;
	actor->sprite->priority = 1;

	if (gGameState.scene == (s32)&SceneId_McCoyPalaceGarden)
	{
		McCoyPalaceGarden_ApplyEntryState();
	}

	return 0;
}

static void McCoyPalaceGarden_ApplyEntryState(void)
{
	if (Engine_GameFlagIsSet(0x109) != 0)
	{
		Engine_GameFlagClear(0x200);
	}
	if (Engine_GameFlagIsSet(0xfd2) == 0)
	{
		InitializeStagedActorSceneOrbitingEffect(13);
	}
	if (Engine_GameFlagIsSet(0x84a) != 0)
	{
		Actor_SetPosition(11, 0x1340000, 0x1070000);
		Engine_ActorSetPosition(12, 0x15a0000, 0x1070000);
		if (Engine_GameFlagIsSet(0x84f) == 0)
		{
			if (Engine_GameFlagIsSet(0x845) == 0)
			{
				Engine_ActorSetPosition(11, 0, 0);
				FieldObject_SetTargetAndCallback(12, 0x10000, McCoyPalaceGarden_GuardScript);
			}
		}
	}
	if (Engine_GameFlagIsSet(0x845) != 0)
	{
		Actor_SetPosition(10, 0xe00000, 0x1240000);
		Actor_FaceDirection(10, 0x4000, 0);
		Engine_ActorFaceDirection(8, 0, 0);
		if (Engine_GameFlagIsSet(0x85e) == 0)
		{
			McCoyPalaceGarden_RunGardenScene();
		}
	}
}

static void McCoyPalaceGarden_RunGardenScene(void)
{

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Camera_MoveTo(0x1400000, -1, 0x1400000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Actor_SetPosition(0, 0x1400000, 0x1740000);
	Engine_EventOpenScreen();
	Engine_CameraSetSpeed(0x3333, 0x666);
	Engine_CameraMoveTo(0x1400000, -1, 0x1220000, 1);
	Actor_SetSpeed(0, 0x9999, 0x4ccc);
	Actor_WalkToAndWait(0, 0x140, 0x136);
	Actor_FaceDirection(11, 0x3000, 10);
	Engine_ActorStartRepeatedMotion(11, 2);
	Actor_ShowEmote(11, 0x100, 60);
	Engine_EventSetMessage(MsgBilibinOhItS);
	Engine_EventShowMessageAndWait(11, 0, 10);
	Actor_FaceDirection(12, 0x5000, 10);
	Engine_ActorStartRepeatedMotion(12, 2);
	Actor_ShowEmote(12, 0x100, 60);
	Engine_EventShowMessageAndWait(12, 0, 20);
	Actor_FaceDirection(11, 0x1000, 0);
	Actor_FaceDirection(12, 0x7000, 40);
	Actor_FaceDirection(11, 0x3000, 0);
	Actor_FaceDirection(12, 0x5000, 10);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventShowMessageAndWait(11, 0, 10);
	Engine_ActorRunRepeatedMotion(12, 1);
	Engine_EventOpenMessage(12, 0);
	Actor_FaceDirection(0, 0xe000, 0);
	while (Engine_EventChooseYesNo(0, 0) != 0)
	{
		Engine_ActorShowEmote(12, 0x100, 60);
		Engine_EventSetMessage(MsgBilibinNotTrueWitnesses);
		Engine_EventShowMessageAndWait(12, 0, 10);
		Engine_ActorStartRepeatedMotion(12, 2);
		Engine_EventOpenMessage(12, 0);
	}
	Engine_EventWait(10);
	Actor_FaceDirection(11, 0x3000, 0);
	Actor_FaceDirection(12, 0x5000, 20);
	Engine_ActorSetAnimation(11, 3);
	Engine_ActorSetAnimationAndWait(12, 3);
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(11, 1);
	Engine_EventSetMessage(MsgBilibinUnderArrest);
	Engine_EventShowMessageAndWait(11, 0, 10);
	Actor_SetSpeed(11, 0x10000, 0x8000);
	Actor_WalkToAndWait(11, 0x13a, 0x118);
	Actor_FaceDirection(0, 0xc000, 20);
	Engine_EventShowMessageAndWait(11, 0, 40);
	Actor_SetAttachedEffect(0, 0x102);
	Engine_EventWait(60);
	if (Engine_GameFlagIsSet(0x84a) == 0)
	{
		Engine_ActorSetSpeed(12, 0x10000, 0x8000);
		Object_GetById(12)->steeringFlags &= 254;
		Engine_ActorWalkToAndWait(12, 0x15a, 0x107);
		Engine_EventWait(1);
		{
			struct FieldActor *record = Object_GetById(12);

			record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
		}
	}
	Actor_SetSpeed(11, 0xcccc, 0x6666);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Actor_WalkTo(11, 0x148, 0x106);
	Actor_WalkToAndWait(0, 0x148, 0x116);
	Engine_ActorSetAnimation(11, 1);
	FieldScene_OpenGate();
	Engine_EventWait(40);
	Actor_WalkTo(0, 0x148, 242);
	Actor_WalkToAndWait(11, 0x148, 242);
	Engine_ActorSetPosition(11, 0, 0);
	Engine_ActorWaitForMove(0);
	Engine_ActorSetPosition(0, 0, 0);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(10);
	Engine_EventEnd();
}

static void FieldScene_OpenGate(void)
{
	Engine_AudioPlayCue(0xBC);
	Engine_MapAnimateCells(Garden_GateCells, 0x34, 0xB);
	Engine_GameFlagSet(0x200);
}

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_orbit.h"

u16 Garden_GateCells[] = {
	MAP_CELL_STEP(34, 25, 1, 3, 5), MAP_CELL_STEP(35, 25, 1, 3, 5), MAP_CELL_STEP(36, 25, 1, 3, 5), MAP_CELL_STEPS_END,
};

s32 McCoyPalaceGarden_GuardScript[] = {
	SC_CALL(OverlayObject_UpdateFacingTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

struct SceneEntrance Placement_Scripts[] = {
	{ 1, CONDITION_ALWAYS, 320, 0, 328, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 328, 0, 272, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 Placement_Messages[] = {
	SCENE_EXITS(34),
	SCENE_EXIT(1, 30, 2),
	SCENE_EXIT(2, 33, 1),
	SCENE_EXIT(10, 33, 29),
	SCENE_EXITS_END,
};

struct ScenePlacement gMcCoyPalaceGardenPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gMcCoyPalaceGardenPlacements[] = {
	{ 0x87, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(103), 0, PIXELS(280), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x87, CONDITION_ALWAYS, ACTOR_WANDER,
		PIXELS(135), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x45, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(327), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(320), 0, PIXELS(263), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(334), 0, PIXELS(263), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x16, 0xfd2, 7,
		PIXELS(216), 0, PIXELS(344), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
	{ 0xf8, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(388), FACING_SOUTH, TALK_KEEP_FACING, 0 },
#endif
	{ SCENE_TABLE_END },
};

struct SceneEvent gMcCoyPalaceGardenEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gMcCoyPalaceGardenEvents[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_TOUCH) },
	{ EVENT_TALK, 8, 0x845, MSGNO(MsgBilibinTheLumberIOrderedFromKolima) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgBilibinNowThatTheCurseIsBroken) },
	{ EVENT_TALK, 9, 0x845, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBilibinItsOverOverMilordDecidedNot) },
	{ EVENT_TALK, 10, 0x845, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinHmphMyPalaceIsNotFinished) },
	{ EVENT_TALK, 11, 0x84f, MSGNO(MsgBilibinLordMccoyIsSoUpsetAbout) },
	{ EVENT_TALK, 11, 0x84e, MSGNO(MsgBilibinICantTellYouAnythingElse) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
	{ EVENT_TALK, 12, 0x84f, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 12, 0x84e, MSGNO(MsgBilibinWhatAShameHuhIGuess) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinICouldNeverHaveGoneTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x845, MSGNO(MsgBilibinIfThingsLikeThisAreGoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBilibinWedStillBeWorkingIfWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x845, MSGNO(MsgBilibinIAmGladIDidntKnow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinThatWasCloseBuildingACursed) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x845, MSGNO(MsgBilibinIfAllThisTroubleWasCaused) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinHeSaidHeStoppedConstructionSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x84a, MSGNO(MsgBilibinNoOneBelievedTheyWouldTurn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x84e, MSGNO(MsgBilibinThatBarricadeIsPrettyShabbyIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinIWillBeAReliableWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x84a, MSGNO(MsgBilibinIsThereNoOneWhoCan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x84f, MSGNO(MsgBilibinLordMccoyWouldNeverHireAnyone) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x84e, MSGNO(MsgBilibinIDidntThinkTheydLetYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinDontLeaveIsaacBeMyMentor) },
	{ SEARCH(SEARCH_UNNAMED), 90, CONDITION_ALWAYS, EVENT_SCRIPT(EV90_SEARCH) },
	{ PSYNERGY_ON_ACTOR(ABILITY_CATCH), 13, 0xfd2, EVENT_SCRIPT(EV13_PSYNERGY) },
	{ SCENE_EVENTS_END },
};
