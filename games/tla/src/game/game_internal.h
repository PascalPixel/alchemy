#ifndef ALCHEMY_GAME_INTERNAL_H
#define ALCHEMY_GAME_INTERNAL_H

/// @file
/// What every part of the game object shares: its includes, the declarations
/// its parts disagreed on and one inventory helper. game.c includes it
/// before the parts, and each part includes it too, so that a part read on its
/// own still resolves.

#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "edition.h"
#include "version.h"
#include "string.h"
#include "system/heap_release.h"
#include "system/dma.h"
#include "system/iwram_call.h"
#include "system/frame.h"
#include "fixed_math.h"
#include "graphics/resource.h"
#include "system/scene.h"
#include "system/main_routine_state.h"
#include "game/game_flags.h"
#include "game/owner_state.h"
#include "game/owner_value.h"
#include "game/item.h"
#include "game/inventory.h"
#include "battle/battle_type.h"
#include "battle/battle_unit.h"
#include "battle/battle_summon.h"
#include "battle/battle_random.h"
#include "game/enemy_definition.h"
#include "ability_ids.h"
#include "djinn_ids.h"
#include "enemy_ids.h"
#include "item_ids.h"
#include "battle/range_falloff.h"

// The game object's functions its parts call before their definitions, or
// with declarations of their own that disagree.
struct AbilityDefinition *Ability_GetDirect(s32 actionId);

/// Keeps occupied slots in order and clears the unused inventory tail, for
/// Inventory_ClearSlot and Inventory_Remove (game_inventory.c) and
/// InventorySnapshot_Save (game_snapshot.c). A slot is empty when it is 0.
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

/// The rare item counters RareItemStock_Adjust keeps (an index from here on
/// has no stock), and the most one counts.
enum { ITEM_COUNTER_COUNT = 300, ITEM_COUNTER_MAX = 99 };

/// A character's element group (gCharacter_ElementGroupTable): which two
/// lighthouses Party_RestorePpOfLitElements restores its PP in.
enum ElementGroup { ELEMENT_GROUP_VENUS_MARS, ELEMENT_GROUP_MERCURY_JUPITER };

/// The marks on a unit's Psynergy slot of an action its class or an item gives
/// it, cleared before the class's own are listed again.
enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

/// Scratch for Unit_LevelUp, which takes it from EWRAM first
/// (Heap_AllocEwram) and frees it before returning.
struct LevelUpState {
  s32 classIndex; ///< the unit's class, written and not read again
  s32 level;      ///< the level before this one
  const struct CharacterDefinition *growth; ///< the character's growth curves
  u8 unused[32];                            ///< never written
};

/// The running totals Unit_Calculate builds a unit's stats in.
struct StatState {
  s32 hp;          ///< maximum HP
  s32 pp;          ///< maximum PP
  s32 attack;      ///< Attack, 0 to UNIT_STAT_MAX once clamped
  s32 defense;     ///< Defense, 0 to UNIT_STAT_MAX once clamped
  s32 agility;     ///< Agility, 0 to UNIT_STAT_MAX once clamped
  s32 unused;      ///< never written; a gap between Agility and Luck
  s32 luck;        ///< Luck, 0 to UNIT_LUCK_MAX once clamped
  s32 actionCount; ///< actions a turn
  s32 hpRegen;     ///< HP regained each turn
  s32 ppRegen;     ///< PP regained each turn
  struct {
    s32 power;
    s32 resistance;
  } element[ELEMENT_COUNT];    ///< each element's power and resistance
  s32 kind;                    ///< the item effect being applied
  s32 unusedAfterKind[2];      ///< never written
  s32 amount;                  ///< the amount of the item effect being applied
  struct ItemDefinition *item; ///< the equipped item being applied
  s32 unusedAfterItem;         ///< never written
};

/// How many of each rare item the shops may still sell, 0 to 99, by counter
/// number (gItem_ArtifactSlotTable; RareItemStock_Adjust).
extern u8 gRareItemStock[ITEM_COUNTER_COUNT];

#endif
