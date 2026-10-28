#include "types.h"
#include "sequence_parent_work.h"

s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second) {
  u32 packed = (kind << 10) | (id & 0x3fff);

  if (second == 0)
    gSequenceParentWork.packedEvents[0] = packed;
  else {
    struct SequenceParentWork *party = &gSequenceParentWork;

    /* FAKEMATCH: a one-pass block loads gSequenceParentWork before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->packedEvents[1] = packed;
  }
  return 1;
}
