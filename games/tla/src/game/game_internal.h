#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/* What every part of the game object shares: its includes, the declarations its
   parts disagreed on and one inventory helper. game.c includes it before the
   parts, which rely on it and do not include it again. */

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "edition.h"
#include "string.h"
#include "game/game_flags.h"
#include "game/inventory.h"
#include "ability_ids.h"
#include "enemy_ids.h"
#include "item_ids.h"

/* Keeps occupied slots in order and clears the unused inventory tail, for
   gmInventoryClearSlot and gmInventoryRemove (game_inventory.c) and
   gmInventorySnapshotSave (game_snapshot.c). A slot is empty when it is 0. */
static inline void gmInventoryCompactSlots(Sint16 *inventory)
{
	Sint32 usedSlots = 0;
	Sint32 slot;

	for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
		if(inventory[slot] != 0)
			inventory[usedSlots++] = inventory[slot];
	}
	for(; usedSlots < INVENTORY_SLOTS; usedSlots++)
		inventory[usedSlots] = 0;
}

/* The rare item counters gmRareItemStockAdjust keeps (an index from here on has
   no stock), and the most one counts. */
enum { ITEM_COUNTER_COUNT = 300, ITEM_COUNTER_MAX = 99 };

/* A character's element group (CharacterElementGroupTable): which two
   lighthouses gmPartyRestorePpInLighthouse restores its PP in. */
typedef enum ELEMENT_GROUP {
	ELEMENT_GROUP_VENUS_MARS,
	ELEMENT_GROUP_MERCURY_JUPITER
} ELEMENT_GROUP;

/* The marks on a unit's Psynergy slot of an action its class or an item gives
   it, cleared before the class's own are listed again. */
enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

/* Scratch for gmUnitLevelUp, which takes it from EWRAM first (syHeapAllocEWRAM)
   and frees it before returning. */
typedef struct LEVEL_UP_STATE {
	Sint32 classIdx; /* the unit's class, written and not read again */
	Sint32 level;	 /* the level before this one */
	const CHARACTER_DEFINITION *growth;		/* the character's growth curves */
	Uint8						unused[32]; /* never written */
} LEVEL_UP_STATE;

/* The running totals gmUnitCalc builds a unit's stats in. */
typedef struct STAT_STATE {
	Sint32 hp;		  /* maximum HP */
	Sint32 pp;		  /* maximum PP */
	Sint32 attack;	  /* Attack, 0 to UNIT_STAT_MAX once clamped */
	Sint32 defense;	  /* Defense, 0 to UNIT_STAT_MAX once clamped */
	Sint32 agility;	  /* Agility, 0 to UNIT_STAT_MAX once clamped */
	Sint32 unused;	  /* never written; a gap between Agility and Luck */
	Sint32 luck;	  /* Luck, 0 to UNIT_LUCK_MAX once clamped */
	Sint32 actionCnt; /* actions a turn */
	Sint32 hpRegen;	  /* HP regained each turn */
	Sint32 ppRegen;	  /* PP regained each turn */

	struct {
		Sint32 power;
		Sint32 resistance;
	} element[ELEMENT_COUNT]; /* each element's power and resistance */

	Sint32			 kind;				 /* the item effect being applied */
	Sint32			 unusedAfterKind[2]; /* never written */
	Sint32			 amount; /* the amount of the item effect being applied */
	ITEM_DEFINITION *item;	 /* the equipped item being applied */
	Sint32			 unusedAfterItem; /* never written */
} STAT_STATE;

#endif
