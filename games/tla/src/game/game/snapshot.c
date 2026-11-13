/// @file
/// Putting characters 4 to 7's inventories aside, and giving them back.
// Part of game.c.

#include "../game_internal.h"
#include "module/game/game.h"

// Added in patch 1; the Japanese release has no such function.
#if TLA_PATCH_1
s32 Func_080b106c(s32 id) {
  switch (id & 0x1ff) {
  case 191:
  case 192:
  case 193:
  case 194:
  case 195:
  case 196:
    return 1;
  }
  return 0;
}
#endif

#if TLA_PATCH_1
s32 Func_080b106c(s32 item);
#endif

/* VIEW(game): variable gInventorySnapshot as u16 gInventorySnapshot[];
 * module/tla.h has u8 gInventorySnapshot[]. */
extern u16 gData_020023c4__game[] __asm__("gInventorySnapshot");

// XXX: what this snapshot puts aside and when; the twin of The Broken Seal's
// Colosso snapshot, its callers in this game would settle it.
void InventorySnapshot_Save(void) {
  u16 *save;
  struct BattleUnit *unit;
  s16 *inventory;
  s32 owner;
  s32 i;
  s16 first;
  s16 second;

  save = gData_020023c4__game;
  if (*save != 0x6774) {
    *save++ = 0x6774;
    first = gMainRoutineWork.packedEvents[0];
    second = gMainRoutineWork.packedEvents[1];
    for (owner = 4; owner < 8; owner++) {
      unit = Unit_Get(owner);
      for (i = 0; i < 15; i++)
        *save++ = unit->inventory[i];
      for (i = 0; i < 15; i++) {
        struct ItemDefinition *item = Item_GetDirect(unit->inventory[i]);

        if ((u8)(item->type - 1) <= 3 && (unit->inventory[i] & 0x200) == 0)
          unit->inventory[i] = 0;
#if TLA_PATCH_1
        // Added in patch 1; the Japanese release has no such check.
        if (Func_080b106c(unit->inventory[i]))
          unit->inventory[i] = 0;
#endif
      }
      inventory = (s16 *)unit->inventory;
      Inventory_CompactSlots(inventory);
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
    }
    *save++ = first;
    *save++ = second;
    inventory = (s16 *)Data_02000458;
    save[0] = inventory[0];
    save[1] = inventory[1];
  }
  GameFlag_SetBit__game(0x9ff);
  Party_RestoreHpPp(1);
  for (i = 0xe50; i <= 0xe59; i++)
    GameFlag_ClearBit(i);
}

void InventorySnapshot_Restore(void) {
  u16 *src = gData_020023c4__game;
  s32 owner;

  if (*src++ == 0x6774) {
    for (owner = 4; owner <= 7; owner++) {
      u16 *dst = ((struct OwnerInventoryState *)Unit_Get(owner))->inventory;
      s32 i;

      for (i = 14; i >= 0; i--)
        *dst++ = *src++;
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
    }
    gMainRoutineWork.packedEvents[0] = *src++;
    gMainRoutineWork.packedEvents[1] = *src++;
#ifdef UBFIX
    gMainRoutineWork.activeOwners[0] = src[0];
    gMainRoutineWork.activeOwners[1] = src[0] >> 8;
    gMainRoutineWork.activeOwners[2] = src[1];
    gMainRoutineWork.activeOwners[3] = src[1] >> 8;
#else
    {
      // UB: the party order's four bytes are stored as two halfwords.
      u16 *owners = (u16 *)gMainRoutineWork.activeOwners;

      /* FAKEMATCH(temporary): both party-order halfwords stored through one
       * cursor over activeOwners; as stores through
       * *(u16 *)&gMainRoutineWork.activeOwners[0] and [2], 4 of 58
       * instructions differ in tla-en. */
      *owners++ = *src++;
      *owners = *src;
    }
#endif
    src = gData_020023c4__game;
    *src = 0;
  }
  GameFlag_ClearBit(0x9ff);
}
