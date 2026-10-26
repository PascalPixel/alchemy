/// @file
/// Rock rooms (SceneId_RockRooms), a sheet of rooms cut into rock: the game
/// packs rooms that share a tile style onto one map, and this one holds the
/// buildings of Lunpa and the house at Suhalla Gate. Its rooms are told apart
/// by the entrance the party comes in by, and the area name follows it:
///
/// - Lunpa: the three homes, the weapon and armor shop, the item shop, the inn
///   and the crate room, entered by the town's doors (lunpa.c) at
///   ROOM_FIRST_HOME to ROOM_INN and ROOM_CRATE_ROOM.
/// - Lunpa Temple: ROOM_TEMPLE from the town, and ROOM_TEMPLE_SECOND_ENTRANCE,
///   where a party defeated in Lunpa wakes.
/// - Gate to Suhalla: the house beside Suhalla Gate (suhalla_gate.c), entered
///   at ROOM_GATE_TO_SUHALLA (ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT),
///   the sheet's one room that is not Lunpa's, where a traveling Healer offers
///   the Sanctum's services.
///
/// A room's entrance number is also its exit kind and the step trigger
/// of its door (ROOM_DOOR), so each door leads back out the way the party came
/// in. Scene_GetTriggers gives the house at Suhalla Gate its own table, and
/// Lunpa's rooms the sealed town's (gInteriorSealedTriggers) until Donpa's
/// judgment sets FLAG_DODONPA_LOCKED_UP, then the reopened town's
/// (gInteriorReopenedTriggers). While the town is sealed the merchants neither
/// buy nor sell and the innkeeper offers only his home; talking to the inn's
/// chef or reading his mind tells of the prisoner he takes meals to
/// (FLAG_LUNPA_HEARD_OF_PRISONER). Once it reopens, the shops, the inn and the
/// temple serve across their counters.
///
/// A merchant or Healer serves only a leader who faces north across the
/// counter (FACING_IS_NORTH); otherwise they talk. The counters are blank
/// actors in front of the merchants (ACTOR_WEAPON_COUNTER and the
/// others), so talking across one reaches the merchant's trigger. Entering by
/// ROOM_GATE_TO_SUHALLA makes the house the place a defeated party wakes and
/// hides the location name.

#include "save_block.h"
#include "script_layer.h"
#include "unit/scene/rock_rooms.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/field_services.h"
#include "rock_rooms.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

static void EventScript_TalkToInnkeeper(void);
static void EventScript_TalkToChef(void);
static void EventScript_ReadChefsThoughts(void);
static void EventScript_AskWhetherDonpasGuest(void);
static void EventScript_TalkToHealer(void);
static void EventScript_ReadItemMerchantsThoughts(void);
static void EventScript_TalkToItemMerchant(void);
static void EventScript_ReadWeaponMerchantsThoughts(void);
static void EventScript_TalkToWeaponMerchant(void);
static void EventScript_ReadArmorMerchantsThoughts(void);
static void EventScript_TalkToArmorMerchant(void);
static void EventScript_TalkToTravelingHealer(void);

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
    // XXX (no evidence): what this entrance was meant for; no exit
    // leads to it, and it stands the leader in the empty dark between the crate
    // room and the house at Suhalla Gate.
    { ROOM_NINTH_UNUSED, CONDITION_ALWAYS, 672, 0, 888, FACING_NORTH, 0, 520, 656, 760, 896, 0 },
    { ROOM_GATE_TO_SUHALLA, CONDITION_ALWAYS, 832, 0, 848, FACING_NORTH, 0, 680, 656, 920, 896, 0 },
    { ROOM_TEMPLE_SECOND_ENTRANCE, CONDITION_ALWAYS, 688, 0, 184, FACING_NORTH, 0, 568, 32, 808, 288, 0 },
    { SCENE_TABLE_END },
};

const u32 gInteriorExits[] = {
    SCENE_EXITS(RockRooms),
    SCENE_EXIT(ROOM_FIRST_HOME, Lunpa, LUNPA_FROM_FIRST_HOME),
    SCENE_EXIT(ROOM_SECOND_HOME, Lunpa, LUNPA_FROM_SECOND_HOME),
    SCENE_EXIT(ROOM_THIRD_HOME, Lunpa, LUNPA_FROM_THIRD_HOME),
    SCENE_EXIT(ROOM_ARMS_SHOP, Lunpa, LUNPA_FROM_WEAPON_AND_ARMOR_SHOP),
    SCENE_EXIT(ROOM_ITEM_SHOP, Lunpa, LUNPA_FROM_ITEM_SHOP),
    SCENE_EXIT(ROOM_INN, Lunpa, LUNPA_FROM_INN),
    SCENE_EXIT(ROOM_TEMPLE, Lunpa, LUNPA_FROM_TEMPLE),
    SCENE_EXIT(ROOM_CRATE_ROOM, Lunpa, LUNPA_FROM_CRATE_ROOM),
    SCENE_EXIT(ROOM_GATE_TO_SUHALLA, SuhallaGateCheckpoint, SUHALLA_GATE_CHECKPOINT_FROM_ROCK_ROOMS),
    SCENE_EXITS_END,
};

const struct ActorTemplate gInteriorActorTemplates[] = {
    { SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      PIXELS(144), 0, PIXELS(168), FACING_WEST, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(96), 0, PIXELS(128), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      PIXELS(392), 0, PIXELS(240), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(403), 0, PIXELS(138), FACING_EAST, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(160), 0, PIXELS(508), FACING_SOUTH - FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(88), 0, PIXELS(584), FACING_NORTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(376), 0, PIXELS(531), FACING_SOUTH - FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(464), 0, PIXELS(531), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(720), 0, PIXELS(499), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(264), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(248), 0, PIXELS(808), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      PIXELS(144), 0, PIXELS(888), FACING_NORTHWEST, TALK_FACING_PARTY, 0 },
    { SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(152), 0, PIXELS(744), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(688), 0, PIXELS(152), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(776), 0, PIXELS(800), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(376), 0, PIXELS(544), FACING_SOUTH - FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(464), 0, PIXELS(544), FACING_SOUTH + FACING_STEP, TALK_FACING_PARTY, 0 },
    { SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      PIXELS(720), 0, PIXELS(512), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

const struct Trigger gInteriorSealedTriggers[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_CRATE_ROOM),
    { TRIGGER_TALK, ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeASealed) },
    { TRIGGER_TALK, ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBSealed) },
    { TRIGGER_TALK, ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeASealed) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBSealed) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeASealed) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBSealed) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnClerkSealed) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerSealed) },
    { TRIGGER_TALK, ACTOR_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToChef) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToTravelingHealer) },
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
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnkeeperSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadChefsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestSealedThoughts) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), MB_ROCK_ROOMS_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_ROCK_ROOMS_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), MB_ROCK_ROOMS_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), MB_ROCK_ROOMS_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { TRIGGERS_END },
};

const struct Trigger gInteriorReopenedTriggers[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_CRATE_ROOM),
    { TRIGGER_TALK, ACTOR_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeAReopened) },
    { TRIGGER_TALK, ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBReopened) },
    { TRIGGER_TALK, ACTOR_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeAReopened) },
    { TRIGGER_TALK, ACTOR_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBReopened) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeAReopened) },
    { TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBReopened) },
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhetherDonpasGuest) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerReopened) },
    { TRIGGER_TALK, ACTOR_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsChefReopened) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
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
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
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
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), MB_ROCK_ROOMS_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), MB_ROCK_ROOMS_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), MB_ROCK_ROOMS_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), MB_ROCK_ROOMS_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { TRIGGERS_END },
};
// clang-format on

const struct Trigger gGateToSuhallaTriggers[] = {
    ROOM_DOOR(ROOM_GATE_TO_SUHALLA),
    {TRIGGER_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToTravelingHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsTravelingPriestThoughts)},
    {TRIGGERS_END},
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gInteriorEntrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gInteriorExits; }

/// The townsfolk, shopkeepers and counters of all three areas.
const struct ActorTemplate *Scene_GetActorTemplates(void) {
  return gInteriorActorTemplates;
}

/// The triggers of Gate to Suhalla, or those of Lunpa's buildings before and
/// after Donpa's judgment.
const struct Trigger *Scene_GetTriggers(void) {
  if (gSaveBlock.entrance == ROOM_GATE_TO_SUHALLA) {
    return gGateToSuhallaTriggers;
  }
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    return gInteriorReopenedTriggers;
  }
  return gInteriorSealedTriggers;
}

/// Lunpa, the inn: across the counter after Donpa's judgment, a room at the
/// inn; otherwise the innkeeper offers a room or, while Lunpa is sealed, a bed
/// at home.
static void EventScript_TalkToInnkeeper(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
      Inn_CheckIn(LUNPA_INN, ACTOR_INNKEEPER);
      return;
    }
  }
  FieldScript_Start();
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_SetMessageId(MsgLunpaInnkeeperOffersRoom);
    FieldScript_AskYesNoAndShowResponse(ACTOR_INNKEEPER, 0);
  } else {
    FieldScript_SetMessageId(MsgLunpaInnkeeperOffersOwnHome);
    FieldScript_AskYesNoAndShowResponse(ACTOR_INNKEEPER, 0);
  }
  FieldScript_End();
}

/// Lunpa, the inn, before Donpa's judgment: the chef offers the story of
/// taking meals to the fortress; either way the party has heard of the
/// prisoner.
static void EventScript_TalkToChef(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaChefOffersStory);
  FieldScript_AskYesNoAndShowResponse(ACTOR_CHEF, 0);
  GameFlag_Set(FLAG_LUNPA_HEARD_OF_PRISONER);
  FieldScript_End();
}

/// Lunpa, the inn: Mind Read on the chef, who pities the prisoner in the
/// dungeon.
static void EventScript_ReadChefsThoughts(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaChefThinksOfPrisoner);
  FieldScript_ShowMessageAndWait(ACTOR_CHEF, 0);
  GameFlag_Set(FLAG_LUNPA_HEARD_OF_PRISONER);
  FieldScript_End();
}

/// Lunpa, the inn, after Donpa's judgment: a woman asks whether the party is
/// Donpa's guest.
static void EventScript_AskWhetherDonpasGuest(void) {
  FieldScript_Start();
  FieldScript_SetMessageId(MsgLunpaInnClerkAsksAboutGuest);
  FieldScript_AskYesNoAndShowResponse(ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, 0);
  FieldScript_End();
}

/// Lunpa Temple: across the counter, the Sanctum's services; otherwise the
/// Healer talks about Dodonpa.
static void EventScript_TalkToHealer(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Sanctum_Run(ACTOR_HEALER);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgLunpaTemplePriestReopened);
    FieldScript_ShowMessageAndWait(ACTOR_HEALER, 0);
    FieldScript_End();
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgLunpaTemplePriestSealed);
    FieldScript_ShowMessageAndWait(ACTOR_HEALER, 0);
    FieldScript_End();
  }
}

/// Lunpa, the item shop: Mind Read on the merchant.
static void EventScript_ReadItemMerchantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgLunpaIsntWeaponsVendors);
    FieldScript_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    FieldScript_End();
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgLunpaItemMerchantSealedThoughts);
    FieldScript_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    FieldScript_End();
  }
}

/// Lunpa, the item shop: across the counter, the shop; otherwise small talk.
static void EventScript_TalkToItemMerchant(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Shop_Run(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgLunpaItemMerchantReopened);
    FieldScript_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    FieldScript_End();
  } else {
    FieldScript_Start();
    FieldScript_SetMessageId(MsgLunpaItemMerchantSealed);
    FieldScript_ShowMessageAndWait(ACTOR_ITEM_MERCHANT, 0);
    FieldScript_End();
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the weapon merchant.
static void EventScript_ReadWeaponMerchantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_SetMessageId(MsgLunpaWeaponMerchantReopenedThoughts);
    FieldScript_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    FieldScript_SetMessageId(MsgLunpaWeaponMerchantSealedThoughts);
    FieldScript_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the weapon shop; otherwise small talk.
static void EventScript_TalkToWeaponMerchant(void) {
  struct Actor *leader;
  s32 facing;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Shop_Run(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
    } else {
      FieldScript_Start();
      FieldScript_SetMessageId(MsgLunpaWeaponMerchantReopened);
      FieldScript_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
      FieldScript_End();
    }
  } else {
    FieldScript_SetMessageId(MsgLunpaWeaponMerchantSealed);
    FieldScript_ShowMessageAndWait(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the armor merchant.
static void EventScript_ReadArmorMerchantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    FieldScript_SetMessageId(MsgLunpaArmorMerchantReopenedThoughts);
    FieldScript_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  } else {
    FieldScript_SetMessageId(MsgLunpaArmorMerchantSealedThoughts);
    FieldScript_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the armor shop; otherwise small talk.
static void EventScript_TalkToArmorMerchant(void) {
  struct Actor *leader;
  s32 facing;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Shop_Run(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
    } else {
      FieldScript_Start();
      FieldScript_SetMessageId(MsgLunpaArmorMerchantReopened);
      FieldScript_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
      FieldScript_End();
    }
  } else {
    FieldScript_SetMessageId(MsgLunpaArmorMerchantSealed);
    FieldScript_ShowMessageAndWait(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Sets the buildings up: entered by Gate to Suhalla, the house is where a
/// defeated party wakes, and the counters are drawn behind their keepers.
s32 Scene_Initialize(void) {
  s16 entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gSaveBlock.entrance;
  if (entrance == ROOM_GATE_TO_SUHALLA) {
    GameFlag_Clear(FLAG_SHOW_LOCATION_NAME);
    gSaveBlock.restartScene = (s32)&SceneId_RockRooms;
    gSaveBlock.restartEntrance = entrance;
  }
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_WEAPON_COUNTER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ARMOR_COUNTER), 0);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_ITEM_COUNTER), 0);
  return 0;
}

/// Gate to Suhalla: the traveling Healer offers the Sanctum's services across
/// the counter; otherwise he speaks of his good will.
static void EventScript_TalkToTravelingHealer(void) {
  struct Actor *leader;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    // BUG: the temple's Healer, not the traveling one. The Sanctum only takes
    // its keeper's portrait from the actor (Sanctum_Run), and both
    // Healers are placed in this map in the same sprite, so the player sees no
    // difference.
#ifdef BUGFIX
    Sanctum_Run(ACTOR_TRAVELING_HEALER);
#else
    Sanctum_Run(ACTOR_HEALER);
#endif
  } else {
    FieldScript_SetMessageId(MsgLunpaTravelingPriest);
    FieldScript_ShowMessageAndWait(ACTOR_TRAVELING_HEALER, 0);
  }
}
