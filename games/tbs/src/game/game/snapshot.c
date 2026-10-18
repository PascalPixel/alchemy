// Part of game.c, the game object, compiled only through it.
// Equipping a given item, and the equipment put aside for the Colosso.

#include "../game_internal.h"

// Gives a unit `itemId` and equips every exactly matching inventory entry.
void Inventory_AddAndEquip(s32 owner, s32 itemId) {
  struct BattleUnit *unit = UnitGet(owner);
  u16 *entry;
  s32 slot;

  Inventory_AddItem(owner, itemId);
  slot = 0;
  entry = unit->inventory;
  do {
    if (*entry++ == itemId)
      Inventory_Equip(owner, slot);
    slot++;
  } while (slot < INVENTORY_SLOTS);
}

// The party's equipment, put aside while the Colosso takes it away; the
// sentinel marks the snapshot as taken.
#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

// Saves the first four characters' inventories, shortcuts and party order,
// then keeps only the Psynergy-granting items (type 6), gives Isaac item 16,
// sets flag 0x952 and restores everyone. Nothing happens when a snapshot is
// already saved, apart from the restore.
void InventorySnapshot_Save(void) {
  u16 *save;
  struct BattleUnit *unit;
  s16 *inventory;
  s32 owner;
  s32 slot;
  s16 firstShortcut;
  s16 secondShortcut;

  save = gInventorySnapshot;
  if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
    *save++ = INVENTORY_SNAPSHOT_SENTINEL;
    firstShortcut = gGameState.firstShortcut;
    secondShortcut = gGameState.secondShortcut;
    // Save each inventory before retaining its Psynergy-granting items.
    for (owner = 0; owner < 4; owner++) {
      unit = UnitGet(owner);
      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *save++ = unit->inventory[slot];
      for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if (Item_GetDirect(unit->inventory[slot])->type != 6)
          unit->inventory[slot] = 0;
      }
      inventory = unit->inventory;
      Inventory_CompactSlots(inventory);
      Unit_UpdateClass(owner);
      UnitCalc(owner);
    }
    // Keep shortcuts and party order with the inventory snapshot.
    *save++ = firstShortcut;
    *save++ = secondShortcut;
#ifdef UBFIX
    // Party order is a byte array; pack its two saved halfwords explicitly.
    save[0] = gGameState.activeOwners[0] | (gGameState.activeOwners[1] << 8);
    save[1] = gGameState.activeOwners[2] | (gGameState.activeOwners[3] << 8);
#else
    inventory = (s16 *)gGameState.activeOwners;
    save[0] = inventory[0];
    save[1] = inventory[1];
#endif
    Inventory_AddAndEquip(0, 16);
    GameFlag_SetBit(0x952);
  }
  Party_RestoreHpPp(1);
}

// Gives back the inventories, shortcuts and party order InventorySnapshot_Save
// put aside.
void InventorySnapshot_Restore(void) {
  s32 owner;
  u16 *source = gInventorySnapshot;

  if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
    owner = 0;
    do {
      u16 *inventory = UnitGet(owner)->inventory;
      s32 remaining = 14;

      do {
        *inventory++ = *source++;
        remaining--;
      } while (remaining >= 0);
      Unit_UpdateClass(owner);
      UnitCalc(owner);
      owner++;
    } while (owner <= 3);

    gGameState.firstShortcut = *source++;
    gGameState.secondShortcut = *source++;
#ifdef UBFIX
    gGameState.activeOwners[0] = source[0];
    gGameState.activeOwners[1] = source[0] >> 8;
    gGameState.activeOwners[2] = source[1];
    gGameState.activeOwners[3] = source[1] >> 8;
    source++;
#else
    *(u16 *)gGameState.activeOwners = *source++;
    *(u16 *)((u8 *)gGameState.activeOwners + 2) = *source;
#endif
    source = gInventorySnapshot;
    *source = 0;
    GameFlag_ClearBit(0x952);
  }
}
