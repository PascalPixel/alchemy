// Near miss, differ 4 of 126 (tla-en, 2026-10-31): the chance to run from
// battle. Left, two rows sched2 orders differently in the block before the
// first list call: the failed attempts' ldrb (priority 49) and the call's
// movs r0, #1 (priority 37) are ready in the same cycle, one after the spill
// store, so the ldrb goes first; the native's order needs the ldrb ready a
// cycle later. The level total's movs r6, #0 (priority 1) feeds nothing
// before the block's jump, so it fills the slot after the call instead of
// the ldrb's load delay.
// The same code, diff for diff: the first loop from 0 rather than from
// escaped (CSE takes escaped's zero from its slot either way), the attempts
// counted through escapeState rather than a pointer local, and
// gMainRoutineWork.battleRule24b read plainly: the barrier this draft kept
// before it is no longer needed.
// Tried (26 October 2026), one edit at a time: an empty asm barrier after
// each statement, each adjacent pair of statements swapped, a register tie
// on each local after each statement, each local pinned to each register,
// each call argument copied into its argument register (pinned) at the call
// or up to three statements before it; the total's 0 behind a barrier (CSE
// takes escaped's zero from its slot); none reaches 0.
// Tried (31 October 2026), all differ 4: the attempts through a pointer, a
// u8 or s32 local; chance's sum reversed or split; the total unsigned;
// livingUnits as u16; the list call's side in a local set early, with or
// without a barrier after it (CSE folds it back to the constant). The
// permuter (4 minutes) found only a one-pass block around the first
// levelTotal = 0: it keeps the 0 before the call, still differ 4.
#include "types.h"
#include "battle_side.h"
#include "battle_escape.h"
#include "owner_state.h"
#include "main_routine_work.h"
#include "ram_buffer.h"

s32 __divsi3(s32, s32);
u32 Random16(void);

struct BattleEscapeState {
  u8 reserved00[0x45];
  u8 guaranteed;
  u8 failedAttempts;
};

s32 BattleEscape_CheckSuccess(void) {
  s32 escaped;
  s16 livingUnits[14];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleEscapeState *escapeState;

  escaped = 0;
  escapeState = (struct BattleEscapeState *)Ram_HeapSlots->battleWork;
  if (escapeState->guaranteed == 1) {
    escaped = 1;
  } else {
    levelTotal = 0;
    chance = 0x1388 + (escapeState->failedAttempts * 0x7D0);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += ((u8 *)Unit_Get((s32)livingUnits[unitIndex]))[0x0f];
    }
    chance += __divsi3(levelTotal * 0x1F4, livingCount);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_ENEMIES, livingUnits);
    levelTotal = 0;
    for (unitIndex = 0; unitIndex < livingCount; unitIndex++) {
      levelTotal += ((u8 *)Unit_Get((s32)livingUnits[unitIndex]))[0x0f];
    }
    chance -= __divsi3(levelTotal * 0x1F4, livingCount);
    if ((chance > 0) &&
        ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
      escaped = 1;
    }
    escapeState->failedAttempts++;
  }
  if (gMainRoutineWork.battleRule24b == 2) {
    escaped = 0;
  }
  return escaped;
}
