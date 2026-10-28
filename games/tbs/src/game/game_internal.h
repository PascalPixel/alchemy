#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/// @file
/// What every part of the game object shares: its includes and the inventory
/// helper two parts use. game.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "module/game/game.h"
#include "module/system/system.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "heap_release.h"
#include "unit.h"
#include "character.h"
#include "main_routine_work.h"
#include "runtime_interfaces.h"
#include "item.h"
#include "game_flags.h"
#include "inventory.h"
#include "battle_sprite_id.h"
#include "resource_ids.h"
#include "dma.h"
#include "main_routine_work.h"
#include "frame.h"
#include "save.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_unit.h"
#include "lib.h"
#include "edition.h"
#include "battle_system.h"
#include "text/msg_ids.h"
#include "battle_sprites.h"
#include "enemy_element_preset.h"
#include "iwram_call.h"
#include "range_falloff.h"
#include "battle_random.h"
#include "battle_action_run.h"
#include "battle_effect_chance.h"
#include "battle_side.h"
#include "event.h"
#include "module/game/starting_equipment.h"
#include "flags.h"
#include "character_ids.h"
#include "ability_ids.h"
#include "macros.h"
#include "item_ids.h"
#include "djinn_ids.h"
#include "enemy_ids.h"

/// Keeps occupied slots in order and clears the unused inventory tail, for
/// Inventory_Remove (inventory.c) and InventorySnapshot_Save (snapshot.c). A
/// slot is empty when it is 0.
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
