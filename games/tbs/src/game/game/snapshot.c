/// @file
/// Equipping a given item, and the equipment put aside for the Colosso.
// Part of game.c.

#include "../game_internal.h"

/// Gives a unit `itemId` and equips every exactly matching inventory entry.
/// Called by InventorySnapshot_Save to arm Isaac for the Colosso.
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
#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

/// What gInventorySnapshot holds while the Colosso has the party's
/// equipment: 65 halfwords of the 70 it reserves.
struct InventorySnapshot {
  u16 sentinel; ///< INVENTORY_SNAPSHOT_SENTINEL while it holds them
  u16 inventories[4][INVENTORY_SLOTS]; ///< Isaac's to Mia's
  u16 firstShortcut;
  u16 secondShortcut;
  u8 party[4]; ///< the party order
};

/// Saves the first four characters' inventories, shortcuts and party order,
/// then keeps only the Psynergy-granting items, gives Isaac the Short
/// Sword, sets FLAG_COLOSSO_HOLDS_EQUIPMENT and restores everyone's HP and PP.
/// Nothing happens when a snapshot is already saved, apart from the restore.
/// Called by the Colosseum entrance scene as Isaac enters the Colosso.
void InventorySnapshot_Save(void) {
  struct InventorySnapshot *snapshot =
      (struct InventorySnapshot *)gInventorySnapshot;
  s32 unitId;
  s32 slot;

  if (snapshot->sentinel != INVENTORY_SNAPSHOT_SENTINEL) {
    snapshot->sentinel = INVENTORY_SNAPSHOT_SENTINEL;
    // Save each inventory before keeping only its Psynergy-granting items.
    for (unitId = 0; unitId < 4; unitId++) {
      struct Unit *unit = Unit_Get(unitId);

      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        snapshot->inventories[unitId][slot] = unit->inventory[slot];
      for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if (Item_GetDirect(unit->inventory[slot])->type != ITEM_TYPE_PSYNERGY)
          unit->inventory[slot] = 0;
      }
      Inventory_CompactSlots(unit->inventory);
      Unit_UpdateClass(unitId);
      Unit_Calculate(unitId);
    }
    snapshot->firstShortcut = gMainRoutineWork.firstShortcut;
    snapshot->secondShortcut = gMainRoutineWork.secondShortcut;
    for (slot = 0; slot < 4; slot++)
      snapshot->party[slot] = gMainRoutineWork.party[slot];
    Inventory_AddAndEquip(ISAAC, ITEM_SHORT_SWORD);
    GameFlag_Set(FLAG_COLOSSO_HOLDS_EQUIPMENT);
  }
  Party_RestoreHpPp(1);
}

/// Gives back the inventories, shortcuts and party order InventorySnapshot_Save
/// put aside, then clears the snapshot and FLAG_COLOSSO_HOLDS_EQUIPMENT. Called
/// by the Babi palace scene after the Colosso, and by the File Screen.
void InventorySnapshot_Restore(void) {
  struct InventorySnapshot *snapshot =
      (struct InventorySnapshot *)gInventorySnapshot;
  s32 unitId;
  u16 *source = gInventorySnapshot;

  if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
    unitId = 0;
    do {
      u16 *inventory = Unit_Get(unitId)->inventory;
      s32 remaining = 14;

      do {
        *inventory++ = *source++;
        remaining--;
      } while (remaining >= 0);
      Unit_UpdateClass(unitId);
      Unit_Calculate(unitId);
      unitId++;
    } while (unitId <= 3);

    gMainRoutineWork.firstShortcut = *source++;
    gMainRoutineWork.secondShortcut = *source++;
#ifdef UBFIX
    gMainRoutineWork.party[0] = source[0];
    gMainRoutineWork.party[1] = source[0] >> 8;
    gMainRoutineWork.party[2] = source[1];
    gMainRoutineWork.party[3] = source[1] >> 8;
    source++;
#else
    *(u16 *)gMainRoutineWork.party = *source++;
    *(u16 *)((u8 *)gMainRoutineWork.party + 2) = *source;
#endif
    source = gInventorySnapshot;
    *source = 0;
    GameFlag_Clear(FLAG_COLOSSO_HOLDS_EQUIPMENT);
  }
}

#undef INVENTORY_SNAPSHOT_SENTINEL
