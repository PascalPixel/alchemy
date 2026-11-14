#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/// @file
/// What every part of the game object shares: its includes and the inventory
/// helper two parts use. game.c includes it before the parts, and each part
/// includes it too, so that a part read on its own still resolves.

#include "ability_ids.h"
#include "debug/debug.h"
#include "djinn_ids.h"
#include "edition.h"
#include "enemy_ids.h"
#include "fixed_math.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "resource_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "battle/battle_action_run.h"
#include "battle/battle_random.h"
#include "battle/battle_side.h"
#include "battle/battle_sprites.h"
#include "battle/battle_unit.h"
#include "battle/playback_queue.h"
#include "battle/range_falloff.h"
#include "field/event.h"
#include "field/field.h"
#include "game/character.h"
#include "game/character_ids.h"
#include "game/enemy_element_preset.h"
#include "game/game.h"
#include "game/game_flags.h"
#include "game/inventory.h"
#include "game/item.h"
#include "game/starting_equipment.h"
#include "gamepak/far_call.h"
#include "graphics/battle_sprite_id.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/dma.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/iwram_call.h"
#include "system/main_routine_work.h"
#include "system/save.h"
#include "system/system.h"
#include "text/msg_ids.h"

/// Keeps occupied slots in order and clears the unused inventory tail, for
/// Inventory_Remove (game_inventory.c) and InventorySnapshot_Save
/// (game_snapshot.c). A slot is empty when it is 0.
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
