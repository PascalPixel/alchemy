#include "lemurian_ship.h"

s32 Func_02001c38(void) {
  u32 i;

  i = 0;
next:
  if (GameFlag_Test(i + 0x300)) {
    struct PartyState *party = &gPartyState;

    /* FAKEMATCH: the loop note ends the scheduling block: the native loads the
     * party state's address before it builds the field's offset (2 differ
     * without it). */
    do {
    } while (0);
    if (party->unk256 == 1) {
      GameFlag_SetBit(i + 0x930);
      GameFlag_SetBit(i + 0x220);
    }
  }
  GameFlag_ClearBit(i + 0x300);
  i++;
  if (i < 16)
    goto next;
  return 0;
}
