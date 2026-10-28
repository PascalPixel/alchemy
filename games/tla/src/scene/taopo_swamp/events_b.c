#include "local.h"

void Func_020002f4(s32 a) { Func_02002958_(a, 65, 0x301); }

void Func_02000304(s32 a) { Func_02002958_(a, 64, 0x300); }

void Func_02000314(void) { Func_02002990_(76, 70, 14, 8, 14, 37, 0); }

void Func_02000334(void) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);

  Func_02002990_(76, 70, 14, 8, 14, 37, 0);
  if (GameFlag_Test_(0xf99) == 0) {
    Func_020028d0_(64, 0x1080000, 0x2980000);
    if (leader->x >> 20 == 16 && leader->z >> 20 == 41) {
      Func_02002898();
      Func_02002968_(0);
      Func_02002910_(gSequenceParentWork.currentOwner, 0x102);
      Actor_OffsetPositionAndResetMotion_(gSequenceParentWork.currentOwner, 0,
                                                 -16);
      leader->velocityY = 0x60000;
      Actor_WaitForMoveById(gSequenceParentWork.currentOwner);
      Func_020028a0();
    }
  }
}
