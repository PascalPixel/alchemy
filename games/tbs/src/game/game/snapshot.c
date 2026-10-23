/// @file
/// Equipping a given item, and the equipment put aside for the Colosso.
// Part of game.c.

#include "../game_internal.h"

/// Gives a unit `itemId` and equips every exactly matching inventory entry.
/// Called by InventorySnapshot_Save to arm Isaac for the Colosso.
void Inventory_AddAndEquip(s32 owner, s32 itemId) {
  struct Unit *unit = Unit_Get(owner);
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

/// The first halfword of gInventorySnapshot while it holds the party's
/// equipment, put aside while the Colosso takes it away. After it come four
/// inventories of 15 slots, the two shortcuts and the four party order
/// bytes: 65 halfwords of the 70 that gInventorySnapshot reserves.
#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

/// Saves the first four characters' inventories, shortcuts and party order,
/// then keeps only the Psynergy-granting items, gives Isaac the Short
/// Sword, sets FLAG_COLOSSO_HOLDS_EQUIPMENT and restores everyone's HP and PP.
/// Nothing happens when a snapshot is already saved, apart from the restore.
/// Called by the Colosseum entrance map as Isaac enters the Colosso.
void InventorySnapshot_Save(void) {
  u16 *save;
  struct Unit *unit;
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
      unit = Unit_Get(owner);
      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *save++ = unit->inventory[slot];
      for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        if (Item_GetDirect(unit->inventory[slot])->type != ITEM_TYPE_PSYNERGY)
          unit->inventory[slot] = 0;
      }
      inventory = unit->inventory;
      Inventory_CompactSlots(inventory);
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
    }
    // Keep shortcuts and party order with the inventory snapshot.
    *save++ = firstShortcut;
    *save++ = secondShortcut;
#ifdef UBFIX
    // Party order is a byte array; pack its two saved halfwords explicitly.
    save[0] = gGameState.party[0] | (gGameState.party[1] << 8);
    save[1] = gGameState.party[2] | (gGameState.party[3] << 8);
#else
    inventory = (s16 *)gGameState.party;
    save[0] = inventory[0];
    save[1] = inventory[1];
#endif
    Inventory_AddAndEquip(ISAAC, ITEM_SHORT_SWORD);
    GameFlag_SetBit(FLAG_COLOSSO_HOLDS_EQUIPMENT);
  }
  Party_RestoreHpPp(1);
}

/// Gives back the inventories, shortcuts and party order InventorySnapshot_Save
/// put aside, then clears the snapshot and FLAG_COLOSSO_HOLDS_EQUIPMENT. Called
/// by the Babi palace map after the Colosso, and by the file select screen.
void InventorySnapshot_Restore(void) {
  s32 owner;
  u16 *source = gInventorySnapshot;

  if (*source++ == INVENTORY_SNAPSHOT_SENTINEL) {
    owner = 0;
    do {
      u16 *inventory = Unit_Get(owner)->inventory;
      s32 remaining = 14;

      do {
        *inventory++ = *source++;
        remaining--;
      } while (remaining >= 0);
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
      owner++;
    } while (owner <= 3);

    gGameState.firstShortcut = *source++;
    gGameState.secondShortcut = *source++;
#ifdef UBFIX
    gGameState.party[0] = source[0];
    gGameState.party[1] = source[0] >> 8;
    gGameState.party[2] = source[1];
    gGameState.party[3] = source[1] >> 8;
    source++;
#else
    *(u16 *)gGameState.party = *source++;
    *(u16 *)((u8 *)gGameState.party + 2) = *source;
#endif
    source = gInventorySnapshot;
    *source = 0;
    GameFlag_ClearBit(FLAG_COLOSSO_HOLDS_EQUIPMENT);
  }
}

#undef INVENTORY_SNAPSHOT_SENTINEL
