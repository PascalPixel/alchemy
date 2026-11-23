/// @file
/// The equipment put aside for the Trial Road, as The Broken Seal's for the
/// Colosso: Felix's, Jenna's, Sheba's and Piers's inventories, and giving them
/// back.
// Part of game.c.

#include "game_internal.h"
#include "game/game.h"

// Added in patch 1; the Japanese release has no such function.
#if TLA_PATCH_1
/// Returns 1 for the six items that raise a stat for good: Power Bread,
/// Cookie, Apple, Hard Nut, Mint and Lucky Pepper (an inventory entry may be
/// passed).
s32 Item_RaisesStat(s32 id) {
  switch (id & INVENTORY_ITEM_ID_MASK) {
  case ITEM_POWER_BREAD:
  case ITEM_COOKIE:
  case ITEM_APPLE:
  case ITEM_HARD_NUT:
  case ITEM_MINT:
  case ITEM_LUCKY_PEPPER:
    return 1;
  }
  return 0;
}
#endif

/// The first halfword of gInventorySnapshot while it holds the party's
/// equipment, put aside while the Trial Road takes it away.
#define INVENTORY_SNAPSHOT_SENTINEL 26484

/// Saves the inventories of Felix's party (FELIX to PIERS), the shortcuts and
/// the party order as the leader and Moapa set off on the Trial Road
/// (ShamanVillage_TakeLeftPath and its twin), then drops every weapon, armor,
/// shield and helm not equipped (and, from patch 1, every item that raises a
/// stat), sets FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT and restores everyone's HP and
/// PP. As The Broken Seal's Colosso snapshot; nothing is saved again while a
/// snapshot is held.
void InventorySnapshot_Save(void) {
  u16 *save;
  struct Unit *unit;
  s16 *inventory;
  s32 unitId;
  s32 slot;
  s16 firstShortcut;
  s16 secondShortcut;

  save = (u16 *)gInventorySnapshot;
  if (*save != INVENTORY_SNAPSHOT_SENTINEL) {
    *save++ = INVENTORY_SNAPSHOT_SENTINEL;
    firstShortcut = gMainRoutineWork.firstShortcut;
    secondShortcut = gMainRoutineWork.secondShortcut;
    // Save each inventory before dropping what the Trial Road takes away.
    for (unitId = FELIX; unitId <= PIERS; unitId++) {
      unit = Unit_Get(unitId);
      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *save++ = unit->inventory[slot];
      for (slot = 0; slot < INVENTORY_SLOTS; slot++) {
        struct ItemDefinition *item = Item_GetDirect(unit->inventory[slot]);

        // ITEM_TYPE_WEAPON to ITEM_TYPE_HELM, as one unsigned test: boots,
        // shirts, rings and the rest are kept.
        if ((u8)(item->type - ITEM_TYPE_WEAPON) <=
                ITEM_TYPE_HELM - ITEM_TYPE_WEAPON &&
            (unit->inventory[slot] & INVENTORY_EQUIPPED) == 0)
          unit->inventory[slot] = 0;
#if TLA_PATCH_1
        // Added in patch 1; the Japanese release has no such check.
        if (Item_RaisesStat(unit->inventory[slot]))
          unit->inventory[slot] = 0;
#endif
      }
      inventory = (s16 *)unit->inventory;
      Inventory_CompactSlots(inventory);
      Unit_UpdateClass(unitId);
      Unit_Calculate(unitId);
    }
    // Keep shortcuts and party order with the inventory snapshot.
    *save++ = firstShortcut;
    *save++ = secondShortcut;
    inventory = (s16 *)gMainRoutineWork.party;
    save[0] = inventory[0];
    save[1] = inventory[1];
  }
  GameFlag_Set(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT);
  Party_RestoreHpPp(1);
  // `slot` counts the Trial Road's stage item flags here.
  for (slot = FLAG_TRIAL_ROAD_STAGE_ITEM_FIRST;
       slot <= FLAG_TRIAL_ROAD_STAGE_ITEM_LAST; slot++)
    GameFlag_Clear(slot);
}

/// Gives back the inventories, shortcuts and party order InventorySnapshot_Save
/// put aside, empties the snapshot and clears FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT:
/// at the end of the Trial Road, and from the clear scene.
void InventorySnapshot_Restore(void) {
  u16 *src = (u16 *)gInventorySnapshot;
  s32 unitId;

  if (*src++ == INVENTORY_SNAPSHOT_SENTINEL) {
    for (unitId = FELIX; unitId <= PIERS; unitId++) {
      u16 *dst = Unit_Get(unitId)->inventory;
      s32 slot;

      for (slot = 0; slot < INVENTORY_SLOTS; slot++)
        *dst++ = *src++;
      Unit_UpdateClass(unitId);
      Unit_Calculate(unitId);
    }
    gMainRoutineWork.firstShortcut = *src++;
    gMainRoutineWork.secondShortcut = *src++;
#ifdef UBFIX
    gMainRoutineWork.party[0] = src[0];
    gMainRoutineWork.party[1] = src[0] >> 8;
    gMainRoutineWork.party[2] = src[1];
    gMainRoutineWork.party[3] = src[1] >> 8;
#else
    {
      // UB: the party order's four bytes are stored as two halfwords.
      u16 *owners = (u16 *)gMainRoutineWork.party;

      *owners++ = *src++;
      *owners = *src;
    }
#endif
    src = (u16 *)gInventorySnapshot;
    *src = 0;
  }
  GameFlag_Clear(FLAG_TRIAL_ROAD_HOLDS_EQUIPMENT);
}
