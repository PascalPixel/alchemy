#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

// What every part of the game object shares: its includes and the inventory helper two parts use.
// game.c includes it once, before the parts.

#include "ability_ids.h"
#include "debug/debug.h"
#include "enemy_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "battle/battle_random.h"
#include "battle/battle_side.h"
#include "field/script.h"
#include "game/game_flags.h"
#include "game/starting_equipment.h"
#include "graphics/graphics.h"
#include "system/bup.h"
#include "text/msg_ids.h"

// Keeps occupied slots in order and clears the unused inventory tail, for gmInventoryRemove
// (game_inventory.c) and gmInventorySnapshotSave (game_snapshot.c). A slot is empty when it is 0.
static inline void gmInventoryCompactSlots(Sint16 *inventory)
{
    Sint32 usedSlots = 0;
    Sint32 slot;

    for (slot = 0; slot < INVENTORY_SLOTS; slot++)
    {
        if (inventory[slot] != 0)
            inventory[usedSlots++] = inventory[slot];
    }
    for (; usedSlots < INVENTORY_SLOTS; usedSlots++)
        inventory[usedSlots] = 0;
}

#endif
