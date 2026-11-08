#include "types.h"
#include "main_routine_work.h"
#include "battle_unit.h"
#include "item.h"
#include "version.h"

void *Unit_Get(s32 owner);
void Unit_Calculate(s32 unitId);
void Unit_UpdateClass(s32 owner);
struct ItemDefinition *Item_GetDirect(s32 item);
#if TLA_PATCH_1
s32 Func_080b106c(s32 item);
#endif
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
  GameFlag_SetBit(0x9ff);
  Func_080ae16c(1);
  for (i = 0xe50; i <= 0xe59; i++)
    GameFlag_ClearBit(i);
}
