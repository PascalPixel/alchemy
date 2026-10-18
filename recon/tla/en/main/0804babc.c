// Draft, differ 8 of 110: sorts the living party's action slots (one per
// actionEntryCount) for a turn: a unit that is asleep, or confused, charmed
// or stunned (the word at delusion masked past its first byte) gets a forced
// action record (command 8, target 384, at its Agility) appended at *cursor;
// the others list their unit id in ready. Stores the forced count, advances
// *cursor past the records and returns the ready count. Left: instruction
// order only, in the inner loop's setup (the stack spill of the id pointer
// and the copies to r9 and lr). Tried: the record indexed as entries[forced]
// (differ 126); forced++ inside the address (differ 26).
#include "types.h"
#include "battle_unit.h"
#include "battle_session.h"

void *Runtime_BumpAllocateAlternatePool(s32 size);
s32 BattleParty_ListLivingUnitsFar(s32 sideMask, u16 *unitIds);
void *UnitGet(s32 owner);

s32 Func_0804babc(u16 *ready, struct BattleActionRecord **cursor,
                  s32 *forcedCount) {
  u16 *unitIds = Runtime_BumpAllocateAlternatePool(9);
  struct BattleActionRecord *entries = *cursor;
  s32 count = BattleParty_ListLivingUnitsFar(1, unitIds);
  s32 readyCount = 0;
  s32 forced = 0;
  s32 i;
  s32 action;

  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = UnitGet(unitIds[i]);

    for (action = 0; action < unit->actionEntryCount; action++) {
      if (unit->sleep != 0 || (*(u32 *)&unit->delusion & 0xffffff00) != 0) {
        struct BattleActionRecord *entry = &entries[forced];

        entry->unitId = unitIds[i];
        entry->priority = unit->agi;
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 384;
        forced++;
      } else {
        ready[readyCount++] = unitIds[i];
      }
    }
  }
  *forcedCount = forced;
  entries += forced;
  *cursor = entries;
  return readyCount;
}
