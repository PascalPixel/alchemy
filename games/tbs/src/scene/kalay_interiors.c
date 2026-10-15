#include "game_state.h"
#include "scene_ids.h"
#include "unit/scene/kalay_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "resource_3a9.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

// 前方宣言
static void SceneState_ClearSlotsBySubState(void);

const struct SceneEntrance *Scene_GetEntrances(void)
{
	s16 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KalayInteriors1)
	{
		return gKalayInteriorsEntrances1;
	}
	if (scene == (s32)&SceneId_KalayInteriors2)
	{
		return gKalayInteriorsEntrances2;
	}
	return gKalayInteriorsEntrancesOther;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *KalayInteriors_GetExits(void)
{
	return KalayInteriors_Exits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
	s32 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KalayInteriors1)
	{
		s32							 entrance = gGameState.entrance;
		const struct ScenePlacement *table;

		switch (entrance)
		{
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 17:
			table = gKalayInteriorsPlacements1Entrance9;
			break;
		default:
			table = gKalayInteriorsPlacements1;
			break;
		}
		FieldScene_PrepareActors(table);
		return table;
	}
	if (scene == (s32)&SceneId_KalayInteriors2)
	{
		return gKalayInteriorsPlacements2;
	}
	return gKalayInteriorsPlacementsOther;
}

const struct SceneEvent *Scene_GetEvents(void)
{
	s32 scene = gGameState.scene;

	if (scene == (s32)&SceneId_KalayInteriors1)
	{
		s32 entrance = gGameState.entrance;

		switch (entrance)
		{
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 17:
			return gKalayInteriorsEvents1Entrance9;
		default:
			return gKalayInteriorsEvents1;
		}
	}
	if (scene == (s32)&SceneId_KalayInteriors2)
	{
		return gKalayInteriorsEvents2;
	}
	return gKalayInteriorsEventsOther;
}

static void EV12_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgKalayPleaseFinishEatingIfTaking);
	Engine_EventShowMessage(0xC, 0);
	Engine_GameFlagSet(0x910);
	Engine_EventEnd();
}

static void EV16_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage(MsgKalayDoKnowAboutContinentSouth);
	Engine_EventAskYesNo(16, 0);
	Engine_EventEnd();
}

static void EV08_TALK(void)
{
	struct FieldActor *p = Object_GetById(0);

	if ((u16)(p->facing - 0x6001) <= 0x3FFE)
	{
		Engine_InnOpen(7, 8);
	}
	else
	{
		Engine_EventBegin();

		if (Engine_GameFlagIsSet(0x911) != 0)
		{
			Engine_EventSetMessage(MsgKalayOurInnFeelsEmptyNow);
			Engine_EventShowMessage(8, 0);
		}
		else
		{
			Engine_EventSetMessage(MsgKalayGoingTolbiAlso);
			Engine_EventAskYesNo(8, 0);
			Engine_GameFlagSet(0x910);
		}

		Engine_EventEnd();
	}
}

static void EV08_TALK_2(void)
{

	struct FieldActor *p = Object_GetById(0);

	if ((u16)(p->facing + 0x5FFF) <= 0x3FFE)
	{
		Engine_SanctumOpen(8);
	}
	else
	{
		Engine_EventBegin();
		Engine_EventSetMessage(MsgKalayCanLiveInPeaceIn);
		Engine_EventShowMessage(8, 0);
		Engine_EventEnd();
	}
}

static void FieldScene_RunArrivalPlacement(void)
{

	struct EventWork  *work = gEventWork;
	u32				   slot;
	s32				   idx;
	struct FieldActor *p;

	Engine_EventBegin();

	for (slot = 8; slot <= 65; slot++)
	{
		struct FieldActor *rec = Object_GetById(slot);

		if (rec != 0)
		{
			rec->motionFlags = 0;
		}
	}

	switch (work->touchedTrigger)
	{
	case 12:
		idx = 0;
		break;
	case 13:
		idx = 1;
		break;
	case 16:
		idx = 2;
		break;
	case 19:
		idx = 3;
		break;
	default:
		return;
	}

	Engine_AudioPlayCue(158);

	{
		u32 x = KalayInteriors_ArrivalPlacements[idx].x;
		u32 y = KalayInteriors_ArrivalPlacements[idx].y;

		Engine_MapAnimateCells(KalayInteriors_ArrivalPlacements[idx].steps, x, y);
	}

	Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x00008000, 0x00004000);

	p = Object_GetById(0);
	p->motionFlags = 0;

	Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
	Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 3, -8);
	Engine_EventWait(10);

	Engine_EventRequestExit(work->touchedTrigger);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
	gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	if (gGameState.scene == (s32)&SceneId_KalayInteriors1)
	{
		SceneState_ClearSlotsBySubState();
	}
	return 0;
}

static void SceneState_ClearSlotsBySubState(void)
{
	s16 sub = gGameState.entrance;

	switch (sub)
	{
	case 3:
	{
		Map_CopyCellsTo(30, 14, 30, 16, 4, 2);
		return;
	}
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
	case 17:
		if (Engine_GameFlagIsSet(0x911) != 0)
		{

			Engine_ActorDestroy(10);
			Engine_ActorDestroy(11);
			Engine_ActorDestroy(12);
			Engine_ActorDestroy(13);
			Engine_ActorDestroy(14);
			Engine_ActorDestroy(17);
			Engine_ActorDestroy(18);
			Engine_ActorDestroy(19);
			Engine_ActorDestroy(15);
		}
		else
		{
			Engine_ActorSetChildValue(13, 2);
		}
		return;
	default:
		if (Engine_GameFlagIsSet(0x911) != 0)
		{
			Engine_ActorDestroy(16);
			Engine_ActorDestroy(17);
		}
		break;
	}
}

struct SceneEntrance gKalayInteriorsEntrancesOther[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKalayInteriorsEntrances1[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 144, 0, 192, FACING_NORTH, 0, 5, 45, 245, 230, 0 },
	{ 2, CONDITION_ALWAYS, 400, 0, 192, FACING_NORTH, 0, 255, 45, 485, 230, 0 },
	{ 3, CONDITION_ALWAYS, 624, 0, 240, FACING_NORTH, 0, 500, 45, 735, 275, 0 },
	{ 4, CONDITION_ALWAYS, 128, 0, 432, FACING_NORTH, 0, 35, 290, 275, 468, 0 },
	{ 5, CONDITION_ALWAYS, 384, 0, 384, FACING_NORTH, 0, 305, 255, 545, 485, 0 },
	{ 6, CONDITION_ALWAYS, 464, 0, 448, FACING_NORTH, 0, 305, 255, 545, 485, 0 },
	{ 7, CONDITION_ALWAYS, 720, 0, 496, FACING_NORTH, 0, 630, 305, 875, 535, 0 },
	{ 8, CONDITION_ALWAYS, 800, 0, 440, FACING_NORTH, 0, 630, 305, 875, 535, 0 },
	{ 9, CONDITION_ALWAYS, 48, 0, 640, FACING_NORTH, 0, 0, 495, 305, 775, 0 },
	{ 10, CONDITION_ALWAYS, 224, 0, 736, FACING_NORTH, 0, 0, 495, 305, 775, 0 },
	{ 11, CONDITION_ALWAYS, 408, 0, 616, FACING_SOUTH, 0, 355, 525, 690, 710, 0 },
	{ 12, CONDITION_ALWAYS, 552, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0 },
	{ 13, CONDITION_ALWAYS, 632, 0, 648, FACING_SOUTH, 0, 355, 525, 690, 710, 0 },
	{ 14, CONDITION_ALWAYS, 552, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0 },
	{ 15, CONDITION_ALWAYS, 632, 0, 680, FACING_NORTH, 0, 355, 525, 690, 710, 0 },
	{ 16, CONDITION_ALWAYS, 808, 0, 616, FACING_SOUTH, 0, 745, 570, 935, 720, 0 },
	{ 17, CONDITION_ALWAYS, 440, 0, 872, FACING_NORTH, 0, 340, 740, 520, 900, 0 },
	{ 18, CONDITION_ALWAYS, 584, 0, 872, FACING_NORTH, 0, 540, 740, 730, 900, 0 },
	{ 19, CONDITION_ALWAYS, 840, 0, 792, FACING_SOUTH, 0, 710, 740, 940, 900, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEntrance gKalayInteriorsEntrances2[] = {
	{ 0, CONDITION_ALWAYS, 168, 0, 168, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 248, 0, 304, FACING_NORTH, 0, 110, 35, 390, 345, 0 },
	{ 2, CONDITION_ALWAYS, 248, 0, 128, FACING_SOUTH, 0, 110, 35, 390, 345, 0 },
	{ SCENE_TABLE_END },
};

u32 KalayInteriors_Exits[] = {
	SCENE_EXITS(100),
	SCENE_EXIT(1, 99, 1),
	SCENE_EXIT(2, 99, 2),
	SCENE_EXIT(3, 99, 3),
	SCENE_EXIT(4, 99, 4),
	SCENE_EXIT(5, 99, 12),
	SCENE_EXIT(6, 99, 5),
	SCENE_EXIT(7, 99, 6),
	SCENE_EXIT(8, 99, 13),
	SCENE_EXIT(9, 100, 11),
	SCENE_EXIT(10, 99, 7),
	SCENE_EXIT(11, 100, 9),
	SCENE_EXIT(12, 100, 17),
	SCENE_EXIT(13, 100, 18),
	SCENE_EXIT(14, 100, 19),
	SCENE_EXIT(15, 100, 16),
	SCENE_EXIT(16, 100, 15),
	SCENE_EXIT(17, 100, 12),
	SCENE_EXIT(18, 100, 13),
	SCENE_EXIT(19, 100, 14),
	SCENE_EXIT(0, 101, 0),
	SCENE_EXIT(1, 99, 8),
	SCENE_EXITS_END,
};

struct ScenePlacement gKalayInteriorsPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriorsPlacements1[] = {
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(101), 0, PIXELS(181), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(167), 0, PIXELS(109), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(383), 0, PIXELS(108), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(346), 0, PIXELS(109), FACING_NORTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x7e, 0x0, ACTOR_WANDER,
		PIXELS(573), 0, PIXELS(163), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(670), 0, PIXELS(154), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x7f, 0x0, ACTOR_STAND,
		PIXELS(183), 0, PIXELS(416), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(369), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(856), 0, PIXELS(628), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(780), 0, PIXELS(633), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x8f, 0x0, ACTOR_STAND,
		PIXELS(795), 0, PIXELS(806), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x8f, 0x0, ACTOR_STAND,
		PIXELS(857), 0, PIXELS(841), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriorsPlacements1Entrance9[] = {
	{ 0x77, 0x0, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(627), FACING_EAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_WANDER,
		PIXELS(505), 0, PIXELS(665), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x84, 0x0, ACTOR_STAND,
		PIXELS(66), 0, PIXELS(622), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x94, 0x0, ACTOR_STAND,
		PIXELS(196), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0xab, 0x0, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(586), FACING_SOUTH + FACING_STEP, TALK_KEEP_FACING, 0 },
	{ 0xab, 0x0, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(570), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x72, 0x0, ACTOR_STAND,
		PIXELS(207), 0, PIXELS(672), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(643), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(247), 0, PIXELS(736), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9d, 0x0, ACTOR_STAND,
		PIXELS(263), 0, PIXELS(627), FACING_SOUTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x43, 0x0, ACTOR_STAND,
		PIXELS(417), 0, PIXELS(810), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ 0x42, 0x0, ACTOR_STAND,
		PIXELS(459), 0, PIXELS(820), FACING_WEST + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gKalayInteriorsPlacements2[] = {
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(106), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gKalayInteriorsEventsOther[] = {
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriorsEvents1[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 16, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunArrivalPlacement) },
	{ EVENT_EXIT, 18, CONDITION_ALWAYS, 18 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 19, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunArrivalPlacement) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImShockedToHearTheRighteous) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpaWasAKindThiefHis) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSomehowTheNorthHasBecomeThe) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsAMerchantFromVaultHadBeen) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsNoOneIsAllowedToGo) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDoNotWantMySon) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantSkillForA) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonReceivedHisKnackFor) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIBetterGetReadyTheColosso) },
	{ EVENT_TALK, 18, 0x911, MSGNO(MsgKalayInteriorsWeFledFromGondowanToThe) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeCameHereToEscapeThe) },
	{ EVENT_TALK, 19, 0x911, MSGNO(MsgKalayInteriorsLaliveroWhereImFromIsThe) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeEscapedFromTheNortheasternRegion) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheRegionNorthOfVaultWas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsLunpasReputationIsVeryBadNowadays) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsThreeYearsAgoThievesRaidedMt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheySayOtherNorthernPartsAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImAnAdultNowIDont) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIDontCareHowMySon) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheMostImportantThingIsNot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMySonIsVeryTightWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsPeopleFromTheContinentSouthOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImPrettyExcitedBecauseThisIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x911, MSGNO(MsgKalayInteriorsIFledAllThisWayNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhatColossoIsLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x911, MSGNO(MsgKalayInteriorsItLooksLikeColossoWillSoon) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoScaredComingTo) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf83, FIND_COINS(11) },
	{ SEARCH(SEARCH_OVEN), 101, 0xf84, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf85, FIND_ITEM(ITEM_ELIXIR) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
	{ SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriorsEvents1Entrance9[] = {
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 12, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunArrivalPlacement) },
	{ TOUCH_WALKING_INTO(FACING_NORTH), 13, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunArrivalPlacement) },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_EXIT, 15, CONDITION_ALWAYS, 15 },
	{ EVENT_EXIT, 17, CONDITION_ALWAYS, 17 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, 0x911, MSGNO(MsgKalayInteriorsEveryoneIsGoingToTolbiTo) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenOurChefLeftSoWe) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIveSignedOnForATour) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsTheColossoGladiatorsAreAllStrong) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEveryoneLooksForwardToGoingTo) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsSurelyLordHammetWillPopUp) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWouldHaveEnteredColossoIf) },
	{ EVENT_TALK, 16, 0x911, MSGNO(MsgKalayInteriorsManyOfTheGuestsHereSeem) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsUhOhHesStillHereEven) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsArentYouGoingToColossoYou) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIfYouAreEnteringColossoYou) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWereSoStrongMakingTheFinals) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x911, MSGNO(MsgKalayInteriorsHesAShrewdMerchantIfHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIdHaveGoneOnTheColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x911, MSGNO(MsgKalayInteriorsEveryYearEveryoneGoesToTolbi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsEvenIfMyHusbandWantedTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsJoiningATourGroupIsThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWonderWhichWarriorWillWin) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsItsBusinessSoYouCantHelp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImExhaustedFromPreparingThisTour) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIWasSoExcitedICouldnt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsMyAnimalInstinctsStartWakingUp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x911, MSGNO(MsgKalayInteriorsColossoIsSoCommonWhatsSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICantSeemToRelaxNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsImGoingToSignUpFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsIHavePowerAndSeanHas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsWeAreTheToughestOfAll) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf83, FIND_COINS(11) },
	{ SEARCH(SEARCH_OVEN), 101, 0xf84, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf85, FIND_ITEM(ITEM_ELIXIR) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsShesToastingSomeBreadItSmells)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingCurryItsSpicyScent)) },
	{ SEARCH(SEARCH_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsHesMakingRoastedEggplantWithCheese)) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsAKalaySaladWithCucumber)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKalayInteriorsItsPicklesWithGarlicThePickle)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent gKalayInteriorsEvents2[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_2) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayInteriorsICameHereToSpreadGood) },
	{ SCENE_EVENTS_END },
};

u16 KalayInteriors_ArrivalPlacementData[] = {
	MAP_CELL_STEP(7, 125, 1, 2, 5), MAP_CELL_STEP(8, 125, 1, 2, 5), MAP_CELL_STEPS_END,
};

struct MapCellAnim KalayInteriors_ArrivalPlacements[] = {
	{ KalayInteriors_ArrivalPlacementData, 34, 98 },
	{ KalayInteriors_ArrivalPlacementData, 39, 98 },
	{ KalayInteriors_ArrivalPlacementData, 50, 96 },
	{ KalayInteriors_ArrivalPlacementData, 52, 107 },
};
