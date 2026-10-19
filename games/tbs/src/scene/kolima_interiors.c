/// @file
/// Kolima's houses and shops, whose residents stand as trees while the curse
/// lasts.

#include "game_state.h"
#include "flags.h"
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

#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "item_ids.h"

enum KolimaInteriorActor {
  ACTOR_FIRST_VILLAGER = 8,
  ACTOR_WEAPON_MERCHANT = 16,
  ACTOR_ARMOR_MERCHANT = 17,
  ACTOR_ITEM_MERCHANT = 18,
  ACTOR_INNKEEPER = 19,
  ACTOR_LAST_VILLAGER = 22
};

/// Replace the cursed residents' sprites and disable their movement and
/// turning.
static void
KolimaInteriors_TurnVillagersIntoTrees(struct ScenePlacement *placement) {
  u32 villagerIndex;
  u8 talkFacing;
  s32 behavior;
  u16 treeSprite;
  u16 alternateTreeSprite;

  villagerIndex = 0;
  talkFacing = TALK_KEEP_FACING;
  behavior = ACTOR_STAND;
  treeSprite = 0x69;
  alternateTreeSprite = 0x6E;
  do {
    placement->talkFacing = talkFacing;
    placement->behavior = behavior;
    placement->sprite = treeSprite;
    if (villagerIndex == 4 || villagerIndex == 7) {
      placement->sprite = alternateTreeSprite;
    }
    villagerIndex++;
    placement++;
  } while (villagerIndex < ACTOR_LAST_VILLAGER - ACTOR_FIRST_VILLAGER + 1);
}

/// Return the entrances for Kolima's houses and shops.
const struct SceneEntrance *KolimaInteriors_GetEntrances(void) {
  return gKolimaInteriors_Entrances;
}

/// Kolima's interiors have no scene regions.
const struct SceneRegion *KolimaInteriors_GetRegions(void) { return NULL; }

/// Return the doors and stairs linking the interiors.
const u32 *KolimaInteriors_GetExits(void) { return gKolimaInteriors_Exits; }

/// Apply the curse before preparing the residents' placement records.
const struct ScenePlacement *KolimaInteriors_GetPlacements(void) {
  struct ScenePlacement *placements;

  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    KolimaInteriors_TurnVillagersIntoTrees(gKolimaInteriors_Placements);
  }
  placements = gKolimaInteriors_Placements;
  FieldScene_PrepareActors(placements);
  return placements;
}

/// Offer weapons at the counter or discuss Fuchin Temple.
static void KolimaInteriors_TalkToWeaponMerchant(void) {
  u32 facing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (FACING_IS_NORTH(facing)) {
    Engine_ShopOpen(13, ACTOR_WEAPON_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgKolimaFuchinTempleOnOtherSide);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Offer armor at the counter or discuss the bridge.
static void KolimaInteriors_TalkToArmorMerchant(void) {
  u32 facing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (FACING_IS_NORTH(facing)) {
    Engine_ShopOpen(14, ACTOR_ARMOR_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgKolimaGrandBridgeAcrossRiverPride);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Offer items at the counter or discuss life after the curse.
static void KolimaInteriors_TalkToItemMerchant(void) {
  u32 facing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();
  if (FACING_IS_NORTH(facing)) {
    Engine_ShopOpen(15, ACTOR_ITEM_MERCHANT);
  } else {
    Engine_EventSetMessage(MsgKolimaWhenWasTreeLearnedAppreciate);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
  }
  Engine_EventEnd();
}

/// Offer a room at the counter or discuss McCoy's construction.
static void KolimaInteriors_TalkToInnkeeper(void) {
  u32 facing = Object_GetById(ACTOR_PARTY_LEADER)->facing;

  Engine_EventBegin();

  if (FACING_IS_NORTH(facing)) {
    Engine_InnOpen(3, ACTOR_INNKEEPER);
  } else {
    Engine_EventSetMessage(MsgKolimaTheySayMccoyHaltedConstruction);
    Engine_EventShowMessage(ACTOR_INNKEEPER, 0);
  }

  Engine_EventEnd();
}

/// Return the residents' conversations, thoughts and searchable objects.
const struct SceneEvent *KolimaInteriors_GetEvents(void) {
  return gKolimaInteriors_Events;
}

/// Clear resident sprite flags and select the room layout for this entrance.
s32 KolimaInteriors_Initialize(void) {
  struct EventWork *work = gEventWork;
  s32 actorId;
  u32 entrance;

  work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);

  // The residents remain visible as trees, with their sprite flags cleared.
  if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
    actorId = ACTOR_FIRST_VILLAGER;
    do {
      struct FieldActor *actor = Object_GetById(actorId);

      actorId++;
      Engine_ActorSetSpriteFlags(actor, 0);
    } while ((u32)actorId <= ACTOR_LAST_VILLAGER);
  }

  entrance = (u16)gGameState.entrance;

  // Entrance 7 uses one layout; entrances 8 and 9 use the other.
  // The unsigned packed comparison below tests the low 16-bit difference.
  if ((s16)entrance == 7) {
    s32 height;
    s32 width;
    width = 13;
    height = 8;
    Map_CopyCellsTo(34, 34, 18, 16, width, height);
    Map_CopyCellsTo(34, 94, 18, 76, width, height);
    Map_CopyCellsTo(94, 34, 78, 16, width, height);
  } else if (((entrance - 8) << 16) <= (1u << 16)) {
    s32 height;
    s32 width;
    width = 11;
    height = 8;
    Map_CopyCellsTo(34, 43, 19, 23, width, height);
    Map_CopyCellsTo(34, 94, 19, 83, width, height);
    Map_CopyCellsTo(94, 34, 79, 23, width, height);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
  }
  return 0;
}

struct SceneEntrance gKolimaInteriors_Entrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 128, 0, 192, FACING_EAST, 0, 16, 56, 256, 216, 0},
    {2, CONDITION_ALWAYS, 192, 0, 168, FACING_SOUTHWEST, 0, 16, 56, 256, 216,
     0},
    {3, CONDITION_ALWAYS, 392, 0, 200, FACING_NORTH, 0, 288, 64, 512, 232, 0},
    {4, CONDITION_ALWAYS, 648, 0, 200, FACING_NORTH, 0, 528, 64, 768, 232, 0},
    {5, CONDITION_ALWAYS, 704, 0, 168, FACING_SOUTHWEST, 0, 528, 64, 768, 224,
     0},
    {6, CONDITION_ALWAYS, 128, 0, 416, FACING_EAST, 0, 16, 280, 256, 440, 0},
    {7, CONDITION_ALWAYS, 392, 0, 472, FACING_NORTH, 0, 288, 264, 512, 504, 0},
    {8, CONDITION_ALWAYS, 392, 0, 328, FACING_SOUTH, 0, 288, 264, 512, 496, 0},
    {9, CONDITION_ALWAYS, 392, 0, 344, FACING_NORTH, 0, 288, 264, 512, 496, 0},
    {10, CONDITION_ALWAYS, 648, 0, 424, FACING_NORTH, 0, 528, 288, 768, 456, 0},
    {11, CONDITION_ALWAYS, 704, 0, 392, FACING_SOUTHWEST, 0, 528, 288, 768, 448,
     0},
    {12, CONDITION_ALWAYS, 128, 0, 688, FACING_EAST, 0, 16, 552, 256, 712, 0},
    {13, CONDITION_ALWAYS, 192, 0, 664, FACING_SOUTHWEST, 0, 16, 552, 256, 712,
     0},
    {14, CONDITION_ALWAYS, 400, 0, 688, FACING_NORTH, 0, 288, 552, 528, 712, 0},
    {SCENE_TABLE_END},
};

u32 gKolimaInteriors_Exits[] = {
    SCENE_EXITS(37),       SCENE_EXIT(1, 37, 11), SCENE_EXIT(2, 37, 14),
    SCENE_EXIT(3, 36, 4),  SCENE_EXIT(4, 36, 3),  SCENE_EXIT(5, 37, 12),
    SCENE_EXIT(6, 37, 13), SCENE_EXIT(7, 36, 5),  SCENE_EXIT(8, 36, 7),
    SCENE_EXIT(9, 36, 6),  SCENE_EXIT(10, 37, 1), SCENE_EXIT(11, 37, 5),
    SCENE_EXIT(12, 37, 6), SCENE_EXIT(13, 37, 2), SCENE_EXIT(14, 38, 1),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gKolimaInteriors_Placements[] = {
    { 0x67, 0, ACTOR_WANDER,
        PIXELS(166), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6f, 0, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(144), FACING_EAST, 3, 0 },
    { 0x65, 0, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(472), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x73, 0, ACTOR_STAND,
        PIXELS(360), 0, PIXELS(440), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6b, 0, 3,
        PIXELS(440), 0, PIXELS(456), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x74, 0, ACTOR_STAND,
        PIXELS(376), 0, PIXELS(616), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, 0, ACTOR_STAND,
        PIXELS(448), 0, PIXELS(680), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x6b, 0, ACTOR_WANDER,
        PIXELS(424), 0, PIXELS(648), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x7c, 0, ACTOR_STAND,
        PIXELS(624), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x7d, 0, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(360), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x76, 0, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x77, 0, ACTOR_STAND,
        PIXELS(648), 0, PIXELS(136), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x80, 0, ACTOR_WANDER,
        PIXELS(624), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x74, 0, ACTOR_WANDER,
        PIXELS(120), 0, PIXELS(656), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xab, 0, ACTOR_STAND,
        PIXELS(144), 0, PIXELS(368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKolimaInteriors_Events[] = {
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
    { EVENT_TALK, 8, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIHeardThatTravelingWarriorsFrom) },
    { EVENT_TALK, 9, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsBigStrongWarriorsWithGreatBeards) },
    { EVENT_TALK, 10, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsYouSavedKolimaWowThatMakes) },
    { EVENT_TALK, 11, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsMyHusbandSeemsToBeKeeping) },
    { EVENT_TALK, 12, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDaddyToldMeHisSecret) },
    { EVENT_TALK, 13, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImNeverGoingToUseAn) },
    { EVENT_TALK, 14, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDadWillYouPleaseGoBack) },
    { EVENT_TALK, 15, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsDadHowAreWeGoingTo) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(KolimaInteriors_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(KolimaInteriors_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(KolimaInteriors_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_INNKEEPER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), EVENT_SCRIPT(KolimaInteriors_TalkToInnkeeper) },
    { EVENT_TALK, 20, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsWithNoMoreWorkHereKolima) },
    { EVENT_TALK, 21, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIveBeenALumberjackForYears) },
    { EVENT_TALK, 22, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImOnlyAMerchantButI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIDontReallyKnowButYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsAllWarriorsAreBigAndAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItsHiddenDeepInTheForest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIThinkHeFoundSomethingValuable) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsStillTellingMeItsOutsideThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsPeopleTurnedIntoTreesBecauseOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsKnowingMyDadHellStartWorking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsHowAmISupposedToGrow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsFuchinTemplesMasterNyunpaIsSaid) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItsAShameWeHadTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsItFeltSoGoodWhenMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsTheVillageWasReallyBoomingWith) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsRemainingAQuietVillageMeansRemaining) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsImALumberjackAndImOkay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, IF_FLAG_SET(FLAG_KOLIMA_CURSE_LIFTED), MSGNO(MsgKolimaInteriorsIWishIdGrabbedSomeOf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatAreTheyStaringAtThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIDreamtThatAMightyWarrior) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhatWillHappenToMyTreasure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsSoonAllOfKolimaWillBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIWonderWhatFathersSecretIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsMaybeWeTurnedIntoTreesBecause) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsAtLeastMyFatherWasntThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsTheHolyTreeOfKolimaHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsIUsedToCarveWeaponsOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsManImStuckInsideThatMeans) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsNoIDontWantToBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgKolimaInteriorsWhoCausedAllThisSomeoneMust) },
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
// clang-format on
