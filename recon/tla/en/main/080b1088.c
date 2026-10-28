// Draft, differ 79 of 165: once (marker 0x6774 at gData_020023c4) backs up
// units 4 to 7's inventories with gSequenceParentWork halfwords at 0x240 and
// 0x242 and two from Data_02000458, and strips each of those units' unequipped
// items of types 1 to 4 and every item Func_080b106c picks, compacts the
// inventory and refreshes the unit; always sets flag 0x9ff, runs
// Func_080ae16c(1) and clears flags 0xe50 to 0xe59. Left: the compaction
// loop tests each item as (item << 16) != 0 after its ldrh in the native,
// where the draft compares the loaded halfword directly; the count and the
// read pointer swap registers (r4/r5) from there; and the four closing
// stores of the backup are scheduled differently. Tried: the item as a u16
// local (ldrsh, differ 98); the inventory pointer taken once (differ 79,
// from 82).
#include "types.h"
#include "sequence_parent_work.h"
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

void Func_080b1088(void) {
  u16 *backup = gData_020023c4;
  s32 owner;
  s32 i;
  s32 count;
  s32 flag;

  if (*backup != 0x6774) {
    s16 first;
    s16 second;

    *backup++ = 0x6774;
    first = gSequenceParentWork.packedEvents[0];
    second = gSequenceParentWork.packedEvents[1];
    for (owner = 4; owner < 8; owner++) {
      struct BattleUnit *unit = Unit_Get(owner);
      u16 *inventory;
      u16 *from;
      u16 *to;

      for (i = 0; i < 15; i++)
        *backup++ = unit->inventory[i];
      for (i = 0; i < 15; i++) {
        struct ItemDefinition *item = Item_GetDirect(unit->inventory[i]);

        if ((u8)(item->type - 1) <= 3 && (unit->inventory[i] & 0x200) == 0)
          unit->inventory[i] = 0;
        if (Func_080b106c(unit->inventory[i]))
          unit->inventory[i] = 0;
      }
      count = 0;
      inventory = unit->inventory;
      from = to = inventory;
      for (i = 0; i < 15; i++) {
        s32 item = *from++;

        if ((u16)item != 0) {
          *to++ = item;
          count++;
        }
      }
      for (; count < 15; count++)
        inventory[count] = 0;
      Unit_UpdateClass(owner);
      Unit_Calculate(owner);
    }
    *backup++ = first;
    *backup++ = second;
    backup[0] = Data_02000458[0];
    backup[1] = Data_02000458[1];
  }
  GameFlag_SetBit(0x9ff);
  Func_080ae16c(1);
  for (flag = 0xe50; flag <= 0xe59; flag++)
    GameFlag_ClearBit(flag);
}
