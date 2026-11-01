#include "types.h"
#include "battle_unit.h"
#include "battle_action.h"

void *Heap_AllocEwram(s32 size);
s32 BattleParty_ListLivingUnitsFar(s32 sideMask, u16 *unitIds);
void *Unit_Get(s32 owner);

s32 Func_0804babc(u16 *ready, struct BattleActionRecord **cursor,
                  s32 *forcedCount) {
  u16 *unitIds = Heap_AllocEwram(9);
  struct BattleActionRecord *entries = *cursor;
  s32 count = BattleParty_ListLivingUnitsFar(1, unitIds);
  s32 readyCount = 0;
  s32 forced = 0;
  s32 i;
  s32 action;

  for (i = 0; i < count; i++) {
    struct BattleUnit *unit = Unit_Get(unitIds[i]);

    for (action = 0; action < unit->actionEntryCount; action++) {
      if (unit->sleep != 0 || unit->confusion != 0 || unit->charm != 0 ||
          unit->stun != 0) {
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
