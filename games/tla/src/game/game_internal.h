#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/// @file
/// What every part of the game object shares: its includes, the declarations
/// its parts disagreed on and one inventory helper. game.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves.

#include "types.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "version.h"
#include "string.h"
#include "heap_release.h"
#include "dma.h"
#include "iwram_call.h"
#include "frame.h"
#include "fixed_math.h"
#include "resource.h"
#include "scene.h"
#include "main_routine_work.h"
#include "game_flags.h"
#include "owner_state.h"
#include "owner_value.h"
#include "item.h"
#include "inventory.h"
#include "battle_type.h"
#include "battle_unit.h"
#include "battle_summon.h"
#include "battle_random.h"
#include "enemy_definition.h"
#include "range_falloff.h"
#include "module/game/game.h"

// The game object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
/* VIEW(game): routine GameFlag_SetBit as s32 GameFlag_SetBit(s32 flag); flag_call.h has void GameFlag_SetBit(s32 flag). */
extern s32 GameFlag_SetBit__game(s32 flag) __asm__("GameFlag_SetBit");
struct AbilityDefinition *Ability_GetDirect(s32 actionId);

/// Keeps occupied slots in order and clears the unused inventory tail, for
/// Func_080af0e4 and Inventory_Remove (inventory.c) and InventorySnapshot_Save
/// (snapshot.c). A slot is empty when it is 0.
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
