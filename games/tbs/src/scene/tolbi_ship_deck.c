#include "game_state.h"
#include "unit/scene/tolbi_ship_deck.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/field/field.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
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
#include "field_scene.h"
#include "call.h"
#include "field_service.h"
#include "global_cells.h"
#include "edition.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "objcmd.h"

// 前方宣言
static void TolbiShipDeck_ConfrontCrewman(void);
static void TolbiShipDeck_ApplyEntryState(void);
static void TolbiShipDeck_PlaceRandomDeckActors(void);
static void SceneActor_PlaceActors20To27(void);
static void TolbiShipDeck_SetupRaisedCrewman(void);
static void TolbiShipDeck_SetupDeckCrewman(void);
static void TolbiShipDeck_PlaceDeckActors(void);
static void TolbiShipDeck_SetupHiddenLeaderDeparture(void);
static void TolbiShipDeck_SetupVisibleLeaderDeparture(void);
static void TolbiShipDeck_AnnouncePreparationsReady(s32 a0, s32 a1);
static void FieldScene_RunActorTwentyDialogueSequence(void);
static void FieldScene_RunShipDeckEventScript(void);
static void TolbiShipDeck_RunDeckCrewScene(void);
static void FieldScene_RunActorSequence(void);
static void TolbiShipDeck_RunJumpScene(void);
static void FieldScene_ConfigureLeadActors(void);
static void FieldScene_ConfigureThreeActors(void);
static void FieldScene_RunPartyRosterScene(void);
static s32 SceneState_FindFirstSetFlagOfGroup(u32 selection);
static void TolbiShipDeck_RockDeck(void);
static void SceneEffect_InitSlotsEightToNineteen(void);
static void OverlayObject_ActivateSlotWithMode3(s32 a);
static void TolbiShipDeck_ChooseSlotModes(void);
static s32 SceneData_SelectTableEntryByFlagGroup(u32 selection);
static void SceneState_ConfigureEntries8Through19(void);
static void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot);
static void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags);
static void SceneState_InitActorSlots8To19(void);
static void SceneState_ApplyFiveRectsAtColumn78(void);
static void DialogueLayout_ConfigureTwoRegions(void);
static void FieldScene_RunStepThen10(s32 a);
static void FieldScene_CallPairWith10(s32 a, s32 b);
static void TolbiShipDeck_ArriveAtTolbi(void);
static void TolbiShipDeck_RunIsaacTalk(void);
static void TolbiShipDeck_PlaceLeaderOnDeck(void);

// 戦闘開始の種類 (battle/party/party.c と同じ)

s32 Math_RemainderUnsigned(s32, s16);

// ここに共通ソースが入る（ROM の並びのとおり）
#include "common/object_pulse.h"

static s32 SceneState_ApplyArgMode1AndReturnZero(s32 a)
{
	Engine_ActorSetSpriteFlags(a, 1);
	return 0;
}

struct HoverGull
{
	u8	unk00[0x4c];
	s32 drift;
	u8	unk50[0x12];
	u8	step;
	u8	rightSide;
	s16 driftingLeft;
	s16 sinking;
};

#define GULL(obj) ((struct HoverGull *)(obj))

static s32 TolbiShipDeck_UpdateHoverGullA(struct FieldActor *obj)
{
	if (GULL(obj)->step != 0)
	{
		switch (GULL(obj)->step)
		{
		case 1:
			obj->speed = 0x40000;
			obj->acceleration = 0x20000;
			FieldObject_SetPosition((s32)obj, 0x10c0000, 0x140000, 0x2b40000);
			GULL(obj)->step++;
			break;
		case 2:
			GULL(obj)->step++;
			break;
		case 3:
			if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
				&& obj->targetZ == obj->targetY)
			{
				GULL(obj)->step++;
				Event_PlayCue(146);
				if (GULL(obj)->rightSide != 0)
					Actor_FaceDirection(21, 0xd000, 0);
				else
					Actor_FaceDirection(21, 0xb000, 0);
				if (((u32)Engine_RandomNext() << 2) >> 16 != 0)
				{
					Actor_Get(21)->velocityY = 0x20000;
				}
				else
				{
					Actor_ShowEmote(21, 0x103, 0);
					Actor_Get(21)->velocityY = 0x60000;
				}
			}
			break;
		case 4:
			GULL(obj)->step++;
			break;
		case 5:
			if (GULL(obj)->rightSide != 0)
				FieldObject_SetPosition((s32)obj, 0x11a0000, 0, 0x2920000);
			else
				FieldObject_SetPosition((s32)obj, 0xfe0000, 0, 0x29c0000);
			GULL(obj)->step++;
			break;
		case 6:
			GULL(obj)->step++;
			break;
		case 7:
			if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
				&& obj->targetZ == obj->targetY)
			{
				obj->speed = 0x20000;
				obj->acceleration = 0x10000;
				GULL(obj)->driftingLeft = 0;
				GULL(obj)->sinking = 0;
				GULL(obj)->step++;
				GULL(obj)->drift = 0;
			}
			break;
		case 8:
			GULL(obj)->step = 0;
			break;
		}
	}
	else
	{
		if (GULL(obj)->driftingLeft != 0)
		{
			GULL(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
			if (GULL(obj)->drift < -0x4000)
				GULL(obj)->driftingLeft = 0;
		}
		else
		{
			GULL(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
			if (GULL(obj)->drift > 0x4000)
				GULL(obj)->driftingLeft = 1;
		}
		if (obj->x.fixed > 0xf80000 && obj->x.fixed < 0x1240000)
			obj->x.fixed += GULL(obj)->drift;
		if (GULL(obj)->sinking != 0)
		{
			obj->y.fixed = obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
			if (obj->y.fixed < 0)
				GULL(obj)->sinking = 0;
		}
		else
		{
			obj->y.fixed = obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
			if (obj->y.fixed > 0x80000)
				GULL(obj)->sinking = 1;
		}
	}
	if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
		GULL(obj)->step = 1;
	return 1;
}

static s32 TolbiShipDeck_UpdateHoverGullB(struct FieldActor *obj)
{
	if (GULL(obj)->step != 0)
	{
		switch (GULL(obj)->step)
		{
		case 1:
			obj->speed = 0x40000;
			obj->acceleration = 0x20000;
			FieldObject_SetPosition((s32)obj, 0x1000000, 0x140000, 0x2800000);
			GULL(obj)->step++;
			break;
		case 2:
			GULL(obj)->step++;
			break;
		case 3:
			if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
				&& obj->targetZ == obj->targetY)
			{
				GULL(obj)->step++;
				Event_PlayCue(146);
				if (GULL(obj)->rightSide != 0)
					Actor_FaceDirection(22, 0xd000, 0);
				else
					Actor_FaceDirection(22, 0xb000, 0);
				if (((u32)Engine_RandomNext() << 2) >> 16 != 0)
				{
					Actor_Get(22)->velocityY = 0x20000;
				}
				else
				{
					Actor_ShowEmote(22, 0x103, 0);
					Actor_Get(22)->velocityY = 0x60000;
				}
			}
			break;
		case 4:
			GULL(obj)->step++;
			break;
		case 5:
			if (GULL(obj)->rightSide != 0)
				FieldObject_SetPosition((s32)obj, 0x1080000, 0, 0x2580000);
			else
				FieldObject_SetPosition((s32)obj, 0xf20000, 0, 0x25c0000);
			GULL(obj)->step++;
			break;
		case 6:
			GULL(obj)->step++;
			break;
		case 7:
			if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
				&& obj->targetZ == obj->targetY)
			{
				obj->speed = 0x20000;
				obj->acceleration = 0x10000;
				GULL(obj)->driftingLeft = 0;
				GULL(obj)->sinking = 0;
				GULL(obj)->step++;
				GULL(obj)->drift = 0;
			}
			break;
		case 8:
			GULL(obj)->step = 0;
			break;
		}
	}
	else
	{
		if (GULL(obj)->driftingLeft != 0)
		{
			GULL(obj)->drift -= ((u32)Engine_RandomNext() << 12) >> 16;
			if (GULL(obj)->drift < -0x4000)
				GULL(obj)->driftingLeft = 0;
		}
		else
		{
			GULL(obj)->drift += ((u32)Engine_RandomNext() << 12) >> 16;
			if (GULL(obj)->drift > 0x4000)
				GULL(obj)->driftingLeft = 1;
		}
		if (obj->x.fixed > 0xe80000 && obj->x.fixed < 0x1100000)
			obj->x.fixed += GULL(obj)->drift;
		if (GULL(obj)->sinking != 0)
		{
			obj->y.fixed = obj->y.fixed - (((u32)Engine_RandomNext() << 15) >> 16) - 0x8000;
			if (obj->y.fixed < 0)
				GULL(obj)->sinking = 0;
		}
		else
		{
			obj->y.fixed = obj->y.fixed + (((u32)Engine_RandomNext() << 15) >> 16) + 0x8000;
			if (obj->y.fixed > 0x80000)
				GULL(obj)->sinking = 1;
		}
	}
	if (((u32)Engine_RandomNext() * 100) >> 16 == 0)
		GULL(obj)->step = 1;
	return 1;
}

struct MapLayer
{
	u8	unk00[8];
	s32 x;
	s32 y;
	u8	unk10[32];
};

struct MapWork
{
	s32			   *camera;
	u8				unk04[16];
	struct MapLayer layers[8];
};

static u16 sSlotPhase[4];

static s32 sWaveAngleY;
static s32 sSlotWork;

static u16 sSlotValue[8];

static s32 sLayerScroll[2];

static s32 sWaveAngleX;
static s32 sDeckSpare;

static s32 sLayerSpeed[2];

static s32 SceneActor_OscillateHeightBetweenLimits(struct FieldActor *actor)
{
	s16 *flag = (s16 *)&actor->work;
	s32	 val;
	s32	 tmp;

	if (*flag != 0)
	{
		val = actor->y.fixed - (((u32)(Engine_RandomNext() << 15)) >> 16) - 0x8000;
		actor->y.fixed = val;
		if (val >= 0x40000)
			goto done;
		tmp = 0;
	}
	else
	{
		val = actor->y.fixed + (((u32)(Engine_RandomNext() << 15)) >> 16) + 0x8000;
		actor->y.fixed = val;
		if (val <= 0xC0000)
			goto done;
		tmp = 1;
	}
	*flag = tmp;
done:
	return 1;
}

static s32 SceneActor_SetFacingFromSample(struct FieldActor *actor)
{
	u32 v = ((u32)(Engine_RandomNext() << 5)) >> 16;

	if (v == 6)
		actor->facing = 0xd000;
	else if (v == 9)
		actor->facing = 0xb000;
	return 1;
}

static s32 TolbiShipDeck_UpdateFlyByForActor21(struct FieldActor *obj)
{
	switch (obj->timer)
	{
	case 0:
		if ((((u32)Engine_RandomNext() * 40) >> 16) == 0)
		{
			obj->timer++;
		}
		break;
	case 1:
		obj->timer++;
		break;
	case 2:
		obj->velocityY = 0x40000;
		obj->speed = 0x40000;
		obj->acceleration = 0x20000;
		FieldObject_SetPosition((s32)obj, 0x1080000, 0, 0x2960000);
		obj->timer++;
		break;
	case 3:
		obj->timer++;
		break;
	case 4:
		if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
			&& obj->targetZ == obj->targetY)
		{
			obj->timer++;
			Engine_AudioPlayCue(152);
			if (obj->riseEnabled != 0)
			{
				Actor_FaceDirection(21, 0xb000, 0);
			}
			else
			{
				Actor_FaceDirection(21, 0x5000, 0);
			}
			if (((u32)Engine_RandomNext() << 2) >> 16 != 0)
			{
				Object_GetById(21)->velocityY = 0x20000;
			}
			else
			{
				Actor_ShowEmote(21, 0x103, 0);
				Object_GetById(21)->velocityY = 0x60000;
			}
		}
		break;
	case 5:
		obj->timer++;
		break;
	case 6:
		obj->timer++;
		obj->velocityY = 0x40000;
		obj->speed = 0x20000;
		obj->acceleration = 0x10000;
		if (obj->riseEnabled != 0)
		{
			FieldObject_SetPosition((s32)obj, 0xfc0000, 0, 0x2860000);
		}
		else
		{
			FieldObject_SetPosition((s32)obj, 0x1000000, 0, 0x2ae0000);
		}
		break;
	case 7:
		obj->timer++;
		break;
	case 8:
		if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
			&& obj->targetZ == obj->targetY)
		{
			obj->timer++;
		}
		break;
	case 9:
		obj->timer = 0;
		break;
	}
	return 1;
}

static s32 TolbiShipDeck_UpdateFlyByForActor22(struct FieldActor *obj)
{
	switch (obj->timer)
	{
	case 0:
		if ((((u32)Engine_RandomNext() * 40) >> 16) == 0)
		{
			obj->timer++;
		}
		break;
	case 1:
		obj->timer++;
		break;
	case 2:
		obj->velocityY = 0x40000;
		obj->speed = 0x40000;
		obj->acceleration = 0x20000;
		FieldObject_SetPosition((s32)obj, 0xb00000, 0, 0x2b80000);
		obj->timer++;
		break;
	case 3:
		obj->timer++;
		break;
	case 4:
		if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
			&& obj->targetZ == obj->targetY)
		{
			obj->timer++;
			Engine_AudioPlayCue(152);
			if (obj->riseEnabled != 0)
			{
				Actor_FaceDirection(22, 0xd000, 0);
			}
			else
			{
				Engine_ActorFaceDirection(22, 0, 0);
			}
			if (((u32)Engine_RandomNext() << 2) >> 16 != 0)
			{
				Object_GetById(22)->velocityY = 0x20000;
			}
			else
			{
				Actor_ShowEmote(22, 0x103, 0);
				Object_GetById(22)->velocityY = 0x60000;
			}
		}
		break;
	case 5:
		obj->timer++;
		break;
	case 6:
		obj->timer++;
		obj->velocityY = 0x40000;
		obj->speed = 0x20000;
		obj->acceleration = 0x10000;
		if (obj->riseEnabled != 0)
		{
			FieldObject_SetPosition((s32)obj, 0xb80000, 0, 0x2a00000);
		}
		else
		{
			FieldObject_SetPosition((s32)obj, 0xca0000, 0, 0x2b40000);
		}
		break;
	case 7:
		obj->timer++;
		break;
	case 8:
		if (obj->targetX == ACTOR_NO_TARGET && obj->targetY == obj->targetX
			&& obj->targetZ == obj->targetY)
		{
			obj->timer++;
		}
		break;
	case 9:
		obj->timer = 0;
		break;
	}
	return 1;
}

static s32 TolbiShipDeck_RandomlyStartActorHop(struct FieldActor *actor)
{
	if ((((u32)(Engine_RandomNext() * 40)) >> 16) == 0)
		actor->velocityY = 0x40000;
	return 1;
}

static void OverlayObject_DecayFields24And28(struct FieldActor *actor)
{
	if (actor->scaleX > 0x10000)
	{
		actor->scaleX += 0xFFFFF800;
		actor->scaleY += 0xFFFFF800;
	}
}

static s32 TolbiShipDeck_IdleTurn(struct FieldActor *actor)
{
	if (actor->riseCounter != 0)
	{
		actor->riseCounter--;
	}
	else
	{
		u32 roll = (u32)(Engine_RandomNext() * 300) >> 16;

		if (roll > 200)
		{
			actor->facing = 0xd000;
		}
		else if (roll > 100)
		{
			actor->facing = 0x5000;
		}
		else
		{
			actor->facing = 0;
		}
		actor->riseCounter = ((u32)(Engine_RandomNext() * 80) >> 16) + 80;
	}
	return 1;
}

u8 *TolbiShipDeck_GetEntrances(void)
{
	return gTolbiShipDeck_SceneTableA;
}

u8 *TolbiShipDeck_GetRegions(void)
{
	return gTolbiShipDeck_SceneTableB;
}

u8 *TolbiShipDeck_GetExits(void)
{
	return gTolbiShipDeck_SceneTableC;
}

u8 *TolbiShipDeck_GetPlacements(void)
{
	s32 v;

	if (Engine_GameFlagIsSet(0x93e))
		return gTolbiShipDeckPlacementsFlag93e;
	if (Engine_GameFlagIsSet(0x927))
		return gTolbiShipDeckPlacementsFlag927;
	v = Engine_GameFlagIsSet(0x928);
	if (v != 0)
		return gTolbiShipDeckPlacementsFlag928;
	if (Engine_GameFlagIsSet(0x911))
	{
		if (Engine_GameFlagIsSet(0x925))
		{
			STORE_CONST(gTolbiShipDeckPlacementsFlag911[13].talkFacing) = v;
			STORE_CONST(gTolbiShipDeckPlacementsFlag911[17].talkFacing) = 2;
			STORE_CONST(gTolbiShipDeckPlacementsFlag911[18].talkFacing) = 2;
		}
		else if (Engine_GameFlagIsSet(0x922))
		{
			STORE_CONST(gTolbiShipDeckPlacementsFlag911[17].talkFacing) = 1;
			STORE_CONST(gTolbiShipDeckPlacementsFlag911[18].talkFacing) = 1;
		}
		return gTolbiShipDeckPlacementsFlag911;
	}
	return gTolbiShipDeckPlacements;
}

u8 *TolbiShipDeck_GetEvents(void)
{
	if (Engine_GameFlagIsSet(0x93e))
		return gTolbiShipDeckEventsFlag93e;
	if (Engine_GameFlagIsSet(0x8A0))
		return gTolbiShipDeckEventsFlag8a0;
	if (Engine_GameFlagIsSet(0x928))
		return gTolbiShipDeckEventsFlag928;
	return gTolbiShipDeckEvents;
}

static void FieldScene_RunOpeningAuxiliarySequence(void)
{
	struct FieldActor *rec7;
	s32				   record;
	s32				   shown;

	Engine_EventBegin();
	if (GameFlag_IsSet(0x925) != 0)
	{
		Engine_EventSetMessage(MsgTolbiShipNowWeHaveProtectShip);
		Event_ShowMessage(21, 0);
	}
	else
	{
		if (GameFlag_IsSet(0x922) != 0)
		{
			Engine_ActorRunRepeatedMotion(21, 2);
			Engine_EventSetMessage(MsgTolbiShipSomebodyStopThem);
			Event_ShowMessage(21, 0);
			rec7 = Object_GetById(21);
			record = Random_Next();
			shown = ((u32)(90 * record) >> 16) + 60;
			rec7->timer = shown;
			Engine_ActorEnableActionCallback(21, gTolbiShipDeck_RandomActorActions);
		}
		else
		{
			Actor_ShowEmote(21, 0x103, 0);
			Engine_ActorStartRepeatedMotion(21, 3);
			Engine_EventSetMessage(MsgTolbiShipToldWereLeavingSoonSet);
			Event_ShowMessage(21, 0);
		}
	}
	Engine_EventEnd();
}

static void EV24_TALK(void)
{
	struct FieldActor *rec7;
	s32				   record;
	s32				   shown;

	Engine_EventBegin();
	if (GameFlag_IsSet(0x925) != 0)
	{
		Engine_EventSetMessage(MsgTolbiShipHaveMakeThemPromiseHelp);
		Event_ShowMessage(24, 0);
	}
	else
	{
		if (GameFlag_IsSet(0x922) != 0)
		{
			Engine_ActorRunRepeatedMotion(24, 2);
			Engine_EventSetMessage(MsgTolbiShipTheyCantPlanningMutiny);
			Event_ShowMessage(24, 0);
			rec7 = Object_GetById(24);
			record = Random_Next();
			shown = ((u32)(90 * record) >> 16) + 60;
			rec7->timer = shown;
			Engine_ActorEnableActionCallback(24, gTolbiShipDeck_RandomActorActions);
		}
		else
		{
			Actor_ShowEmote(24, 0x103, 0);
			Engine_ActorStartRepeatedMotion(24, 3);
			Engine_EventSetMessage(MsgTolbiShipIfWeDontLeaveSoon);
			Event_ShowMessage(24, 0);
		}
	}
	Engine_EventEnd();
}

static void SceneDialogue_RunActor21Line(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgTolbiShipHeadedColosso);
	Engine_EventAskYesNo(21, 0);
	Engine_EventEnd();
}

static void TolbiShipDeck_OpenFirstDoor(void)
{
	if (GameFlag_IsSet(0x271) == 0)
	{
		Event_PlayCue(158);
		Map_CopyCellsTo(30, 94, 13, 94, 1, 3);
		GameFlag_Set(0x271);
	}
}

static void TolbiShipDeck_OpenSecondDoor(void)
{
	if (GameFlag_IsSet(0x272) == 0)
	{
		Event_PlayCue(158);
		Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
		GameFlag_Set(0x272);
	}
}

static void TolbiShipDeck_RunDeckStateEvent(void)
{

	s32				  v5;
	struct EventWork *p6;

	p6 = gEventWork;
	Engine_EventBegin();
	v5 = 0;
	switch (p6->touchedTrigger)
	{
	case 1:
		v5 = 1;
		TolbiShipDeck_OpenFirstDoor();
		break;
	case 3:
		v5 = 1;
		TolbiShipDeck_OpenSecondDoor();
		break;
	}
	if (v5 != 0)
	{
		Actor_SetSpeed(0, 0x9999, 0x4ccc);
		FieldObjectMotion_CenterOnColumn(0, 1, -10);
		Engine_EventWait(10);
	}
	else
	{
		Engine_AudioPlayCue(123);
	}
	Engine_EventRequestExit(p6->touchedTrigger);
}

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
static void TolbiShipDeck_RunDeckStateEventFromEntry(void)
{
	if ((u16)(Object_GetById(ACTOR_PARTY_LEADER)->facing - 0x6001) <= 0x3ffe)
	{
		Engine_EventBegin();
		Actor_WalkTo(ACTOR_PARTY_LEADER, 214, 664);
		Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
		TolbiShipDeck_RunDeckStateEvent();
	}
}
#endif

static void EV10_TOUCH(void)
{
	struct FieldActor *record;

	if (GameFlag_IsSet(0x911) != 0)
	{
		Engine_EventBegin();
		Battle_ResetEffectCounter();
		Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
		Camera_SetSpeed(0x19999, 0x3333);
		Camera_MoveTo(0xbe0000, -1, 0x2c40000, 1);
		Engine_CameraWaitForMove();
		Engine_EventWait(40);
		Engine_ActorRunRepeatedMotion(22, 1);
		Engine_EventSetMessage(MsgTolbiShipWonderCouldHaveHappened);
		FieldScene_RunStepThen10(0x4016);
		Actor_ShowEmote(20, 0x102, 60);
		Engine_ActorStartRepeatedMotion(20, 2);
		FieldScene_RunStepThen10(20);
		Engine_ActorRunRepeatedMotion(22, 1);
		Actor_FaceDirection(22, 0x5000, 0);
		FieldScene_RunStepThen10(0x4016);
		Engine_ActorRunRepeatedMotion(20, 1);
		FieldScene_CallPairWith10(20, 0xb000);
		FieldScene_RunStepThen10(20);
		FieldScene_CallPairWith10(23, 0x3000);
		Engine_ActorSetAnimation(23, 3);
		FieldScene_RunStepThen10(0x4017);
		Actor_ShowEmote(22, 0x101, 40);
		Actor_FaceDirection(22, 0x8000, 20);
		FieldScene_RunStepThen10(0x4016);
		FieldScene_CallPairWith10(23, 0);
		Engine_ActorSetAnimationAndWait(23, 4);
		FieldScene_RunStepThen10(0x4017);
		Actor_ShowEmote(20, 0x100, 40);
		Engine_ActorStartRepeatedMotion(20, 2);
		FieldScene_RunStepThen10(20);
		Engine_ActorSetAnimationAndWait(22, 3);
		FieldScene_RunStepThen10(0x4016);
		FieldScene_CallPairWith10(20, 0xd000);
		Engine_ActorSetAnimation(23, 3);
		Engine_ActorSetAnimationAndWait(20, 3);
		Engine_EventWait(60);
		Actor_ShowEmote(22, 0x106, 40);
		FieldScene_CallPairWith10(22, 0x5000);
		Engine_EventSetMessage(MsgTolbiShipTheresNothingWeCanDo);
		Engine_ActorStartRepeatedMotion(22, 1);
		FieldScene_RunStepThen10(0x4016);
		Actor_ShowEmote(20, 0x101, 40);
		Engine_ActorStartRepeatedMotion(20, 2);
		FieldScene_RunStepThen10(20);
		Actor_ShowEmote(22, 0x108, 20);
		Event_ShowMessageAndWait(0x4016, 0, 20);
		Actor_ShowEmote(23, 0x102, 60);
		FieldScene_RunStepThen10(0x4017);
		FieldScene_CallPairWith10(22, 0x8000);
		Engine_ActorSetAnimationAndWait(22, 3);
		Event_ShowMessageAndWait(0x4016, 0, 20);
		Actor_ShowEmote(20, 0x102, 40);
		Engine_ActorStartRepeatedMotion(20, 2);
		FieldScene_RunStepThen10(20);
		FieldScene_CallPairWith10(22, 0x5000);
		Engine_ActorSetAnimation(22, 4);
		FieldScene_RunStepThen10(22);
		Actor_FaceDirection(20, 0xb000, 0);
		Actor_FaceDirection(23, 0x3000, 40);
		Actor_FaceDirection(23, 0, 0);
		Actor_FaceDirection(20, 0xd000, 20);
		Engine_ActorRunRepeatedMotion(22, 2);
		Engine_EventWait(20);
		FieldScene_RunStepThen10(0x4016);
		Actor_SetAttachedEffect(23, 0x102);
		Actor_SetAttachedEffect(20, 0x102);
		Engine_EventWait(40);
		Engine_ActorSetAnimationAndWait(22, 3);
		FieldScene_RunStepThen10(0x4016);
		Camera_SetSpeed(0xcccc, 0x1999);
		Camera_MoveTo(0xb60000, -1, 0x2f80000, 1);
		Actor_SetSpeed(23, 0xcccc, 0x6666);
		Engine_ActorEnableActionCallback(23, gTolbiShipDeck_PresentationActionsA);
		Actor_SetSpeed(22, 0xcccc, 0x6666);
		Engine_ActorEnableActionCallback(22, gTolbiShipDeck_PresentationActionsB);
		Actor_SetSpeed(20, 0xcccc, 0x6666);
		Actor_WalkToAndWait(20, 182, 0x2f8);
		Engine_ActorStartRepeatedMotion(20, 2);
		Actor_ShowEmote(20, 0x100, 60);
		FieldScene_CallPairWith10(20, 0xd000);
		Event_ShowMessageAndWait(20, 0, 20);
		Engine_ActorSetAnimationAndWait(20, 3);
		Engine_ActorJump(20, 4, 0);
		Actor_FaceDirection(20, 0x3000, 40);
		Camera_SetSpeed(0x10000, 0x2000);
		Camera_MoveTo(0xd80000, -1, 0x3160000, 1);
		{
			struct FieldActor *record = Object_GetById(20);
			record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
		}
		Actor_SetSpeed(20, 0x13333, 0x9999);
		Actor_WalkToAndWait(20, 182, 0x30e);
		Actor_WalkToAndWait(20, 192, 0x328);
		Actor_WalkToAndWait(20, 216, 0x328);
		FieldScene_CallPairWith10(20, 0xd000);
		Engine_ActorRunRepeatedMotion(20, 2);
		TolbiShipDeck_OpenSecondDoor();
		Actor_WalkToAndWait(20, 216, 0x31e);
		Actor_SetPosition(20, 0, 0);
		record = Object_GetById(20);
		record->scaleX = 0x10000;
		record = Object_GetById(20);
		record->scaleY = 0x10000;
		GameFlag_Set(0x920);
		Engine_EventEnd();
	}
}

static void EV11_TOUCH(void)
{
	u8 bits;

	if (GameFlag_IsSet(0x911) != 0)
	{
		if (GameFlag_IsSet(0x922) == 0)
		{
			Engine_EventBegin();
			Battle_ResetEffectCounter();
			TolbiShipDeck_ConfrontCrewman();
			Actor_SetSpeed(20, 0x6666, 0x3333);
			Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(20, 232, 0x330);
			Engine_EventWait(1);
			bits = 1;
			{
				u8 *record = (s32)Object_GetById(20);
				u8	value = record[90];

				record[90] = value | bits;
			}
			Engine_EventWait(20);
			Engine_ActorStartRepeatedMotion(20, 2);
			FieldScene_RunStepThen10(20);
			Actor_SetSpeed(20, 0x13333, 0x9999);
			Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(20, 244, 0x324);
			Engine_EventWait(1);
			{
				struct FieldActor *record = Object_GetById(20);

				bits |= record->steeringFlags;
				record->steeringFlags = bits;
			}
			Engine_EventWait(20);
			Actor_SetSpeed(20, 0x33333, 0x19999);
			Actor_WalkToAndWait(20, 248, 0x30a);
			Actor_WalkToAndWait(20, 248, 0x2bc);
			Actor_SetPosition(20, 0xf60000, 0x2000000);
			Actor_FaceDirection(20, 0, 0);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
			Engine_EventEnd();
		}
	}
}

static void EV12_TOUCH(void)
{
	u8 bits;

	if (GameFlag_IsSet(0x911) != 0)
	{
		if (GameFlag_IsSet(0x922) == 0)
		{
			Engine_EventBegin();
			Battle_ResetEffectCounter();
			TolbiShipDeck_ConfrontCrewman();
			Actor_SetSpeed(20, 0x6666, 0x3333);
			Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(20, 202, 0x330);
			Engine_EventWait(1);
			bits = 1;
			{
				u8 *record = (s32)Object_GetById(20);
				u8	value = record[90];

				record[90] = value | bits;
			}
			Engine_EventWait(20);
			Engine_ActorStartRepeatedMotion(20, 2);
			FieldScene_RunStepThen10(20);
			Actor_SetSpeed(20, 0x13333, 0x9999);
			Object_GetById(20)->steeringFlags &= ~ACTOR_FACES_MOTION;
			Actor_WalkToAndWait(20, 192, 0x324);
			Engine_EventWait(1);
			{
				struct FieldActor *record = Object_GetById(20);

				bits |= record->steeringFlags;
				record->steeringFlags = bits;
			}
			Engine_EventWait(20);
			Actor_SetSpeed(20, 0x33333, 0x19999);
			Actor_WalkToAndWait(20, 180, 0x30a);
			Actor_WalkToAndWait(20, 180, 0x2bc);
			Actor_SetPosition(20, 0xf60000, 0x2000000);
			Actor_FaceDirection(20, 0, 0);
			Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 60);
			Engine_EventEnd();
		}
	}
}

static void TolbiShipDeck_ConfrontCrewman(void)
{
	Camera_SetSpeed(0x19999, 0x3333);
	Camera_MoveTo(0xd80000, -1, 0x3380000, 1);
	Engine_CameraWaitForMove();
	Engine_EventWait(20);
	TolbiShipDeck_OpenSecondDoor();
	Map_CopyCellsTo(30, 108, 13, 108, 1, 2);
	Engine_EventWait(10);
	Actor_SetPosition(20, 0xd80000, 0x3200000);
	Actor_SetSpeed(20, 0x13333, 0x9999);
	Actor_WalkToAndWait(20, 216, 0x32e);
	Actor_FaceActor(ACTOR_PARTY_LEADER, 20, 10);
	Engine_ActorSetAnimationAndWait(20, 4);
	Engine_ActorStartRepeatedMotion(20, 2);
	Actor_ShowEmote(20, 0x100, 20);
	Actor_FaceActor(20, ACTOR_PARTY_LEADER, 20);
	Engine_ActorStartRepeatedMotion(20, 2);
	Engine_EventSetMessage(MsgTolbiShipDidntDoAnything);
	Event_ShowMessageAndWait(20, 0, 20);
	Engine_ActorShowEmote(20, 0x102, 0);
	GameFlag_Set(0x923);
}

s32 TolbiShipDeck_ResetDeck(void)
{
	u8 **base = ((u8 **)gMapWork);
	u8	*map = base[0] + 0x104;

	Engine_GameFlagClear(0x11c);
	*(s32 *)(base[19] + 0x1c0) = 0x209;
	*(s32 *)(map + 0x1c) = 0;
	sWaveAngleX = (u16)Engine_RandomNext();
	sWaveAngleY = (u16)Engine_RandomNext();
	sLayerScroll[0] = 0;
	sLayerScroll[1] = 0;
	sLayerSpeed[0] = 0;
	Engine_MapRedraw();
	Engine_TaskWait(1);
	TolbiShipDeck_ApplyEntryState();
	return 0;
}

static void TolbiShipDeck_ApplyEntryState(void)
{
	s32 flag;

	Engine_GameFlagSet(0x144);
	if (Engine_GameFlagIsSet(0x109) != 0)
	{
		Engine_GameFlagClear(0x271);
		Engine_GameFlagClear(0x272);
	}
	if (Engine_GameFlagIsSet(0x93e) != 0)
	{
		SceneState_ApplyFiveRectsAtColumn78();
		DialogueLayout_ConfigureTwoRegions();
		Engine_ActorSetChildValue(24, 2);
	}
	else if (Engine_GameFlagIsSet(0x8a0) != 0)
	{
		SceneState_ApplyFiveRectsAtColumn78();
		DialogueLayout_ConfigureTwoRegions();
	}
	else if ((flag = Engine_GameFlagIsSet(0x927)) != 0)
	{
		SceneState_ApplyFiveRectsAtColumn78();
		SceneEffect_InitSlotsEightToNineteen();
		Task_AddCallback(SceneState_ConfigureEntries8Through19, 0xc80);
		sLayerScroll[1] = 0x200000;
		sLayerSpeed[1] = 0x13333;
		FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, 0xc80);
	}
	else if (Engine_GameFlagIsSet(0x928) != 0)
	{
		SceneState_ApplyFiveRectsAtColumn78();
		sLayerScroll[1] = flag;
		sLayerSpeed[1] = flag;
		FieldScheduler_AddCallback(TolbiShipDeck_RockDeck, 0xc80);
	}
	if (Engine_GameFlagIsSet(0x927) == 0)
	{
		SceneState_InitActorSlots8To19();
	}
	switch (gGameState.entrance)
	{
	case 4:
		Object_GetById(0)->sprite->priority = 1;
		break;
	case 10:
		if (Engine_GameFlagIsSet(0x928) != 0)
		{
			TolbiShipDeck_SetupVisibleLeaderDeparture();
		}
		else
		{
			TolbiShipDeck_SetupHiddenLeaderDeparture();
		}
		return;
	case 11:
		FieldScene_RunActorTwentyDialogueSequence();
		return;
	case 12:
		FieldScene_RunShipDeckEventScript();
		return;
	case 13:
		TolbiShipDeck_RunDeckCrewScene();
		return;
	case 14:
		FieldScene_RunActorSequence();
		return;
	case 15:
		TolbiShipDeck_RunJumpScene();
		return;
	case 16:
		if (Engine_GameFlagIsSet(0x109) != 0)
		{
			TolbiShipDeck_SetupDeckCrewman();
		}
		else
		{
			FieldScene_ConfigureLeadActors();
		}
		return;
	case 17:
		FieldScene_ConfigureThreeActors();
		return;
	case 18:
		if (Engine_GameFlagIsSet(0x109) == 0)
		{
			FieldScene_RunPartyRosterScene();
		}
		return;
	case 19:
		TolbiShipDeck_ArriveAtTolbi();
		return;
	}
	if (Engine_GameFlagIsSet(0x93e) != 0)
	{
		*(s32 *)(((u8 *)gMapWork[0]) + 236) = 0x410000;
	}
	else if (Engine_GameFlagIsSet(0x8a0) != 0)
	{
		TolbiShipDeck_PlaceDeckActors();
	}
	else if (Engine_GameFlagIsSet(0x92b) != 0)
	{
		TolbiShipDeck_SetupDeckCrewman();
	}
	else if (Engine_GameFlagIsSet(0x928) != 0)
	{
		TolbiShipDeck_SetupRaisedCrewman();
	}
	else if (Engine_GameFlagIsSet(0x925) != 0)
	{
		SceneActor_PlaceActors20To27();
	}
	else if (Engine_GameFlagIsSet(0x911) != 0)
	{
		TolbiShipDeck_PlaceRandomDeckActors();
	}
}

static void TolbiShipDeck_PlaceRandomDeckActors(void)
{
	struct FieldActor *actor;
	s32				   flag;

	Engine_ActorSetSpritePriority(27, 1);
	Engine_ActorSetSpritePriority(23, 1);
	Engine_ActorSetSpritePriority(22, 1);
	Engine_ActorSetSpritePriority(26, 1);
	Engine_ActorSetSpritePriority(24, 1);
	if (Engine_GameFlagIsSet(0x920) != 0)
	{
		Engine_ActorSetPosition(22, 0xa20000, 0x29a0000);
		Object_GetById(22)->facing = 0x8000;
		Engine_ActorSetPosition(23, 0, 0);
		Engine_ActorSetPosition(20, 0, 0);
	}
	flag = Engine_GameFlagIsSet(0x922);
	if (flag != 0)
	{
		Engine_ActorSetPosition(21, 0x1080000, 0x2be0000);
		Object_GetById(21)->facing = 0x5000;
		actor = Object_GetById(21);
		actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
		Engine_ActorEnableActionCallback(21, gTolbiShipDeck_RandomActorActions);
		Engine_ActorSetPosition(24, 0xf80000, 0x2a80000);
		actor = Actor_Get(24);
		actor->timer = Math_RemainderUnsigned(Engine_RandomNext(), 90) + 60;
		Engine_ActorEnableActionCallback(24, gTolbiShipDeck_RandomActorActions);
		Engine_ActorSetPosition(22, 0, 0);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x923) != 0)
		{
			Actor_SetPosition(20, 0xf60000, 0x2000000);
			Object_GetById(20)->facing = flag;
		}
	}
}

static void SceneActor_PlaceActors20To27(void)
{
	Engine_ActorSetPosition(21, 0x1060000, 0x2C20000);
	Object_GetById(21)->facing = 0x5000;
	Engine_ActorSetPosition(24, 0xA40000, 0x2880000);
	Object_GetById(24)->facing = 0;
	Engine_ActorSetSpritePriority(24, 1);
	Engine_ActorSetPosition(25, 0xC60000, 0x2990000);
	Object_GetById(25)->facing = 0x8000;
	Engine_ActorSetSpritePriority(25, 1);
	Engine_ActorSetPosition(26, 0xBC0000, 0x2A60000);
	Object_GetById(26)->facing = 0xb000;
	Engine_ActorSetPosition(27, 0xBA0000, 0x27B0000);
	Object_GetById(27)->facing = 0x5000;
	Engine_ActorSetPosition(22, 0, 0);
	Engine_ActorSetPosition(23, 0, 0);
	Engine_ActorSetPosition(20, 0, 0);
}

static void TolbiShipDeck_SetupRaisedCrewman(void)
{
	struct FieldActor *record;
	u8				   bits;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Actor_SetPosition(20, 0, 0);
	Actor_SetPosition(23, 0xee0000, 0x2720000);
	Actor_SetPosition(22, 0xcc0000, 0x2090000);
	record = Object_GetById(22);
	record->y.fixed = 0x100000;
	bits = 128;
	{
		struct FieldActor *record = Object_GetById(22);
		u8				   value = record->collisionFlags;

		record->collisionFlags = value | bits;
	}
	Actor_SetSpeed(22, 0x9999, 0x4ccc);
	Engine_ActorEnableActionCallback(22, gTolbiShipDeck_LeadActionsA);
	{
		struct FieldActor *record = Object_GetById(21);

		bits |= record->collisionFlags;
		record->collisionFlags = bits;
	}
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Engine_ActorEnableActionCallback(21, gTolbiShipDeck_LeadActionsB);
	if (GameFlag_IsSet(0x109) != 0)
	{
		TolbiShipDeck_PlaceLeaderOnDeck();
	}
	Engine_EventEnd();
}

static void TolbiShipDeck_SetupDeckCrewman(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Actor_SetPosition(20, 0, 0);
	Actor_SetPosition(23, 0xee0000, 0x2720000);
	Actor_SetPosition(22, 0x10c0000, 0x2a60000);
	record = Object_GetById(22);
	record->facing = 0;
	Engine_ActorEnableActionCallback(22, gTolbiShipDeck_LeadActionsC);
	{
		struct FieldActor *record = Object_GetById(21);
		record->collisionFlags = 128 | record->collisionFlags;
	}
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Engine_ActorEnableActionCallback(21, gTolbiShipDeck_LeadActionsB);
	if (GameFlag_IsSet(0x109) != 0)
	{
		TolbiShipDeck_PlaceLeaderOnDeck();
	}
	Engine_EventEnd();
}

static void TolbiShipDeck_PlaceDeckActors(void)
{
	struct FieldActor *record;
	s32				   v5;

	*(s32 *)((s32)gMapWork[0] + 236) = 0x410000;
	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScriptE);
	Engine_TaskWait(1);
	Engine_ActorDestroy(24);
	Actor_SetPosition(23, 0xee0000, 0x2720000);
	v5 = 192;
	record = Object_GetById(23);
	record->facing = (v5 << 6);
	if (Engine_GameFlagIsSet(0x903) != 0)
	{
		Actor_SetPosition(22, 0xa20000, 0x27a0000);
		record = Object_GetById(22);
		record->facing = (v5 << 6);
		Actor_SetPosition(21, 0xa20000, 0x2a40000);
		record = Object_GetById(21);
		record->facing = 0xd000;
	}
	else
	{
		Engine_ActorSetPosition(22, 0xa00000, 0x28c0000);
		record = Object_GetById(22);
		record->facing = (v5 << 6);
		Actor_SetPosition(21, 0xa60000, 0x29c0000);
		record = Object_GetById(21);
		record->facing = 0xb000;
	}
	if (gGameState.entrance == 6)
	{
		TolbiShipDeck_RunIsaacTalk();
	}
	Engine_EventEnd();
}

static void TolbiShipDeck_SetupHiddenLeaderDeparture(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Camera_MoveTo(-1, -1, -1, 0);
	Engine_TaskWait(1);
	Actor_SetPosition(20, 0, 0);
	Actor_SetPosition(22, 0, 0);
	Actor_SetPosition(24, 0, 0);
	Actor_SetPosition(25, 0, 0);
	Actor_SetPosition(26, 0, 0);
	Actor_SetPosition(27, 0, 0);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
	Actor_SetPosition(23, 0, 0);
	record = Object_GetById(23);
	record->facing = 0x3000;
	Actor_SetPosition(21, 0xe80000, 0x28a0000);
	record = Object_GetById(21);
	record->facing = 0xb000;
	Camera_MoveTo(0xe80000, -1, 0x27c0000, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	TolbiShipDeck_AnnouncePreparationsReady(23, 21);
}

static void TolbiShipDeck_SetupVisibleLeaderDeparture(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x27c0000);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Engine_TaskWait(1);
	Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
	Engine_MapRedraw();
	Engine_TaskWait(1);
	Engine_ActorStop(22);
	Engine_ActorStop(21);
	Engine_TaskWait(1);
	Actor_SetPosition(22, 0, 0);
	Actor_SetPosition(21, 0, 0);
	Actor_SetPosition(20, 0, 0);
	record = Object_GetById(20);
	record->facing = 0x3000;
	Actor_SetPosition(23, 0xe80000, 0x28a0000);
	record = Object_GetById(23);
	record->facing = 0xb000;
	Engine_TaskWait(1);
	TolbiShipDeck_AnnouncePreparationsReady(20, 23);
}

static void TolbiShipDeck_AnnouncePreparationsReady(s32 a0, s32 a1)
{
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	TolbiShipDeck_OpenFirstDoor();
	Actor_SetPosition(a0, 0xd80000, 0x24c0000);
	Actor_SetSpeed(a0, 0xcccc, 0x6666);
	Actor_WalkToAndWait(a0, 216, 0x258);
	Actor_WalkToAndWait(a0, 218, 0x25c);
	Actor_WalkToAndWait(a0, 234, 0x25c);
	Actor_WalkToAndWait(a0, 236, 0x26a);
	Actor_FaceDirection(a0, 0x5000, 20);
	Engine_ActorSetAnimationAndWait(a0, 3);
	Engine_EventWait(20);
	FieldScene_CallPairWith10(a1, 0x5000);
	Engine_ActorJump(a1, 4, 40);
	Engine_ActorStartRepeatedMotion(a1, 2);
	Engine_EventSetMessage(MsgTolbiShipPreparationsReady);
	Event_ShowMessageAndWait(a1, 0, 20);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(10);
}

static void FieldScene_RunActorTwentyDialogueSequence(void)
{
	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Engine_ActorSetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 0);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ActorRunRepeatedMotion(20, 1);
	Engine_EventSetMessage(MsgTolbiShipAyeCaptainSeaMonsters);
	Engine_EventShowMessageAndWait(20, 0, 10);
	FieldScene_CallPairWith10(22, 0x5000);
	Engine_ActorJump(22, 4, 20);
	Engine_ActorStartRepeatedMotion(22, 2);
	Engine_EventShowMessageAndWait(0x6016, 0, 20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(11);
}

static void FieldScene_RunShipDeckEventScript(void)
{
	s32 v;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScriptB);
	Engine_TaskWait(1);
	Actor_SetPosition(21, 16252928, 47710208);
	Engine_ActorSetChildValue(0, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Actor_SetSpeed(21, 104857, 52428);
	Actor_WalkToAndWait(21, 242, 692);
	Actor_WalkToAndWait(21, 196, 678);
	Actor_WalkToAndWait(21, 182, 654);
	Engine_ActorRunRepeatedMotion(21, 2);
	Engine_EventSetMessage(MsgTolbiShipNoUseLate);
	FieldScene_RunStepThen10(40981);
	Actor_SetSpeed(0, 157286, 78643);
	FieldObjectMotion_ResetAndSetPosition(0, 154, 609);
	Engine_AudioPlayCue(146);
	v = 0;
	Object_GetById(24)->timer = v;
	Object_GetById(25)->timer = v;
	Object_GetById(26)->timer = v;
	Actor_SetPosition(24, 2097152, 31719424);
	Actor_SetPosition(25, 5505024, 32505856);
	Actor_SetPosition(26, 1048576, 39059456);
	Actor_SetSpeed(24, 157286, 78643);
	Actor_SetSpeed(25, 157286, 78643);
	Actor_SetSpeed(26, 157286, 78643);
	v = (s32)gTolbiShipDeck_DeckEventActions;
	Engine_ActorEnableActionCallback(24, v);
	Engine_ActorEnableActionCallback(25, v);
	Engine_ActorEnableActionCallback(26, v);
	Engine_ActorSetChildValue(24, 3);
	Engine_ActorSetChildValue(25, 3);
	Engine_ActorSetChildValue(26, 3);
	do
	{
		Engine_TaskWait(1);
	} while (Object_GetById(24)->timer == 0);
	TolbiShipDeck_OpenFirstDoor();
	/* FAKEMATCH: the do/while sets r0 = 21 first, straight after the call. Plain call: differ 2; Engine_ActorWalkTo directly: differ 4. Measured 2026-10-13: Actor_WalkTo as a do/while statement macro at every use changes 111 functions in the six editions, so the block is this site's alone. */
	do
	{
		Actor_WalkTo(21, 196, 612);
	} while (0);
	Object_RefreshSelectorById(24);
	Engine_EventWait(10);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(10);
	Graphics_EnableObjLayerAndCallbacks();
	Ui_SetRenderResultFromObject(21);
	UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
	ObjectDispatch_StopCallbacksAndHideLayers();
	Engine_EventRequestExit(12);
}

static void TolbiShipDeck_RunDeckCrewScene(void)
{

	Engine_EventBegin();
	Engine_ActorSetChildValue(0, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScriptC);
	Engine_TaskWait(1);
	Engine_ActorSetAnimation(31, 0);
	Engine_ActorSetSpriteFlags(Object_GetById(24), 1);
	Engine_ActorSetSpriteFlags(Object_GetById(25), 1);
	Engine_ActorSetSpriteFlags(Object_GetById(26), 1);
	Engine_ActorSetSpriteFlags(Object_GetById(27), 1);
	Engine_ActorSetSpriteFlags(Object_GetById(28), 1);
	Engine_ActorSetSpriteFlags(Object_GetById(29), 1);
	Actor_SetPosition(22, 0x1000000, 0x2800000);
	Engine_ActorEnableActionCallback(22, gTolbiShipDeck_CrewActionsD);
	Actor_SetPosition(21, 0x10c0000, 0x2b40000);
	Engine_ActorEnableActionCallback(22, gTolbiShipDeck_CrewActionsD);
	Actor_SetPosition(24, 0xf20000, 0x25c0000);
	Actor_SetPosition(25, 0x1080000, 0x2580000);
	Actor_SetPosition(26, 0xfe0000, 0x29c0000);
	Engine_ActorSetPosition(27, 0x11a0000, 0x2920000);
	Object_GetById(24)->riseEnabled = 0;
	Object_GetById(25)->riseEnabled = 1;
	Object_GetById(26)->riseEnabled = 0;
	Object_GetById(27)->riseEnabled = 1;
	Engine_ActorEnableActionCallback(24, gTolbiShipDeck_CrewActionsB);
	Engine_ActorEnableActionCallback(25, gTolbiShipDeck_CrewActionsB);
	Engine_ActorEnableActionCallback(26, gTolbiShipDeck_CrewActionsA);
	Engine_ActorEnableActionCallback(27, gTolbiShipDeck_CrewActionsA);
	Engine_ActorSetPosition(20, 0, 0);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(0x190);
	Actor_SetPosition(28, 0xfe0000, 0x2e40000);
	Actor_SetPosition(29, 0x180000, 0x24a0000);
	Actor_SetSpeed(28, 0x19999, 0xcccc);
	Actor_SetSpeed(29, 0x19999, 0xcccc);
	Actor_SetDestination(29, 172, 0x284);
	FieldObject_MotionSetPositionAndCommit(28, 200, 0x294);
	Actor_SetSpeed(0, 0x40000, 0x20000);
	Actor_SetDestination(0, 174, 0x26c);
	Engine_ObjectMotionSetPositionAndCommit(28, 180, 0x244);
	Engine_AudioPlayCue(146);
	Engine_ActorEnableActionCallback(28, gTolbiShipDeck_CrewActionsC);
	Engine_ActorEnableActionCallback(29, gTolbiShipDeck_CrewActionsC);
	Engine_AudioPlayCue(240);
	Actor_SetPosition(31, 0x860000, 0x2520000);
	Engine_ActorEnableActionCallback(31, gTolbiShipDeck_CrewActionsE);
	Engine_EventWait(10);
	Actor_SetPosition(30, 0x860000, 0x2480000);
	Actor_SetSpeed(30, 0x40000, 0x20000);
	Object_GetById(30)->velocityY = 0x80000;
	Engine_ObjectMotionSetPositionAndCommit(30, 186, 0x264);
	Engine_ActorSetSpriteFlags(Object_GetById(30), 1);
	Engine_EventWait(10);
	Actor_SetSpeed(30, 0x20000, 0x10000);
	Engine_ObjectMotionSetPositionAndCommit(30, 216, 0x258);
	FieldScene_CallPairWith10(30, 0xc000);
	TolbiShipDeck_OpenFirstDoor();
	Engine_EventWait(10);
	Engine_ActorEnableActionCallback(30, gTolbiShipDeck_SailorActions);
	Engine_EventWait(10);
	Engine_ActorEnableActionCallback(28, gTolbiShipDeck_SailorActions);
	Engine_EventWait(10);
	Object_SetActionCallbackAndRefreshById(29, gTolbiShipDeck_SailorActions);
	Engine_EventWait(20);
	Engine_AudioPlayCue(147);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_ActorStop(24);
	Engine_ActorStop(25);
	Engine_ActorStop(26);
	Engine_ActorStop(27);
	Engine_EventWait(10);
	Graphics_EnableObjLayerAndCallbacks();
	Ui_SetRenderResultFromObject(21);
	UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
	ObjectDispatch_StopCallbacksAndHideLayers();
	Engine_EventRequestExit(13);
}

static void FieldScene_RunActorSequence(void)
{
	struct FieldActor *rec8;
	struct FieldActor *record;
	s32				   base5_0;
	const u8		  *sailor_actions;

	Engine_EventBegin();
	Engine_ActorSetChildValue(0, 15);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScriptD);
	Engine_TaskWait(1);
	Engine_ActorSetPosition(22, 0xb00000, 0x2b80000);
	record = Object_GetById(22);
	record->facing = 0xd000;
	Engine_ActorSetPosition(21, 0x1080000, 0x2960000);
	record = Object_GetById(21);
	record->facing = 0xb000;
	Actor_SetPosition(24, 0xb80000, 0x2a00000);
	Actor_SetPosition(25, 0xca0000, 0x2b40000);
	Actor_SetPosition(26, 0xfc0000, 0x2860000);
	Actor_SetPosition(27, 0x1000000, 0x2ae0000);
	Actor_SetPosition(28, 0xac0000, 0x2780000);
	Engine_ActorSetPosition(29, 0x1000000, 0x26e0000);
	Object_GetById(24)->riseEnabled = 0;
	Object_GetById(25)->riseEnabled = 1;
	Object_GetById(26)->riseEnabled = 0;
	Object_GetById(27)->riseEnabled = 2;
	Engine_ActorSetPosition(20, 0, 0);
	Engine_ActorEnableActionCallback(24, gTolbiShipDeck_DeckActionsA);
	Engine_ActorEnableActionCallback(25, gTolbiShipDeck_DeckActionsA);
	Engine_ActorEnableActionCallback(26, gTolbiShipDeck_DeckActionsB);
	Engine_ActorEnableActionCallback(27, gTolbiShipDeck_DeckActionsB);
	Engine_ActorEnableActionCallback(28, gTolbiShipDeck_DeckActionsC);
	Engine_ActorEnableActionCallback(29, gTolbiShipDeck_DeckActionsC);
	Engine_ActorSetChildValue(24, 3);
	Engine_ActorSetChildValue(25, 3);
	Engine_ActorSetChildValue(26, 3);
	Engine_ActorSetChildValue(27, 3);
	Engine_ActorSetChildValue(28, 3);
	Engine_ActorSetChildValue(29, 3);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(80);
	Engine_AudioPlayCue(147);
	rec8 = Object_GetById(31);
	rec8->scaleX = 0x1999;
	rec8->scaleY = 0x1999;
	rec8->x.fixed = 0xc20000;
	rec8->z.fixed = 0x2820000;
	for (base5_0 = 0; (u32)base5_0 < 16; base5_0++)
	{
		rec8->scaleX += 0xf5c;
		rec8->scaleY += 0xf5c;
		Engine_TaskWait(1);
	}
	rec8 = Object_GetById(30);
	rec8->scaleX = 0x11999;
	rec8->scaleY = 0x11999;
	rec8->x.fixed = 0xc20000;
	rec8->y.fixed = 0x500000;
	rec8->z.fixed = 0x2820000;
	rec8->facing = 0x5000;
	rec8->bounce = 0x6666;
	rec8->gravity = 0x20000;
	Engine_EventWait(80);
	Engine_AudioPlayCue(147);
	Engine_ActorSetPosition(31, 0, 0);
	record = Object_GetById(30);
	Engine_ActorSetSpriteFlags(record, 1);
	Actor_SetSpeed(0, 0x19999, 0xcccc);
	rec8 = Object_GetById(0);
	rec8->motionFlags = 0;
	Actor_SetDestination(0, 216, 0x264);
	Actor_SetSpeed(30, 0x19999, 0xcccc);
	FieldObject_MotionSetPositionAndCommit(30, 196, 0x258);
	FieldObject_MotionSetPositionAndCommit(30, 216, 0x258);
	Engine_ActorStop(28);
	Engine_TaskWait(1);
	Actor_SetSpeed(28, 0x19999, 0xcccc);
	sailor_actions = gTolbiShipDeck_SailorActions;
	Engine_ActorEnableActionCallback(28, sailor_actions);
	FieldScene_CallPairWith10(30, 0xd000);
	TolbiShipDeck_OpenFirstDoor();
	Engine_EventWait(10);
	Engine_ActorEnableActionCallback(30, sailor_actions);
	Engine_ActorStop(29);
	Engine_TaskWait(1);
	Actor_SetSpeed(29, 0x19999, 0xcccc);
	Object_SetActionCallbackAndRefreshById(29, sailor_actions);
	Engine_EventWait(20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_ActorStop(24);
	Engine_ActorStop(25);
	Engine_ActorStop(26);
	Engine_ActorStop(27);
	Engine_ActorStop(28);
	Engine_ActorStop(29);
	Engine_EventWait(10);
	Graphics_EnableObjLayerAndCallbacks();
	Ui_SetRenderResultFromObject(21);
	UiText_ShowCenteredMessage(MsgTolbiShipArrgh, 1, 0);
	ObjectDispatch_StopCallbacksAndHideLayers();
	Engine_EventRequestExit(14);
}

static void TolbiShipDeck_RunJumpScene(void)
{
	struct FieldActor *actor;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((s32)gTolbiShipDeck_CrewScriptE);
	Engine_TaskWait(1);
	Engine_CameraFollowActor(25, 1);
	Engine_TaskWait(1);
	Engine_ActorSetAnimation(21, 5);
	Engine_ActorSetSpriteFlags(Object_GetById(21), 0);
	Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	actor->facing = FACING_SOUTH;
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	TolbiShipDeck_OpenFirstDoor();
	Engine_EventWait(10);
	Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(216), PIXELS(586));
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 598);
	Engine_EventWait(20);
	FieldScene_CallPairWith10(0, 0x6000);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 2, 10);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 194, 624);
	Engine_AudioPlayCue(181);
	Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
	Engine_EventWait(20);
	Work_SetValuesIfNonNegative(-1, -1, 0xe666);
	Engine_EventWait(10);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	Object_GetById(25)->motionFlags = 0;
	Actor_SetSpeed(25, 0x20000, 0x10000);
	Engine_ActorSetDestination(25, 216, 612);
	Engine_AudioPlayCue(149);
	Engine_ActorSetSpritePriority(22, 2);
	Engine_ActorSetAnimation(22, 5);
	actor = Object_GetById(22);
	actor->velocityY = 0x80000;
	actor->gravity = 0xb333;
	actor->scaleX = 0x1a000;
	actor->scaleY = 0x1a000;
	actor->update = (void (*)(union FieldObject *))OverlayObject_DecayFields24And28;
	actor->bounce = 0x8000;
	Actor_SetSpeed(22, 0x60000, 0x30000);
	Engine_ObjectMotionSetPositionAndCommit(22, 182, 0x26a);
	Engine_ActorSetSpriteFlags(Object_GetById(22), 0);
	FieldScene_CallPairWith10(0, 0xa000);
	Engine_ActorJump(ACTOR_PARTY_LEADER, 6, 80);
	Actor_SetDestination(25, 232, 564);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 204, 610);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 208, 598);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 248, 598);
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	if (actor != 0)
	{
		Engine_ActorSetPosition(GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	if (actor != 0)
	{
		Engine_ActorSetPosition(IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = Object_GetById(ACTOR_PARTY_LEADER);
	if (actor != 0)
	{
		Engine_ActorSetPosition(MIA, actor->x.fixed, actor->z.fixed);
	}
	Actor_SetSpeed(GARET, 0x10000, 0x8000);
	Actor_SetSpeed(IVAN, 0x10000, 0x8000);
	Actor_SetSpeed(MIA, 0x10000, 0x8000);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 250, 584);
	Actor_WalkTo(GARET, 240, 600);
	Actor_WalkTo(IVAN, 254, 600);
	Engine_ActorWalkToAndWait(MIA, 248, 616);
	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
	Engine_ActorSetAnimation(GARET, 1);
	Engine_ActorSetAnimation(IVAN, 1);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	Actor_FaceDirection(GARET, FACING_NORTH, 0);
	Actor_FaceDirection(IVAN, FACING_NORTH, 0);
	Engine_ActorFaceDirection(MIA, FACING_NORTH, 20);
	Engine_AudioPlayCue(149);
	Engine_EventWait(40);
	Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
	Actor_SetAttachedEffect(GARET, 0x102);
	Actor_SetAttachedEffect(IVAN, 0x102);
	Engine_ActorSetAttachedEffect(MIA, 0x102);
	Engine_EventWait(60);
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
	Actor_SetSpeed(GARET, 0xcccc, 0x6666);
	Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
	Actor_SetSpeed(MIA, 0xcccc, 0x6666);
	Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 564);
	Actor_WalkTo(GARET, 248, 564);
	Engine_EventWait(20);
	Actor_WalkTo(IVAN, 248, 564);
	Actor_WalkTo(MIA, 248, 564);
	Engine_EventWait(20);
	gGameState.savedScene = (s32)&SceneId_TolbiShipInteriors;
	gGameState.savedEntrance = 30;
	Battle_SetStart(3);
	Party_SetFields1ceAnd1d0((s32)&SceneId_TolbiShipDeck, 16);
	BattleFx_SetWeightedResult(62, 3);
	Engine_EventEnd();
}

static void FieldScene_ConfigureLeadActors(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Actor_SetPosition(20, 0xb60000, 0x26a0000);
	Actor_SetPosition(23, 0xee0000, 0x2720000);
	Actor_SetPosition(22, 0x10c0000, 0x2a60000);
	record = Object_GetById(22);
	record->facing = 0;
	Engine_ActorEnableActionCallback(22, gTolbiShipDeck_LeadActionsC);
	{
		struct FieldActor *record = Object_GetById(21);
		record->collisionFlags = 128 | record->collisionFlags;
	}
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Engine_ActorEnableActionCallback(21, gTolbiShipDeck_LeadActionsB);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Actor_SetSpeed(20, 0x19999, 0xcccc);
	Actor_WalkToAndWait(20, 182, 0x224);
	FieldScene_CallPairWith10(20, 0);
	FieldScene_CallPairWith10(0, 0x8000);
	Engine_ActorRunRepeatedMotion(20, 1);
	Engine_EventSetMessage(MsgTolbiShipThankIsaacDidGoodAgainst);
	FieldScene_RunStepThen10(20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_EventWait(40);
	Actor_FaceDirection(20, 0x5000, 20);
	Actor_ShowEmote(20, 0x105, 60);
	Event_ShowMessageAndWait(20, 0, 40);
	FieldScene_CallPairWith10(20, 0);
	FieldScene_RunStepThen10(20);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(20, 3);
	Actor_WalkToAndWait(20, 182, 0x258);
	Actor_WalkToAndWait(20, 216, 0x258);
	FieldScene_CallPairWith10(20, 0xc000);
	TolbiShipDeck_OpenFirstDoor();
	Engine_EventWait(10);
	Actor_WalkToAndWait(20, 216, 0x244);
	Actor_SetPosition(20, 0, 0);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	GameFlag_Set(0x92b);
	GameFlag_Clear(0x302);
	Engine_EventEnd();
}

static void FieldScene_ConfigureThreeActors(void)
{
	struct FieldActor *record;

	Engine_EventBegin();
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	record = Object_GetById(0);
	Engine_ActorSetSpriteFlags(record, 0);
	Event_CallWithLastActiveObjectId((u32)gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Actor_SetPosition(20, 0xc40000, 0x1f60000);
	record = Object_GetById(20);
	record->facing = 0xa000;
	Actor_SetPosition(22, 0xb80000, 0x20c0000);
	record = Object_GetById(22);
	record->facing = 0xb000;
	Engine_ActorSetSpritePriority(21, 1);
	Actor_SetPosition(21, 0xb80000, 0x2780000);
	record = Object_GetById(21);
	record->facing = 0xb000;
	/* FAKEMATCH: needed to match: the work pointer is read as a word, which may alias the facing stores, so they stay ahead of its load. gEventWork->startTransition: differ 2; the records held as union FieldObject: differ 2. Measured 2026-10-13: with gEventWork declared a word (u32, cast at each use) the plain store matches and, in all six editions, only ValeHome_RunActorPairEffectScene changes (differ 2); this word view on every startTransition store changes 49 functions. */
	((struct EventWork *)*(u32 *)&gEventWork)->startTransition =
		SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_ActorJump(22, 4, 10);
	Engine_ActorJump(22, 6, 20);
	Engine_EventSetMessage(MsgTolbiShipCanSeeLand);
	FieldScene_RunStepThen10(22);
	Engine_ActorSetAnimationAndWait(20, 3);
	Actor_SetSpeed(21, 0x30000, 0x18000);
	Actor_WalkToAndWait(21, 180, 0x222);
	Engine_ActorFaceDirection(21, 0xb000, 40);
	Engine_ActorRunRepeatedMotion(21, 1);
	FieldScene_RunStepThen10(21);
	Engine_EventRequestExit(15);
}

static void FieldScene_RunPartyRosterScene(void)
{
	s32				   roster[4];
	s32				   matched[4];
	s32				   category[4];
	u32				   index;
	s32				   found;
	s32				   actor;
	s32				   member;
	struct FieldActor *record;

	found = 0;
	for (index = 0; index <= 3; index++)
	{
		roster[index] = SceneState_FindFirstSetFlagOfGroup(index);
		category[index] = 0;
	}

	member = 23;
	for (index = 0; index <= 3; index++)
	{
		if (roster[index] == member)
		{
			matched[found] = member;
			category[found] = found;
			found++;
			break;
		}
	}
	member = 24;
	for (index = 0; index <= 3; index++)
	{
		if (roster[index] == member)
		{
			matched[found] = member;
			category[found] = found;
			found++;
			break;
		}
	}
	member = 25;
	for (index = 0; index <= 3; index++)
	{
		if (roster[index] == member)
		{
			matched[found] = member;
			category[found] = found;
			found++;
			break;
		}
	}
	member = 27;
	for (index = 0; index <= 3; index++)
	{
		if (roster[index] == member)
		{
			matched[found] = member;
			category[found] = found;
			found++;
			break;
		}
	}
	if (found != 4)
	{
		member = 28;
		for (index = 0; index <= 3; index++)
		{
			if (roster[index] == member)
			{
				matched[found] = member;
				category[found] = found;
				found++;
				break;
			}
		}
		if (found != 4)
		{
			member = 29;
			for (index = 0; index <= 3; index++)
			{
				if (roster[index] == member)
				{
					matched[found] = member;
					category[found] = found;
					found++;
					break;
				}
			}
			if (found != 4)
			{
				member = 26;
				for (index = 0; index <= 3; index++)
				{
					if (roster[index] == member)
					{
						matched[found] = member;
						category[found] = 10;
						found++;
						break;
					}
				}
				if (found != 4)
				{
					member = 30;
					for (index = 0; index <= 3; index++)
					{
						if (roster[index] == member)
						{
							matched[found] = member;
							category[found] = 11;
							found++;
							break;
						}
					}
					if (found != 4)
					{
						member = 31;
						for (index = 0; index <= 3; index++)
						{
							if (roster[index] == member)
							{
								matched[found] = member;
								category[found] = 20;
								break;
							}
						}
					}
				}
			}
		}
	}

	Engine_EventBegin();
	Engine_ActorSetChildValue(0, 15);
	Engine_ActorSetSpriteFlags(Object_GetById(0), 0);
	record = Object_GetById(0);
	if (record != 0)
	{
		Engine_ActorSetPosition(32, record->x.fixed, record->z.fixed);
	}
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Engine_EventWait(20);
	Engine_CameraFollowActor(32, 1);

	TolbiShipDeck_OpenFirstDoor();

	Engine_EventWait(10);

	for (index = 0; index <= 3; index++)
	{
		actor = matched[index];
		Actor_SetPosition(actor, 216 << 16, 146 << 18);
		if (category[index] == 20)
		{
			Actor_SetSpeed(actor, 0xcccc, 0x6666);
		}
		else
		{
			Actor_SetSpeed(actor, 128 << 9, 128 << 8);
		}
		Actor_WalkToAndWait(actor, 216, 150 << 2);
		Actor_WalkToAndWait(actor, 192, 0x26a);
		Engine_ActorWalkToAndWait(actor, 192, 164 << 2);

		switch (category[index])
		{
		case 0:
			Engine_ActorShowEmote(actor, 129 << 1, 60);
			Engine_EventSetMessage(MsgTolbiShipMadeFinallyLucky);
			break;
		case 1:
			FieldScene_CallPairWith10(actor, 208 << 8);
			Engine_ActorShowEmote(actor, 129 << 1, 60);
			Engine_EventSetMessage(MsgTolbiShipOtherPassengersAlready);
			break;
		case 2:
			Engine_ActorShowEmote(actor, 0x105, 60);
			Engine_EventSetMessage(MsgTolbiShipDontRowAnymore);
			break;
		case 3:
			Engine_ActorRunRepeatedMotion(actor, 1);
			Engine_EventSetMessage(MsgTolbiShipOhhhHaventWorkout);
			break;
		case 10:
			Engine_ActorSetAnimationAndWait(actor, 3);
			Engine_EventSetMessage(MsgTolbiShipRowingShipMore);
			break;
		case 11:
			Engine_ActorSetAnimation(actor, 4);
			Engine_EventSetMessage(MsgTolbiShipTotallyLostOcean);
			break;
		case 20:
			Engine_ActorSetAnimationAndWait(actor, 4);
			Engine_ActorShowEmote(actor, 0x107, 40);
			Engine_EventSetMessage(MsgTolbiShipMakeOldMan);
			break;
		default:
			break;
		}

		FieldScene_RunStepThen10(actor);
		Engine_ActorEnableActionCallback(actor, gTolbiShipDeck_RosterActions);
	}

	Object_RefreshSelectorById(actor);
	Engine_EventWait(40);
	Engine_ActorSetPosition(0, 216 << 16, 146 << 18);
	Engine_TaskWait(1);
	Engine_ActorSetChildValue(0, 0);
	Engine_ActorSetSpriteFlags(Object_GetById(0), 1);
	Actor_SetSpeed(0, 0xcccc, 0x6666);
	Actor_WalkToAndWait(0, 216, 150 << 2);
	Actor_WalkToAndWait(0, 190, 153 << 2);
	Engine_EventSetMessage(MsgTolbiShipThanksHardWork);
	FieldScene_RunStepThen10(20);
	Engine_ActorFaceDirection(0, 192 << 8, 0);

	Object_GetById(32)->motionFlags = 0;
	Actor_SetSpeed(32, 128 << 10, 128 << 9);
	Actor_SetDestination(32, 196, 141 << 2);

	Actor_SetSpeed(20, 0xcccc, 0x6666);
	Actor_WalkToAndWait(20, 182, 0x22b);
	Actor_FaceDirection(20, 192 << 6, 20);
	Engine_ActorRunRepeatedMotion(20, 1);
	FieldScene_RunStepThen10(20);
	Engine_ActorSetAnimationAndWait(20, 3);
	FieldScene_RunStepThen10(20);
	Engine_ActorFaceDirection(20, 128 << 8, 40);
	FieldScene_RunStepThen10(20);
	FieldScene_CallPairWith10(20, 192 << 6);
	FieldScene_RunStepThen10(20);
	Engine_ActorSetAnimationAndWait(20, 3);
	Engine_CameraFollowActor(0, 1);
	Actor_WalkToAndWait(20, 188, 128 << 2);
	Engine_ActorFaceDirection(20, 192 << 6, 0);
	Engine_ActorSetPosition(32, 0, 0);

	Engine_GameFlagClear(0x12f);
	Engine_EventEnd();
}

static s32 SceneState_FindFirstSetFlagOfGroup(u32 selection)
{
	s32 v = 0;
	s32 id = 23;
	u32 i;

	switch (selection)
	{
	case 0:
		v = 0x92C;
		break;
	case 1:
		v = 0x935;
		break;
	case 2:
		v = 0x917;
		break;
	case 3:
		v = 0x990;
		break;
	}
	for (i = 0; i < 9; i++)
	{
		if (Engine_GameFlagIsSet(v) != 0)
			return id;
		v++;
		id++;
	}
	return 0;
}

static void TolbiShipDeck_RockDeck(void)
{
	struct MapWork	*map = gMapWork[0];
	s32				*camera = map->camera;
	s32				 dx = Engine_MathCos(sWaveAngleX);
	s32				 dy = Engine_MathSin(sWaveAngleY);
	struct MapLayer *layer;

	*camera++ += dx >> 1;
	*camera += dy;
	sWaveAngleX += (u32)(Engine_RandomNext() * 3 << 7) >> 16;
	sWaveAngleY += (u32)(Engine_RandomNext() << 9) >> 16;
	sWaveAngleX &= 0xffff;
	sWaveAngleY &= 0xffff;
	layer = &map->layers[5];
	layer->x = sLayerScroll[0];
	sLayerScroll[0] -= sLayerSpeed[0];
	if (sLayerScroll[0] < 0)
	{
		sLayerScroll[0] += 0x200000;
	}
	if (sLayerScroll[0] > 0x200000)
	{
		sLayerScroll[0] -= 0x200000;
	}
	layer->y = sLayerScroll[1];
	sLayerScroll[1] -= sLayerSpeed[1];
	if (sLayerScroll[1] < 0)
	{
		sLayerScroll[1] += 0x200000;
	}
}

s32 gTolbiShipDeck_SlotDepth[8];

static void SceneEffect_InitSlotsEightToNineteen(void)
{
	Object_GetById(8)->collisionFlags = 0;
	Object_GetById(9)->collisionFlags = 0;
	Object_GetById(10)->collisionFlags = 0;
	Object_GetById(11)->collisionFlags = 0;
	OverlayObject_ActivateSlotWithMode3(8);
	OverlayObject_ActivateSlotWithMode3(9);
	OverlayObject_ActivateSlotWithMode3(10);
	OverlayObject_ActivateSlotWithMode3(11);
	OverlayObject_ActivateSlotWithMode3(12);
	OverlayObject_ActivateSlotWithMode3(13);
	OverlayObject_ActivateSlotWithMode3(14);
	OverlayObject_ActivateSlotWithMode3(15);
	gTolbiShipDeck_SlotDepth[0] = Object_GetById(12)->z.fixed;
	gTolbiShipDeck_SlotDepth[1] = Object_GetById(13)->z.fixed;
	gTolbiShipDeck_SlotDepth[2] = Object_GetById(14)->z.fixed;
	gTolbiShipDeck_SlotDepth[3] = Object_GetById(15)->z.fixed;
	OverlayObject_ActivateSlotWithMode3(16);
	OverlayObject_ActivateSlotWithMode3(17);
	OverlayObject_ActivateSlotWithMode3(18);
	OverlayObject_ActivateSlotWithMode3(19);
	Object_GetById(16)->scaleX = 0xffff0000;
	Object_GetById(17)->scaleX = 0xffff0000;
	Object_GetById(18)->scaleX = 0xffff0000;
	Object_GetById(19)->scaleX = 0xffff0000;
	gTolbiShipDeck_SlotDepth[4] = Object_GetById(16)->z.fixed;
	gTolbiShipDeck_SlotDepth[5] = Object_GetById(17)->z.fixed;
	gTolbiShipDeck_SlotDepth[6] = Object_GetById(18)->z.fixed;
	gTolbiShipDeck_SlotDepth[7] = Object_GetById(19)->z.fixed;
	TolbiShipDeck_ChooseSlotModes();
}

static void OverlayObject_ActivateSlotWithMode3(s32 a)
{
	struct FieldActor *p = Object_GetById(a);

	if (p != 0)
	{
		Engine_ActorSetSpritePriority(a, 3);
		Engine_ActorSetSpriteFlags(p, 0);
		p->collisionFlags = 0;
		{
			s32 c;
			c = 2 | p->priorityFlags;
			p->priorityFlags = c;
		}
	}
}

u32 gTolbiShipDeck_SlotMode[8];

struct SlotSprite
{
	s32 unk00[7];
	s16 unk1c;
	s16 tilt;
};

static void TolbiShipDeck_ChooseSlotModes(void)
{
	u32 *mode = gTolbiShipDeck_SlotMode;
	u32	 i;

	for (i = 0; i <= 3; i++)
	{
		if (SceneState_FindFirstSetFlagOfGroup(i) != 0)
			*mode = SceneData_SelectTableEntryByFlagGroup(i);
		else
			*mode = 3;
		mode++;
	}
	if (SceneState_FindFirstSetFlagOfGroup(0) != 0)
		gTolbiShipDeck_SlotMode[0] = SceneData_SelectTableEntryByFlagGroup(0);
	else
		gTolbiShipDeck_SlotMode[0] = 3;
	if (SceneState_FindFirstSetFlagOfGroup(2) != 0)
		gTolbiShipDeck_SlotMode[1] = SceneData_SelectTableEntryByFlagGroup(2);
	else
		gTolbiShipDeck_SlotMode[1] = 3;
	gTolbiShipDeck_SlotMode[2] = 3;
	gTolbiShipDeck_SlotMode[3] = 3;
	if (SceneState_FindFirstSetFlagOfGroup(1) != 0)
		gTolbiShipDeck_SlotMode[4] = SceneData_SelectTableEntryByFlagGroup(1);
	else
		gTolbiShipDeck_SlotMode[4] = 3;
	if (SceneState_FindFirstSetFlagOfGroup(3) != 0)
		gTolbiShipDeck_SlotMode[5] = SceneData_SelectTableEntryByFlagGroup(3);
	else
		gTolbiShipDeck_SlotMode[5] = 3;
	gTolbiShipDeck_SlotMode[6] = 3;
	gTolbiShipDeck_SlotMode[7] = 3;
}

static s32 SceneData_SelectTableEntryByFlagGroup(u32 selection)
{
	s32 base = 0;
	u32 i;

	switch (selection)
	{
	case 0:
		base = 0x92C;
		break;
	case 1:
		base = 0x935;
		break;
	case 2:
		base = 0x917;
		break;
	case 3:
		base = 0x990;
		break;
	}
	for (i = 0; i < 9; i++)
	{
		if (Engine_GameFlagIsSet(base + i) != 0)
			return gTolbiShipDeck_FlagGroupEntries[i];
	}
	return 0;
}

static void SceneState_ConfigureEntries8Through19(void)
{
	SceneEffect_AdvanceSlotByValueBand(8, 0);
	sSlotPhase[0] = 0;
	SceneEffect_AdvanceSlotByValueBand(9, 1);
	sSlotPhase[1] += 0x80;
	SceneEffect_AdvanceSlotByValueBand(10, 2);
	sSlotPhase[2] += 0x100;
	SceneEffect_AdvanceSlotByValueBand(11, 3);
	sSlotPhase[3] += 0x200;
	SceneEffect_SelectSlotValueAndPosition(12, 0, 0);
	SceneEffect_SelectSlotValueAndPosition(13, 1, 0);
	SceneEffect_SelectSlotValueAndPosition(14, 2, 0);
	SceneEffect_SelectSlotValueAndPosition(15, 3, 0);
	SceneEffect_SelectSlotValueAndPosition(16, 4, 1);
	SceneEffect_SelectSlotValueAndPosition(17, 5, 1);
	SceneEffect_SelectSlotValueAndPosition(18, 6, 1);
	SceneEffect_SelectSlotValueAndPosition(19, 7, 1);
}

static void SceneEffect_AdvanceSlotByValueBand(s32 actor, s32 slot)
{
	u16 value = sSlotValue[slot];

	if (value >= 0x6801 && value <= 0x6fff)
	{
		sSlotPhase[slot] += 0x70;
		Engine_ActorSetAnimation(actor, 3);
	}
	else if (value >= 0xe801 && value <= 0xefff)
	{
		sSlotPhase[slot] += 0xe0;
		Engine_ActorSetAnimation(actor, 3);
	}
	else if (value >= 0x7001 && value <= 0xefff)
	{
		sSlotPhase[slot] += 0x1c0;
		Engine_ActorSetAnimation(actor, 2);
	}
	else
	{
		sSlotPhase[slot] += 0x300;
		Engine_ActorSetAnimation(actor, 1);
	}
}

static void SceneEffect_SelectSlotValueAndPosition(s32 id, s32 slot, s32 flags)
{
	struct FieldActor *actor = Object_GetById(id);
	struct SlotSprite *sprite = (struct SlotSprite *)actor->sprite;
	s32				   sine;

	if ((flags & 2) == 0)
	{
		switch (gTolbiShipDeck_SlotMode[slot])
		{
		case 1:
			sSlotValue[slot] = sSlotPhase[0];
			ObjectVisual_CopyAttributes(id, 8);
			break;
		case 2:
			sSlotValue[slot] = sSlotPhase[1];
			ObjectVisual_CopyAttributes(id, 9);
			break;
		case 3:
			sSlotValue[slot] = sSlotPhase[2];
			ObjectVisual_CopyAttributes(id, 10);
			break;
		case 4:
			sSlotValue[slot] = sSlotPhase[3];
			ObjectVisual_CopyAttributes(id, 11);
			break;
		}
	}
	if (flags & 1)
	{
		sine = Engine_MathSin(sSlotValue[slot]);
		sprite->tilt = Engine_MathSin(sSlotValue[slot] + 0x8000) >> 5;
		actor->z.fixed = gTolbiShipDeck_SlotDepth[slot] - (sine << 2) - (sine << 1);
	}
	else
	{
		sine = Engine_MathSin(sSlotValue[slot] + 0x8000);
		sprite->tilt = Engine_MathSin(sSlotValue[slot]) >> 5;
		actor->z.fixed = gTolbiShipDeck_SlotDepth[slot] + (sine << 2) + (sine << 1);
	}
}

static void SceneState_InitActorSlots8To19(void)
{
	struct FieldActor *actor;
	u32				   i;

	for (i = 0; i <= 7; i++)
		sSlotValue[i] = 0xc000;
	OverlayObject_ActivateSlotWithMode3(8);
	Engine_ActorSetPosition(9, 0, 0);
	Engine_ActorSetPosition(10, 0, 0);
	Engine_ActorSetPosition(11, 0, 0);
	Engine_ActorSetPosition(12, 0, 0);
	OverlayObject_ActivateSlotWithMode3(13);
	OverlayObject_ActivateSlotWithMode3(14);
	OverlayObject_ActivateSlotWithMode3(15);
	gTolbiShipDeck_SlotMode[0] = 0;
	gTolbiShipDeck_SlotMode[1] = 0;
	gTolbiShipDeck_SlotMode[2] = 0;
	gTolbiShipDeck_SlotMode[3] = 0;
	gTolbiShipDeck_SlotDepth[0] = Object_GetById(8)->z.fixed;
	gTolbiShipDeck_SlotDepth[1] = Object_GetById(13)->z.fixed;
	gTolbiShipDeck_SlotDepth[2] = Object_GetById(14)->z.fixed;
	gTolbiShipDeck_SlotDepth[3] = Object_GetById(15)->z.fixed;
	OverlayObject_ActivateSlotWithMode3(16);
	OverlayObject_ActivateSlotWithMode3(17);
	OverlayObject_ActivateSlotWithMode3(18);
	OverlayObject_ActivateSlotWithMode3(19);
	Object_GetById(16)->scaleX = 0xffff0000;
	Object_GetById(17)->scaleX = 0xffff0000;
	Object_GetById(18)->scaleX = 0xffff0000;
	Object_GetById(19)->scaleX = 0xffff0000;
	gTolbiShipDeck_SlotMode[4] = 0;
	gTolbiShipDeck_SlotMode[5] = 0;
	gTolbiShipDeck_SlotMode[6] = 0;
	gTolbiShipDeck_SlotMode[7] = 0;
	gTolbiShipDeck_SlotDepth[4] = Object_GetById(16)->z.fixed;
	gTolbiShipDeck_SlotDepth[5] = Object_GetById(17)->z.fixed;
	gTolbiShipDeck_SlotDepth[6] = Object_GetById(18)->z.fixed;
	gTolbiShipDeck_SlotDepth[7] = Object_GetById(19)->z.fixed;
	actor = Object_GetById(0);
	if (actor != NULL)
		Engine_ActorSetPosition(8, actor->x.fixed, actor->z.fixed);
	Engine_TaskWait(1);
	ObjectVisual_CopyAttributes(13, 8);
	ObjectVisual_CopyAttributes(14, 8);
	ObjectVisual_CopyAttributes(15, 8);
	ObjectVisual_CopyAttributes(16, 8);
	ObjectVisual_CopyAttributes(17, 8);
	ObjectVisual_CopyAttributes(18, 8);
	ObjectVisual_CopyAttributes(19, 8);
	Object_GetById(8)->unk5c = 1;
	Object_GetById(13)->unk5c = 1;
	Object_GetById(14)->unk5c = 1;
	Object_GetById(15)->unk5c = 1;
	Object_GetById(16)->unk5c = 1;
	Object_GetById(17)->unk5c = 1;
	Object_GetById(18)->unk5c = 1;
	Object_GetById(19)->unk5c = 1;
	Engine_TaskWait(1);
	Engine_ActorSetPosition(8, 0x840000, 0x2780000);
	Engine_TaskWait(1);
	SceneEffect_SelectSlotValueAndPosition(8, 0, 2);
	SceneEffect_SelectSlotValueAndPosition(13, 1, 2);
	SceneEffect_SelectSlotValueAndPosition(14, 2, 2);
	SceneEffect_SelectSlotValueAndPosition(15, 3, 2);
	SceneEffect_SelectSlotValueAndPosition(16, 4, 3);
	SceneEffect_SelectSlotValueAndPosition(17, 5, 3);
	SceneEffect_SelectSlotValueAndPosition(18, 6, 3);
	SceneEffect_SelectSlotValueAndPosition(19, 7, 3);
}

static void SceneState_ApplyFiveRectsAtColumn78(void)
{
	Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
	Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
	Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
	Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
	{

		Map_CopyCellAttributes(17, 38, 5, 2, 17, 40);
	}
}

static void DialogueLayout_ConfigureTwoRegions(void)
{
	Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
	Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

static void FieldScene_RunStepThen10(s32 a)
{
	Engine_EventShowMessage(a, 0);
	Engine_EventWait(10);
}

static void FieldScene_CallPairWith10(s32 a, s32 b)
{
	Engine_ActorFaceDirection(a, b, 10);
}

static void TolbiShipDeck_ArriveAtTolbi(void)
{
	Engine_EventBegin();
	sLayerScroll[0] = 0x40000;
	sLayerSpeed[0] = -0x8000;
	Event_CallWithLastActiveObjectId(gTolbiShipDeck_CrewScript);
	Engine_TaskWait(1);
	Actor_SetPosition(21, 0xb60000, 0x26a0000);
	Actor_Get(21)->facing = 0xc000;
	Actor_SetPosition(20, 0xda0000, 0x2040000);
	Actor_Get(20)->facing = 0xb000;
	Actor_SetPosition(22, 0xcc0000, 0x20e0000);
	Actor_Get(22)->facing = 0xb000;
	Actor_SetPosition(23, 0, 0);
	Actor_SetChildValue(ACTOR_PARTY_LEADER, 15);
	Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_PARTY_LEADER), 0);
	Engine_TaskWait(1);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	Engine_EventOpenScreen();
	Engine_EventWaitForScreen();
	Actor_SetSpeed(21, 0xcccc, 0x6666);
	Actor_WalkToAndWait(21, 182, 0x214);
	Actor_FaceDirection(21, 0xb000, 40);
	Engine_EventSetMessage(MsgTolbiShipFinallyReachedTolbi);
	FieldScene_RunStepThen10(21);
	Engine_ActorRunRepeatedMotion(20, 2);
	Engine_ActorSetAnimation(20, 4);
	FieldScene_RunStepThen10(0x6014);
	Actor_FaceDirection(21, 0xd000, 0);
	Actor_FaceDirection(22, 0xd000, 0);
	Actor_ShowEmote(21, 0x101, 0);
	Actor_ShowEmote(22, 0x101, 60);
	Engine_ActorSetAnimation(20, 3);
	FieldScene_RunStepThen10(0x6014);
	Actor_SetAttachedEffect(21, 0x102);
	Actor_SetAttachedEffect(22, 0x102);
	Engine_EventWait(80);
	Actor_ShowEmote(21, 0x100, 20);
	Actor_SetSpeed(21, 0x19999, 0xcccc);
	Actor_WalkToAndWait(21, 194, 0x1f4);
	Actor_FaceDirection(21, 0xb000, 20);
	Actor_SetSpeed(22, 0xcccc, 0x6666);
	Actor_WalkToAndWait(22, 192, 0x206);
	Actor_FaceDirection(22, 0xb000, 0);
	Actor_SetSpeed(20, 0x10000, 0x8000);
	Actor_WalkToAndWait(20, 210, 0x1fc);
	FieldScene_CallPairWith10(20, 0xb000);
	Engine_ActorRunRepeatedMotion(21, 1);
	FieldScene_RunStepThen10(0x5015);
	Engine_ActorSetAnimationAndWait(20, 3);
	FieldScene_CallPairWith10(22, 0xd000);
	FieldScene_RunStepThen10(0x9016);
	Actor_FaceDirection(20, 0x8000, 20);
	Engine_ActorSetAnimation(20, 4);
	FieldScene_RunStepThen10(0xa014);
	Actor_WalkToAndWait(20, 204, 0x218);
	Actor_FaceDirection(22, 0xb000, 0);
	Actor_WalkToAndWait(20, 182, 0x224);
	Actor_WalkToAndWait(20, 182, 0x250);
	Actor_WalkTo(20, 182, 0x298);
	Engine_EventWait(40);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(16);
}

static void FieldScene_RunThreeActorEncounter(void)
{
	struct FieldActor *leader;
	u8				   bits;

	Engine_EventBegin();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 180, 0x28e);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != 0)
	{
		Actor_SetPosition(GARET, leader->x.fixed, leader->z.fixed);
	}
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != 0)
	{
		Actor_SetPosition(IVAN, leader->x.fixed, leader->z.fixed);
	}
	leader = Object_GetById(ACTOR_PARTY_LEADER);
	if (leader != 0)
	{
		Actor_SetPosition(MIA, leader->x.fixed, leader->z.fixed);
	}
	Actor_SetSpeed(GARET, 0x13333, 0x9999);
	Actor_SetSpeed(IVAN, 0x10000, 0x8000);
	Actor_SetSpeed(MIA, 0x13333, 0x9999);
	Actor_WalkTo(GARET, 194, 0x280);
	Actor_WalkTo(IVAN, 198, 0x28e);
	Actor_WalkToAndWait(MIA, 194, 0x2a0);
	Engine_ActorSetAnimation(GARET, 1);
	Engine_ActorSetAnimation(IVAN, 1);
	Engine_EventWait(10);
	Actor_FaceDirection(GARET, 0x8000, 0);
	Actor_FaceDirection(IVAN, 0x8000, 0);
	FieldScene_CallPairWith10(3, 0x8000);
	FieldScene_CallPairWith10(22, 0);
	Engine_EventSetMessage(MsgTolbiShipIsaacTalkedPassengersDidntTour);
	FieldScene_RunStepThen10(22);
	FieldScene_CallPairWith10(21, 0xd000);
	Event_ShowMessageAndWait(21, 0, 40);
	Actor_ShowEmote(22, 0x100, 20);
	Engine_ActorRunRepeatedMotion(22, 1);
	Event_OpenMessage(22, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		Engine_ActorSetAnimationAndWait(IVAN, 4);
		FieldScene_RunStepThen10(2);
		FieldScene_CallPairWith10(3, 0xa000);
		Engine_ActorSetAnimation(MIA, 3);
		FieldScene_RunStepThen10(3);
		FieldScene_CallPairWith10(1, 0x6000);
		Engine_ActorRunRepeatedMotion(GARET, 1);
		Event_OpenMessage(GARET, 0);
		for (;;)
		{
			if (Engine_EventChooseYesNo(0, 0) == 1)
			{
				Engine_ActorRunRepeatedMotion(IVAN, 1);
				Engine_EventSetMessage(MsgTolbiShipIsaacDontTalkLikeShouldnt);
				Event_OpenMessage(IVAN, 0);
			}
			else
				break;
		}
	}
	Engine_EventWait(20);
	Engine_ActorSetAnimationAndWait(22, 3);
	Engine_EventSetMessage(MsgTolbiShipSeeYoureGoingGoFor);
	FieldScene_RunStepThen10(22);
	Actor_SetSpeed(22, 0x10000, 0x8000);
	Actor_SetSpeed(21, 0x10000, 0x8000);
	Object_GetById(22)->steeringFlags &= 254;
	Actor_WalkToAndWait(22, 162, 0x27a);
	Engine_EventWait(1);
	bits = ACTOR_FACES_MOTION;
	{
		struct FieldActor *actor = Object_GetById(22);
		u8				   value = actor->steeringFlags;

		actor->steeringFlags = value | bits;
	}
	Object_GetById(21)->steeringFlags &= 254;
	Actor_WalkToAndWait(21, 162, 0x2a4);
	Engine_EventWait(1);
	{
		struct FieldActor *actor = Object_GetById(21);

		bits |= actor->steeringFlags;
		actor->steeringFlags = bits;
	}
	Actor_FaceDirection(22, 0x3000, 0);
	FieldScene_CallPairWith10(21, 0xd000);
	FieldScene_RunStepThen10(22);
	Actor_WalkTo(GARET, 180, 0x28e);
	Actor_WalkTo(IVAN, 180, 0x28e);
	Actor_WalkToAndWait(MIA, 180, 0x28e);
	Engine_ActorDestroy(GARET);
	Engine_ActorDestroy(IVAN);
	Engine_ActorDestroy(MIA);
	GameFlag_Set(0x903);
	Engine_EventEnd();
}

static void TolbiShipDeck_RunIsaacTalk(void)
{
	Engine_EventBegin();
	Event_CallWithLastActiveObjectId(gTolbiShipDeck_ClosingCrewScript);
	Engine_TaskWait(1);
	Engine_EventOpenScreen();
	Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
	Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 148, 0x290);
	Actor_ShowEmote(22, 0x100, 0);
	Engine_ActorRunRepeatedMotion(22, 1);
	FieldScene_CallPairWith10(22, 0x5000);
	Engine_EventSetMessage(MsgTolbiShipHowWasIsaacDidExplore);
	Event_OpenMessage(0x2016, 0);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	if (Engine_EventChooseYesNo(0, 0) == 1)
	{
		FieldScene_RunStepThen10(0x2016);
		Engine_EventEnd();
		return;
	}
	gEventWork->message++;
	Event_AskYesNo(0x2016, 0);
	TolbiShipDeck_OpenFirstDoor();
	Actor_SetPosition(26, 0xd80000, 0x24c0000);
	Actor_SetSpeed(26, 0x13333, 0x9999);
	Actor_WalkToAndWait(26, 216, 0x254);
	Actor_WalkToAndWait(26, 188, 0x268);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 0);
	Actor_FaceDirection(21, 0xd000, 0);
	Actor_FaceDirection(22, 0xd000, 0);
	FieldScene_CallPairWith10(26, 0x5000);
	Engine_ActorJump(26, 2, 0);
	Engine_ActorSetAnimation(26, 4);
	Event_ShowMessage(26, 0);
	Actor_SetPosition(20, 0xb40000, 0x3090000);
	Actor_SetSpeed(20, 0x10000, 0x8000);
	Actor_WalkToAndWait(20, 180, 0x298);
	Actor_FaceDirection(20, 0xd000, 0);
	FieldScene_RunStepThen10(0x2014);
	Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
	Actor_FaceDirection(22, 0x3000, 0);
	Actor_ShowEmote(26, 0x101, 60);
	Engine_ActorRunRepeatedMotion(20, 1);
	FieldScene_RunStepThen10(0x2014);
	Engine_ActorStartRepeatedMotion(21, 2);
	FieldScene_RunStepThen10(21);
	Actor_FaceDirection(20, 0x5000, 20);
	Engine_ActorSetAnimation(20, 3);
	FieldScene_RunStepThen10(0x6014);
	Engine_ActorJump(26, 2, 20);
	Engine_ActorSetAnimation(26, 4);
	FieldScene_RunStepThen10(26);
	Actor_WalkToAndWait(20, 182, 0x280);
	Actor_FaceDirection(20, 0xd000, 0);
	FieldScene_RunStepThen10(0x8014);
	Actor_ShowEmote(26, 0x100, 20);
	Engine_ActorStartRepeatedMotion(26, 2);
	FieldScene_RunStepThen10(26);
	Engine_ActorSetAnimationAndWait(20, 3);
	FieldScene_CallPairWith10(22, 0);
	Engine_ActorRunRepeatedMotion(22, 1);
	FieldScene_RunStepThen10(22);
	Actor_SetSpeed(22, 0x19999, 0xcccc);
	Engine_ActorEnableActionCallback(22, gTolbiShipDeck_LeaveActions);
	Actor_SetSpeed(21, 0x19999, 0xcccc);
	Actor_WalkToAndWait(21, 168, 0x278);
	Engine_ActorEnableActionCallback(21, gTolbiShipDeck_LeaveActions);
	Engine_EventWait(80);
	Engine_ActorEnableActionCallback(26, gTolbiShipDeck_LeaveActions);
	Engine_EventWait(40);
	FieldScene_CallPairWith10(20, 0x8000);
	FieldScene_RunStepThen10(0x2014);
	FieldScene_CallPairWith10(ACTOR_PARTY_LEADER, 0xe000);
	Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
	Engine_ActorSetAnimationAndWait(20, 3);
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventRequestExit(17);
}

static void TolbiShipDeck_PlaceLeaderOnDeck(void)
{
	struct FieldActor *record;

	Camera_MoveTo(0xe80000, -1, 0x2a40000, 0);
	Engine_MapRedraw();
	Actor_SetPosition(ACTOR_PARTY_LEADER, 0xe80000, 0x2a40000);
	record = Object_GetById(0);
	record->facing = 0x4000;
	Engine_TaskWait(1);
}

const s32 gTolbiShipDeck_PresentationActionsA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(202), 0, PIXELS(600)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

const s32 gTolbiShipDeck_PresentationActionsB[] = {
	SC_WAIT(20),
	SC_ARG(2),
	SC_MOVE(PIXELS(162), 0, PIXELS(666)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_END,
};

const s32 gTolbiShipDeck_RandomActorActions[] = {
	SC_CALL(SceneActor_UpdateScalePulse),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_DeckEventActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(140), 0, PIXELS(586)),
	SC_SYNC,
	SC_SE(153),
	SC_SET(SW_VELOCITY_Y, 0x80000),
	SC_MOVE(PIXELS(184), 0, PIXELS(598)),
	SC_SYNC,
	SC_SET(0x23, 1),
	SC_SET(SW_VELOCITY_Y, 0x40000),
	SC_CALL(SceneState_ApplyArgMode1AndReturnZero),
	SC_MOVE(PIXELS(216), 0, PIXELS(596)),
	SC_SYNC,
	SC_MOVE(PIXELS(216), 0, PIXELS(586)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

const s32 gTolbiShipDeck_LeadActionsA[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(228), 0, PIXELS(521)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_WAIT(40),
	SC_ARG(4),
	SC_WAIT(40),
	SC_ARG(2),
	SC_MOVE(PIXELS(204), 0, PIXELS(521)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xb000),
	SC_WAIT(40),
	SC_ARG(4),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_LeadActionsB[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(672)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(70),
	SC_SET(SW_FACING, 0xb000),
	SC_WAIT(50),
	SC_ARG(2),
	SC_MOVE(PIXELS(196), 0, PIXELS(672)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xd000),
	SC_WAIT(30),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(70),
	SC_ARG(2),
	SC_MOVE(PIXELS(196), 0, PIXELS(636)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(50),
	SC_SET(SW_FACING, 0x5000),
	SC_WAIT(30),
	SC_ARG(2),
	SC_MOVE(PIXELS(168), 0, PIXELS(636)),
	SC_SYNC,
	SC_ARG(1),
	SC_SET(SW_FACING, 0xb000),
	SC_WAIT(70),
	SC_SET(SW_FACING, 0x3000),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

const s32 gTolbiShipDeck_CrewActionsA[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(0x23, 0),
	SC_SET(0x24, 0),
	SC_SET(0x27, 0),
	SC_SET(0x17, 0),
	SC_CALL(TolbiShipDeck_UpdateHoverGullA),
};

const s32 gTolbiShipDeck_CrewActionsB[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(0x23, 0),
	SC_SET(0x24, 0),
	SC_SET(0x27, 0),
	SC_SET(0x17, 0),
	SC_CALL(TolbiShipDeck_UpdateHoverGullB),
};

const s32 gTolbiShipDeck_CrewActionsC[] = {
	SC_SET(SW_MOTION, 0),
	SC_SET(0x24, 0),
	SC_CALL(SceneActor_OscillateHeightBetweenLimits),
};

const s32 gTolbiShipDeck_CrewActionsD[] = {
	SC_CALL(SceneActor_SetFacingFromSample),
};

const s32 gTolbiShipDeck_CrewActionsE[] = {
	SC_ARG(1),
	SC_SET(SW_SCALE_X, 0xb333),
	SC_SET(SW_SCALE_Y, 0xb333),
	SC_WAIT(1),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 3932),
	SC_ADD(SW_SCALE_Y, 0x147a),
	SC_WAIT(1),
	SC_LOOP(24, SC_TO(1)),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

const s32 gTolbiShipDeck_SailorActions[] = {
	SC_MOVE(PIXELS(216), 0, PIXELS(586)),
	SC_SYNC,
	SC_POS(0, 0, 0),
	SC_END,
};

const s32 gTolbiShipDeck_DeckActionsB[] = {
	SC_SET(0x23, 0),
	SC_CALL(TolbiShipDeck_UpdateFlyByForActor21),
};

const s32 gTolbiShipDeck_DeckActionsA[] = {
	SC_SET(0x23, 0),
	SC_CALL(TolbiShipDeck_UpdateFlyByForActor22),
};

const s32 gTolbiShipDeck_DeckActionsC[] = {
	SC_CALL(TolbiShipDeck_RandomlyStartActorHop),
};

const s32 gTolbiShipDeck_RosterActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(82), 0, PIXELS(656)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_END,
};

const s32 gTolbiShipDeck_LeaveActions[] = {
	SC_ARG(2),
	SC_MOVE(PIXELS(186), 0, PIXELS(604)),
	SC_SYNC,
	SC_MOVE(PIXELS(216), 0, PIXELS(604)),
	SC_SYNC,
	SC_MOVE(PIXELS(216), 0, PIXELS(586)),
	SC_SYNC,
	SC_ARG(1),
	SC_POS(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

const s32 gTolbiShipDeck_LeadActionsC[] = {
	SC_SET(0x27, 0),
	SC_CALL(TolbiShipDeck_IdleTurn),
};

const struct SceneEntrance gTolbiShipDeck_SceneTableA[] = {
	{ 0, CONDITION_ALWAYS, 179, 0, 653, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 216, 0, 600, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 2, CONDITION_ALWAYS, 232, 0, 664, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 3, CONDITION_ALWAYS, 216, 0, 808, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 4, CONDITION_ALWAYS, 216, 4, 648, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 5, CONDITION_ALWAYS, 312, -4, 656, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ 6, CONDITION_ALWAYS, 120, -4, 656, FACING_EAST, 0, 65, -1, -1, -1, 0 },
	{ 10, CONDITION_ALWAYS, 232, 0, 664, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 11, CONDITION_ALWAYS, 212, 0, 521, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 12, CONDITION_ALWAYS, 192, 0, 650, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 13, CONDITION_ALWAYS, 232, 0, 654, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 14, CONDITION_ALWAYS, 216, 0, 657, FACING_EAST, 0, -1, -1, -1, -1, 0 },
	{ 15, CONDITION_ALWAYS, 216, 0, 640, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 16, CONDITION_ALWAYS, 216, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 17, CONDITION_ALWAYS, 216, 0, 540, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 18, CONDITION_ALWAYS, 196, 0, 652, FACING_SOUTH, 0, 65, -1, -1, -1, 0 },
	{ 19, CONDITION_ALWAYS, 212, 0, 521, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

const s16 gTolbiShipDeck_SceneTableB[] = {
	208, 24, 634, 224, 40, 650, -1, 5,
	-1, 0, 0, 0, 0, 0, 0, 0,
};

const u32 gTolbiShipDeck_SceneTableC[] = {
	SCENE_EXITS(109),
	SCENE_EXIT(1, 111, 1),
	SCENE_EXIT(2, 111, 3),
	SCENE_EXIT(3, 111, 5),
	SCENE_EXIT(4, 107, 2),
	SCENE_EXIT(5, 110, 1),
	SCENE_EXIT(6, 112, 1),
	SCENE_EXIT(7, 2, 74),
	SCENE_EXIT(10, 110, 10),
	SCENE_EXIT(11, 111, 14),
	SCENE_EXIT(12, 111, 15),
	SCENE_EXIT(13, 111, 17),
	SCENE_EXIT(14, 111, 19),
	SCENE_EXIT(15, 2, 70),
	SCENE_EXIT(16, 111, 22),
	SCENE_EXIT(17, 111, 24),
	SCENE_EXITS_END,
};

const struct ScenePlacement gTolbiShipDeckPlacements[] = {
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x91, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(266), 0, PIXELS(703), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsFlag911[] = {
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x91, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(183), 0, PIXELS(720), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x92, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(256), 0, PIXELS(712), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(198), 0, PIXELS(706), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(172), 0, PIXELS(706), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x84, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(230), 0, PIXELS(602), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(236), 0, PIXELS(621), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(182), 0, PIXELS(556), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsFlag927[] = {
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(216), 0, PIXELS(657), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsFlag928[] = {
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScript[] = {
	{ 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(512), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x43, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(634), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(212), 0, PIXELS(521), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x92, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_ClosingCrewScript[] = {
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptB[] = {
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptC[] = {
	{ 0xc4, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc4, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc4, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc4, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc4, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc4, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xc6, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xd6, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptD[] = {
	{ 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xc5, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0x16, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeck_CrewScriptE[] = {
	{ GARET, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ IVAN, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ MIA, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0x43, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(178), 0, PIXELS(642), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x42, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(182), 0, PIXELS(534), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x92, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xc2, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(540), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		PIXELS(216), 0, PIXELS(640), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

const struct ScenePlacement gTolbiShipDeckPlacementsFlag93e[] = {
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(132), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(632), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(664), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(696), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0xe1, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(300), 0, PIXELS(728), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(188), 0, PIXELS(512), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x91, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(162), 0, PIXELS(634), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x92, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(162), 0, PIXELS(676), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xab, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x84, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x94, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x72, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x16, CONDITION_ALWAYS, 7,
		0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

const struct SceneEvent gTolbiShipDeckEvents[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	{ TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#else
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEventFromEntry) },
#endif
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_TALK, 20, 0x923, MSGNO(MsgTolbiShipDeckThereAreALotOfPassengers) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckOhAtTheRiskOfSounding) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x923, MSGNO(MsgTolbiShipDeckTheReplacementShipFromTolbiShould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIHidTheAnchorCharmNo) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeApologizeForAnyTroubleThat) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunOpeningAuxiliarySequence) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_TALK) },
	{ EVENT_TALK, 25, 0x922, MSGNO(MsgTolbiShipDeckIWonderWhatsGoingOnWeve) },
	{ EVENT_TALK, 25, 0x925, MSGNO(MsgTolbiShipDeckThoseWarriorsGotTiredOfWaiting) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIfWeJustPromiseToHelp) },
	{ EVENT_TALK, 26, 0x922, MSGNO(MsgTolbiShipDeckIWonderIfImTheOnly) },
	{ EVENT_TALK, 26, 0x925, MSGNO(MsgTolbiShipDeckIWonderIfThoseTwoAre) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIllAgreeToAnythingEvenRowing) },
	{ EVENT_TALK, 27, 0x922, MSGNO(MsgTolbiShipDeckTheKaragolIsLikeAHumongous) },
	{ EVENT_TALK, 27, 0x925, MSGNO(MsgTolbiShipDeckHaHahThoseMercenariesAreGoing) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThatSeaDogKajaIsA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThereIsNothingToFearAs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x922, MSGNO(MsgTolbiShipDeckWhenAreWeGoingWhenAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x925, MSGNO(MsgTolbiShipDeckUsingForceToLaunchTheShip) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAreYouSeriousAboutSettingSail) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x922, MSGNO(MsgTolbiShipDeckHmmmThisIsSoFrustratingWhy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x925, MSGNO(MsgTolbiShipDeckTheyWereJustAboutToLaunch) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItProbablyWontMakeAnyDifference) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x922, MSGNO(MsgTolbiShipDeckTravelingByShipIsFasterThan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x925, MSGNO(MsgTolbiShipDeckThoseThugsAreTooImpatientTrying) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUltimatelyTheyreJustBeingThreatenedInto) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, 0x922, MSGNO(MsgTolbiShipDeckMostOfThePeopleGoingTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, 0x925, MSGNO(MsgTolbiShipDeckHeeHeeIfWeCanGet) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckBesidesRowingIsAMansJob) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, 0x922, MSGNO(MsgTolbiShipDeckTheKaragolSeaIsPrettyFoggy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, 0x925, MSGNO(MsgTolbiShipDeckThoseGuysSeemPrettySureOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUsHelpWithTheRowingGet) },
	{ EVENT_TOUCH, 10, 0x920, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 11, 0x923, EVENT_SCRIPT(EV11_TOUCH) },
	{ EVENT_TOUCH, 12, 0x923, EVENT_SCRIPT(EV12_TOUCH) },
	{ SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsFlag928[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	{ TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#else
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEventFromEntry) },
#endif
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouMustFeelAwfullyImportantOrdering) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckItLooksLikeAnotherOarsmanWas) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIFeelSorryForTheOarsmen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckAnOarsmanWasInjuredByA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeHaveNoIdeaWhenThose) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckUhnnnWhenILookAtThe) },
	{ SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsFlag93e[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	{ TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#else
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEventFromEntry) },
#endif
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckYouLookinForSeanAndOuranos) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(SceneDialogue_RunActor21Line) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisGroupThatCameOnOur) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantSeanAndOuranosTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckImJustGoingToHangOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWonderWhatHappenedToThat) },
	{ SCENE_EVENTS_END },
};

const struct SceneEvent gTolbiShipDeckEventsFlag8a0[] = {
	{ TOUCH_WALKING_INTO(FACING_NORTH), 1, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#if defined(TBS_EDITION_JA) || defined(TBS_EDITION_EN)
	{ TOUCH_WALKING_INTO(FACING_WEST), 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
#else
	{ EVENT_TOUCH, 2, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEventFromEntry) },
#endif
	{ TOUCH_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipDeck_RunDeckStateEvent) },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 7 },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckKajaAndTheCaptainAreProfessionals) },
	{ EVENT_TALK, 22, 0x903, EVENT_SCRIPT(FieldScene_RunThreeActorEncounter) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckTheresNoTellingWhatKindOf) },
	{ EVENT_TALK, 21, 0x903, EVENT_SCRIPT(FieldScene_RunThreeActorEncounter) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWeveDoneAGoodJobOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckIWantToBeASailor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0x903, MSGNO(MsgTolbiShipDeckIGiveUpWhatAreWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckThisIslandGivesMeTheCreeps) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x903, MSGNO(MsgTolbiShipDeckThatsEnoughBickering) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipDeckWhatWeCanDoNowIs) },
	{ SCENE_EVENTS_END },
};

const s32 gTolbiShipDeck_FlagGroupEntries[] = {
	SC_MOVE(0x2, 0x3, 0x1),
	SC_MOVE_BY(0x4, 0x2, 0x1),
	SC_SYNC,
};
