/// @file
/// Rock rooms (SceneId_RockRooms), a sheet of rooms cut into rock.
///
/// The game packs rooms that share a tile style onto one map. This one holds
/// the buildings of Lunpa and the house at Suhalla Gate. Its rooms are told
/// apart by the entrance the party comes in by, and the area name follows it:
///
/// - Lunpa: the three homes, the weapon and armor shop, the item shop, the inn
///   and the crate room. They are entered by the town's doors (lunpa.c) at
///   ROOM_FIRST_HOME to ROOM_INN and ROOM_CRATE_ROOM.
/// - Lunpa Temple: ROOM_TEMPLE from the town, and ROOM_TEMPLE_SECOND_ENTRANCE,
///   where a party defeated in Lunpa wakes.
/// - Gate to Suhalla: the house beside Suhalla Gate (suhalla_gate.c), entered
///   at ROOM_GATE_TO_SUHALLA (ROCK_ROOMS_FROM_SUHALLA_GATE_CHECKPOINT). It is
///   the sheet's one room that is not Lunpa's, and a traveling Healer offers
///   the Sanctum's services there.
///
/// Each door leads back out the way the party came in. That works because a
/// room's entrance number is also its exit kind and the step trigger of its
/// door (ROOM_DOOR).
///
/// Scene_GetTriggers gives the house at Suhalla Gate its own table. Lunpa's
/// rooms take the sealed town's table (gRockRooms_SealedTriggers) until Donpa's
/// judgment sets FLAG_DODONPA_LOCKED_UP, and the reopened town's
/// (gRockRooms_ReopenedTriggers) after it.
///
/// While the town is sealed, the merchants neither buy nor sell and the
/// innkeeper offers only his home. Talking to the inn's chef or reading his
/// mind tells of the prisoner he takes meals to (FLAG_LUNPA_HEARD_OF_PRISONER).
/// Once the town reopens, the shops, the inn and the temple serve across their
/// counters.
///
/// A merchant or Healer serves only a leader who faces north across the counter
/// (FACING_IS_NORTH). Otherwise they talk. The counters are blank actors in
/// front of the merchants (ACTOR_WEAPON_COUNTER and the others), so talking
/// across one reaches the merchant's trigger.
///
/// Entering by ROOM_GATE_TO_SUHALLA makes the house the place a defeated party
/// wakes and hides the area name.

#include "types.h"
#include "field/event.h"
#include "field/scene_tables.h"
#include "system/main_routine_work.h"

extern const struct SceneEntrance gRockRooms_Entrances[];
extern const u32 gRockRooms_Exits[];
extern const struct ActorPlacement gRockRooms_ActorPlacements[];
extern const struct Trigger gRockRooms_SealedTriggers[];
extern const struct Trigger gRockRooms_ReopenedTriggers[];
extern const struct Trigger gRockRooms_GateToSuhallaTriggers[];

#include "text/msg_ids.h"
#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"

/// The rooms by the entrance the party comes in by; each is also its door's
/// trigger and exit kind (ROOM_DOOR).
enum RockRoomsRoom {
  ROOM_FIRST_HOME = 1,
  ROOM_SECOND_HOME,
  ROOM_THIRD_HOME,
  ROOM_ARMS_SHOP,
  ROOM_ITEM_SHOP,
  ROOM_INN,
  ROOM_TEMPLE,
  ROOM_CRATE_ROOM,
  ROOM_NINTH_UNUSED, ///< no exit leads here
  ROOM_GATE_TO_SUHALLA,
  ROOM_TEMPLE_SECOND_ENTRANCE
};

#define ROOM_DOOR(room) {TRIGGER_EXIT, (room), CONDITION_ALWAYS, (room)}

/// The cell behaviors the rooms' examine triggers answer; each door's cells
/// carry the number of its exit.
enum RockRoomsCellBehavior {
  CELL_BEHAVIOR_ROCK_ROOMS_WOODEN_BOX = 100,
  CELL_BEHAVIOR_ROCK_ROOMS_BARREL,
  CELL_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN = 200,
  CELL_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN
};

/// The placed actors of Lunpa's houses, shops, inn and temple, numbered in the
/// order of gRockRooms_ActorPlacements. Two people live in each of the three
/// homes, named by their sprites and by what they say while the town is sealed.
enum RockRoomsActorId {
  /// In the first home: this man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR)
  /// and a woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  ACTOR_MAN_ON_HIS_LATE_GRANDFATHER = ACTOR_FIRST_PLACED,
  ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA,
  /// In the second home: this burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR) and a woman in a white apron
  /// (SPRITE_WOMAN_IN_PINK_APRON).
  ACTOR_MAN_ON_DONPAS_FORTRESS,
  ACTOR_WOMAN_ON_THE_INNS_CHEF,
  /// In the third home: this bent old man with a cane
  /// (SPRITE_OLD_MAN_WITH_CANE) and an old woman (SPRITE_OLD_WOMAN).
  ACTOR_OLD_MAN_ON_LUNPAS_DEATH,
  ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD,
  ACTOR_WEAPON_MERCHANT,
  ACTOR_ARMOR_MERCHANT,
  ACTOR_ITEM_MERCHANT,
  ACTOR_INNKEEPER,
  /// A woman in a white apron (SPRITE_WOMAN_IN_PINK_APRON) at the inn, beside
  /// the innkeeper.
  ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST,
  ACTOR_INN_WORKER,
  ACTOR_CHEF,
  /// Behind the temple's altar; across it, the Sanctum's services
  /// (RockRooms_TalkToHealer).
  ACTOR_HEALER,
  /// In the Healer's sprite, a bearded man in a long robe
  /// (SPRITE_BEARDED_MAN_IN_LONG_ROBE), in the house by Suhalla Gate.
  ACTOR_TRAVELING_HEALER,
  /// SPRITE_BLANK on the counter before each merchant, so that talking across
  /// it reaches the merchant's event.
  ACTOR_WEAPON_COUNTER,
  ACTOR_ARMOR_COUNTER,
  ACTOR_ITEM_COUNTER
};

/// Lunpa's shops, as Shop_Run numbers them.
enum { SHOP_LUNPA_ITEMS = 25, SHOP_LUNPA_WEAPONS = 29, SHOP_LUNPA_ARMOR = 30 };

/// Lunpa's inn, as Inn_CheckIn numbers the inns.
enum { LUNPA_INN = 8 };

static void RockRooms_TalkToInnkeeper(void);
static void RockRooms_TalkToChef(void);
static void RockRooms_ReadChefsThoughts(void);
static void RockRooms_AskWhetherDonpasGuest(void);
static void RockRooms_TalkToHealer(void);
static void RockRooms_ReadItemMerchantsThoughts(void);
static void RockRooms_TalkToItemMerchant(void);
static void RockRooms_ReadWeaponMerchantsThoughts(void);
static void RockRooms_TalkToWeaponMerchant(void);
static void RockRooms_ReadArmorMerchantsThoughts(void);
static void RockRooms_TalkToArmorMerchant(void);
static void RockRooms_TalkToTravelingHealer(void);

// clang-format off
const struct SceneEntrance gRockRooms_Entrances[] = {
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

const u32 gRockRooms_Exits[] = {
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

const struct ActorPlacement gRockRooms_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      144, 0, 168, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      96, 0, 128, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      392, 0, 240, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      403, 0, 138, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      160, 0, 508, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      88, 0, 584, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      376, 0, 531, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      464, 0, 531, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      720, 0, 499, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      264, 0, 744, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      248, 0, 808, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
      144, 0, 888, FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      152, 0, 744, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      688, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      776, 0, 800, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      376, 0, 544, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      464, 0, 544, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
      720, 0, 512, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

const struct Trigger gRockRooms_SealedTriggers[] = {
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
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(RockRooms_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(RockRooms_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(RockRooms_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnClerkSealed) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerSealed) },
    { TRIGGER_TALK, ACTOR_CHEF, CONDITION_ALWAYS, EVENT(RockRooms_TalkToChef) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToHealer) },
    { TRIGGER_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToTravelingHealer) },
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
      EVENT(RockRooms_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnkeeperSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnClerkSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsInnWorkerSealedThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadChefsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
      MSGNO(MsgLunpaInteriorsTemplePriestSealedThoughts) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_ROCK_ROOMS_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_ROCK_ROOMS_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { TRIGGER_TABLE_END },
};

const struct Trigger gRockRooms_ReopenedTriggers[] = {
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
    { TRIGGER_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, EVENT(RockRooms_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, EVENT(RockRooms_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, EVENT(RockRooms_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_ITEM_COUNTER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToItemMerchant) },
    { TRIGGER_TALK, ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToWeaponMerchant) },
    { TRIGGER_TALK, ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToArmorMerchant) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToInnkeeper) },
    { TRIGGER_TALK, ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, CONDITION_ALWAYS, EVENT(RockRooms_AskWhetherDonpasGuest) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsInnWorkerReopened) },
    { TRIGGER_TALK, ACTOR_CHEF, CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsChefReopened) },
    { TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS, EVENT(RockRooms_TalkToHealer) },
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
      EVENT(RockRooms_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_COUNTER, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadWeaponMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_COUNTER, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadArmorMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadItemMerchantsThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_COUNTER, CONDITION_ALWAYS,
      EVENT(RockRooms_ReadItemMerchantsThoughts) },
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
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_ROCK_ROOMS_WOODEN_BOX, FLAG_LUNPA_WOODEN_BOX_LUCKY_MEDAL,
      FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_ROCK_ROOMS_BARREL, FLAG_LUNPA_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsFirstOven)) },
    { EXAMINE_TRIGGER(EXAMINE_OVEN), CELL_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLunpaInteriorsSecondOven)) },
    { TRIGGER_TABLE_END },
};
// clang-format on

const struct Trigger gRockRooms_GateToSuhallaTriggers[] = {
    ROOM_DOOR(ROOM_GATE_TO_SUHALLA),
    {TRIGGER_TALK, ACTOR_TRAVELING_HEALER, CONDITION_ALWAYS,
     EVENT(RockRooms_TalkToTravelingHealer)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAVELING_HEALER,
     CONDITION_ALWAYS, MSGNO(MsgLunpaInteriorsTravelingPriestThoughts)},
    {TRIGGER_TABLE_END},
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gRockRooms_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return NULL; }

const u32 *Scene_GetExits(void) { return gRockRooms_Exits; }

/// The townsfolk, shopkeepers and counters of all three areas.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gRockRooms_ActorPlacements;
}

/// The triggers of Gate to Suhalla, or those of Lunpa's buildings before and
/// after Donpa's judgment.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.entrance == ROOM_GATE_TO_SUHALLA) {
    return gRockRooms_GateToSuhallaTriggers;
  }
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    return gRockRooms_ReopenedTriggers;
  }
  return gRockRooms_SealedTriggers;
}

/// Lunpa, the inn: across the counter after Donpa's judgment, a room at the
/// inn; otherwise the innkeeper offers a room or, while Lunpa is sealed, a bed
/// at home.
static void RockRooms_TalkToInnkeeper(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
      Inn_CheckIn(LUNPA_INN, ACTOR_INNKEEPER);
      return;
    }
  }
  Event_Start();
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaInnkeeperOffersRoom);
    // "Do you want rest in our inn?"
    // Yes: "I see. In that case, go ahead and stay. I won't ask why."
    // No: "Well, do as you like. Feel free to stay here anytime you like."
    Event_AskYesNo(ACTOR_INNKEEPER, 0);
  } else {
    Event_SetMessageId(MsgLunpaInnkeeperOffersOwnHome);
    // "Do you want to sleep at my place?"
    // Yes: "You must be kidding. Go rest in your own home!"
    // No: "Of course not! With no travelers in town, no one wants to..."
    Event_AskYesNo(ACTOR_INNKEEPER, 0);
  }
  Event_End();
}

/// Lunpa, the inn, before Donpa's judgment: the chef offers the story of
/// taking meals to the fortress; either way the party has heard of the
/// prisoner.
static void RockRooms_TalkToChef(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaChefOffersStory);
  // "Do you want to hear about how I delivered meals to the fortress?"
  // Yes: "I was blindfolded and guided along a cool path. I walked for..."
  // No: "I was so relieved when I got back to the village. The guy I..."
  Event_AskYesNo(ACTOR_CHEF, 0);
  GameFlag_Set(FLAG_LUNPA_HEARD_OF_PRISONER);
  Event_End();
}

/// Lunpa, the inn: Mind Read on the chef, who pities the prisoner in the
/// dungeon.
static void RockRooms_ReadChefsThoughts(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaChefThinksOfPrisoner);
  // "I feel bad for that poor man they've got locked up in the..."
  Event_ShowMessage(ACTOR_CHEF, 0);
  GameFlag_Set(FLAG_LUNPA_HEARD_OF_PRISONER);
  Event_End();
}

/// Lunpa, the inn, after Donpa's judgment: a woman asks whether the party is
/// Donpa's guest.
static void RockRooms_AskWhetherDonpasGuest(void) {
  Event_Start();
  Event_SetMessageId(MsgLunpaInnClerkAsksAboutGuest);
  // "Are you Donpa's guest?"
  // Yes: "So, you're the one we were told to give the royal treatment..."
  // No: "Really, you're not? Well, then, who is Donpa's guest?"
  Event_AskYesNo(ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST, 0);
  Event_End();
}

/// Lunpa Temple: across the counter, the Sanctum's services; otherwise the
/// Healer talks about Dodonpa.
static void RockRooms_TalkToHealer(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Sanctum_Run(ACTOR_HEALER);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_Start();
    Event_SetMessageId(MsgLunpaTemplePriestReopened);
    // "A number of Dodonpa's men set out not too long ago. I wonder..."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_End();
  } else {
    Event_Start();
    Event_SetMessageId(MsgLunpaTemplePriestSealed);
    // "I think that thief Dodonpa is a terrible man. But it's not..."
    Event_ShowMessage(ACTOR_HEALER, 0);
    Event_End();
  }
}

/// Lunpa, the item shop: Mind Read on the merchant.
static void RockRooms_ReadItemMerchantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_Start();
    Event_SetMessageId(MsgLunpaIsntWeaponsVendors);
    // "It isn't just the weapons vendors whose inventory is..."
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  } else {
    Event_Start();
    Event_SetMessageId(MsgLunpaItemMerchantSealedThoughts);
    // "I bought a lot of stock, but I'm already running out..."
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  }
}

/// Lunpa, the item shop: across the counter, the shop; otherwise small talk.
static void RockRooms_TalkToItemMerchant(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    Shop_Run(SHOP_LUNPA_ITEMS, ACTOR_ITEM_MERCHANT);
  } else if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_Start();
    Event_SetMessageId(MsgLunpaItemMerchantReopened);
    // "The weapons and armor vendors are looking desperate. The..."
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  } else {
    Event_Start();
    Event_SetMessageId(MsgLunpaItemMerchantSealed);
    // "They bought a lot of stock, which is why they're holding up,..."
    Event_ShowMessage(ACTOR_ITEM_MERCHANT, 0);
    Event_End();
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the weapon merchant.
static void RockRooms_ReadWeaponMerchantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaWeaponMerchantReopenedThoughts);
    // "All those weapons, but I doubt Dodonpa's henchmen can use them!"
    Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  } else {
    Event_SetMessageId(MsgLunpaWeaponMerchantSealedThoughts);
    // "I'll have to close this shop soon if things don't change..."
    Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the weapon shop; otherwise small talk.
static void RockRooms_TalkToWeaponMerchant(void) {
  struct Actor *leader;
  s32 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Shop_Run(SHOP_LUNPA_WEAPONS, ACTOR_WEAPON_MERCHANT);
    } else {
      Event_Start();
      Event_SetMessageId(MsgLunpaWeaponMerchantReopened);
      // "The end is near! Doom is upon us! To arms! To arms! Once..."
      Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
      Event_End();
    }
  } else {
    Event_SetMessageId(MsgLunpaWeaponMerchantSealed);
    // "The village is sealed, so we can't buy or sell anything...."
    Event_ShowMessage(ACTOR_WEAPON_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: Mind Read on the armor merchant.
static void RockRooms_ReadArmorMerchantsThoughts(void) {
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    Event_SetMessageId(MsgLunpaArmorMerchantReopenedThoughts);
    // "Once I sell this armor, I'm closing shop... It's kind of a..."
    Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  } else {
    Event_SetMessageId(MsgLunpaArmorMerchantSealedThoughts);
    // "I should have bought more stock while I had the chance..."
    Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Lunpa, the weapon and armor shop: after Donpa's judgment, across the
/// counter, the armor shop; otherwise small talk.
static void RockRooms_TalkToArmorMerchant(void) {
  struct Actor *leader;
  s32 facing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  facing = leader->facing;
  if (GameFlag_Get(FLAG_DODONPA_LOCKED_UP) != 0) {
    if (FACING_IS_NORTH(facing)) {
      Shop_Run(SHOP_LUNPA_ARMOR, ACTOR_ARMOR_MERCHANT);
    } else {
      Event_Start();
      Event_SetMessageId(MsgLunpaArmorMerchantReopened);
      // "What do you think? I've got some incredible products here,..."
      Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
      Event_End();
    }
  } else {
    Event_SetMessageId(MsgLunpaArmorMerchantSealed);
    // "I'll sell you as much armor as you like, once the village is..."
    Event_ShowMessage(ACTOR_ARMOR_MERCHANT, 0);
  }
}

/// Sets the buildings up: entered by Gate to Suhalla, the house is where a
/// defeated party wakes, and the counters are drawn behind their keepers.
s32 Scene_Initialize(void) {
  s16 entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  entrance = gMainRoutineWork.entrance;
  if (entrance == ROOM_GATE_TO_SUHALLA) {
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
    gMainRoutineWork.restartScene = SCENE_ROCK_ROOMS;
    gMainRoutineWork.restartEntrance = entrance;
  }
  Actor_SetSpriteFlags(Actor_Get(ACTOR_WEAPON_COUNTER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ARMOR_COUNTER), 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ITEM_COUNTER), 0);
  return 0;
}

/// Gate to Suhalla: the traveling Healer offers the Sanctum's services across
/// the counter; otherwise he speaks of his good will.
static void RockRooms_TalkToTravelingHealer(void) {
  struct Actor *leader;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  if (FACING_IS_NORTH(leader->facing)) {
    // BUG: the temple's Healer, not the traveling one. The Sanctum only takes
    // its keeper's portrait from the actor (Sanctum_Run), and both
    // Healers are placed in this scene in the same sprite, so the player sees
    // no difference.
#ifdef BUGFIX
    Sanctum_Run(ACTOR_TRAVELING_HEALER);
#else
    Sanctum_Run(ACTOR_HEALER);
#endif
  } else {
    Event_SetMessageId(MsgLunpaTravelingPriest);
    // "I have been traveling the lands spreading good will."
    Event_ShowMessage(ACTOR_TRAVELING_HEALER, 0);
  }
}
