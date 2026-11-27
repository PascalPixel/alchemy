#ifndef ALCHEMY_LOG_ROOMS_A_H
#define ALCHEMY_LOG_ROOMS_A_H

/* The metatile behaviors of the layout of Vault's houses, shops and inn, which
   SceneId_LogRoomsA (log_rooms_a.c) and SceneId_LogRoomsB (log_rooms_b.c) both
   answer, and the stock and inn numbers their counters open. */

/* The stock and inn numbers Vault's counters open (mnShopRun, mnInnCheckIn). */
enum {
	VAULT_WEAPON_STOCK = 4,
	VAULT_ARMOR_STOCK = 5,
	VAULT_ITEM_STOCK = 6,
	VAULT_INN = 1
};

/* The metatiles the rooms' step cues and examine cues answer; the exits have
   metatile behaviors of their own. */
typedef enum LOG_ROOMS_A_METATILE_BEHAVIOR {
	/* The stairs down from the inn's guests' room. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_STAIRS = 7,
	/* Inside the guests' room, where the suspects first see Ivan. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM = 10,
	/* The first of the eight metatiles (to 18) that move the fleeing suspects
	   on along their routes. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_SUSPECT_ROUTE = 11,
	METATILE_BEHAVIOR_LOG_ROOMS_A_LAST_SUSPECT_ROUTE = 18,
	METATILE_BEHAVIOR_LOG_ROOMS_A_GUESTS_ROOM_DOOR = 20,
	/* In the guests' room, where the party sets out to corner the suspects. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_CORNERING_START = 21,
	/* The way east behind the crate from the inn's attic into the loft. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_IN = 26,
	/* The way west out of the loft, back into the attic. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_LOFT_WAY_OUT = 27,
	/* Either side of the dark hole in the attic's floor, which the leader
	   jumps. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_ATTIC_FLOOR_HOLE = 40,
	/* The barrel beside the girl at the inn, where the Bones are once the
	   thieves are jailed. */
	METATILE_BEHAVIOR_LOG_ROOMS_A_INN_BARREL = 41,
	METATILE_BEHAVIOR_LOG_ROOMS_A_JAR = 100,
	METATILE_BEHAVIOR_LOG_ROOMS_A_FIRST_BARREL = 101,
	METATILE_BEHAVIOR_LOG_ROOMS_A_SECOND_BARREL = 102,
	METATILE_BEHAVIOR_LOG_ROOMS_A_WOODEN_BOX = 103,
	METATILE_BEHAVIOR_LOG_ROOMS_A_HISTORY_BOOKCASE = 200,
	METATILE_BEHAVIOR_LOG_ROOMS_A_GEMS_BOOKCASE = 201
} LOG_ROOMS_A_METATILE_BEHAVIOR;

#endif
