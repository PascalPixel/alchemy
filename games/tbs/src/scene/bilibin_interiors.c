// Bilibin's shops, inn and dining room before and after Kolima's curse ends.
// The event table keeps the two sets of conversations in their native order.

#include "unit/scene/bilibin_interiors.h"
#include "flags.h"
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
#include "call.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"

enum {
  BILIBIN_WEAPON_SHOP = 7,
  BILIBIN_ARMOR_SHOP = 8,
  BILIBIN_ITEM_SHOP = 9,
  BILIBIN_INN = 2
};

// Actor slots follow the placement rows, starting at ACTOR_FIRST_PLACED.
enum {
  BILIBIN_MIA_ADMIRER = 10, // wants to see "that angelic" Mia again
  BILIBIN_WEAPON_SHOPKEEPER = 16,
  BILIBIN_ARMOR_SHOPKEEPER = 17,
  BILIBIN_ITEM_SHOPKEEPER = 18,
  BILIBIN_INNKEEPER = 19,
  BILIBIN_CHEF = 21,
  BILIBIN_KITCHEN_WORKER = 22,
  BILIBIN_WAITRESS = 23,
  BILIBIN_HUNGRY_DINER = 24,
  BILIBIN_DINER = 27
};

// Returns the entrances for the rooms in this overlay.
const struct SceneEntrance *BilibinInteriors_GetEntrances(void) {
  return gBilibinInteriorsEntrances;
}

// These rooms have no exit-region table.
const struct SceneRegion *BilibinInteriors_GetRegions(void) { return 0; }

// Returns the destination table used by the rooms' numbered exits.
const u32 *BilibinInteriors_GetExits(void) { return gBilibinInteriorsExits; }

// Marks placements outside the current room's view before actors are spawned.
const struct ScenePlacement *BilibinInteriors_GetPlacements(void) {
  ScenePlacement_ClipToView(gBilibinInteriorsPlacements);
  return gBilibinInteriorsPlacements;
}

// Opens the weapon shop from across the counter; otherwise discusses Kolima.
static void Event_TalkToWeaponShopkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(BILIBIN_WEAPON_SHOP, BILIBIN_WEAPON_SHOPKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_EventSetMessage(MsgBilibinThereWasAbleHealerIn);
    } else {
      Engine_EventSetMessage(MsgBilibinOurWeaponsBestCanFind);
    }
    Engine_EventShowMessage(BILIBIN_WEAPON_SHOPKEEPER, 0);
  }

  Engine_EventEnd();
}

// Opens the item shop from across the counter, or talks about the curse.
static void Event_TalkToItemShopkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(BILIBIN_ITEM_SHOP, BILIBIN_ITEM_SHOPKEEPER);
  } else {
    if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_EventSetMessage(MsgBilibinMustUsedTypesDangerBeing);
    } else {
      Engine_EventSetMessage(MsgBilibinTurnedOutWarriorsHiredBy);
    }
    Engine_EventShowMessage(BILIBIN_ITEM_SHOPKEEPER, 0);
  }

  Engine_EventEnd();
}

// Returns both story stages' events; conditions choose the current dialogue.
const struct SceneEvent *BilibinInteriors_GetEvents(void) {
  return gBilibinInteriorsEvents;
}

// Opens the armor shop across the counter, or asks about seeing the ocean.
static void Event_TalkToArmorShopkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();
  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_ShopOpen(BILIBIN_ARMOR_SHOP, BILIBIN_ARMOR_SHOPKEEPER);
  } else {
    if (GameFlag_IsSet(FLAG_KOLIMA_CURSE_LIFTED) == 0) {
      Engine_EventSetMessage(MsgBilibinHaveEverHeardOcean);
      Actor_FaceActor(BILIBIN_ARMOR_SHOPKEEPER, ACTOR_PARTY_LEADER, 0);
      Engine_EventWait(10);
      Event_AskYesNo(BILIBIN_ARMOR_SHOPKEEPER, 0);
      Actor_FaceDirection(BILIBIN_ARMOR_SHOPKEEPER,
                          FACING_SOUTHEAST + FACING_STEP, 10);
    } else {
      Engine_EventSetMessage(MsgBilibinForSomeReasonOceanFills);
      Event_ShowMessage(BILIBIN_ARMOR_SHOPKEEPER, 0);
    }
  }
  Engine_EventEnd();
}

// The chef cannot concentrate on cooking while his hometown is cursed.
static void Event_TalkToWorriedChef(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinImReallyWorriedAboutKolima);
  Actor_FaceActor(BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(BILIBIN_CHEF, 0);
  Actor_FaceDirection(BILIBIN_CHEF, FACING_NORTH, 10);
  Engine_EventEnd();
}

// A diner's growling stomach leads into a yes/no question about the sound.
static void Event_TalkToHungryDiner(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinGrrr);
  Event_ShowMessageAndWait(BILIBIN_HUNGRY_DINER, 0, 20);
  Actor_FaceActor(BILIBIN_HUNGRY_DINER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_OpenMessage(BILIBIN_HUNGRY_DINER, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(BILIBIN_HUNGRY_DINER, 0);
  Actor_FaceDirection(BILIBIN_HUNGRY_DINER, FACING_SOUTH, 10);
  Engine_EventEnd();
}

// Asks whether a tree spirit can curse a person, then turns back to the table.
static void Event_AskAboutTreeSpirit(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoBelieveTreeSpiritCan);
  Actor_FaceActor(BILIBIN_DINER, ACTOR_PARTY_LEADER, 0);
  Engine_EventWait(10);
  Event_OpenMessage(BILIBIN_DINER, 0);
  if (Engine_EventChooseYesNo(0, 0) != 0) {
    gEventWork->message++;
  }
  Event_ShowMessage(BILIBIN_DINER, 0);
  Actor_FaceDirection(BILIBIN_DINER, FACING_SOUTH, 10);
  Engine_EventEnd();
}

// Asks whether Kolima's holy tree survived.
static void Event_AskAboutHolyTree(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoKnowIfHolyTree);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

// Asks whether the party knows what silk is.
static void Event_AskAboutSilk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinDoKnowSilk);
  Engine_EventAskYesNo(13, 0);
  Engine_EventEnd();
}

// Opens the inn across the counter, or discusses the curse in Kolima.
static void Event_TalkToInnkeeper(void) {
  u32 leaderFacing;

  leaderFacing = Object_GetById(ACTOR_PARTY_LEADER)->facing;
  Engine_EventBegin();

  if (FACING_IS_NORTH(leaderFacing)) {
    Engine_InnOpen(BILIBIN_INN, BILIBIN_INNKEEPER);
  } else if (Engine_GameFlagIsSet(FLAG_KOLIMA_CURSE_LIFTED) != 0) {
    Engine_EventSetMessage(MsgBilibinWasntCurseInKolimaHorrifying);
    Engine_EventAskYesNo(BILIBIN_INNKEEPER, 0);
  } else {
    Engine_EventSetMessage(MsgBilibinCurseOnKolimaScaryDevelopment);
    Engine_EventShowMessage(BILIBIN_INNKEEPER, 0);
  }

  Engine_EventEnd();
}

// The chef calls out orders once, then refers diners to the waitress.
static void Event_TalkToBusyChef(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x300) == 0) {
    // The first time (0x300, kept while in the rooms), the chef shouts orders
    // and the kitchen worker answers.
    Engine_EventSetMessage(MsgBilibinTwoSpecialsOneDinnerOne);
    Event_ShowMessage(BILIBIN_CHEF, 0);
    Actor_FaceDirection(BILIBIN_CHEF, FACING_WEST, 20);
    Event_ShowMessage(BILIBIN_CHEF, 0);
    Engine_ActorStartRepeatedMotion(BILIBIN_KITCHEN_WORKER, 2);
    Actor_SetAttachedEffect(BILIBIN_KITCHEN_WORKER, 0x102);
    Engine_EventWait(60);
    Event_ShowMessage(BILIBIN_KITCHEN_WORKER, 0);
    Engine_EventWait(10);
    GameFlag_Set(0x300);
  }
  // Then, as every later time, he sends the party to the waitress.
  Actor_FaceActor(BILIBIN_CHEF, ACTOR_PARTY_LEADER, 0);
  Engine_EventSetMessage(MsgBilibinIfWantMealSpeakWaitress);
  Event_ShowMessage(BILIBIN_CHEF, 0);
  Actor_FaceDirection(BILIBIN_CHEF, FACING_NORTH, 10);
  Engine_EventEnd();
}

// The kitchen worker complains about the chef's renewed energy.
static void Event_TalkToKitchenWorker(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinReallyThinkHeadChefHas);
  Engine_EventShowMessage(BILIBIN_KITCHEN_WORKER, 0);
  Engine_ActorFaceActor(BILIBIN_KITCHEN_WORKER, ACTOR_PARTY_LEADER, 0);
  Engine_EventShowMessage(BILIBIN_KITCHEN_WORKER, 0);
  Engine_ActorFaceDirection(BILIBIN_KITCHEN_WORKER, FACING_EAST, 10);
  Engine_EventEnd();
}

// The waitress works through serving water and laying the silverware.
static void Event_TalkToWaitress(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinLetsSeeServeThemWater);
  Event_ShowMessage(BILIBIN_WAITRESS, 0);
  Actor_FaceActor(BILIBIN_WAITRESS, ACTOR_PARTY_LEADER, 0);
  Event_ShowMessage(BILIBIN_WAITRESS, 0);
  Actor_FaceDirection(BILIBIN_WAITRESS, FACING_NORTH, 10);
  Engine_EventEnd();
}

// A satisfied diner asks whether the party enjoys the food.
static void Event_TalkAboutMeal(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgBilibinWasSomeMealDontJust);
  Engine_EventAskYesNo(BILIBIN_DINER, 0);
  Engine_EventEnd();
}

// Mind Read on the villager who admires Mia: with Mia in the party (flags 0 to
// 7 say who is), even frozen Imil must feel warm with her around.
static void Event_ReadMiaAdmirersMind(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(MIA) != 0) {
    Engine_EventSetMessage(MsgBilibinEvenFrozenImilMustFeel);
  } else {
    Engine_EventSetMessage(MsgBilibinWhenSpringComesWantGo);
  }
  Engine_EventShowMessage(BILIBIN_MIA_ADMIRER, 0);
  Engine_EventEnd();
}

// The rooms open through a window; the diner at the dining room table is drawn
// at sprite priority 2, never by the automatic priority.
s32 BilibinInteriors_Initialize(void) {
  struct FieldActor *actor;
  u8 *record;
  s32 bits;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  actor = Object_GetById(BILIBIN_DINER);

  // priorityFlags and the image's priority bits (2 to 3 of byte 9), as bytes.
  ((u8 *)actor)[0x23] = bits = 0;
  record = (u8 *)actor->sprite;
  bits -= 13;
  bits &= record[9];
  bits |= 8;
  record[9] = bits;
  return 0;
}

struct SceneEntrance gBilibinInteriorsEntrances[] = {
    {0, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 176, 0, 216, FACING_NORTH, 0, 24, 32, 264, 240, 0},
    {2, CONDITION_ALWAYS, 400, 0, 264, FACING_NORTH, 0, 256, 0, 496, 288, 0},
    {3, CONDITION_ALWAYS, 688, 0, 264, FACING_NORTH, 0, 544, 0, 784, 288, 0},
    {4, CONDITION_ALWAYS, 240, 0, 504, FACING_NORTH, 0, 72, 336, 312, 528, 0},
    {5, CONDITION_ALWAYS, 576, 0, 536, FACING_NORTH, 0, 400, 304, 640, 552, 0},
    {6, CONDITION_ALWAYS, 352, 0, 872, FACING_NORTH, 0, 0, 640, 400, 896, 0},
    {7, CONDITION_ALWAYS, 656, 0, 696, FACING_WEST, 0, 432, 640, 848, 896, 0},
    {8, CONDITION_ALWAYS, 344, 0, 696, FACING_EAST, 0, 0, 640, 400, 896, 0},
    {SCENE_TABLE_END},
};

u32 gBilibinInteriorsExits[] = {
    SCENE_EXITS(31),      SCENE_EXIT(1, 30, 4), SCENE_EXIT(2, 30, 5),
    SCENE_EXIT(3, 30, 6), SCENE_EXIT(4, 30, 8), SCENE_EXIT(5, 30, 7),
    SCENE_EXIT(6, 30, 3), SCENE_EXIT(7, 31, 8), SCENE_EXIT(8, 31, 7),
    SCENE_EXITS_END,
};

// clang-format off
struct ScenePlacement gBilibinInteriorsPlacements[] = {
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

struct SceneEvent gBilibinInteriorsEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_EXIT, 6, CONDITION_ALWAYS, 6 },
    { EVENT_EXIT, 7, CONDITION_ALWAYS, 7 },
    { EVENT_EXIT, 8, CONDITION_ALWAYS, 8 },
    { EVENT_TALK, 8, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThatTreeAtTheEntranceIs) },
    { EVENT_TALK, 9, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinMccoyHadTheTreesCutDown) },
    { EVENT_TALK, BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheSnowIsAlreadyPilingUp) },
    { EVENT_TALK, 11, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeRarelySeeAnySnowAround) },
    { EVENT_TALK, 12, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHowArePeopleDoingInKolima) },
    { EVENT_TALK, 13, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIKnowThatGuyWhoTurned) },
    { EVENT_TALK, 14, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinLongAgoTheLighthouseNearImil) },
    { EVENT_TALK, 15, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinButEvenTheLegendaryWaterOf) },
    { EVENT_TALK, BILIBIN_WEAPON_SHOPKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToWeaponShopkeeper) },
    { EVENT_TALK, BILIBIN_ARMOR_SHOPKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToArmorShopkeeper) },
    { EVENT_TALK, BILIBIN_ITEM_SHOPKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToItemShopkeeper) },
    { EVENT_TALK, BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, 20, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYouCantReachKolimaNoMatter) },
    { EVENT_TALK, BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToWorriedChef) },
    { EVENT_TALK, BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeyMisterHeadChefAreYou) },
    { EVENT_TALK, BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinStartCookingAlready) },
    { EVENT_TALK, BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_TalkToHungryDiner) },
    { EVENT_TALK, 25, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinWeCameHereBecauseLordMccoy) },
    { EVENT_TALK, 26, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIThinkThatBigTreeDeep) },
    { EVENT_TALK, BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_AskAboutTreeSpirit) },
    { EVENT_TALK, 28, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantWaitToSeeTodays) },
    { EVENT_TALK, 29, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWentToLordMccoyLooking) },
    { EVENT_TALK, 30, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHeardSongsThatTellOf) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutHolyTree) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgBilibinThankGoodnessTheyStoppedConstructionOf) },
    { EVENT_TALK, BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIAlwaysMissAltinInThe) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherSaysThatItsWarm) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgBilibinTheRoadToKolimaIsOpen) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskAboutSilk) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgBilibinIThoughtISawLightsIn) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgBilibinHisEyesAreRedBecauseHe) },
    { EVENT_TALK, BILIBIN_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponShopkeeper) },
    { EVENT_TALK, BILIBIN_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorShopkeeper) },
    { EVENT_TALK, BILIBIN_ITEM_SHOPKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemShopkeeper) },
    { EVENT_TALK, BILIBIN_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgBilibinThereWillBeFewerGuestsNow) },
    { EVENT_TALK, BILIBIN_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToBusyChef) },
    { EVENT_TALK, BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToKitchenWorker) },
    { EVENT_TALK, BILIBIN_WAITRESS, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWaitress) },
    { EVENT_TALK, BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinImALittleSurprisedThatThe) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoBusinessHereSoIm) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyServedSoMuchFoodToday) },
    { EVENT_TALK, BILIBIN_DINER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkAboutMeal) },
    { EVENT_TALK, 28, CONDITION_ALWAYS, MSGNO(MsgBilibinOooohThatsItITriedMy) },
    { EVENT_TALK, 29, CONDITION_ALWAYS, MSGNO(MsgBilibinTheIncidentWasResolvedBeforeI) },
    { EVENT_TALK, 30, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderWhatThatEerieHowling) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinThisIsAllLordMccoysFault) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAhLadyMccoyIWouldJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_MIA_ADMIRER, FLAG_KOLIMA_CURSE_LIFTED, EVENT_SCRIPT(Event_ReadMiaAdmirersMind) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItMustBeDifficultToLive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICantImagineThatEveryoneIn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinHeSeemedSoHappyAndThen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinCouldTheWaterOfHermesSave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWaterOfHermesVanishedLong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WEAPON_SHOPKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinTheWorldWillNeverSeeSuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ARMOR_SHOPKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIWishICouldSeeAnother) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ITEM_SHOPKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinOnlySomeoneReallyBraveWouldGo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_INNKEEPER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontBelieveInCursesFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinAnyTravelersHeadingEastAreStuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_CHEF, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinFatherMotherIHopeTheyreAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_KITCHEN_WORKER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinShouldIJustForgetThatIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WAITRESS, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIveHadAllTheCustomerComplaints) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_HUNGRY_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinArghhhThatsItIJustWant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinICameToBilibinHopingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinISeeWhyTheyCallThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinIDontCareWhoDoesIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinYippeeIsItReadyYetMy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinImGettingALittleScaredHearing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, FLAG_KOLIMA_CURSE_LIFTED, MSGNO(MsgBilibinItHasBeenSaidThatA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgBilibinItTakesManyYearsForA) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgBilibinSometimesIWishICouldSee) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_MIA_ADMIRER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWonderIfThereAreStill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgBilibinMyFatherHasTraveledAllOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgBilibinTravelingMakesYoungWarriorsStronger) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgBilibinClothesMadeWithSilkAreSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgBilibinTheresNoDoubtAboutItIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgBilibinWhatsTheBigDealWhyAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WEAPON_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinAltinsGotGoodWeaponsButI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ARMOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldGoOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_ITEM_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinTheyCallThemselvesWarriorsButYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgBilibinWellIWasOnlyActingScared) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgBilibinItdBeBetterForBusinessIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_CHEF, CONDITION_ALWAYS, MSGNO(MsgBilibinIllGoToKolimaWhenThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_KITCHEN_WORKER, CONDITION_ALWAYS, MSGNO(MsgBilibinLetsSeeSoupSaladAndI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgBilibinTheHeadChefWasSoLazy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_HUNGRY_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinItUsedToTakeThemSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgBilibinWhenTheyStoppedBuildingThePalace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgBilibinIWishICouldEatLike) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), BILIBIN_DINER, CONDITION_ALWAYS, MSGNO(MsgBilibinIdBetterEatALotThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 28, CONDITION_ALWAYS, MSGNO(MsgBilibinTheFoodIsSuchAGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 29, CONDITION_ALWAYS, MSGNO(MsgBilibinIfAKidLikeThisCould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 30, CONDITION_ALWAYS, MSGNO(MsgBilibinItLooksLikeScaryThingsAre) },
    { SEARCH(SEARCH_BARREL), 100, 0xf52, FIND_ITEM(ITEM_ANTIDOTE) },
    { SEARCH(SEARCH_BED), 101, 0xf53, FIND_ITEM(ITEM_SLEEP_BOMB) },
    { SEARCH(SEARCH_BARREL), 102, 0xf54, FIND_ITEM(ITEM_HARD_NUT) },
    { SEARCH(SEARCH_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinLooksLikeEggSoupSoftEggs)) },
    { SEARCH(SEARCH_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinFreshOmelettesBilibinoOmelettesUseWild)) },
    { SEARCH(SEARCH_BOOKCASE), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgBilibinTheresABookEntitledAllAbout)) },
    { SCENE_EVENTS_END },
};
// clang-format on
