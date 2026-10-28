#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "sequence_parent_work.h"

extern const u8 gData_080f1140[];

void Leader_StopMoving(void);
void wait(s32 frames);
void Func_080d46a4(s32 a0, s32 a1);

void BattleEffect_RunVisibilityTransition(void) {
  struct SequenceParentWork *party;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    Leader_StopMoving();
    Func_080d46a4(0x9d89, 6);
    while (gInput.held & 0x200)
      wait(1);
    party = &gSequenceParentWork;
    /* FAKEMATCH: a one-pass block loads gSequenceParentWork before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    if (party->ownerOverride != 0)
      Func_080d46a4(0x10000, 6);
    else
      Func_080d46a4(0x10000, 6);
  }
}

/// Returns the flag entry table, 20-byte entries ended by a zero flag.
const u8 *FlagEntries_GetTable(void) { return gData_080f1140; }
