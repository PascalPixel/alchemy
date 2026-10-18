#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

// What every part of the game object shares: its includes and the inventory
// helper two parts use. game.c includes it before the parts, and each part
// includes it too, so that a part read on its own still resolves.

#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "scene.h"
#include "owner_state.h"
#include "character.h"
#include "party_state.h"
#include "runtime_interfaces.h"
#include "item.h"
#include "game_flags.h"
#include "inventory.h"
#include "resource.h"
#include "resource_ids.h"
#include "dma.h"
#include "game_state.h"
#include "frame.h"
#include "save_state.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_unit.h"
#include "lib.h"
#include "edition.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "battle_summon.h"
#include "preset_table.h"
#include "iwram_call.h"
#include "battle_calc.h"
#include "battle_random.h"
#include "battle_type.h"
#include "battle_effect_chance.h"
#include "battle_party.h"
#include "field_event.h"
#include "unit/game/starting_equipment.h"
#include "flags.h"
#include "character_ids.h"
#include "ability_ids.h"
#include "macros.h"

// Keeps occupied slots in order and clears the unused inventory tail, for
// Inventory_Remove (inventory.c) and InventorySnapshot_Save (snapshot.c).
static inline void Inventory_CompactSlots(s16 *inventory) {
  s32 usedSlots = 0;
  s32 slot;

  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (inventory[slot] != 0)
      inventory[usedSlots++] = inventory[slot];
  }
  for (; usedSlots < INVENTORY_SLOTS; usedSlots++)
    inventory[usedSlots] = 0;
}

#endif
