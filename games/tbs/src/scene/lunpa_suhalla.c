/// @file
/// The buildings of Lunpa, and the house by the gate to Suhalla: homes, the
/// weapon, armor and item shops, the inn and the temple. The shops and the inn
/// only trade once Lunpa has reopened.

#include "game_state.h"
#include "field_event.h"
#include "unit/scene/lunpa_suhalla.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "lunpa_suhalla.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

static void Event_TalkToInnkeeper(void);
static void Event_TalkToChef(void);
static void Event_ReadChefsThoughts(void);
static void Event_TalkToInnClerk(void);
static void Event_TalkToTemplePriest(void);
static void Event_ReadItemMerchantsThoughts(void);
static void Event_TalkToItemMerchant(void);
static void Event_ReadWeaponMerchantsThoughts(void);
static void Event_TalkToWeaponMerchant(void);
static void Event_ReadArmorMerchantsThoughts(void);
static void Event_TalkToArmorMerchant(void);
static void Event_TalkToTravelingPriest(void);

// clang-format off
const struct SceneEntrance gInteriorEntrances[] = {
    { 0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { ROOM_FIRST_HOME, CONDITION_ALWAYS, 192, 0, 184, FACING_NORTH, 0, 16, 0, 256, 224, 0 },
    { ROOM_SECOND_HOME, CONDITION_ALWAYS, 432, 0, 264, FACING_NORTH, 0, 296, 48, 536, 288, 0 },
    { ROOM_THIRD_HOME, CONDITION_ALWAYS, 160, 0, 584, FACING_NORTH, 0, 16, 400, 256, 624, 0 },
    { ROOM_ARMS_SHOP, CONDITION_ALWAYS, 464, 0, 616, FACING_NORTH, 0, 296, 384, 536, 640, 0 },
    { ROOM_ITEM_SHOP, CONDITION_ALWAYS, 704, 0, 584, FACING_NORTH, 0, 576, 352, 816, 608, 0 },
    { ROOM_INN, CONDITION_ALWAYS, 288, 0, 840, FACING_NORTH, 0, 40, 656, 368, 944, 0 },
    { ROOM_TEMPLE, CONDITION_ALWAYS, 688, 0, 280, FACING_NORTH, 0, 568, 32, 808, 304, 0 },
    { ROOM_EIGHTH, CONDITION_ALWAYS, 560, 0, 880, FACING_NORTH, 0, 400, 656, 640, 912, 0 },
    { ROOM_NINTH, CONDITION_ALWAYS, 672, 0, 888, FACING_NORTH, 0, 520, 656, 760, 896, 0 },
    { ROOM_SUHALLA_GATE_HOUSE, CONDITION_ALWAYS, 832, 0, 848, FACING_NORTH, 0, 680, 656, 920, 896, 0 },
    { ROOM_TEMPLE_SECOND_ENTRANCE, CONDITION_ALWAYS, 688, 0, 184, FACING_NORTH, 0, 568, 32, 808, 288, 0 },
    { SCENE_TABLE_END },
};

const u32 gInteriorExits[] = {
    SCENE_EXITS(SCENE_LUNPA_SUHALLA),
    SCENE_EXIT(ROOM_FIRST_HOME, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_FIRST_HOME),
    SCENE_EXIT(ROOM_SECOND_HOME, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_SECOND_HOME),
    SCENE_EXIT(ROOM_THIRD_HOME, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_THIRD_HOME),
    SCENE_EXIT(ROOM_ARMS_SHOP, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_ARMS_SHOP),
    SCENE_EXIT(ROOM_ITEM_SHOP, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_ITEM_SHOP),
    SCENE_EXIT(ROOM_INN, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_INN),
    SCENE_EXIT(ROOM_TEMPLE, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_TEMPLE),
    SCENE_EXIT(ROOM_EIGHTH, SCENE_LUNPA, LUNPA_ENTRANCE_FROM_EIGHTH_ROOM),
    SCENE_EXIT(ROOM_SUHALLA_GATE_HOUSE, SCENE_SUHALLA_GATE, SUHALLA_GATE_ENTRANCE_FROM_LUNPA),
    SCENE_EXITS_END,
};

const struct ScenePlacement gInteriorPlacements[] = {
    { SPRITE_TOWNSPERSON_D, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(144), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_C, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(96), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_TOWNSPERSON_A, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(392), 0, PIXELS(240), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_F, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(403), 0, PIXELS(138), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_B, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(160), 0, PIXELS(508), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_E, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(88), 0, PIXELS(584), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WEAPON_MERCHANT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(376), 0, PIXELS(531), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_ARMOR_MERCHANT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(464), 0, PIXELS(531), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_ITEM_MERCHANT, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(720), 0, PIXELS(499), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_INNKEEPER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(264), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_F, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(248), 0, PIXELS(808), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_TOWNSPERSON_C, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(144), 0, PIXELS(888), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(152), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PRIEST, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(688), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_PRIEST, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(776), 0, PIXELS(800), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_COUNTER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(376), 0, PIXELS(544), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_COUNTER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(464), 0, PIXELS(544), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_COUNTER, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(720), 0, PIXELS(512), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct SceneEvent gInteriorSealedEvents[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_EIGHTH),
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeASealed) },
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBSealed) },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeASealed) },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBSealed) },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeASealed) },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBSealed) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_CLERK, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnClerkSealed) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerSealed) },
    { EVENT_TALK, ACTOR_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToChef) },
    { EVENT_TALK, ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTemplePriest) },
    { EVENT_TALK, ACTOR_TRAVELING_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTravelingPriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeASealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeBSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeASealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeBSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeASealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeBSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnkeeperSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_CLERK, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadChefsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestSealedThoughts) },
    { SEARCH(SEARCH_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), TRIGGER_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { SEARCH(SEARCH_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { SCENE_EVENTS_END },
};

const struct SceneEvent gInteriorReopenedEvents[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_EIGHTH),
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeAReopened) },
    { EVENT_TALK, ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBReopened) },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeAReopened) },
    { EVENT_TALK, ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBReopened) },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeAReopened) },
    { EVENT_TALK, ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBReopened) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_INN_CLERK, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnClerk) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerReopened) },
    { EVENT_TALK, ACTOR_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsChefReopened) },
    { EVENT_TALK, ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTemplePriest) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeAReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeBReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeAReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeBReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_A, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeAReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_THIRD_HOME_RESIDENT_B, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeBReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnkeeperReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_CLERK, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsChefReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TEMPLE_PRIEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestReopenedThoughts) },
    { SEARCH(SEARCH_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), TRIGGER_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { SEARCH(SEARCH_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { SCENE_EVENTS_END },
};
// clang-format on

const struct SceneEvent gSuhallaGateHouseEvents[] = {
    ROOM_DOOR(ROOM_SUHALLA_GATE_HOUSE),
    {EVENT_TALK, ACTOR_TRAVELING_PRIEST, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToTravelingPriest)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_PRIEST,
     CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsTravelingPriestThoughts)},
    {SCENE_EVENTS_END},
};

/// Where the party can enter each building.
const struct SceneEntrance *Scene_GetEntrances(void) {
  return gInteriorEntrances;
}

/// The buildings have no regions.
const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

/// Where each door leads.
const u32 *Scene_GetExits(void) { return gInteriorExits; }

/// The townsfolk, shopkeepers and the counters.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gInteriorPlacements;
}

/// The gate house's events, or the town's before and after it reopens.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.entrance == ROOM_SUHALLA_GATE_HOUSE) {
    return gSuhallaGateHouseEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    return gInteriorReopenedEvents;
  }
  return gInteriorSealedEvents;
}

/// Across the counter after Lunpa reopens, a room at the inn; otherwise the
/// innkeeper offers a room or, while Lunpa is sealed, a bed at home.
static void Event_TalkToInnkeeper(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
      Engine_InnOpen(INN_LUNPA, ACTOR_INNKEEPER);
      return;
    }
  }
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaInnkeeperOffersRoom);
    Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaInnkeeperOffersOwnHome);
    Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
  }
  Engine_EventEnd();
}

/// The chef offers the story of taking meals to the fortress; either way
/// the party has heard of the prisoner.
static void Event_TalkToChef(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaChefOffersStory);
  Engine_EventAskYesNo(ACTOR_CHEF, 0);
  Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
  Engine_EventEnd();
}

/// Mind Read on the chef, who pities the prisoner in the dungeon.
static void Event_ReadChefsThoughts(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaChefThinksOfPrisoner);
  Engine_EventShowMessage(ACTOR_CHEF, 0);
  Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
  Engine_EventEnd();
}

/// The inn's clerk asks whether the party is Donpa's guest.
static void Event_TalkToInnClerk(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLunpaInnClerkAsksAboutGuest);
  Engine_EventAskYesNo(ACTOR_INN_CLERK, 0);
  Engine_EventEnd();
}

/// Across the counter, the sanctum; otherwise the priest talks about
/// Dodonpa.
static void Event_TalkToTemplePriest(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_SanctumOpen(ACTOR_TEMPLE_PRIEST);
  } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgLunpaTemplePriestReopened);
    Engine_EventShowMessage(ACTOR_TEMPLE_PRIEST, 0);
    Engine_EventEnd();
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgLunpaTemplePriestSealed);
    Engine_EventShowMessage(ACTOR_TEMPLE_PRIEST, 0);
    Engine_EventEnd();
  }
}

/// Mind Read on the item merchant.
static void Event_ReadItemMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgLunpaIsntWeaponsVendors);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgLunpaItemMerchantSealedThoughts);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Across the counter, the item shop; otherwise small talk.
static void Event_TalkToItemMerchant(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_ShopOpen(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgLunpaItemMerchantReopened);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  } else {
    Engine_EventBegin();
    Engine_EventSetMessage(MsgLunpaItemMerchantSealed);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Mind Read on the weapon merchant.
static void Event_ReadWeaponMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaWeaponMerchantReopenedThoughts);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaWeaponMerchantSealedThoughts);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// After Lunpa reopens, across the counter, the weapon shop; otherwise
/// small talk.
static void Event_TalkToWeaponMerchant(void) {
  struct FieldActor *leader;
  s32 facing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Engine_ShopOpen(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
    } else {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgLunpaWeaponMerchantReopened);
      Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
      Engine_EventEnd();
    }
  } else {
    Engine_EventSetMessage(MsgLunpaWeaponMerchantSealed);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Mind Read on the armor merchant.
static void Event_ReadArmorMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    Engine_EventSetMessage(MsgLunpaArmorMerchantReopenedThoughts);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaArmorMerchantSealedThoughts);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// After Lunpa reopens, across the counter, the armor shop; otherwise
/// small talk.
static void Event_TalkToArmorMerchant(void) {
  struct FieldActor *leader;
  s32 facing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (Engine_GameFlagIsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Engine_ShopOpen(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
    } else {
      Engine_EventBegin();
      Engine_EventSetMessage(MsgLunpaArmorMerchantReopened);
      Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
      Engine_EventEnd();
    }
  } else {
    Engine_EventSetMessage(MsgLunpaArmorMerchantSealed);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Sets the buildings up: the gate house is where a defeated party wakes,
/// and the counters are drawn behind their keepers.
s32 Scene_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  entrance = gGameState.entrance;
  if (entrance == ROOM_SUHALLA_GATE_HOUSE) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    gGameState.savedScene = (s32)&SceneId_LunpaSuhalla;
    gGameState.savedEntrance = entrance;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WEAPON_COUNTER), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ARMOR_COUNTER), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ITEM_COUNTER), 0);
  return 0;
}

/// The traveling priest: across the counter, the temple priest's sanctum;
/// otherwise his good will.
static void Event_TalkToTravelingPriest(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_SanctumOpen(ACTOR_TEMPLE_PRIEST);
  } else {
    Engine_EventSetMessage(MsgLunpaTravelingPriest);
    Engine_EventShowMessage(ACTOR_TRAVELING_PRIEST, 0);
  }
}
