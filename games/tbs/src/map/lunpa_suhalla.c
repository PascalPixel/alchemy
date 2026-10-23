/// @file
/// The buildings of Lunpa and the house at Suhalla Gate: one scene
/// (SceneId_LunpaSuhalla) whose rooms are told apart by the entrance the party
/// comes in by, and whose area name follows it:
///
/// - Lunpa: the three homes, the weapon and armor shop, the item shop, the inn
///   and the crate room, entered by the town's doors (lunpa.c) at
///   ROOM_FIRST_HOME to ROOM_INN and ROOM_CRATE_ROOM.
/// - Lunpa Temple: ROOM_TEMPLE from the town, and ROOM_TEMPLE_SECOND_ENTRANCE,
///   where a party defeated in Lunpa wakes.
/// - Gate to Suhalla: the house beside Suhalla Gate (suhalla_gate.c), entered
///   at ROOM_GATE_TO_SUHALLA, where a traveling Healer offers the Sanctum's
///   services.
///
/// A room's entrance number is also its exit kind and the trigger of its door
/// (ROOM_DOOR), so each door leads back out the way the party came in.
/// Scene_GetEvents gives the house at Suhalla Gate its own table, and Lunpa's
/// rooms the sealed town's (gInteriorSealedEvents) until Donpa's judgment
/// sets FLAG_DODONPA_LOCKED_UP, then the reopened town's
/// (gInteriorReopenedEvents). While the town is sealed the merchants neither
/// buy nor sell and the innkeeper offers only his home; talking to the inn's
/// chef or reading his mind tells of the prisoner he takes meals to
/// (FLAG_LUNPA_HEARD_OF_PRISONER). Once it reopens, the shops, the inn and the
/// temple serve across their counters.
///
/// A merchant or Healer serves only a leader who faces north across the
/// counter (FACING_IS_NORTH); otherwise they talk. The counters are blank
/// actors in front of the merchants (ACTOR_WEAPON_COUNTER and the others),
/// so talking across one reaches the merchant's event. Entering by
/// ROOM_GATE_TO_SUHALLA makes the house the place a defeated party wakes and
/// hides the location name.
///
/// Open questions:
/// - XXX (no evidence): what ROOM_NINTH_UNUSED was meant for, an entrance no
///   exit leads to; its camera box takes half the crate room and half the
///   house at Suhalla Gate, and it stands the leader in the empty dark between
///   them (https://alchemy.pascalpixel.com/places/lunpa-suhalla/map-9.png), so
///   the map holds no room of its own for it.

#include "game_state.h"
#include "script_layer.h"
#include "unit/map/lunpa_suhalla.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "lunpa_suhalla.h"
#include "map_ids.h"
#include "text/msg_ids.h"

static void Event_TalkToInnkeeper(void);
static void Event_TalkToChef(void);
static void Event_ReadChefsThoughts(void);
static void Event_AskWhetherDonpasGuest(void);
static void Event_TalkToHealer(void);
static void Event_ReadItemMerchantsThoughts(void);
static void Event_TalkToItemMerchant(void);
static void Event_ReadWeaponMerchantsThoughts(void);
static void Event_TalkToWeaponMerchant(void);
static void Event_ReadArmorMerchantsThoughts(void);
static void Event_TalkToArmorMerchant(void);
static void Event_TalkToTravelingHealer(void);

// clang-format off
const struct SceneEntrance gInteriorEntrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { ROOM_FIRST_HOME, CONDITION_ALWAYS, 192, 0, 184, FACING_NORTH, 0, 16, 0, 256, 224, 0 },
    { ROOM_SECOND_HOME, CONDITION_ALWAYS, 432, 0, 264, FACING_NORTH, 0, 296, 48, 536, 288, 0 },
    { ROOM_THIRD_HOME, CONDITION_ALWAYS, 160, 0, 584, FACING_NORTH, 0, 16, 400, 256, 624, 0 },
    { ROOM_ARMS_SHOP, CONDITION_ALWAYS, 464, 0, 616, FACING_NORTH, 0, 296, 384, 536, 640, 0 },
    { ROOM_ITEM_SHOP, CONDITION_ALWAYS, 704, 0, 584, FACING_NORTH, 0, 576, 352, 816, 608, 0 },
    { ROOM_INN, CONDITION_ALWAYS, 288, 0, 840, FACING_NORTH, 0, 40, 656, 368, 944, 0 },
    { ROOM_TEMPLE, CONDITION_ALWAYS, 688, 0, 280, FACING_NORTH, 0, 568, 32, 808, 304, 0 },
    { ROOM_CRATE_ROOM, CONDITION_ALWAYS, 560, 0, 880, FACING_NORTH, 0, 400, 656, 640, 912, 0 },
    { ROOM_NINTH_UNUSED, CONDITION_ALWAYS, 672, 0, 888, FACING_NORTH, 0, 520, 656, 760, 896, 0 },
    { ROOM_GATE_TO_SUHALLA, CONDITION_ALWAYS, 832, 0, 848, FACING_NORTH, 0, 680, 656, 920, 896, 0 },
    { ROOM_TEMPLE_SECOND_ENTRANCE, CONDITION_ALWAYS, 688, 0, 184, FACING_NORTH, 0, 568, 32, 808, 288, 0 },
    { SCENE_TABLE_END },
};

const u32 gInteriorExits[] = {
    SCENE_EXITS(LunpaSuhalla),
    SCENE_EXIT(ROOM_FIRST_HOME, Lunpa, LUNPA_FROM_FIRST_HOME),
    SCENE_EXIT(ROOM_SECOND_HOME, Lunpa, LUNPA_FROM_SECOND_HOME),
    SCENE_EXIT(ROOM_THIRD_HOME, Lunpa, LUNPA_FROM_THIRD_HOME),
    SCENE_EXIT(ROOM_ARMS_SHOP, Lunpa, LUNPA_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(ROOM_ITEM_SHOP, Lunpa, LUNPA_FROM_ITEM_SHOP),
    SCENE_EXIT(ROOM_INN, Lunpa, LUNPA_FROM_INN),
    SCENE_EXIT(ROOM_TEMPLE, Lunpa, LUNPA_FROM_TEMPLE),
    SCENE_EXIT(ROOM_CRATE_ROOM, Lunpa, LUNPA_FROM_CRATE_ROOM),
    SCENE_EXIT(ROOM_GATE_TO_SUHALLA, SuhallaGateCheckpoint, SUHALLA_GATE_CHECKPOINT_FROM_LUNPA_SUHALLA),
    SCENE_EXITS_END,
};

const struct ScenePlacement gInteriorPlacements[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(144), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(96), 0, PIXELS(128), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(392), 0, PIXELS(240), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(403), 0, PIXELS(138), FACING_EAST, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(160), 0, PIXELS(508), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(88), 0, PIXELS(584), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(376), 0, PIXELS(531), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(464), 0, PIXELS(531), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(720), 0, PIXELS(499), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(264), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(248), 0, PIXELS(808), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, ACTOR_WANDER,
      PIXELS(144), 0, PIXELS(888), FACING_NORTHWEST, TALK_FACE_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(152), 0, PIXELS(744), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(688), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(776), 0, PIXELS(800), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(376), 0, PIXELS(544), FACING_SOUTH - FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
      PIXELS(464), 0, PIXELS(544), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, ACTOR_STAND,
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
    ROOM_DOOR(ROOM_CRATE_ROOM),
    { EVENT_TALK, ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeASealed) },
    { EVENT_TALK, ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBSealed) },
    { EVENT_TALK, ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeASealed) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBSealed) },
    { EVENT_TALK, ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeASealed) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBSealed) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnClerkSealed) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerSealed) },
    { EVENT_TALK, ACTOR_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToChef) },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { EVENT_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToTravelingHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeASealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeBSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeASealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeBSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeASealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS,
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      EVENT_SCRIPT(Event_ReadChefsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
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
    ROOM_DOOR(ROOM_CRATE_ROOM),
    { EVENT_TALK, ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeAReopened) },
    { EVENT_TALK, ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBReopened) },
    { EVENT_TALK, ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeAReopened) },
    { EVENT_TALK, ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBReopened) },
    { EVENT_TALK, ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeAReopened) },
    { EVENT_TALK, ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBReopened) },
    { EVENT_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToItemMerchant) },
    { EVENT_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToWeaponMerchant) },
    { EVENT_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToArmorMerchant) },
    { EVENT_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToInnkeeper) },
    { EVENT_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, EVENT_SCRIPT(Event_AskWhetherDonpasGuest) },
    { EVENT_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerReopened) },
    { EVENT_TALK, ACTOR_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsChefReopened) },
    { EVENT_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(Event_TalkToHealer) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeAReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeBReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeAReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeBReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeAReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS,
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
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsChefReopenedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestReopenedThoughts) },
    { SEARCH(SEARCH_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SEARCH(SEARCH_BARREL), TRIGGER_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { SEARCH(SEARCH_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { SEARCH(SEARCH_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { SCENE_EVENTS_END },
};
// clang-format on

const struct SceneEvent gGateToSuhallaEvents[] = {
    ROOM_DOOR(ROOM_GATE_TO_SUHALLA),
    {EVENT_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS,
     EVENT_SCRIPT(Event_TalkToTravelingHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsTravelingPriestThoughts)},
    {SCENE_EVENTS_END},
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gInteriorEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gInteriorExits; }

/// The townsfolk, shopkeepers and counters of all three places.
const struct ScenePlacement *Scene_GetPlacements(void) {
  return gInteriorPlacements;
}

/// The events of Gate to Suhalla, or those of Lunpa's buildings before and
/// after Donpa's judgment.
const struct SceneEvent *Scene_GetEvents(void) {
  if (gGameState.entrance == ROOM_GATE_TO_SUHALLA) {
    return gGateToSuhallaEvents;
  }
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    return gInteriorReopenedEvents;
  }
  return gInteriorSealedEvents;
}

/// Lunpa, the inn: across the counter after Donpa's judgment, a room at the
/// inn; otherwise the innkeeper offers a room or, while Lunpa is sealed, a bed
/// at home.
static void Event_TalkToInnkeeper(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
      Engine_InnOpen(LUNPA_INN, ACTOR_INNKEEPER);
      return;
    }
  }
  Engine_EventStart();
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_EventSetMessage(MsgLunpaInnkeeperOffersRoom);
    Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaInnkeeperOffersOwnHome);
    Engine_EventAskYesNo(ACTOR_INNKEEPER, 0);
  }
  Engine_EventEnd();
}

/// Lunpa, the inn, before Donpa's judgment: the chef offers the story of
/// taking meals to the fortress; either way the party has heard of the
/// prisoner.
static void Event_TalkToChef(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgLunpaChefOffersStory);
  Engine_EventAskYesNo(ACTOR_CHEF, 0);
  Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
  Engine_EventEnd();
}

/// Lunpa, the inn: Mind Read on the chef, who pities the prisoner in the
/// dungeon.
static void Event_ReadChefsThoughts(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgLunpaChefThinksOfPrisoner);
  Engine_EventShowMessage(ACTOR_CHEF, 0);
  Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
  Engine_EventEnd();
}

/// Lunpa, the inn, after Donpa's judgment: a woman asks whether the party is
/// Donpa's guest.
static void Event_AskWhetherDonpasGuest(void) {
  Engine_EventStart();
  Engine_EventSetMessage(MsgLunpaInnClerkAsksAboutGuest);
  Engine_EventAskYesNo(ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, 0);
  Engine_EventEnd();
}

/// Lunpa Temple: across the counter, the Sanctum's services; otherwise the
/// Healer talks about Dodonpa.
static void Event_TalkToHealer(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_SanctumOpen(ACTOR_HEALER);
  } else if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_EventStart();
    Engine_EventSetMessage(MsgLunpaTemplePriestReopened);
    Engine_EventShowMessage(ACTOR_HEALER, 0);
    Engine_EventEnd();
  } else {
    Engine_EventStart();
    Engine_EventSetMessage(MsgLunpaTemplePriestSealed);
    Engine_EventShowMessage(ACTOR_HEALER, 0);
    Engine_EventEnd();
  }
}

/// Lunpa, the item shop: Mind Read on the merchant.
static void Event_ReadItemMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_EventStart();
    Engine_EventSetMessage(MsgLunpaIsntWeaponsVendors);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  } else {
    Engine_EventStart();
    Engine_EventSetMessage(MsgLunpaItemMerchantSealedThoughts);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Lunpa, the item shop: across the counter, the shop; otherwise small talk.
static void Event_TalkToItemMerchant(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_ShopOpen(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_EventStart();
    Engine_EventSetMessage(MsgLunpaItemMerchantReopened);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  } else {
    Engine_EventStart();
    Engine_EventSetMessage(MsgLunpaItemMerchantSealed);
    Engine_EventShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Engine_EventEnd();
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the weapon merchant.
static void Event_ReadWeaponMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_EventSetMessage(MsgLunpaWeaponMerchantReopenedThoughts);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaWeaponMerchantSealedThoughts);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the weapon shop; otherwise small talk.
static void Event_TalkToWeaponMerchant(void) {
  struct FieldActor *leader;
  s32 facing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Engine_ShopOpen(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
    } else {
      Engine_EventStart();
      Engine_EventSetMessage(MsgLunpaWeaponMerchantReopened);
      Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
      Engine_EventEnd();
    }
  } else {
    Engine_EventSetMessage(MsgLunpaWeaponMerchantSealed);
    Engine_EventShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the armor merchant.
static void Event_ReadArmorMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_EventSetMessage(MsgLunpaArmorMerchantReopenedThoughts);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  } else {
    Engine_EventSetMessage(MsgLunpaArmorMerchantSealedThoughts);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the armor shop; otherwise small talk.
static void Event_TalkToArmorMerchant(void) {
  struct FieldActor *leader;
  s32 facing;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Engine_ShopOpen(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
    } else {
      Engine_EventStart();
      Engine_EventSetMessage(MsgLunpaArmorMerchantReopened);
      Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
      Engine_EventEnd();
    }
  } else {
    Engine_EventSetMessage(MsgLunpaArmorMerchantSealed);
    Engine_EventShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Sets the buildings up: entered by Gate to Suhalla, the house is where a
/// defeated party wakes, and the counters are drawn behind their keepers.
s32 Scene_Initialize(void) {
  s16 entrance;

  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gGameState.entrance;
  if (entrance == ROOM_GATE_TO_SUHALLA) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    gGameState.savedScene = (s32)&SceneId_LunpaSuhalla;
    gGameState.savedEntrance = entrance;
  }
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_WEAPON_COUNTER), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ARMOR_COUNTER), 0);
  Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_ITEM_COUNTER), 0);
  return 0;
}

/// Gate to Suhalla: the traveling Healer offers the Sanctum's services across
/// the counter; otherwise he speaks of his good will.
static void Event_TalkToTravelingHealer(void) {
  struct FieldActor *leader;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    // BUG: the temple's Healer, not the traveling one. The Sanctum only takes
    // its keeper's portrait from the actor (Shop_ConfirmAct), and both Healers
    // are placed in this scene in the same sprite, so the player sees no
    // difference.
#ifdef BUGFIX
    Engine_SanctumOpen(ACTOR_TRAVELING_HEALER);
#else
    Engine_SanctumOpen(ACTOR_HEALER);
#endif
  } else {
    Engine_EventSetMessage(MsgLunpaTravelingPriest);
    Engine_EventShowMessage(ACTOR_TRAVELING_HEALER, 0);
  }
}
