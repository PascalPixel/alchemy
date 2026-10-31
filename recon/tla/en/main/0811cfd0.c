// Near miss, differ 2 of 193 (tla-en, 2026-10-31): one pair one slot apart,
// the encounter mode's ldrb ahead of the entry pointer's add after
// entryCount's store. sched2 issues that zero-extended load one cycle after
// the store (cost 1), where the ROM waits two; -mthumb-slow-loads charges the
// wait only to whole loads (alchemy-gcc README, Scheduling), so the C cannot
// move it. Counting the shuffle up from 0 rather than down from 31 settled
// the shuffle count's mov r8 against the list reload (a sched2 tie broken by
// insn order); four more minutes of permute found nothing.
// Earlier, at differ 6, one edit at a time: an empty asm barrier after each
// statement, each adjacent pair of statements swapped, a register tie on each
// local after each statement, each local pinned to each register, each call
// argument copied into its argument register (pinned) at the call or up to
// three statements before it, five minutes of permute; none reached 0.
// Draft: the enemies' actions for the round, ☀️'s
// Battle_BuildOpponentActions (games/tbs/src/battle/battle.c) with ⚓️'s
// changes: each action records its index, and an enemy acting several times
// a turn acts later by steps (four actions: 5/6, 4/6, then 1/2 of its
// Agility; three: 3/4, then 1/2; otherwise 1/2).
#include "types.h"
#include "battle.h"
#include "battle_unit.h"
#include "battle_action.h"
#include "battle_side.h"
#include "battle_work.h"
#include "ram_buffer.h"
#include "system.h"

struct BattleUnit *Unit_Get(s32 unitId);
void Func_08122514(struct BattleActionRecord *entry, s32 mode);

s32 Battle_BuildOpponentActions(struct BattleActionRecord *entries) {
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

  for (shuffleOrUnitIndex = 0; shuffleOrUnitIndex < 32; shuffleOrUnitIndex++) {
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
    struct BattleUnit *unit = Unit_Get(unitId);
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
