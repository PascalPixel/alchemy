// Draft, differ 6 of 193: the enemies' actions for the round, ☀️'s
// BattlePresentation_BuildOpponentEntries (games/tbs/src/battle/battle.c) with ⚓️'s
// changes: each action records its index, and an enemy acting several times
// a turn acts later by steps (four actions: 5/6, 4/6, then 1/2 of its
// Agility; three: 3/4, then 1/2; otherwise 1/2). Left: instruction order
// only, three pairs one slot apart: the heap address's movs ahead of the two
// parameter stores, the shuffle count's mov r8 ahead of the list reload, and
// the encounter mode's ldrb ahead of the entry pointer's add.
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_session.h"
#include "battle_party.h"
#include "battle_work.h"
#include "ram_buffer.h"
#include "system.h"

struct BattleUnit *UnitGet(s32 unitId);
void Func_08122514(struct BattleActionRecord *entry, s32 mode);

s32 BattlePres_BuildOpponentEntries(struct BattleActionRecord *entries) {
  u16 unitIds[14];
  s32 entryCount = 0;
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  s32 unitCount;
  s32 shuffleOrUnitIndex;

  if (battle->encounterMode == 1)
    return 0;

  unitCount = BattleParty_ListLivingUnits(2, unitIds);
  if (unitCount == 0)
    return 0;

  for (shuffleOrUnitIndex = 31; shuffleOrUnitIndex >= 0; shuffleOrUnitIndex--) {
    u32 first = (unitCount * Random16()) >> 16;
    u32 second = (unitCount * Random16()) >> 16;
    s32 swap = unitIds[first];
    unitIds[first] = unitIds[second];
    unitIds[second] = swap;
  }

  if (battle->encounterMode == 2) {
    s32 limit = ((Random16() * 5) >> 16) + 1;

    if (limit <= 1)
      limit = 2;
    if (limit < unitCount)
      unitCount = limit;
  }

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < unitCount;
       shuffleOrUnitIndex++) {
    s32 unitId = unitIds[shuffleOrUnitIndex];
    struct BattleUnit *unit = UnitGet(unitId);
    s32 copyIndex;

    for (copyIndex = 0; copyIndex < unit->actionEntryCount; copyIndex++) {
      struct BattleActionRecord *entry = &entries[entryCount];
      s32 agility;

      entry->unitId = unitId;
      agility = unit->agi;
      entry->priority = agility;
      entry->unk0e = copyIndex;
      if (unit->actionEntryCount == 4) {
        if (copyIndex == 0)
          ;
        else if (copyIndex == 1)
          entry->priority = (s16)agility * 5 / 6;
        else if (copyIndex == 2)
          entry->priority = (s16)agility * 4 / 6;
        else
          entry->priority = (s16)agility / 2;
      } else if (unit->actionEntryCount == 3) {
        if (copyIndex == 0)
          ;
        else if (copyIndex == 1)
          entry->priority = (s16)agility * 3 / 4;
        else
          entry->priority = (s16)agility / 2;
      } else if (copyIndex != 0) {
        entry->priority = (s16)agility / 2;
      }

      if (unit->sleep != 0 || unit->stun != 0) {
        entry->command = 8;
        entry->parameter = 0;
        entry->target = 0x100;
      } else {
        Func_08122514(entry, 0);
      }

      entryCount++;

      if (battle->encounterMode == 2)
        break;
    }
  }

  return entryCount;
}
