#include "tundaria_tower.h"

void Func_02002fc8(void);

void Func_02000de4(void) {
  Func_02002fc8();
  GameFlag_ClearBit(0x212);
  if ((u16)(gAdventure.entrance - 6) <= 1) {
    Func_02002fc0(39, 100, 53, 100, 3, 3);
    Call6((void (*)())Func_02002fe8, 43, 41, 3, 3, 53, 37);
    if (Actor_GetById(gAdventure.currentOwner)->z >> 20 == 38) {
      s16 angle = Actor_GetById(gAdventure.currentOwner)->angle;
      s32 owner = gAdventure.currentOwner;

      if (angle < 0)
        Func_02003088(owner, 0x3680000, 0x2580000);
      else
        Func_02003088(owner, 0x3680000, 0x2780000);
    }
  }
}
