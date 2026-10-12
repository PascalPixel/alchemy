#include "unit/field/biribino_heya/heya.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
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
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

u8 *SceneData_GetPrimaryTable(void)
{
	return (u8 *)BiribinoHeya_PrimaryTable;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
	return (u8 *)BiribinoHeya_SecondaryTable;
}

s32 BiribinoHeya_PreparePlacements(void)
{
	ScenePlacement_ClipToView((s32)gBiribinoHeyaPlacements);
	return (s32)gBiribinoHeyaPlacements;
}

static void EV16_TALK(void)
{

	u32 dir;

	dir = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(7, 16);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x845) == 0)
		{
			Engine_EventSetMessage((s32)MsgBiribinoThereWasAbleHealerIn);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgBiribinoOurWeaponsBestCanFind);
		}
		Engine_EventShowMessage(16, 0);
	}

	Engine_EventEnd();
}

static void EV18_TALK(void)
{

	u32 dir;

	dir = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(9, 18);
	}
	else
	{
		if (Engine_GameFlagIsSet(0x845) == 0)
		{
			Engine_EventSetMessage((s32)MsgBiribinoMustUsedTypesDangerBeing);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgBiribinoTurnedOutWarriorsHiredBy);
		}
		Engine_EventShowMessage(18, 0);
	}

	Engine_EventEnd();
}

u8 *SceneData_GetTertiaryTable(void)
{
	return (u8 *)BiribinoHeya_TertiaryTable;
}

static void EV17_TALK(void)
{
	u32 dir;

	dir = Object_GetById(0)->facing;
	Engine_EventBegin();
	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(8, 17);
	}
	else
	{
		if (GameFlag_IsSet(0x845) == 0)
		{
			Engine_EventSetMessage((s32)MsgBiribinoHaveEverHeardOcean);
			Actor_FaceActor(17, ACTOR_PARTY_LEADER, 0);
			Engine_EventWait(10);
			Event_AskYesNo(17, 0);
			Actor_FaceDirection(17, 0x3000, 10);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgBiribinoForSomeReasonOceanFills);
			Event_ShowMessage(17, 0);
		}
	}
	Engine_EventEnd();
}

static void FieldScene_ConfigureActor21Scene(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoImReallyWorriedAboutKolima);
	Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
	Event_ShowMessage(21, 0);
	Actor_FaceDirection(21, 0xc000, 10);
	Engine_EventEnd();
}

static void EV24_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoGrrr);
	Event_ShowMessageAndWait(24, 0, 20);
	Actor_FaceActor(24, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(10);
	Event_OpenMessage(24, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	Event_ShowMessage(24, 0);
	Actor_FaceDirection(24, 0x4000, 10);
	Engine_EventEnd();
}

static void FieldScene_RunActor27Sequence(void)
{
	void Actor_FaceDirection();

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoDoBelieveTreeSpiritCan);
	Actor_FaceActor(27, ACTOR_PARTY_LEADER, 0);
	Engine_EventWait(10);
	Event_OpenMessage(27, 0);
	if (Engine_EventChooseYesNo(0, 0) != 0)
	{
		gEventWork->message++;
	}
	Event_ShowMessage(27, 0);
	Actor_FaceDirection(27, 0x4000, 10);
	Engine_EventEnd();
}

static void EV08_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoDoKnowIfHolyTree);
	Engine_EventAskYesNo(8, 0);
	Engine_EventEnd();
}

static void EV13_TALK(void)
{
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoDoKnowSilk);
	Engine_EventAskYesNo(13, 0);
	Engine_EventEnd();
}

static void EV19_TALK(void)
{

	u32 dir;

	dir = Object_GetById(0)->facing;
	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_InnOpen(2, 19);
	}
	else if (Engine_GameFlagIsSet(0x845) != 0)
	{
		Engine_EventSetMessage((s32)MsgBiribinoWasntCurseInKolimaHorrifying);
		Engine_EventAskYesNo(19, 0);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgBiribinoCurseOnKolimaScaryDevelopment);
		Engine_EventShowMessage(19, 0);
	}

	Engine_EventEnd();
}

static void FieldScene_RunActor21SequenceOnFlag300(void)
{
	void Actor_FaceDirection();

	Engine_EventBegin();
	if (GameFlag_IsSet(0x300) == 0)
	{
		Engine_EventSetMessage((s32)MsgBiribinoTwoSpecialsOneDinnerOne);
		Event_ShowMessage(21, 0);
		Actor_FaceDirection(21, 0x8000, 20);
		Event_ShowMessage(21, 0);
		Engine_ActorStartRepeatedMotion(22, 2);
		Actor_SetAttachedEffect(22, 0x102);
		Engine_EventWait(60);
		Event_ShowMessage(22, 0);
		Engine_EventWait(10);
		GameFlag_Set(0x300);
	}
	Actor_FaceActor(21, ACTOR_PARTY_LEADER, 0);
	Engine_EventSetMessage((s32)MsgBiribinoIfWantMealSpeakWaitress);
	Event_ShowMessage(21, 0);
	Actor_FaceDirection(21, 0xc000, 10);
	Engine_EventEnd();
}

static void EV22_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoReallyThinkHeadChefHas);
	Engine_EventShowMessage(0x16, 0);
	Engine_ActorFaceActor(0x16, ACTOR_PARTY_LEADER, 0);
	Engine_EventShowMessage(0x16, 0);
	Engine_ActorFaceDirection(0x16, 0, 0xA);
	Engine_EventEnd();
}

static void EV23_TALK(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoLetsSeeServeThemWater);
	Event_ShowMessage(23, 0);
	Actor_FaceActor(23, ACTOR_PARTY_LEADER, 0);
	Event_ShowMessage(23, 0);
	Actor_FaceDirection(23, 0xc000, 10);
	Engine_EventEnd();
}

static void FieldScene_RunActor27Message(void)
{

	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgBiribinoWasSomeMealDontJust);
	Engine_EventAskYesNo(27, 0);
	Engine_EventEnd();
}

static void EV10_PSYNERGY(void)
{

	Engine_EventBegin();
	if (Engine_GameFlagIsSet(3) != 0)
	{
		Engine_EventSetMessage((s32)MsgBiribinoEvenFrozenImilMustFeel);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgBiribinoWhenSpringComesWantGo);
	}
	Engine_EventShowMessage(10, 0);
	Engine_EventEnd();
}

s32 FieldScene_SetupActor27OnEntry(void)
{

	struct FieldActor *actor;
	u8				  *record;
	s32				   bits;

	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
	actor = Object_GetById(27);

	((u8 *)actor)[0x23] = bits = 0;
	record = (u8 *)actor->sprite;
	bits -= 13;
	bits &= record[9];
	bits |= 8;
	record[9] = bits;
	return 0;
}

struct SceneEntrance BiribinoHeya_PrimaryTable[] = {
	{ 0, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, 24, 32, 264, 240, 0 },
	{ 2, CONDITION_ALWAYS, 400, 0, 264, FACING_NORTH, 0, 256, 0, 496, 288, 0 },
	{ 3, CONDITION_ALWAYS, 688, 0, 264, FACING_NORTH, 0, 544, 0, 784, 288, 0 },
	{ 4, CONDITION_ALWAYS, 240, 0, 504, FACING_NORTH, 0, 72, 336, 312, 528, 0 },
	{ 5, CONDITION_ALWAYS, 576, 0, 536, FACING_NORTH, 0, 400, 304, 640, 552, 0 },
	{ 6, CONDITION_ALWAYS, 352, 0, 872, FACING_NORTH, 0, 0, 640, 400, 896, 0 },
	{ 7, CONDITION_ALWAYS, 656, 0, 696, FACING_WEST, 0, 432, 640, 848, 896, 0 },
	{ 8, CONDITION_ALWAYS, 344, 0, 696, FACING_EAST, 0, 0, 640, 400, 896, 0 },
	{ SCENE_TABLE_END },
};

u32 BiribinoHeya_SecondaryTable[] = {
	SCENE_EXITS(31),
	SCENE_EXIT(1, 30, 4),
	SCENE_EXIT(2, 30, 5),
	SCENE_EXIT(3, 30, 6),
	SCENE_EXIT(4, 30, 8),
	SCENE_EXIT(5, 30, 7),
	SCENE_EXIT(6, 30, 3),
	SCENE_EXIT(7, 31, 8),
	SCENE_EXIT(8, 31, 7),
	SCENE_EXITS_END,
};

struct ScenePlacement gBiribinoHeyaPlacements[] = {
	{ 0x6c, 0x0, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(168), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_WANDER,
		PIXELS(200), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(384), 0, PIXELS(192), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_WANDER,
		PIXELS(360), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(640), 0, PIXELS(156), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x66, 0x0, ACTOR_WANDER,
		PIXELS(704), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(456), 0, PIXELS(472), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(488), 0, PIXELS(424), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(200), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(168), 0, PIXELS(440), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(584), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x77, 0x0, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(760), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x84, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(760), FACING_NORTH, TALK_KEEP_FACING, 0 },
	{ 0x84, 0x0, ACTOR_STAND,
		PIXELS(56), 0, PIXELS(760), FACING_EAST, TALK_KEEP_FACING, 0 },
	{ 0x94, 0x0, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(776), FACING_NORTHEAST, TALK_KEEP_FACING, 0 },
	{ 0xab, 0x0, ACTOR_STAND,
		PIXELS(176), 0, PIXELS(744), FACING_SOUTH, TALK_KEEP_FACING, 0 },
	{ 0xab, 0x0, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(768), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, 0x0, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(832), FACING_NORTHWEST, TALK_KEEP_FACING, 0 },
	{ 0x87, 0x0, ACTOR_STAND,
		PIXELS(120), 0, PIXELS(796), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(840), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x90, 0x0, ACTOR_STAND,
		PIXELS(808), 0, PIXELS(774), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x81, 0x0, ACTOR_STAND,
		PIXELS(568), 0, PIXELS(768), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent BiribinoHeya_TertiaryTable[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_TALK, 8, 0x845, MSGNO(MsgBilibinThatTreeAtTheEntranceIs) },
	{ EVENT_TALK, 9, 0x845, MSGNO(MsgBilibinMccoyHadTheTreesCutDown) },
	{ EVENT_TALK, 10, 0x845, MSGNO(MsgBilibinTheSnowIsAlreadyPilingUp) },
	{ EVENT_TALK, 11, 0x845, MSGNO(MsgBilibinWeRarelySeeAnySnowAround) },
	{ EVENT_TALK, 12, 0x845, MSGNO(MsgBilibinHowArePeopleDoingInKolima) },
	{ EVENT_TALK, 13, 0x845, MSGNO(MsgBilibinIKnowThatGuyWhoTurned) },
	{ EVENT_TALK, 14, 0x845, MSGNO(MsgBilibinLongAgoTheLighthouseNearImil) },
	{ EVENT_TALK, 15, 0x845, MSGNO(MsgBilibinButEvenTheLegendaryWaterOf) },
	{ EVENT_TALK, 16, 0x845, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, 0x845, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, 0x845, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, 0x845, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, 0x845, MSGNO(MsgBilibinYouCantReachKolimaNoMatter) },
	{ EVENT_TALK, 21, 0x845, EVENT_SCRIPT(FieldScene_ConfigureActor21Scene) },
	{ EVENT_TALK, 22, 0x845, MSGNO(MsgBilibinHeyMisterHeadChefAreYou) },
	{ EVENT_TALK, 23, 0x845, MSGNO(MsgBilibinStartCookingAlready) },
	{ EVENT_TALK, 24, 0x845, EVENT_SCRIPT(EV24_TALK) },
	{ EVENT_TALK, 25, 0x845, MSGNO(MsgBilibinWeCameHereBecauseLordMccoy) },
	{ EVENT_TALK, 26, 0x845, MSGNO(MsgBilibinIThinkThatBigTreeDeep) },
	{ EVENT_TALK, 27, 0x845, EVENT_SCRIPT(FieldScene_RunActor27Sequence) },
	{ EVENT_TALK, 28, 0x845, MSGNO(MsgBilibinICantWaitToSeeTodays) },
	{ EVENT_TALK, 29, 0x845, MSGNO(MsgBilibinIWentToLordMccoyLooking) },
	{ EVENT_TALK, 30, 0x845, MSGNO(MsgBilibinIveHeardSongsThatTellOf) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBilibinThankGoodnessTheyStoppedConstructionOf) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgBilibinIAlwaysMissAltinInThe) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherSaysThatItsWarm) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinTheRoadToKolimaIsOpen) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIThoughtISawLightsIn) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBilibinHisEyesAreRedBecauseHe) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgBilibinThereWillBeFewerGuestsNow) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor21SequenceOnFlag300) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgBilibinImALittleSurprisedThatThe) },
	{ EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoBusinessHereSoIm) },
	{ EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyServedSoMuchFoodToday) },
	{ EVENT_TALK, 27, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunActor27Message) },
	{ EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgBilibinOooohThatsItITriedMy) },
	{ EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgBilibinTheIncidentWasResolvedBeforeI) },
	{ EVENT_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatThatEerieHowling) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x845, MSGNO(MsgBilibinThisIsAllLordMccoysFault) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x845, MSGNO(MsgBilibinAhLadyMccoyIWouldJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x845, EVENT_SCRIPT(EV10_PSYNERGY) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x845, MSGNO(MsgBilibinItMustBeDifficultToLive) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x845, MSGNO(MsgBilibinICantImagineThatEveryoneIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x845, MSGNO(MsgBilibinHeSeemedSoHappyAndThen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x845, MSGNO(MsgBilibinCouldTheWaterOfHermesSave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x845, MSGNO(MsgBilibinTheWaterOfHermesVanishedLong) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x845, MSGNO(MsgBilibinTheWorldWillNeverSeeSuch) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x845, MSGNO(MsgBilibinIWishICouldSeeAnother) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, 0x845, MSGNO(MsgBilibinOnlySomeoneReallyBraveWouldGo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, 0x845, MSGNO(MsgBilibinIDontBelieveInCursesFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, 0x845, MSGNO(MsgBilibinAnyTravelersHeadingEastAreStuck) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, 0x845, MSGNO(MsgBilibinFatherMotherIHopeTheyreAll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, 0x845, MSGNO(MsgBilibinShouldIJustForgetThatIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, 0x845, MSGNO(MsgBilibinIveHadAllTheCustomerComplaints) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, 0x845, MSGNO(MsgBilibinArghhhThatsItIJustWant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, 0x845, MSGNO(MsgBilibinICameToBilibinHopingTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, 0x845, MSGNO(MsgBilibinISeeWhyTheyCallThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, 0x845, MSGNO(MsgBilibinIDontCareWhoDoesIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, 0x845, MSGNO(MsgBilibinYippeeIsItReadyYetMy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, 0x845, MSGNO(MsgBilibinImGettingALittleScaredHearing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, 0x845, MSGNO(MsgBilibinItHasBeenSaidThatA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBilibinItTakesManyYearsForA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesIWishICouldSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderIfThereAreStill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherHasTraveledAllOver) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinTravelingMakesYoungWarriorsStronger) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinClothesMadeWithSilkAreSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoDoubtAboutItIts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBilibinWhatsTheBigDealWhyAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgBilibinAltinsGotGoodWeaponsButI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldGoOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyCallThemselvesWarriorsButYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgBilibinWellIWasOnlyActingScared) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgBilibinItdBeBetterForBusinessIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgBilibinIllGoToKolimaWhenThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgBilibinLetsSeeSoupSaladAndI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHeadChefWasSoLazy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgBilibinItUsedToTakeThemSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgBilibinWhenTheyStoppedBuildingThePalace) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldEatLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 27, CONDITION_ALWAYS, MSGNO(MsgBilibinIdBetterEatALotThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgBilibinTheFoodIsSuchAGood) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgBilibinIfAKidLikeThisCould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgBilibinItLooksLikeScaryThingsAre) },
	{ SEARCH(SEARCH_BARREL), 100, 0xf52, FIND_ITEM(ITEM_ANTIDOTE) },
	{ SEARCH(SEARCH_BED), 101, 0xf53, FIND_ITEM(ITEM_SLEEP_BOMB) },
	{ SEARCH(SEARCH_BARREL), 102, 0xf54, FIND_ITEM(0xc2) },
	{ SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinLooksLikeEggSoupSoftEggs)) },
	{ SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinFreshOmelettesBilibinoOmelettesUseWild)) },
	{ SEARCH(SEARCH_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresABookEntitledAllAbout)) },
	{ SCENE_EVENTS_END },
};
