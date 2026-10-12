#include "game_state.h"
#include "unit/field/shian_heya/heya.h"
#include "unit/field/common/effect/move.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/objtable.h"
#include "unit/battle/effect/sparkle.h"
#include "unit/battle/effect/evbasic.h"
#include "unit/battle/effect/evactor.h"
#include "unit/battle/effect/evcamera.h"
#include "unit/battle/effect/djinnev.h"
#include "unit/battle/effect/psydisp.h"
#include "unit/battle/effect/psyfx.h"
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
#include "field_effect.h"
#include "text/msg_ids.h"
#include "fieldobj.h"

// ここに共通ソースが入る（ROM の並びのとおり）
#include "../common/effect/create.c"
#include "../common/effect/move.c"
#include "../common/effect/spawn.c"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

s32 ShianHeya_GetEntrances(void)
{
	return (s32)ShianHeya_SceneTable0;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

s32 ShianHeya_GetExits(void)
{
	return (s32)ShianHeya_SceneTable1;
}

s32 ShianHeya_SelectPlacements(void)
{
	if (gGameState.entrance == 8)
	{
		return (s32)ShianHeya_SceneTable2Entrance8;
	}
	SceneTable_Prepare(ShianHeya_SceneTable2);
	return (s32)ShianHeya_SceneTable2;
}

static void EV14_TALK(void)
{
	u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
	u32	 value = state[3];

	Engine_EventBegin();
	if (value >= 0xa001 && value <= 0xdfff)
	{
		Engine_ShopOpen(16, 14);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x895) == 0)
		{
			Engine_EventSetMessage((s32)MsgShianXianHasMartialArtsBut);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgShianHsusInjuryTaughtGoodLesson);
		}
		Engine_EventShowMessage(14, 0);
	}
	Engine_EventEnd();
}

static void EV15_TALK(void)
{
	u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
	u32	 value = state[3];

	Engine_EventBegin();
	if (value >= 0xa001 && value <= 0xdfff)
	{
		Engine_ShopOpen(17, 15);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x895) == 0)
		{
			Engine_EventSetMessage((s32)MsgShianMartialArtistsCantUseHeavy);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgShianKungFuMakesQuickDoes);
		}
		Engine_EventShowMessage(15, 0);
	}
	Engine_EventEnd();
}

static void EV16_TALK(void)
{

	u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
	u32	 value = state[3];

	Engine_EventBegin();
	if (value >= 0xa001 && value <= 0xdfff)
	{
		Engine_ShopOpen(18, 16);
	}
	else if (Engine_GameFlagIsSet(0x895) == 0)
	{
		Engine_EventSetMessage((s32)MsgShianXiansSpecialArmorNotSuited);
		Engine_EventShowMessage(16, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgShianDoPlanCrossDesertWarrior);
		Engine_EventAskYesNo(16, 0);
	}
	Engine_EventEnd();
}

static void EV17_TALK(void)
{

	u16 *state = Object_GetById(ACTOR_PARTY_LEADER);
	u32	 value = state[3];

	Engine_EventBegin();
	if (value < 0x2000 || value > 0xe000)
	{
		Engine_InnOpen(5, 17);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x895) == 0)
		{
			Engine_EventSetMessage((s32)MsgShianWarriorsShouldLearnAboutTowns);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgShianWeHaveFewCustomersThese);
		}
		Engine_EventShowMessage(17, 0);
	}
	Engine_EventEnd();
}

static void EV10_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianSomethingWrongOnSilkRoad);
	Engine_EventAskYesNo(10, 0);
	Engine_EventEnd();
}

static void EV12_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianDidMonstersInAltinSpit);
	Engine_EventAskYesNo(12, 0);
	Engine_EventEnd();
}

static void EV09_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgShianWarriorWelcome);
	Engine_EventShowMessageAndWait(9, 0, 20);
	Engine_ActorFaceActor(9, 10, 0);
	Engine_EventWait(60);
	Engine_ActorFaceActor(9, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(20);
	Engine_EventShowMessage(9, 0);
	Engine_EventEnd();
}

s32 ShianHeya_SelectEvents(void)
{
	if (gGameState.entrance == 8)
	{
		return (s32)ShianHeya_SceneTable3Entrance8;
	}
	return (s32)ShianHeya_SceneTable3;
}

s32 ShianHeya_PrepareWindowTransition(void)
{

	s16 scene;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	scene = gGameState.entrance;
	if (scene == 4 || scene == 7)
	{
		OverlayObject_CreateConfiguredObject(0x00f80000, 0, 0x01a10000, 20);
	}
	else if (scene == 6)
	{
		OverlayObject_CreateConfiguredObject(0x01cc0000, 0, 0x02380000, 20);
		OverlayObject_CreateConfiguredObject(0x01e40000, 0, 0x02380000, 20);
	}
	else if (scene == 8)
	{
		Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
		Engine_ActorSetAnimation(10, 6);
	}
	return 0;
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

struct SceneEntrance ShianHeya_SceneTable0[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 128, 0, 184, FACING_NORTH, 0, 24, 8, 248, 208, 0 },
	{ 2, CONDITION_ALWAYS, 448, 0, 200, FACING_NORTH, 0, 288, 8, 592, 224, 0 },
	{ 3, CONDITION_ALWAYS, 736, 0, 200, FACING_NORTH, 0, 608, 8, 848, 224, 0 },
	{ 4, CONDITION_ALWAYS, 160, 0, 472, FACING_NORTH, 0, 48, 288, 336, 496, 0 },
	{ 5, CONDITION_ALWAYS, 472, 0, 360, FACING_EAST, 0, 384, 288, 672, 496, 0 },
	{ 6, CONDITION_ALWAYS, 512, 0, 696, FACING_NORTH, 0, 352, 496, 656, 720, 0 },
	{ 7, CONDITION_ALWAYS, 120, 0, 360, FACING_EAST, 0, 48, 288, 336, 496, 0 },
	{ 8, CONDITION_ALWAYS, 120, 0, 696, FACING_NORTH, 0, 64, 544, 304, 744, 0 },
	{ SCENE_TABLE_END },
};

u32 ShianHeya_SceneTable1[] = {
	SCENE_EXITS(73),
	SCENE_EXIT(1, 72, 5),
	SCENE_EXIT(2, 72, 6),
	SCENE_EXIT(3, 72, 7),
	SCENE_EXIT(4, 72, 8),
	SCENE_EXIT(5, 73, 7),
	SCENE_EXIT(6, 72, 9),
	SCENE_EXIT(7, 73, 5),
	SCENE_EXIT(8, 61, 4),
	SCENE_EXITS_END,
};

struct ScenePlacement ShianHeya_SceneTable2[] = {
	{ 0xa0, 0x0, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xa2, 0x0, ACTOR_STAND,
		PIXELS(136), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9c, 0x0, ACTOR_STAND,
		PIXELS(496), 0, PIXELS(72), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x9f, 0x0, ACTOR_WANDER,
		PIXELS(368), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xa4, 0x0, ACTOR_STAND,
		PIXELS(760), 0, PIXELS(184), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0xa6, 0x0, ACTOR_STAND,
		PIXELS(680), 0, PIXELS(80), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(472), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(448), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(552), 0, PIXELS(600), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x77, 0x0, ACTOR_STAND,
		PIXELS(232), 0, PIXELS(448), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_WANDER,
		PIXELS(568), 0, PIXELS(416), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x7b, 0x0, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(408), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement ShianHeya_SceneTable2Entrance8[] = {
	{ 0x28, IF_FLAG_SET(0x895), ACTOR_STAND,
		PIXELS(224), 0, PIXELS(656), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x27, IF_FLAG_SET(0x895), ACTOR_STAND,
		PIXELS(200), 0, PIXELS(704), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x29, IF_FLAG_SET(0x895), ACTOR_STAND,
		PIXELS(224), 0, PIXELS(704), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent ShianHeya_SceneTable3[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_TALK, 8, 0x895, MSGNO(MsgShianHeyaTheMulberryHarvestIsOverNow) },
	{ EVENT_TALK, 9, 0x895, MSGNO(MsgShianHeyaFarmWorkIsVeryHardWarriors) },
	{ EVENT_TALK, 10, 0x895, MSGNO(MsgShianHeyaXianSilkIsSentOnSilk) },
	{ EVENT_TALK, 11, 0x895, MSGNO(MsgShianHeyaSilkRoadEndsInTolbiIt) },
	{ EVENT_TALK, 12, 0x895, MSGNO(MsgShianHeyaFollowSilkRoadWhenYouSee) },
	{ EVENT_TALK, 13, 0x895, MSGNO(MsgShianHeyaThereIsAMineInAltin) },
	{ EVENT_TALK, 14, 0x895, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 15, 0x895, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, 0x895, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, 0x895, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, 0x895, MSGNO(MsgShianHeyaNormallyManyMerchantsComeToThe) },
	{ EVENT_TALK, 19, 0x895, MSGNO(MsgShianHeyaAllThePeopleWhoCameTo) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShianMuraWhereAreTheMerchantsNoOne) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShianHeyaMerchantsBringUsCoinsTheyMust) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShianHeyaNoneCanPassThroughLamakanThis) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgShianHeyaTheMineInAltinFilledWith) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgShianHeyaWhenLamakanDesertBurnsItIs) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgShianHeyaHotFoodIsGoodForCrossing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x895, MSGNO(MsgShianHeyaSoonMasterHammetWillComeHe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x895, MSGNO(MsgShianHeyaFarmWorkIsHardOnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x895, MSGNO(MsgShianHeyaSilkRoadIsFullOfSilk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x895, MSGNO(MsgShianHeyaMerchantsSayTolbiIsVeryBeautiful) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x895, MSGNO(MsgShianHeyaAltinHasManyMenAllScary) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x895, MSGNO(MsgShianHeyaMiningIsVeryHardWorkIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x895, MSGNO(MsgShianHeyaSomeSayFightingWithWeaponsIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x895, MSGNO(MsgShianHeyaInWarriorBattlesYouFightWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x895, MSGNO(MsgShianHeyaXianIsAVillageOfCraftsmen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x895, MSGNO(MsgShianHeyaWhenYouGoToANew) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x895, MSGNO(MsgShianHeyaWhenManyMerchantsComeMyBusiness) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x895, MSGNO(MsgShianHeyaThereAreFewerTravelersSoFewer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShianHeyaWhyDoesMasterHammetNotCome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShianHeyaMerchantsSellSilkItIsLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShianHeyaFeizhiSaidTheDesertBecameEvil) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgShianHeyaTheDesertWasVeryHotNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgShianHeyaTheVillageIsFloodedBecauseOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgShianHeyaWaterMonstersHowTerribleForAltin) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgShianHeyaHsuWasPummeledByHugeBoulders) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgShianHeyaWeShouldSellArmorToKung) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgShianHeyaYouLoseStrengthInHeatUse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgShianHeyaCaravansCannotBringMerchantsHereWhen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgShianHeyaTheMerchantsDoNotComeBecause) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgShianHeyaEatWarriorIWillAddMany) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf6f, FIND_ITEM(ITEM_ELIXIR) },
	{ SEARCH(SEARCH_JAR), 101, 0xf70, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaTheyreMakingStirFryTheCabbage)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaItsTheXianGourmetTheBest)) },
	{ SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaTheyreSteamedDumplingsTheyreGreatPiping)) },
	{ SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaRobinAndHisFriendsAreShocked)) },
	{ SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaHesMakingHotBeanCurdTastes)) },
	{ 0xc5b3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaThereAreManyChopsticksButNot)) },
	{ SEARCH(SEARCH_SHELF), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaThereAreMedicinalHerbsTheyHave)) },
	{ SEARCH(SEARCH_SHELF), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaTheresAKimonoItLooksLike)) },
	{ SEARCH(SEARCH_SHELF), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgShianHeyaItsJustASalesReportIt)) },
	{ SCENE_EVENTS_END },
};

struct SceneEvent ShianHeya_SceneTable3Entrance8[] = {
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_TALK, 8, 0x8b3, MSGNO(MsgShianHeyaFeizhisPredictionCameTrueNowI) },
	{ EVENT_TALK, 9, 0x8b3, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, 0x8b3, MSGNO(MsgShianHeyaIWasInjuredByRocksOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8b3, MSGNO(MsgShianHeyaFeizhisAbilityToSenseTroubleIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x8b3, MSGNO(MsgShianHeyaMasterHamaHasGreatPowerYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x8b3, MSGNO(MsgShianHeyaIKnewHelpWouldComeWhen) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShianHeyaFeizhisPredictionCameTrueNowIBelieve) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShianHeyaHsuWasTrappedUnderTheRocks) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShianHeyaIWasInjuredByBouldersOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgShianHeyaFeizhisAbilityToForeseeIsReal) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgShianHeyaMasterHamaHasGreatPowerSome) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgShianHeyaIKnewHelpWouldComeWhenI) },
	{ SCENE_EVENTS_END },
};
