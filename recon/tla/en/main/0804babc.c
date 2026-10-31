// Near miss, differ 2 of 110 (tla-en, 2026-10-31): instruction order only, the
// stack spill of the id pointer (str r5, [sp]) issued three slots later in the
// inner loop's setup. In sched2 that store has no dependents (priority 1) and
// loses to the ready-slot pointer's shift (priority 3); the listing's order
// needs the store to outrank it, a cost-model difference no source form
// reaches. No empty asm before any one statement lowers it. That setup block
// holds only loop.c's hoisted insns and giv inits, and none of the listing's
// instructions there can depend on the store, so only a hidden insn after it
// could raise it. Also tried (2026-10-31): the action loop as a guarded
// do-while (differ 2), with the id pointer a local after the guard (differ
// 103); an empty asm at the top, bottom or else branch of the action loop
// (differ 124) or before action = 0 (differ 22); a one-pass block around the
// ready store, the action loop body or at the guard (differ 2, the same
// store) or around the forced record's unit id (differ 4); four minutes of
// the permuter (differ 2).
// Sorts the living party's action slots (one per actionEntryCount) for a
// turn: a unit that is asleep, confused, charmed or stunned gets a forced
// action record (command 8, target 384, at its Agility) appended at *cursor;
// the others list their unit id in ready. GCC folds the confusion, charm and
// stun tests into one load of the word at delusion masked past its first
// byte, as the listing reads it. Stores the forced count, advances *cursor
// past the records and returns the ready count. Tried: the record indexed as
// entries[forced] (differ 126); forced++ inside the address (differ 26); a
// pointer to unitIds[i] in the outer loop (differ 103); command stored before
// priority (differ 6); forced++ before the target (differ 134); the action
// counter as u8 (differ 128) or u32 (differ 6); forced declared before
// readyCount (differ 18); entries + forced or a split readyCount++ (differ 2).
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
