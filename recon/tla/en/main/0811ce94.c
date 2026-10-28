// Under -mthumb-slow-loads (26 October 2026): differ 6 of 126 in tla-en.
// Tried, one edit at a time: an empty asm barrier after each statement, each
// adjacent pair of statements swapped, a register tie on each local after
// each statement, each local pinned to each register, each call argument
// copied into its argument register (pinned) at the call or up to three
// statements before it; none reaches 0.
// Near miss, differ 6 of 126 (three moved rows; 8 before the barrier):
// the chance to run from battle. Left: escaped lives in a stack slot from
// the start (str r2, [sp, #4]), and the native stores it before it builds
// the heap base; it builds the list call's 1 before it reads the failed
// attempts; and it keeps the level total's 0 in r6 across the first list
// call where here it is set after the call. Tried: the total's 0 behind a
// barrier (11: CSE takes escaped's zero from its slot).
#include "types.h"
#include "battle_party.h"
#include "battle_escape.h"
#include "owner_state.h"
#include "sequence_parent_work.h"
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
  u8 *failedAttempts;
  s16 livingUnits[14];
  s32 livingCount;
  s32 levelTotal;
  s32 unitIndex;
  s32 chance;
  struct BattleEscapeState *escapeState;
  struct SequenceParentWork *party;

  escaped = 0;
  escapeState = (struct BattleEscapeState *)Ram_HeapSlots->battleWork;
  if (escapeState->guaranteed == 1) {
    escaped = 1;
  } else {
    failedAttempts = &escapeState->failedAttempts;
    levelTotal = 0;
    chance = 0x1388 + (escapeState->failedAttempts * 0x7D0);
    livingCount = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, livingUnits);
    for (unitIndex = escaped; unitIndex < livingCount; unitIndex++) {
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
    *failedAttempts += 1;
  }
  party = &gSequenceParentWork;
  /* FAKEMATCH(barrier): gSequenceParentWork's address loaded before 0x24b is
   * built; as gSequenceParentWork.battleRule24b, 8 of 126 instructions differ in
   * tla-en. */
  __asm__ volatile("" : "+l"(party));
  if (party->battleRule24b == 2) {
    escaped = 0;
  }
  return escaped;
}
