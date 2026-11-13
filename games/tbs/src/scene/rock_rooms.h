#ifndef ALCHEMY_ROCK_ROOMS_H
#define ALCHEMY_ROCK_ROOMS_H

#include "types.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "event.h"
#include "scene_tables.h"
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

#endif
