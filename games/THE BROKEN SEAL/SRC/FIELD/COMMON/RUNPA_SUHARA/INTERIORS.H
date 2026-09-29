#ifndef ALCHEMY_INTERIORS_H
#define ALCHEMY_INTERIORS_H

#include "TYPES.H"
#include "ABILITY_IDS.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FLAG_IDS.H"
#include "ITEM_IDS.H"

/*
 * The rooms behind Lunpa's doors: three homes, the weapon and armor shop,
 * the item shop, the inn, the temple, and the house on the way to the
 * Suhalla Gate. Each room's door trigger, exit and entrance share the
 * room's number. Until Lunpa trades again the merchants and the inn turn
 * the party away; afterwards the shops, inn and sanctum open to anyone
 * facing their counter. Counters face south, so the party faces a counter
 * when it looks north.
 */

enum InteriorRoom {
    ROOM_FIRST_HOME = 1,
    ROOM_SECOND_HOME,
    ROOM_THIRD_HOME,
    ROOM_ARMS_SHOP,
    ROOM_ITEM_SHOP,
    ROOM_INN,
    ROOM_TEMPLE,
    ROOM_EIGHTH,
    ROOM_NINTH,
    ROOM_SUHARA_GATE_HOUSE,
    ROOM_TEMPLE_SECOND_ENTRANCE
};

/* A room's door takes the party out through the exit of the same number. */
#define ROOM_DOOR(room) { EVENT_EXIT, (room), CONDITION_ALWAYS, (room) }

enum LunpaEntrance {
    LUNPA_ENTRANCE_FROM_FIRST_HOME = 4,
    LUNPA_ENTRANCE_FROM_SECOND_HOME,
    LUNPA_ENTRANCE_FROM_THIRD_HOME,
    LUNPA_ENTRANCE_FROM_ARMS_SHOP,
    LUNPA_ENTRANCE_FROM_ITEM_SHOP,
    LUNPA_ENTRANCE_FROM_INN,
    LUNPA_ENTRANCE_FROM_TEMPLE,
    LUNPA_ENTRANCE_FROM_EIGHTH_ROOM
};

enum {
    SUHARA_GATE_ENTRANCE_FROM_LUNPA = 3
};

enum InteriorTrigger {
    TRIGGER_WOODEN_BOX = 100,
    TRIGGER_BARREL,
    TRIGGER_FIRST_OVEN = 200,
    TRIGGER_SECOND_OVEN
};

enum InteriorFlag {
    FLAG_INTERIOR_WOODEN_BOX = 0xf8a,
    FLAG_INTERIOR_BARREL
};

enum InteriorActor {
    ACTOR_FIRST_HOME_RESIDENT_A = ACTOR_FIRST_PLACED,
    ACTOR_FIRST_HOME_RESIDENT_B,
    ACTOR_SECOND_HOME_RESIDENT_A,
    ACTOR_SECOND_HOME_RESIDENT_B,
    ACTOR_THIRD_HOME_RESIDENT_A,
    ACTOR_THIRD_HOME_RESIDENT_B,
    ACTOR_WEAPON_MERCHANT,
    ACTOR_ARMOR_MERCHANT,
    ACTOR_ITEM_MERCHANT,
    ACTOR_INNKEEPER,
    ACTOR_INN_CLERK,
    ACTOR_INN_WORKER,
    ACTOR_CHEF,
    ACTOR_TEMPLE_PRIEST,
    ACTOR_TRAVELING_PRIEST,
    ACTOR_WEAPON_COUNTER,
    ACTOR_ARMOR_COUNTER,
    ACTOR_ITEM_COUNTER
};

enum InteriorSprite {
    SPRITE_COUNTER = 0x16,
    SPRITE_TOWNSPERSON_A = 0x65,
    SPRITE_TOWNSPERSON_B = 0x67,
    SPRITE_TOWNSPERSON_C = 0x6a,
    SPRITE_TOWNSPERSON_D = 0x6c,
    SPRITE_TOWNSPERSON_E = 0x6f,
    SPRITE_ITEM_MERCHANT = 0x76,
    SPRITE_INNKEEPER = 0x77,
    SPRITE_PRIEST = 0x7a,
    SPRITE_WEAPON_MERCHANT = 0x7c,
    SPRITE_ARMOR_MERCHANT = 0x7d,
    SPRITE_TOWNSPERSON_F = 0x80,
    SPRITE_CHEF = 0x84
};

enum {
    SHOP_LUNPA_ITEMS = 25,
    SHOP_LUNPA_WEAPONS = 29,
    SHOP_LUNPA_ARMOR = 30,
    INN_LUNPA = 8
};

/* Lines spoken while Lunpa is sealed, then after it trades again. */
enum InteriorMessage {
    MSG_FIRST_HOME_A_SEALED = 0x1bc1,
    MSG_FIRST_HOME_B_SEALED,
    MSG_FIRST_HOME_A_SEALED_THOUGHTS,
    MSG_FIRST_HOME_B_SEALED_THOUGHTS,
    MSG_SECOND_HOME_A_SEALED,
    MSG_SECOND_HOME_B_SEALED,
    MSG_SECOND_HOME_A_SEALED_THOUGHTS,
    MSG_SECOND_HOME_B_SEALED_THOUGHTS,
    MSG_THIRD_HOME_A_SEALED,
    MSG_THIRD_HOME_B_SEALED,
    MSG_THIRD_HOME_A_SEALED_THOUGHTS,
    MSG_THIRD_HOME_B_SEALED_THOUGHTS,
    MSG_INN_CLERK_SEALED = 0x1bd3,
    MSG_INN_WORKER_SEALED,
    MSG_INNKEEPER_SEALED_THOUGHTS = 0x1bd8,
    MSG_INN_CLERK_SEALED_THOUGHTS,
    MSG_INN_WORKER_SEALED_THOUGHTS,
    MSG_TEMPLE_PRIEST_SEALED_THOUGHTS = 0x1bdd,

    MSG_FIRST_HOME_A_REOPENED = 0x24e9,
    MSG_FIRST_HOME_B_REOPENED,
    MSG_FIRST_HOME_A_REOPENED_THOUGHTS,
    MSG_FIRST_HOME_B_REOPENED_THOUGHTS,
    MSG_SECOND_HOME_A_REOPENED,
    MSG_SECOND_HOME_B_REOPENED,
    MSG_SECOND_HOME_A_REOPENED_THOUGHTS,
    MSG_SECOND_HOME_B_REOPENED_THOUGHTS,
    MSG_THIRD_HOME_A_REOPENED,
    MSG_THIRD_HOME_B_REOPENED,
    MSG_THIRD_HOME_A_REOPENED_THOUGHTS,
    MSG_THIRD_HOME_B_REOPENED_THOUGHTS,
    MSG_INN_WORKER_REOPENED = 0x2501,
    MSG_CHEF_REOPENED,
    MSG_INNKEEPER_REOPENED_THOUGHTS,
    MSG_INN_CLERK_REOPENED_THOUGHTS,
    MSG_INN_WORKER_REOPENED_THOUGHTS,
    MSG_CHEF_REOPENED_THOUGHTS,
    MSG_TEMPLE_PRIEST_REOPENED_THOUGHTS = 0x2508,
    MSG_TRAVELING_PRIEST_THOUGHTS = 0x266c,

    MSG_FIRST_OVEN = 0x29ba,
    MSG_SECOND_OVEN
};

extern const struct SceneEntrance gInteriorEntrances[];
extern const u32 gInteriorExits[];
extern const struct ScenePlacement gInteriorPlacements[];
extern const struct SceneEvent gInteriorSealedEvents[];
extern const struct SceneEvent gInteriorReopenedEvents[];
extern const struct SceneEvent gSuharaGateHouseEvents[];

/* Event scripts the tables reach in every run of the overlay's code. */
void Innkeeper_Talk(void);
void Chef_Talk(void);
void Chef_ReadMind(void);
void InnClerk_Talk(void);
void TemplePriest_Talk(void);
void ItemMerchant_ReadMind(void);
void ItemMerchant_Talk(void);
void WeaponMerchant_ReadMind(void);
void WeaponMerchant_Talk(void);
void ArmorMerchant_ReadMind(void);
void ArmorMerchant_Talk(void);
void TravelingPriest_Talk(void);

#endif
