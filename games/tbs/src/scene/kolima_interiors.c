
#include "game_state.h"
#include "unit/scene/kolima_interiors.h"
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

#include "resource_390.h"
#include "resource_390_table.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"

static void SceneData_InitRecordTable(struct Resource390TableEntry *entry)
{
	u32			 i;
	u8	 v16;
	s32 v04;
	u16 def00;
	u16 alt00;

	i = 0;
	v16 = 2;
	v04 = 1;
	def00 = 0x69;
	alt00 = 0x6E;
	do
	{
		entry->unk16 = v16;
		entry->unk04 = v04;
		entry->unk00 = def00;
		if (i == 4 || i == 7)
		{
			entry->unk00 = alt00;
		}
		i++;
		entry = (struct Resource390TableEntry *)((u8 *)entry + 0x18);
	} while (i <= 0xE);
}

u8 *SceneData_GetPrimaryTable(void)
{
	return (u8 *)EntryLayout_PrimaryTable;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

u8 *SceneData_GetSecondaryTable(void)
{
	return (u8 *)EntryLayout_SecondaryTable;
}

u8 *SceneData_GetPreparedRecords(void)
{
	u8 *buf;

	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		SceneData_InitRecordTable((struct Resource390TableEntry *)EntryLayout_Records);
	}
	buf = (u8 *)EntryLayout_Records;
	FieldScene_PrepareActors(buf);
	return buf;
}

static void EV16_TALK(void)
{

	u32 dir = Object_GetById(0)->facing;

	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(13, 16);
	}
	else
	{
		Engine_EventSetMessage(MsgKolimaFuchinTempleOnOtherSide);
		Engine_EventShowMessage(16, 0);
	}

	Engine_EventEnd();
}

static void EV17_TALK(void)
{

	u32 dir = Object_GetById(0)->facing;

	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(14, 17);
	}
	else
	{
		Engine_EventSetMessage(MsgKolimaGrandBridgeAcrossRiverPride);
		Engine_EventShowMessage(17, 0);
	}

	Engine_EventEnd();
}

static void EV18_TALK(void)
{

	u32 dir = Object_GetById(0)->facing;

	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_ShopOpen(15, 18);
	}
	else
	{
		Engine_EventSetMessage(MsgKolimaWhenWasTreeLearnedAppreciate);
		Engine_EventShowMessage(18, 0);
	}

	Engine_EventEnd();
}

static void EV19_TALK(void)
{

	u32 dir = Object_GetById(0)->facing;

	Engine_EventBegin();

	if (dir + 0xFFFF5FFF <= 0x3FFE)
	{
		Engine_InnOpen(3, 19);
	}
	else
	{
		Engine_EventSetMessage(MsgKolimaTheySayMccoyHaltedConstruction);
		Engine_EventShowMessage(19, 0);
	}

	Engine_EventEnd();
}

u8 *SceneData_GetFourthTable(void)
{
	return (u8 *)EntryLayout_FourthTable;
}

s32 FieldScene_SetupEntryLayoutsBySelector(void)
{

	struct EventWork *work = gEventWork;
	s32				  id;
	s16				 *selection_p;
	u32				  selection;

	work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);

	if (Engine_GameFlagIsSet(0x845) == 0)
	{
		id = 8;
		do
		{
			struct FieldActor *record = Object_GetById(id);

			id++;
			Engine_ActorSetSpriteFlags(record, 0);
		} while ((u32)id <= 22);
	}

	{
		s32 off = 450;
		selection_p = (s16 *)((u8 *)&gGameState + off);
		selection = *(u16 *)selection_p;
	}

	if ((s16)selection == 7)
	{
		s32 arg5;
		s32 arg4;
		arg4 = 13;
		arg5 = 8;
		Map_CopyCellsTo(34, 34, 18, 16, arg4, arg5);
		Map_CopyCellsTo(34, 94, 18, 76, arg4, arg5);
		Map_CopyCellsTo(94, 34, 78, 16, arg4, arg5);
	}
	else if (((selection - 8) << 16) <= (128 << 9))
	{
		s32 arg5;
		s32 arg4;
		arg4 = 11;
		arg5 = 8;
		Map_CopyCellsTo(34, 43, 19, 23, arg4, arg5);
		Map_CopyCellsTo(34, 94, 19, 83, arg4, arg5);
		Map_CopyCellsTo(94, 34, 79, 23, arg4, arg5);
		Engine_ActorSetPosition(10, 0, 0);
		Engine_ActorSetPosition(11, 0, 0);
		Engine_ActorSetPosition(12, 0, 0);
	}
	return 0;
}

struct SceneEntrance EntryLayout_PrimaryTable[] = {
	{ 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 128, 0, 192, FACING_EAST, 0, 16, 56, 256, 216, 0 },
	{ 2, CONDITION_ALWAYS, 192, 0, 168, FACING_SOUTHWEST, 0, 16, 56, 256, 216, 0 },
	{ 3, CONDITION_ALWAYS, 392, 0, 200, FACING_NORTH, 0, 288, 64, 512, 232, 0 },
	{ 4, CONDITION_ALWAYS, 648, 0, 200, FACING_NORTH, 0, 528, 64, 768, 232, 0 },
	{ 5, CONDITION_ALWAYS, 704, 0, 168, FACING_SOUTHWEST, 0, 528, 64, 768, 224, 0 },
	{ 6, CONDITION_ALWAYS, 128, 0, 416, FACING_EAST, 0, 16, 280, 256, 440, 0 },
	{ 7, CONDITION_ALWAYS, 392, 0, 472, FACING_NORTH, 0, 288, 264, 512, 504, 0 },
	{ 8, CONDITION_ALWAYS, 392, 0, 328, FACING_SOUTH, 0, 288, 264, 512, 496, 0 },
	{ 9, CONDITION_ALWAYS, 392, 0, 344, FACING_NORTH, 0, 288, 264, 512, 496, 0 },
	{ 10, CONDITION_ALWAYS, 648, 0, 424, FACING_NORTH, 0, 528, 288, 768, 456, 0 },
	{ 11, CONDITION_ALWAYS, 704, 0, 392, FACING_SOUTHWEST, 0, 528, 288, 768, 448, 0 },
	{ 12, CONDITION_ALWAYS, 128, 0, 688, FACING_EAST, 0, 16, 552, 256, 712, 0 },
	{ 13, CONDITION_ALWAYS, 192, 0, 664, FACING_SOUTHWEST, 0, 16, 552, 256, 712, 0 },
	{ 14, CONDITION_ALWAYS, 400, 0, 688, FACING_NORTH, 0, 288, 552, 528, 712, 0 },
	{ SCENE_TABLE_END },
};

u32 EntryLayout_SecondaryTable[] = {
	SCENE_EXITS(37),
	SCENE_EXIT(1, 37, 11),
	SCENE_EXIT(2, 37, 14),
	SCENE_EXIT(3, 36, 4),
	SCENE_EXIT(4, 36, 3),
	SCENE_EXIT(5, 37, 12),
	SCENE_EXIT(6, 37, 13),
	SCENE_EXIT(7, 36, 5),
	SCENE_EXIT(8, 36, 7),
	SCENE_EXIT(9, 36, 6),
	SCENE_EXIT(10, 37, 1),
	SCENE_EXIT(11, 37, 5),
	SCENE_EXIT(12, 37, 6),
	SCENE_EXIT(13, 37, 2),
	SCENE_EXIT(14, 38, 1),
	SCENE_EXITS_END,
};

struct ScenePlacement EntryLayout_Records[] = {
	{ 0x67, 0x0, ACTOR_WANDER,
		PIXELS(166), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6f, 0x0, ACTOR_STAND,
		PIXELS(88), 0, PIXELS(144), FACING_EAST, 3, 0 },
	{ 0x65, 0x0, ACTOR_STAND,
		PIXELS(408), 0, PIXELS(472), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x73, 0x0, ACTOR_STAND,
		PIXELS(360), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, 3,
		PIXELS(440), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x74, 0x0, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0x6a, 0x0, ACTOR_STAND,
		PIXELS(448), 0, PIXELS(680), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x6b, 0x0, ACTOR_WANDER,
		PIXELS(424), 0, PIXELS(648), FACING_WEST, TALK_FACE_PARTY, 0 },
	{ 0x7c, 0x0, ACTOR_STAND,
		PIXELS(624), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x7d, 0x0, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x76, 0x0, ACTOR_STAND,
		PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x77, 0x0, ACTOR_STAND,
		PIXELS(648), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x80, 0x0, ACTOR_WANDER,
		PIXELS(624), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
	{ 0x74, 0x0, ACTOR_WANDER,
		PIXELS(120), 0, PIXELS(656), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ 0xab, 0x0, ACTOR_STAND,
		PIXELS(144), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent EntryLayout_FourthTable[] = {
	{ EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
	{ EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
	{ EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
	{ EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
	{ EVENT_EXIT, 9, CONDITION_ALWAYS, 9 },
	{ EVENT_EXIT, 10, CONDITION_ALWAYS, 10 },
	{ EVENT_EXIT, 11, CONDITION_ALWAYS, 11 },
	{ EVENT_EXIT, 12, CONDITION_ALWAYS, 12 },
	{ EVENT_EXIT, 13, CONDITION_ALWAYS, 13 },
	{ EVENT_EXIT, 14, CONDITION_ALWAYS, 14 },
	{ EVENT_TALK, 8, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsIHeardThatTravelingWarriorsFrom) },
	{ EVENT_TALK, 9, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsBigStrongWarriorsWithGreatBeards) },
	{ EVENT_TALK, 10, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsYouSavedKolimaWowThatMakes) },
	{ EVENT_TALK, 11, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsMyHusbandSeemsToBeKeeping) },
	{ EVENT_TALK, 12, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsDaddyToldMeHisSecret) },
	{ EVENT_TALK, 13, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsImNeverGoingToUseAn) },
	{ EVENT_TALK, 14, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsDadWillYouPleaseGoBack) },
	{ EVENT_TALK, 15, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsDadHowAreWeGoingTo) },
	{ EVENT_TALK, 16, IF_FLAG_SET(0x845), EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, IF_FLAG_SET(0x845), EVENT_SCRIPT(EV17_TALK) },
	{ EVENT_TALK, 18, IF_FLAG_SET(0x845), EVENT_SCRIPT(EV18_TALK) },
	{ EVENT_TALK, 19, IF_FLAG_SET(0x845), EVENT_SCRIPT(EV19_TALK) },
	{ EVENT_TALK, 20, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsWithNoMoreWorkHereKolima) },
	{ EVENT_TALK, 21, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsIveBeenALumberjackForYears) },
	{ EVENT_TALK, 22, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsImOnlyAMerchantButI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsIDontReallyKnowButYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsAllWarriorsAreBigAndAll) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsItsHiddenDeepInTheForest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsIThinkHeFoundSomethingValuable) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsStillTellingMeItsOutsideThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsPeopleTurnedIntoTreesBecauseOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsKnowingMyDadHellStartWorking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsHowAmISupposedToGrow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsFuchinTemplesMasterNyunpaIsSaid) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsItsAShameWeHadTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsItFeltSoGoodWhenMy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsTheVillageWasReallyBoomingWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsRemainingAQuietVillageMeansRemaining) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsImALumberjackAndImOkay) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, IF_FLAG_SET(0x845), MSGNO(MsgKolimaInteriorsIWishIdGrabbedSomeOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatAreTheyStaringAtThey) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIDreamtThatAMightyWarrior) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatWillHappenToMyTreasure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsSoonAllOfKolimaWillBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIWonderWhatFathersSecretIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsMaybeWeTurnedIntoTreesBecause) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsAtLeastMyFatherWasntThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheHolyTreeOfKolimaHas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIUsedToCarveWeaponsOut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsManImStuckInsideThatMeans) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsNoIDontWantToBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhoCausedAllThisSomeoneMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsHowAmISupposedToCharge) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheVillagersWereRightINever) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIfOnlyIdLeftTownA) },
	{ SEARCH(SEARCH_JAR), 100, 0xf59, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ SEARCH(SEARCH_BARREL), 101, 0xf5a, FIND_COINS(7) },
	{ SEARCH(SEARCH_BOOKCASE), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledTheHoly)) },
	{ SEARCH(SEARCH_BOOKCASE), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledDictionaryOf)) },
	{ SEARCH(SEARCH_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgKolimaInteriorsTheresABookEntitledTheArt)) },
	{ SCENE_EVENTS_END },
};
