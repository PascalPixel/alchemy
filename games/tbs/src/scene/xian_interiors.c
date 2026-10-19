#include "game_state.h"
#include "unit/scene/xian_interiors.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
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
#include "field_obj.h"
#include "flags.h"

enum {
  ACTOR_XIAN_WEAPON_MERCHANT = 14,
  ACTOR_XIAN_ARMOR_MERCHANT = 15,
  ACTOR_XIAN_ITEM_MERCHANT = 16,
  ACTOR_XIAN_INNKEEPER = 17,
  XIAN_WEAPON_STOCK = 16,
  XIAN_ARMOR_STOCK = 17,
  XIAN_ITEM_STOCK = 18,
  XIAN_INN = 5
};

/// The room reached from Master Feh's school (scene 61's exit 4), where Master
/// Feh, Feizhi and Hsu wait once Hsu is back: its own actors and events.
enum { XIAN_INTERIORS_ENTRANCE_FROM_SCHOOL = 8 };

/// The actors placed in that room, as its dialogue shows them: Master Feh
/// says Feizhi's prediction came true, Feizhi welcomes the party, and Hsu was
/// injured on his way from Lama Temple.
enum { ACTOR_FEH = ACTOR_FIRST_PLACED, ACTOR_FEIZHI, ACTOR_HSU };

// Shared source is included here in ROM order.
#include "common/effect_create.h"
#include "common/effect_spawn.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

/// Return arrival positions for the homes, shops and recovery room.
const struct SceneEntrance *XianInteriors_GetEntrances(void) {
  return gXianInteriors_Entrances;
}

/// These interiors have no region table.
const struct SceneRegion *XianInteriors_GetRegions(void) { return NULL; }

/// Return the connections to Xian and the surrounding rooms.
const u32 *XianInteriors_GetExits(void) { return gXianInteriors_Exits; }

/// The room behind the school places only Master Feh, Feizhi and Hsu.
const struct ScenePlacement *XianInteriors_GetPlacements(void) {
  if (gGameState.entrance == XIAN_INTERIORS_ENTRANCE_FROM_SCHOOL) {
    return gXianInteriors_SchoolRoomPlacements;
  }
  SceneTable_Prepare(gXianInteriors_Placements);
  return gXianInteriors_Placements;
}

/// Read the leader's facing before a conversation changes actor state.
static inline u32 XianInteriors_GetLeaderFacing(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  return leader->facing;
}

/// Open the weapon shop from its counter side, or discuss martial arts.
static void XianInteriors_TalkToWeaponMerchant(void) {
  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Engine_ShopOpen(XIAN_WEAPON_STOCK, ACTOR_XIAN_WEAPON_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Engine_EventSetMessage(MsgXianXianHasMartialArtsBut);
    } else {
      Engine_EventSetMessage(MsgXianHsusInjuryTaughtGoodLesson);
    }
    Engine_EventShowMessage(ACTOR_XIAN_WEAPON_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Open the armor shop from its counter side, or discuss armor and kung fu.
static void XianInteriors_TalkToArmorMerchant(void) {
  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Engine_ShopOpen(XIAN_ARMOR_STOCK, ACTOR_XIAN_ARMOR_MERCHANT);
  } else {
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Engine_EventSetMessage(MsgXianMartialArtistsCantUseHeavy);
    } else {
      Engine_EventSetMessage(MsgXianKungFuMakesQuickDoes);
    }
    Engine_EventShowMessage(ACTOR_XIAN_ARMOR_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Open the item shop or ask whether the party will cross the desert.
static void XianInteriors_TalkToItemMerchant(void) {

  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing >= FACING_NORTHWEST + 1 &&
      leaderFacing <= FACING_NORTHEAST - 1) {
    Engine_ShopOpen(XIAN_ITEM_STOCK, ACTOR_XIAN_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
    Engine_EventSetMessage(MsgXianXiansSpecialArmorNotSuited);
    Engine_EventShowMessage(ACTOR_XIAN_ITEM_MERCHANT, 0);
  } else {
    Engine_EventSetMessage(MsgXianDoPlanCrossDesertWarrior);
    Engine_EventAskYesNo(ACTOR_XIAN_ITEM_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Offer a room from the counter side, or discuss travel through Xian.
static void XianInteriors_TalkToInnkeeper(void) {

  u32 leaderFacing = XianInteriors_GetLeaderFacing();

  Engine_EventBegin();
  if (leaderFacing < FACING_SOUTHEAST || leaderFacing > FACING_NORTHEAST) {
    Engine_InnOpen(XIAN_INN, ACTOR_XIAN_INNKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_XIAN_HEARD_HSU_TRAPPED) == 0) {
      Engine_EventSetMessage(MsgXianWarriorsShouldLearnAboutTowns);
    } else {
      Engine_EventSetMessage(MsgXianWeHaveFewCustomersThese);
    }
    Engine_EventShowMessage(ACTOR_XIAN_INNKEEPER, 0);
  }
  Engine_EventEnd();
}

/// Ask the resident whether something is wrong on the Silk Road.
static void XianInteriors_AskAboutSilkRoad(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianSomethingWrongOnSilkRoad);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

/// Ask what the party knows about the monsters in Altin.
static void XianInteriors_AskAboutAltinMonsters(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianDidMonstersInAltinSpit);
  Engine_EventAskYesNo(12, 0);
  Engine_EventEnd();
}

/// Feizhi, when the party helped free Hsu, welcomes the party, looks at Hsu,
/// then thanks the party and Master Hama. "Oh, warrior! Welcome!"
static void XianInteriors_FeizhiThanksParty(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgXianWarriorWelcome);
  Engine_EventShowMessageAndWait(ACTOR_FEIZHI, 0, 20);
  Engine_ActorFaceActor(ACTOR_FEIZHI, ACTOR_HSU, 0);
  Engine_EventWait(60);
  Engine_ActorFaceActor(ACTOR_FEIZHI, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_FEIZHI, 0);
  Engine_EventEnd();
}

/// The room behind the school has its own events.
const struct SceneEvent *XianInteriors_GetEvents(void) {
  if (gGameState.entrance == XIAN_INTERIORS_ENTRANCE_FROM_SCHOOL) {
    return gXianInteriors_SchoolRoomEvents;
  }
  return gXianInteriors_Events;
}

/// Places the interiors' effect objects; in the room behind the school, Hsu
/// takes his pose.
s32 XianInteriors_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;
  // Restore the ambient effects shared by connected interior entrances.
  if (entrance == 4 || entrance == 7) {
    OverlayObject_CreateConfiguredObject(PIXELS(248), 0, PIXELS(417), 20);
  } else if (entrance == 6) {
    OverlayObject_CreateConfiguredObject(PIXELS(460), 0, PIXELS(568), 20);
    OverlayObject_CreateConfiguredObject(PIXELS(484), 0, PIXELS(568), 20);
  } else if (entrance == XIAN_INTERIORS_ENTRANCE_FROM_SCHOOL) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    Engine_ActorSetAnimation(ACTOR_HSU, 6);
  }
  return 0;
}

s32 gEffectScript0[] = {
    SC_WAIT(1),      SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),      SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),      SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POS(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

struct SceneEntrance gXianInteriors_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 128, 0, 184, FACING_NORTH, 0, 24, 8, 248, 208, 0},
    {2, CONDITION_ALWAYS, 448, 0, 200, FACING_NORTH, 0, 288, 8, 592, 224, 0},
    {3, CONDITION_ALWAYS, 736, 0, 200, FACING_NORTH, 0, 608, 8, 848, 224, 0},
    {4, CONDITION_ALWAYS, 160, 0, 472, FACING_NORTH, 0, 48, 288, 336, 496, 0},
    {5, CONDITION_ALWAYS, 472, 0, 360, FACING_EAST, 0, 384, 288, 672, 496, 0},
    {6, CONDITION_ALWAYS, 512, 0, 696, FACING_NORTH, 0, 352, 496, 656, 720, 0},
    {7, CONDITION_ALWAYS, 120, 0, 360, FACING_EAST, 0, 48, 288, 336, 496, 0},
    {8, CONDITION_ALWAYS, 120, 0, 696, FACING_NORTH, 0, 64, 544, 304, 744, 0},
    {SCENE_TABLE_END},
};

u32 gXianInteriors_Exits[] = {
    SCENE_EXITS(73),      SCENE_EXIT(1, 72, 5), SCENE_EXIT(2, 72, 6),
    SCENE_EXIT(3, 72, 7), SCENE_EXIT(4, 72, 8), SCENE_EXIT(5, 73, 7),
    SCENE_EXIT(6, 72, 9), SCENE_EXIT(7, 73, 5), SCENE_EXIT(8, 61, 4),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gXianInteriors_Placements[] = {
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

struct ScenePlacement gXianInteriors_SchoolRoomPlacements[] = {
    { 0x28, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(224), 0, PIXELS(656), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x27, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(200), 0, PIXELS(704), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x29, IF_FLAG_SET(FLAG_XIAN_HEARD_HSU_TRAPPED), ACTOR_STAND,
        PIXELS(224), 0, PIXELS(704), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gXianInteriors_Events[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_TALK, 8, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsTheMulberryHarvestIsOverNow) },
    { EVENT_TALK, 9, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsVeryHardWarriors) },
    { EVENT_TALK, 10, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianSilkIsSentOnSilk) },
    { EVENT_TALK, 11, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadEndsInTolbiIt) },
    { EVENT_TALK, 12, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFollowSilkRoadWhenYouSee) },
    { EVENT_TALK, 13, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereIsAMineInAltin) },
    { EVENT_TALK, 14, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToWeaponMerchant) },
    { EVENT_TALK, 15, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToArmorMerchant) },
    { EVENT_TALK, 16, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToItemMerchant) },
    { EVENT_TALK, 17, FLAG_XIAN_HEARD_HSU_TRAPPED, EVENT_SCRIPT(XianInteriors_TalkToInnkeeper) },
    { EVENT_TALK, 18, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsNormallyManyMerchantsComeToThe) },
    { EVENT_TALK, 19, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAllThePeopleWhoCameTo) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgXianWhereAreTheMerchantsNoOne) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsBringUsCoinsTheyMust) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_AskAboutSilkRoad) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsNoneCanPassThroughLamakanThis) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_AskAboutAltinMonsters) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMineInAltinFilledWith) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToWeaponMerchant) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToArmorMerchant) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToItemMerchant) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(XianInteriors_TalkToInnkeeper) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhenLamakanDesertBurnsItIs) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHotFoodIsGoodForCrossing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSoonMasterHammetWillComeHe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsFarmWorkIsHardOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSilkRoadIsFullOfSilk) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMerchantsSayTolbiIsVeryBeautiful) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsAltinHasManyMenAllScary) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsMiningIsVeryHardWorkIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsSomeSayFightingWithWeaponsIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsInWarriorBattlesYouFightWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsXianIsAVillageOfCraftsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenYouGoToANew) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsWhenManyMerchantsComeMyBusiness) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, FLAG_XIAN_HEARD_HSU_TRAPPED, MSGNO(MsgXianInteriorsThereAreFewerTravelersSoFewer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWhyDoesMasterHammetNotCome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMerchantsSellSilkItIsLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhiSaidTheDesertBecameEvil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheDesertWasVeryHotNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheVillageIsFloodedBecauseOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWaterMonstersHowTerribleForAltin) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasPummeledByHugeBoulders) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsWeShouldSellArmorToKung) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsYouLoseStrengthInHeatUse) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsCaravansCannotBringMerchantsHereWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsTheMerchantsDoNotComeBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsEatWarriorIWillAddMany) },
    { SEARCH(SEARCH_BARREL), 100, FLAG_XIAN_INTERIORS_ELIXIR_BARREL, FIND_ITEM(ITEM_ELIXIR) },
    { SEARCH(SEARCH_JAR), 101, FLAG_XIAN_INTERIORS_SLEEP_BOMB_JAR, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreMakingStirFryTheCabbage)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsTheXianGourmetTheBest)) },
    { SEARCH(SEARCH_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheyreSteamedDumplingsTheyreGreatPiping)) },
    { SEARCH(SEARCH_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsIsaacAndHisFriendsAreShocked)) },
    { SEARCH(SEARCH_OVEN), 204, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsHesMakingHotBeanCurdTastes)) },
    { 0xc5b3, 205, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreManyChopsticksButNot)) },
    { SEARCH(SEARCH_SHELF), 206, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsThereAreMedicinalHerbsTheyHave)) },
    { SEARCH(SEARCH_SHELF), 207, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsTheresAKimonoItLooksLike)) },
    { SEARCH(SEARCH_SHELF), 208, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianInteriorsItsJustASalesReportIt)) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gXianInteriors_SchoolRoomEvents[] = {
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_TALK, ACTOR_FEH, 0x8b3, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowI) },
    { EVENT_TALK, ACTOR_FEIZHI, 0x8b3, EVENT_SCRIPT(XianInteriors_FeizhiThanksParty) },
    { EVENT_TALK, ACTOR_HSU, 0x8b3, MSGNO(MsgXianInteriorsIWasInjuredByRocksOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, 0x8b3, MSGNO(MsgXianInteriorsFeizhisAbilityToSenseTroubleIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, 0x8b3, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, 0x8b3, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhen) },
    { EVENT_TALK, ACTOR_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisPredictionCameTrueNowIBelieve) },
    { EVENT_TALK, ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsHsuWasTrappedUnderTheRocks) },
    { EVENT_TALK, ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIWasInjuredByBouldersOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEH, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsFeizhisAbilityToForeseeIsReal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FEIZHI, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsMasterHamaHasGreatPowerSome) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HSU, CONDITION_ALWAYS, MSGNO(MsgXianInteriorsIKnewHelpWouldComeWhenI) },
    { SCENE_EVENTS_END },
};
// clang-format on
