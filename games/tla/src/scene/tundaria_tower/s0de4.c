#include "tundaria_tower.h"
#include "scene_tables.h"

void Func_02002fc8(void);

void Func_02000de4(void) {
  Func_02002fc8();
  GameFlag_ClearBit(0x212);
  if ((u16)(gMainRoutineWork.entrance - 6) <= 1) {
    Func_02002fc0(39, 100, 53, 100, 3, 3);
    Call6((void (*)())Func_02002fe8, 43, 41, 3, 3, 53, 37);
    if (Actor_GetById(gMainRoutineWork.currentOwner)->z >> 20 == 38) {
      s16 angle = Actor_GetById(gMainRoutineWork.currentOwner)->angle;
      s32 owner = gMainRoutineWork.currentOwner;

      if (angle < 0)
        Func_02003088(owner, PIXELS(872), PIXELS(600));
      else
        Func_02003088(owner, PIXELS(872), PIXELS(632));
    }
  }
}
