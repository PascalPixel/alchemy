// Near miss, differ 4 of 126 (tla-en, 2026-10-31): sched2 order in the outer
// loop's preheader. Lists a member's Djinn as packed
// entries (element << 5 | index, owner << 8 when listing all four elements, bit
// 15 when set), all elements when element is -1; the twin of The Broken Seal's
// Djinn_ListUnitEntries. The Djinn words are read as plain arrays in the unit:
// through owner_state.h's union the set word is not hoisted out of the index
// loop (differ 60). Left: the native shifts the owner before it starts the set
// word pointer (movs 132), as if the shift's spill counted two cycles. In the
// sched2 dump the pointer's movs/lsls/add chain has priority 3 and the shift
// and its spill store priority 2 (store cost 1), so no C order moves it. Tried
// (2026-10-31): the shifted owner as a local set before the loop (differ 12),
// memberId <<= 8 there (differ 152), the owner in the set entry's one OR
// expression (differ 8); the permuter's best was differ 11. A two-cycle
// store alone would not do it: the zero for r8 and r9 (movs r1) has two
// dependents and would still issue third. The listing's order needs the shift
// to tie the pointer chain at priority 3 and movs r1 to wait for the spill
// store, a dependence nothing in these eight instructions gives. Also tried
// (2026-10-31): the owner bits as a volatile local (differ 177), a one-element
// array (12, kept in a register), an empty asm before the loop (4) or after
// the local (10), a one-pass block around or after the local (12), the owner
// first in the set entry's OR (8), the set entry built in three |= steps (4).
#include "types.h"

#define DJINN_ENTRY_SET (-0x8000)

struct DjinnUnit {
  u8 unk000[0xf8];
  u32 djinnHeld[4];
  u32 djinnSet[4];
};

struct DjinnUnit *Unit_Get(s32 owner);

s32 Djinn_ListOwnerEntries(u16 *entriesOut, s32 memberId, s32 element) {
  struct DjinnUnit *unit = Unit_Get(memberId);
  s32 count = 0;
  s32 elementIndex;
  s32 djinnIndex;
  s32 encodedDjinn;

  if (element == -1) {
    for (elementIndex = 0; elementIndex < 4; elementIndex++) {
      for (djinnIndex = 0; djinnIndex < 20; djinnIndex++) {
        if (unit->djinnSet[elementIndex] & (1 << djinnIndex)) {
          encodedDjinn = (elementIndex << 5) | djinnIndex | DJINN_ENTRY_SET;
          encodedDjinn |= memberId << 8;
          entriesOut[count++] = encodedDjinn;
        } else if (unit->djinnHeld[elementIndex] & (1 << djinnIndex)) {
          entriesOut[count++] =
              (elementIndex << 5) | djinnIndex | (memberId << 8);
        }
      }
    }
  } else {
    for (djinnIndex = 0; djinnIndex < 20; djinnIndex++) {
      if (unit->djinnSet[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex | DJINN_ENTRY_SET;
      else if (unit->djinnHeld[element] & (1 << djinnIndex))
        entriesOut[count++] = (element << 5) | djinnIndex;
    }
  }
  return count;
}
