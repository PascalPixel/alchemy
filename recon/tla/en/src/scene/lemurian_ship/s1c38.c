#include "../../../../../../games/tla/src/scene/lemurian_ship/lemurian_ship.h"

s32 Func_02001c38(void) {
  u32 i;

  i = 0;
next:
  if (GameFlag_Test(i + 0x300)) {
    struct MainRoutineWork *party = &gMainRoutineWork;

    
    if (party->sequenceResult == 1) {
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
