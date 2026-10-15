#include "types.h"
#include "ram_buffer.h"
#include "input.h"
#include "party_state.h"

extern const u8 gData_080f1140[];

void EventRuntime_PrepareCurrentObject(void);
void WaitFrames(s32 frames);
void Func_080d46a4(s32 a0, s32 a1);

void Func_080ed788(void) {
  struct PartyState *party;

  if (*(s8 *)((u8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    EventRuntime_PrepareCurrentObject();
    Func_080d46a4(0x9d89, 6);
    while (gInput.held & 0x200)
      WaitFrames(1);
    party = &gPartyState;
    /* FAKEMATCH: a one-pass block loads the party state before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    if (party->ownerOverride != 0)
      Func_080d46a4(0x10000, 6);
    else
      Func_080d46a4(0x10000, 6);
  }
}

const u8 *Func_080ed804(void) { return gData_080f1140; }
