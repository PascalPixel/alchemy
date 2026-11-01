// Draft, differ 4 of 165, skeleton gap 2 (tla-en, 2026-11-01)
// Twin of InventorySnapshot_Save (games/tbs/src/game/game/snapshot.c): the same
// 0x6774 sentinel at gData_020023c4 and backup layout, for units 4 to 7 rather
// than 0 to 3, with the two gMainRoutineWork shortcuts (0x240, 0x242) and two
// halfwords of Data_02000458 kept after them. Each unit's unequipped items of
// types 1 to 4 and every item Func_080b106c picks are stripped, the inventory
// closes up through the twin's inline Inventory_CompactSlots, and the unit is
// refreshed. Always sets flag 0x9ff, runs Func_080ae16c(1) and clears flags
// 0xe50 to 0xe59, reusing the slot counter as the flag number.
// Left: one scheduling pair at the first store. The native has mov r1, r8; ldr
// r3 (gMainRoutineWork); strh; movs r0, #144, where the draft has ldr r3; mov
// r1, r8; movs r0, #144; strh. Everything else, registers included, matches.
// Tried, all differ 4 or worse: the store as save[0] or *save with save += 1;
// save as s16 *; gData_020023c4 as s16; the shortcuts declared first, in the
// if block, as s32, or read through a work pointer, a byte offset or
// inventory (the last two differ 103); an empty if with the work in its else;
// the flag counter's declaration moved (differ 14 unless it reuses i).
#include "types.h"
#include "main_routine_work.h"
#include "battle_unit.h"
#include "item.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Unit_UpdateClass(s32 owner);
struct ItemDefinition *Item_GetDirect(s32 item);
s32 Func_080b106c(s32 item);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080ae16c(s32 mode);

extern u16 gData_020023c4[];
extern u16 Data_02000458[];

static inline void Inventory_CompactSlots(s16 *inventory) {
  s32 usedSlots = 0;
  s32 slot;

  for (slot = 0; slot < 15; slot++) {
    if (inventory[slot] != 0)
      inventory[usedSlots++] = inventory[slot];
  }
  for (; usedSlots < 15; usedSlots++)
    inventory[usedSlots] = 0;
}

void Func_080b1088(void) {
  u16 *save;
  struct BattleUnit *unit;
  s16 *inventory;
  s32 owner;
  s32 i;
  s16 first;
  s16 second;

  save = gData_020023c4;
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
        if (Func_080b106c(unit->inventory[i]))
          unit->inventory[i] = 0;
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
  GameFlag_SetBit(0x9ff);
  Func_080ae16c(1);
  for (i = 0xe50; i <= 0xe59; i++)
    GameFlag_ClearBit(i);
}
