#ifndef ALCHEMY_LUNPA_SUHALLA_H
#define ALCHEMY_LUNPA_SUHALLA_H

#include "types.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "field_event.h"
#include "field_scene.h"
#include "flags.h"
#include "item_ids.h"

enum InteriorRoom {
  ROOM_FIRST_HOME = 1,
  ROOM_SECOND_HOME,
  ROOM_THIRD_HOME,
  ROOM_ARMS_SHOP,
  ROOM_ITEM_SHOP,
  ROOM_INN,
  ROOM_TEMPLE,
  ROOM_STOREROOM,
  ROOM_NINTH,
  ROOM_GATE_TO_SUHALLA,
  ROOM_TEMPLE_SECOND_ENTRANCE
};

#define ROOM_DOOR(room) {EVENT_EXIT, (room), CONDITION_ALWAYS, (room)}

enum InteriorTrigger {
  TRIGGER_WOODEN_BOX = 100,
  TRIGGER_BARREL,
  TRIGGER_FIRST_OVEN = 200,
  TRIGGER_SECOND_OVEN
};

/// The placed actors of Lunpa's houses, shops, inn and temple, numbered in the
/// order of gInteriorPlacements. Two villagers live in each of the three homes,
/// named by what they say while the town is sealed.
enum InteriorActor {
  /// In the first home.
  /// ★ Named from the game: "My late grandfather traveled the continent with
  /// Lunpa." (MsgLunpaInteriorsFirstHomeASealed)
  ACTOR_VILLAGER_LATE_GRANDFATHER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "We may live in Lunpa, but we'll never work for
  /// Dodonpa." (MsgLunpaInteriorsFirstHomeBSealed)
  ACTOR_VILLAGER_NEVER_WORKING_FOR_DODONPA,
  /// In the second home.
  /// ★ Named from the game: "The fortress was built during Donpa's rule."
  /// (MsgLunpaInteriorsSecondHomeASealed)
  ACTOR_VILLAGER_ON_DONPAS_FORTRESS,
  /// ★ Named from the game: "The inn's chef is the only one in town who can
  /// enter the fortress." (MsgLunpaInteriorsSecondHomeBSealed)
  ACTOR_VILLAGER_ON_THE_INNS_CHEF,
  /// In the third home.
  /// ★ Named from the game: "Lunpa passed away long ago, when the world was hit
  /// by a huge flood." (MsgLunpaInteriorsThirdHomeASealed)
  ACTOR_VILLAGER_ON_LUNPAS_DEATH,
  /// ★ Named from the game: "Lunpa was caught in the great flood while
  /// traveling with someone named Babi." (MsgLunpaInteriorsThirdHomeBSealed)
  ACTOR_VILLAGER_ON_THE_GREAT_FLOOD,
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
  /// At the inn, beside the innkeeper.
  /// ★ Named from the game: "Are you Donpa's guest?"
  /// (MsgLunpaInnClerkAsksAboutGuest)
  ACTOR_VILLAGER_ASKING_FOR_DONPAS_GUEST,
  /// ★ Named from the game: "He told me to air out the guest-room bedding and
  /// change the sheets." (MsgLunpaInteriorsInnWorkerReopened)
  ACTOR_INN_WORKER,
  /// ★ Named from the game: "The inn's chef is the only one in town who can
  /// enter the fortress." (MsgLunpaInteriorsSecondHomeBSealed)
  ACTOR_CHEF,
  /// Behind the temple's altar; across it, the Sanctum's services
  /// (Event_TalkToHealer).
  /// ★ Named from the game: "Healer" (MsgDebugGlyphName)
  ACTOR_HEALER,
  /// In the Healer's sprite, in the house by Suhalla Gate.
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

enum InteriorSprite {
  SPRITE_TOWNSPERSON_A = 0x65,
  SPRITE_TOWNSPERSON_B = 0x67,
  SPRITE_TOWNSPERSON_C = 0x6a,
  SPRITE_TOWNSPERSON_D = 0x6c,
  SPRITE_TOWNSPERSON_E = 0x6f,
  SPRITE_ITEM_MERCHANT = 0x76,
  SPRITE_INNKEEPER = 0x77,
  SPRITE_HEALER = 0x7a,
  SPRITE_WEAPON_MERCHANT = 0x7c,
  SPRITE_ARMOR_MERCHANT = 0x7d,
  SPRITE_TOWNSPERSON_F = 0x80
};

enum {
  SHOP_LUNPA_ITEMS = 25,
  SHOP_LUNPA_WEAPONS = 29,
  SHOP_LUNPA_ARMOR = 30,
  INN_LUNPA = 8
};

#endif
