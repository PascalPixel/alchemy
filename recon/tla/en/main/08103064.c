// Near miss, differ 4 of 126 (2026-10-26): lists a member's Djinn as packed
// entries (element << 5 | index, owner << 8 when listing all four elements, bit
// 15 when set), all elements when element is -1; the twin of The Broken Seal's
// Djinn_ListUnitEntries. The Djinn words are read as plain arrays in the unit:
// through owner_state.h's union the set word is not hoisted out of the index
// loop (differ 60). Left: the native shifts the owner before it starts the set
// word pointer (movs 132), as if the shift's spill counted two cycles.
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
