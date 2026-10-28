#include "types.h"
#include "main_routine_work.h"

s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second) {
  u32 packed = (kind << 10) | (id & 0x3fff);

  if (second == 0)
    gMainRoutineWork.packedEvents[0] = packed;
  else {
    struct MainRoutineWork *party = &gMainRoutineWork;

    /* FAKEMATCH: a one-pass block loads gMainRoutineWork before its offset is
     * built (one reordered row). */
    do {
    } while (0);
    party->packedEvents[1] = packed;
  }
  return 1;
}
