/// @file
/// The buildings of Lunpa and the house at Suhalla Gate: one map
/// (MapId_LunpaInteriors) whose rooms are told apart by the warp destination
/// the party comes in by, and whose area name follows it:
///
/// - Lunpa: the three homes, the weapon and armor shop, the item shop, the inn
///   and the crate room, entered by the town's doors (lunpa.c) at
///   ROOM_FIRST_HOME to ROOM_INN and ROOM_CRATE_ROOM.
/// - Lunpa Temple: ROOM_TEMPLE from the town, and ROOM_TEMPLE_SECOND_WARP_DEST,
///   where a party defeated in Lunpa wakes.
/// - Gate to Suhalla: the house beside Suhalla Gate (suhalla_gate.c), entered
///   at ROOM_GATE_TO_SUHALLA, where a traveling Healer offers the Sanctum's
///   services.
///
/// A room's warp destination number is also its warp kind and the coord event
/// of its door (ROOM_DOOR), so each door leads back out the way the party came
/// in. Map_GetEvents gives the house at Suhalla Gate its own table, and Lunpa's
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
/// object events in front of the merchants (LOCALID_WEAPON_COUNTER and the
/// others), so talking across one reaches the merchant's map event. Entering by
/// ROOM_GATE_TO_SUHALLA makes the house the place a defeated party wakes and
/// hides the location name.
///
/// Open questions:
/// - XXX (no evidence): what ROOM_NINTH_UNUSED was meant for, a warp
/// destination no
///   warp leads to; its camera box takes half the crate room and half the
///   house at Suhalla Gate, and it stands the leader in the empty dark between
///   them (https://alchemy.pascalpixel.com/places/lunpa-suhalla/map-9.png), so
///   the map holds no room of its own for it.

#include "game_state.h"
#include "script_layer.h"
#include "unit/map/lunpa_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "lunpa_interiors.h"
#include "map_ids.h"
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
const struct MapWarpDest gInteriorWarpDests[] = {
    { WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
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
    { ROOM_TEMPLE_SECOND_WARP_DEST, CONDITION_ALWAYS, 688, 0, 184, FACING_NORTH, 0, 568, 32, 808, 288, 0 },
    { MAP_TABLE_END },
};

const u32 gInteriorWarps[] = {
    MAP_WARPS(LunpaInteriors),
    MAP_WARP(ROOM_FIRST_HOME, Lunpa, LUNPA_FROM_FIRST_HOME),
    MAP_WARP(ROOM_SECOND_HOME, Lunpa, LUNPA_FROM_SECOND_HOME),
    MAP_WARP(ROOM_THIRD_HOME, Lunpa, LUNPA_FROM_THIRD_HOME),
    MAP_WARP(ROOM_ARMS_SHOP, Lunpa, LUNPA_FROM_WEAPON_AND_ARMOR_SHOP),
    MAP_WARP(ROOM_ITEM_SHOP, Lunpa, LUNPA_FROM_ITEM_SHOP),
    MAP_WARP(ROOM_INN, Lunpa, LUNPA_FROM_INN),
    MAP_WARP(ROOM_TEMPLE, Lunpa, LUNPA_FROM_TEMPLE),
    MAP_WARP(ROOM_CRATE_ROOM, Lunpa, LUNPA_FROM_CRATE_ROOM),
    MAP_WARP(ROOM_GATE_TO_SUHALLA, SuhallaGateCheckpoint, SUHALLA_GATE_CHECKPOINT_FROM_LUNPA_INTERIORS),
    MAP_WARPS_END,
};

const struct ObjectEventTemplate gInteriorObjectEventTemplates[] = {
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
    { MAP_TABLE_END },
};

const struct MapEvent gInteriorSealedEvents[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_CRATE_ROOM),
    { MAP_EVENT_TALK, LOCALID_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeASealed) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBSealed) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeASealed) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBSealed) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeASealed) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBSealed) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnClerkSealed) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerSealed) },
    { MAP_EVENT_TALK, LOCALID_CHEF, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToChef) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { MAP_EVENT_TALK, LOCALID_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToTravelingHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeASealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeBSealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeASealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeBSealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeASealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeBSealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnkeeperSealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkSealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerSealedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHEF, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadChefsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestSealedThoughts) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_BARREL), TRIGGER_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { BG_EVENT(BG_EVENT_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { MAP_EVENTS_END },
};

const struct MapEvent gInteriorReopenedEvents[] = {
    ROOM_DOOR(ROOM_FIRST_HOME),
    ROOM_DOOR(ROOM_SECOND_HOME),
    ROOM_DOOR(ROOM_THIRD_HOME),
    ROOM_DOOR(ROOM_ARMS_SHOP),
    ROOM_DOOR(ROOM_ITEM_SHOP),
    ROOM_DOOR(ROOM_INN),
    ROOM_DOOR(ROOM_TEMPLE),
    ROOM_DOOR(ROOM_CRATE_ROOM),
    { MAP_EVENT_TALK, LOCALID_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeAReopened) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsFirstHomeBReopened) },
    { MAP_EVENT_TALK, LOCALID_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeAReopened) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsSecondHomeBReopened) },
    { MAP_EVENT_TALK, LOCALID_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeAReopened) },
    { MAP_EVENT_TALK, LOCALID_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsThirdHomeBReopened) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_ITEM_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToItemMerchant) },
    { MAP_EVENT_TALK, LOCALID_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToWeaponMerchant) },
    { MAP_EVENT_TALK, LOCALID_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToArmorMerchant) },
    { MAP_EVENT_TALK, LOCALID_INNKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToInnkeeper) },
    { MAP_EVENT_TALK, LOCALID_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_AskWhetherDonpasGuest) },
    { MAP_EVENT_TALK, LOCALID_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerReopened) },
    { MAP_EVENT_TALK, LOCALID_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsChefReopened) },
    { MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_TalkToHealer) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_HIS_LATE_GRANDFATHER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeAReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_NEVER_WORKING_FOR_DODONPA, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsFirstHomeBReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_MAN_ON_DONPAS_FORTRESS, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeAReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ON_THE_INNS_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsSecondHomeBReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_MAN_ON_LUNPAS_DEATH, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeAReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_OLD_WOMAN_ON_THE_GREAT_FLOOD, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsThirdHomeBReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT_SCRIPT(EventScript_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INNKEEPER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnkeeperReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_CHEF, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsChefReopenedThoughts) },
    { PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_HEALER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestReopenedThoughts) },
    { BG_EVENT(BG_EVENT_WOODEN_BOX), TRIGGER_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { BG_EVENT(BG_EVENT_BARREL), TRIGGER_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { BG_EVENT(BG_EVENT_OVEN), TRIGGER_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { BG_EVENT(BG_EVENT_OVEN), TRIGGER_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { MAP_EVENTS_END },
};
// clang-format on

const struct MapEvent gGateToSuhallaEvents[] = {
    ROOM_DOOR(ROOM_GATE_TO_SUHALLA),
    {MAP_EVENT_TALK, LOCALID_TRAVELING_HEALER, CONDITION_ALWAYS,
     EVENT_SCRIPT(EventScript_TalkToTravelingHealer)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_TRAVELING_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsTravelingPriestThoughts)},
    {MAP_EVENTS_END},
};

const struct MapWarpDest *Map_GetWarpDests(void) { return gInteriorWarpDests; }

const struct MapWarpRegion *Map_GetWarpRegions(void) { return NULL; }

const u32 *Map_GetWarps(void) { return gInteriorWarps; }

/// The townsfolk, shopkeepers and counters of all three map sections.
const struct ObjectEventTemplate *Map_GetObjectEventTemplates(void) {
  return gInteriorObjectEventTemplates;
}

/// The map events of Gate to Suhalla, or those of Lunpa's buildings before and
/// after Donpa's judgment.
const struct MapEvent *Map_GetEvents(void) {
  if (gGameState.warpDest == ROOM_GATE_TO_SUHALLA) {
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
static void EventScript_TalkToInnkeeper(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
      Engine_InnOpen(LUNPA_INN, LOCALID_INNKEEPER);
      return;
    }
  }
  Engine_ScriptStart();
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_ScriptSetMessage(MsgLunpaInnkeeperOffersRoom);
    Engine_ScriptAskYesNo(LOCALID_INNKEEPER, 0);
  } else {
    Engine_ScriptSetMessage(MsgLunpaInnkeeperOffersOwnHome);
    Engine_ScriptAskYesNo(LOCALID_INNKEEPER, 0);
  }
  Engine_ScriptEnd();
}

/// Lunpa, the inn, before Donpa's judgment: the chef offers the story of
/// taking meals to the fortress; either way the party has heard of the
/// prisoner.
static void EventScript_TalkToChef(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgLunpaChefOffersStory);
  Engine_ScriptAskYesNo(LOCALID_CHEF, 0);
  Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
  Engine_ScriptEnd();
}

/// Lunpa, the inn: Mind Read on the chef, who pities the prisoner in the
/// dungeon.
static void EventScript_ReadChefsThoughts(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgLunpaChefThinksOfPrisoner);
  Engine_ScriptShowMessage(LOCALID_CHEF, 0);
  Engine_GameFlagSet(FLAG_LUNPA_HEARD_OF_PRISONER);
  Engine_ScriptEnd();
}

/// Lunpa, the inn, after Donpa's judgment: a woman asks whether the party is
/// Donpa's guest.
static void EventScript_AskWhetherDonpasGuest(void) {
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgLunpaInnClerkAsksAboutGuest);
  Engine_ScriptAskYesNo(LOCALID_WOMAN_ASKING_FOR_DONPAS_GUEST, 0);
  Engine_ScriptEnd();
}

/// Lunpa Temple: across the counter, the Sanctum's services; otherwise the
/// Healer talks about Dodonpa.
static void EventScript_TalkToHealer(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_SanctumOpen(LOCALID_HEALER);
  } else if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgLunpaTemplePriestReopened);
    Engine_ScriptShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptEnd();
  } else {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgLunpaTemplePriestSealed);
    Engine_ScriptShowMessage(LOCALID_HEALER, 0);
    Engine_ScriptEnd();
  }
}

/// Lunpa, the item shop: Mind Read on the merchant.
static void EventScript_ReadItemMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgLunpaIsntWeaponsVendors);
    Engine_ScriptShowMessage(LOCALID_ITEM_MERCHANT, 0);
    Engine_ScriptEnd();
  } else {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgLunpaItemMerchantSealedThoughts);
    Engine_ScriptShowMessage(LOCALID_ITEM_MERCHANT, 0);
    Engine_ScriptEnd();
  }
}

/// Lunpa, the item shop: across the counter, the shop; otherwise small talk.
static void EventScript_TalkToItemMerchant(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Engine_ShopOpen(SHOP_LUNPA_ITEMS, LOCALID_ITEM_MERCHANT);
  } else if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgLunpaItemMerchantReopened);
    Engine_ScriptShowMessage(LOCALID_ITEM_MERCHANT, 0);
    Engine_ScriptEnd();
  } else {
    Engine_ScriptStart();
    Engine_ScriptSetMessage(MsgLunpaItemMerchantSealed);
    Engine_ScriptShowMessage(LOCALID_ITEM_MERCHANT, 0);
    Engine_ScriptEnd();
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the weapon merchant.
static void EventScript_ReadWeaponMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_ScriptSetMessage(MsgLunpaWeaponMerchantReopenedThoughts);
    Engine_ScriptShowMessage(LOCALID_WEAPON_MERCHANT, 0);
  } else {
    Engine_ScriptSetMessage(MsgLunpaWeaponMerchantSealedThoughts);
    Engine_ScriptShowMessage(LOCALID_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the weapon shop; otherwise small talk.
static void EventScript_TalkToWeaponMerchant(void) {
  struct ObjectEvent *leader;
  s32 facing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  facing = leader->facing;
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Engine_ShopOpen(SHOP_LUNPA_WEAPONS, LOCALID_WEAPON_MERCHANT);
    } else {
      Engine_ScriptStart();
      Engine_ScriptSetMessage(MsgLunpaWeaponMerchantReopened);
      Engine_ScriptShowMessage(LOCALID_WEAPON_MERCHANT, 0);
      Engine_ScriptEnd();
    }
  } else {
    Engine_ScriptSetMessage(MsgLunpaWeaponMerchantSealed);
    Engine_ScriptShowMessage(LOCALID_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the armor merchant.
static void EventScript_ReadArmorMerchantsThoughts(void) {
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    Engine_ScriptSetMessage(MsgLunpaArmorMerchantReopenedThoughts);
    Engine_ScriptShowMessage(LOCALID_ARMOR_MERCHANT, 0);
  } else {
    Engine_ScriptSetMessage(MsgLunpaArmorMerchantSealedThoughts);
    Engine_ScriptShowMessage(LOCALID_ARMOR_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the armor shop; otherwise small talk.
static void EventScript_TalkToArmorMerchant(void) {
  struct ObjectEvent *leader;
  s32 facing;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  facing = leader->facing;
  if (Engine_GameFlagIsSet(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Engine_ShopOpen(SHOP_LUNPA_ARMOR, LOCALID_ARMOR_MERCHANT);
    } else {
      Engine_ScriptStart();
      Engine_ScriptSetMessage(MsgLunpaArmorMerchantReopened);
      Engine_ScriptShowMessage(LOCALID_ARMOR_MERCHANT, 0);
      Engine_ScriptEnd();
    }
  } else {
    Engine_ScriptSetMessage(MsgLunpaArmorMerchantSealed);
    Engine_ScriptShowMessage(LOCALID_ARMOR_MERCHANT, 0);
  }
}

/// Sets the buildings up: entered by Gate to Suhalla, the house is where a
/// defeated party wakes, and the counters are drawn behind their keepers.
s32 Map_Initialize(void) {
  s16 warpDest;

  gFieldWork->transition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  warpDest = gGameState.warpDest;
  if (warpDest == ROOM_GATE_TO_SUHALLA) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
    gGameState.savedMap = (s32)&MapId_LunpaInteriors;
    gGameState.savedWarpDest = warpDest;
  }
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_WEAPON_COUNTER),
                                   0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_ARMOR_COUNTER),
                                   0);
  Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_ITEM_COUNTER),
                                   0);
  return 0;
}

/// Gate to Suhalla: the traveling Healer offers the Sanctum's services across
/// the counter; otherwise he speaks of his good will.
static void EventScript_TalkToTravelingHealer(void) {
  struct ObjectEvent *leader;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    // BUG: the temple's Healer, not the traveling one. The Sanctum only takes
    // its keeper's portrait from the object event (Sanctum_Run), and both
    // Healers are placed in this map in the same sprite, so the player sees no
    // difference.
#ifdef BUGFIX
    Engine_SanctumOpen(LOCALID_TRAVELING_HEALER);
#else
    Engine_SanctumOpen(LOCALID_HEALER);
#endif
  } else {
    Engine_ScriptSetMessage(MsgLunpaTravelingPriest);
    Engine_ScriptShowMessage(LOCALID_TRAVELING_HEALER, 0);
  }
}
