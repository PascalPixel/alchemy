#include "types.h"
#include "adventure.h"

s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second) {
  u32 packed = (kind << 10) | (id & 0x3fff);

  if (second == 0)
    gAdventure.packedEvents[0] = packed;
  else {
    struct Adventure *party = &gAdventure;

    /* FAKEMATCH: a one-pass block loads the adventure before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->packedEvents[1] = packed;
  }
  return 1;
}
