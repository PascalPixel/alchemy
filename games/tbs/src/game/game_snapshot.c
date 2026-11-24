/// @file
/// Equipping a given item, and the equipment put aside for the Colosso.
// Part of game.c.

#include "game_internal.h"

/// Gives a unit `itemId` and equips every exactly matching inventory entry.
void Inventory_AddAndEquip(s32 unitId, s32 itemId) {
  struct Unit *unit = Unit_Get(unitId);
  s32 slot;

  Inventory_AddItem(unitId, itemId);
  for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
    if (unit->inventory[slot] == itemId)
      Inventory_Equip(unitId, slot);
  }
}

/// The first halfword of gInventorySnapshot while it holds the party's
/// equipment, put aside while the Colosso takes it away.
#define INVENTORY_SNAPSHOT_SENTINEL 26484

/// What gInventorySnapshot holds while the Colosso has the party's
/// equipment: 65 halfwords of the 70 it reserves.
struct InventorySnapshot {
  u16 sentinel; ///< INVENTORY_SNAPSHOT_SENTINEL while it holds them
  u16 inventories[4][INVENTORY_SLOTS]; ///< Isaac's to Mia's
  u16 firstShortcut;
  u16 secondShortcut;
  u8 party[4]; ///< the party order
};

STATIC_ASSERT(offsetof(struct InventorySnapshot, firstShortcut) == 122);
STATIC_ASSERT(offsetof(struct InventorySnapshot, party) == 126);

/// Saves Isaac's to Mia's inventories, the shortcuts and the party order,
/// then keeps only the Psynergy-granting items, gives Isaac the Short Sword,
/// sets FLAG_COLOSSO_HOLDS_EQUIPMENT and restores everyone's HP and PP. Nothing
/// happens when a snapshot is already saved, apart from the restore.
void InventorySnapshot_Save(void) {
  u16 *save;
  s16 *inventory;
  s32 unitId;
  s32 slot;
  s16 firstShortcut;
  s16 secondShortcut;

  save = gInventorySnapshot;
  if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
    *save++ = INVENTORY_SNAPSHOT_SENTINEL;
    firstShortcut = gMainRoutineState.firstShortcut;
    secondShortcut = gMainRoutineState.secondShortcut;
    // Save each inventory before retaining its Psynergy-granting items.
    for (unitId = ISAAC; unitId <= MIA; unitId++) {
      struct Unit *unit = Unit_Get(unitId);

      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *save++ = unit->inventory[slot];
      for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if (Item_GetDirect(unit->inventory[slot])->type != ITEM_TYPE_PSYNERGY)
          unit->inventory[slot] = 0;
      }
      inventory = unit->inventory;
      Inventory_CompactSlots(inventory);
      Unit_UpdateClass(unitId);
      Unit_Calculate(unitId);
    }
    *save++ = firstShortcut;
    *save++ = secondShortcut;
    // UB: the party order's four bytes are read as two halfwords, through a
    // pointer of another type.
#ifdef UBFIX
    save[0] = gMainRoutineState.party[0] | (gMainRoutineState.party[1] << 8);
    save[1] = gMainRoutineState.party[2] | (gMainRoutineState.party[3] << 8);
#else
    inventory = (s16 *)gMainRoutineState.party;
    save[0] = inventory[0];
    save[1] = inventory[1];
#endif
    Inventory_AddAndEquip(ISAAC, ITEM_SHORT_SWORD);
    GameFlag_Set(FLAG_COLOSSO_HOLDS_EQUIPMENT);
  }
  Party_RestoreHpPp(1);
}

/// Gives back the inventories, shortcuts and party order InventorySnapshot_Save
/// put aside, then clears the snapshot and FLAG_COLOSSO_HOLDS_EQUIPMENT.
void InventorySnapshot_Restore(void) {
  s32 unitId;
  u16 *source = gInventorySnapshot;

  if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
    for (unitId = ISAAC; unitId <= MIA; unitId++) {
      u16 *inventory = Unit_Get(unitId)->inventory;
      s32 slot;

      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *inventory++ = *source++;
      Unit_UpdateClass(unitId);
      Unit_Calculate(unitId);
    }

    gMainRoutineState.firstShortcut = *source++;
    gMainRoutineState.secondShortcut = *source++;
    // UB: the party order's four bytes are written as two halfwords, through a
    // pointer of another type.
#ifdef UBFIX
    gMainRoutineState.party[0] = source[0];
    gMainRoutineState.party[1] = source[0] >> 8;
    gMainRoutineState.party[2] = source[1];
    gMainRoutineState.party[3] = source[1] >> 8;
    source++;
#else
    *(u16 *)gMainRoutineState.party = *source++;
    *(u16 *)&gMainRoutineState.party[2] = *source;
#endif
    source = gInventorySnapshot;
    *source = 0;
    GameFlag_Clear(FLAG_COLOSSO_HOLDS_EQUIPMENT);
  }
}

#undef INVENTORY_SNAPSHOT_SENTINEL
