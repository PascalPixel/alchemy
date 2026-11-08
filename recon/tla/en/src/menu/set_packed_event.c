#include "types.h"
#include "call.h"
#include "main_routine_work.h"

s32 Party_SetPackedEvent(s32 kind, s32 id, s32 second) {
  u32 packed = (kind << 10) | (id & SPLIT_CONSTANT(0x3fff));

  if (second == 0)
    gMainRoutineWork.packedEvents[0] = packed;
  else {
    struct MainRoutineWork *party = &gMainRoutineWork;


    party->packedEvents[1] = packed;
  }
  return 1;
}
