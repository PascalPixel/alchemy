// Near miss, differ 13 of 78: the native loads the adventure's address again
// for the second entrance test (ldr r3, =gAdventure twice, nothing saved);
// this draft keeps the first load in r5 across the join. A register barrier
// on a party pointer leaves it shared. The same source compiled without the
// global CSE pass (-fno-gcse, diagnosis only, not a build option) gives the
// native instructions up to the second test.
#include "../../../../../games/tla/src/scene/yampi_desert/scene.h"

void Func_02002ebc(void);
void Func_02001bec(void);
void Func_02003da0(s32 a, s32 b);
void Func_02003e3c(s32 a, s32 b, s32 c, s32 d, s32 e);

void Func_0200371c(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 521;
  GameFlag_SetBit(0x144);
  if (gAdventure.entrance == 1 && !Scene_FlagTest(0x88c))
    Func_02002ebc();
  else if (gAdventure.entrance == 2 && !Scene_FlagTest(0x88c))
    Func_02001bec();
  if (Scene_FlagTest(0x8ed)) {
    Func_02003ff0(35, 39, 13, 11, 3, 3);
    Scene_Call4090(9, 0xe80000, 0xc80000);
  }
  Func_02003da0(0, 240);
  Func_02003e3c(0, 9, 0, 0, 64);
  WaitFrames(10);
}
