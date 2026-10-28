#include "scene.h"
#include "task.h"

void Func_0200316c(void) {
  Func_02003820(gData_02004768);
  Func_02004180(0);
  if (gSequenceParentWork.entrance == 1) {
    GameFlag_SetBit(0xa29);
    GameFlag_ClearBit(0xa2a);
    GameFlag_ClearBit(0xa2b);
  }
}

/// Clears actors 8 to 14 after flag 0x8b3, or places them after flag 0x8f0.
void Func_020031b4(void) {
  s32 angleA;
  s32 angleB;
  s32 angle;

  if (GameFlag_Test(0x8b3)) {
    Func_02004090(8, 0, 0);
    Func_02004090(9, 0, 0);
    Func_02004090(10, 0, 0);
    Func_02004090(11, 0, 0);
    Func_02004090(12, 0, 0);
    Func_02004090(13, 0, 0);
    Func_02004090(14, 0, 0);
  } else if (GameFlag_Test(0x8f0)) {
    // The angles go through locals, as overlay 695 writes them (with
    // every angle a constant, differ 38; with the single-use ones
    // constants, they are built after the positions, differ 12).
    angleA = 0xb000;
    Scene_Call4098(8, 520 << 16, 242 << 16, angleA);
    angle = 0x8000;
    Scene_Call4098(9, 533 << 16, 271 << 16, angle);
    angle = 0x3000;
    Scene_Call4098(10, 435 << 16, 232 << 16, angle);
    angle = 0x5000;
    Scene_Call4098(11, 467 << 16, 248 << 16, angle);
    angleB = 0xd000;
    Scene_Call4098(12, 462 << 16, 340 << 16, angleB);
    Scene_Call4098(13, 482 << 16, 340 << 16, angleB);
    Scene_Call4098(14, 489 << 16, 320 << 16, angleA);
  }
  if (GameFlag_Test(0x88d))
    Func_02001594();
  Task_Add(Func_02002010, TASK_PASS_VBLANK);
}
