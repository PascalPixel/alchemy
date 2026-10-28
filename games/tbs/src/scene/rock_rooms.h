#ifndef ALCHEMY_ROCK_ROOMS_H
#define ALCHEMY_ROCK_ROOMS_H

#include "types.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "event_layer.h"
#include "scene_tables.h"
#include "flags.h"
#include "item_ids.h"

/// The rooms by the entrance the party comes in by; each is also its door's
/// trigger and exit kind (ROOM_DOOR).
enum InteriorRoom {
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
  /// ★ Named from the game: "checked the barrel" (MsgVaultIsaacCheckedBarrel)
  CELL_BEHAVIOR_ROCK_ROOMS_BARREL,
  CELL_BEHAVIOR_ROCK_ROOMS_FIRST_OVEN = 200,
  CELL_BEHAVIOR_ROCK_ROOMS_SECOND_OVEN
};

/// The placed actors of Lunpa's houses, shops, inn and temple, numbered in the
/// order of gInteriorActorPlacements. Two people live in each of the three
/// homes, named by their sprites and by what they say while the town is sealed.
enum InteriorActorId {
  /// In the first home: this man with brown hair
  /// (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR)
  /// and a woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR).
  /// ★ Named from the game: "My late grandfather traveled the continent with
  /// Lunpa." (MsgLunpaInteriorsFirstHomeASealed)
  ACTOR_MAN_ON_HIS_LATE_GRANDFATHER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "We may live in Lunpa, but we'll never work for
  /// Dodonpa." (MsgLunpaInteriorsFirstHomeBSealed)
  ACTOR_WOMAN_NEVER_WORKING_FOR_DODONPA,
  /// In the second home: this burly grey-haired man
  /// (SPRITE_BURLY_MAN_WITH_GREY_HAIR) and a woman in a white apron
  /// (SPRITE_WOMAN_IN_PINK_APRON).
  /// ★ Named from the game: "The fortress was built during Donpa's rule."
  /// (MsgLunpaInteriorsSecondHomeASealed)
  ACTOR_MAN_ON_DONPAS_FORTRESS,
  /// ★ Named from the game: "The inn's chef is the only one in town who can
  /// enter the fortress." (MsgLunpaInteriorsSecondHomeBSealed)
  ACTOR_WOMAN_ON_THE_INNS_CHEF,
  /// In the third home: this bent old man with a cane
  /// (SPRITE_OLD_MAN_WITH_CANE) and an old woman (SPRITE_OLD_WOMAN).
  /// ★ Named from the game: "Lunpa passed away long ago, when the world was hit
  /// by a huge flood." (MsgLunpaInteriorsThirdHomeASealed)
  ACTOR_OLD_MAN_ON_LUNPAS_DEATH,
  /// ★ Named from the game: "Lunpa was caught in the great flood while
  /// traveling with someone named Babi." (MsgLunpaInteriorsThirdHomeBSealed)
  ACTOR_OLD_WOMAN_ON_THE_GREAT_FLOOD,
  /// ★ Named from the game: "The weapons and armor vendors are looking
  /// desperate." (MsgLunpaItemMerchantReopened)
  ACTOR_WEAPON_MERCHANT,
  /// ★ Named from the game: "I'll sell you as much armor as you like, once the
  /// village is reopened." (MsgLunpaArmorMerchantSealed)
  ACTOR_ARMOR_MERCHANT,
  /// ★ Named from what the player sees: the merchant behind the item shop's
  /// counter (its render)
  ACTOR_ITEM_MERCHANT,
  /// ★ Named from the game: "Do you want rest in our inn?"
  /// (MsgLunpaInnkeeperOffersRoom)
  ACTOR_INNKEEPER,
  /// A woman in a white apron (SPRITE_WOMAN_IN_PINK_APRON) at the inn, beside
  /// the innkeeper.
  /// ★ Named from the game: "Are you Donpa's guest?"
  /// (MsgLunpaInnClerkAsksAboutGuest)
  ACTOR_WOMAN_ASKING_FOR_DONPAS_GUEST,
  /// ★ Named from the game: "He told me to air out the guest-room bedding and
  /// change the sheets." (MsgLunpaInteriorsInnWorkerReopened)
  ACTOR_INN_WORKER,
  /// ★ Named from the game: "The inn's chef is the only one in town who can
  /// enter the fortress." (MsgLunpaInteriorsSecondHomeBSealed)
  ACTOR_CHEF,
  /// Behind the temple's altar; across it, the Sanctum's services
  /// (RockRooms_TalkToHealer).
  /// ★ Named from the game: "Healer" (MsgDebugGlyphName)
  ACTOR_HEALER,
  /// In the Healer's sprite, a bearded man in a long robe
  /// (SPRITE_BEARDED_MAN_IN_LONG_ROBE), in the house by Suhalla Gate.
  /// ★ Named from the game: "I have been traveling the lands spreading good
  /// will." (MsgLunpaTravelingPriest)
  ACTOR_TRAVELING_HEALER,
  /// SPRITE_BLANK on the counter before each merchant, so that talking across
  /// it reaches the merchant's event.
  /// ★ Named from what the player sees: the counter before the weapon merchant
  /// (its render)
  ACTOR_WEAPON_COUNTER,
  /// ★ Named from what the player sees: the counter before the armor merchant
  /// (its render)
  ACTOR_ARMOR_COUNTER,
  /// ★ Named from what the player sees: the counter before the item merchant
  /// (its render)
  ACTOR_ITEM_COUNTER
};

/// Lunpa's shops, as Shop_Run numbers them.
enum { SHOP_LUNPA_ITEMS = 25, SHOP_LUNPA_WEAPONS = 29, SHOP_LUNPA_ARMOR = 30 };

/// Lunpa's inn, as Inn_CheckIn numbers the inns.
enum { LUNPA_INN = 8 };

#endif
