#include "airs_rock_inside.h"

s32 Func_02000690(u32 layer, s32 x, s32 y);
void Func_02005efc(s32 countdown);
void Actor_ResetAndSetPositionInMode2(s32 actor, s32 x, s32 z);

/// When layer 1's cell at the leader's x and z - 1 reads 30, runs the four
/// steps 65, 66, 67 and 62 and moves the leader to (312, 792).
void Func_02003170(void) {
  struct FieldActor *leader = Actor_GetById(gAdventure.currentOwner);
  s32 width;
  s32 height;

  if (Func_02000690(1, leader->x >> 20, (leader->z >> 20) - 1) >> 8 != 30)
    return;
  Func_02005e14();
  Func_02005f2c(0);
  Func_02005f94(188);
  width = 1;
  height = 2;
  Func_02005d9c(65, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(66, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(67, 76, 19, 92, width, height);
  FieldEvent_Wait(8);
  Func_02005d9c(62, 76, 19, 92, width, height);
  FieldEvent_Wait(15);
  Actor_GetById(gAdventure.currentOwner)->motionFlags = 0;
  Actor_GetById(gAdventure.currentOwner)->speed =
      Actor_GetById(gAdventure.currentOwner)->acceleration = 0x8000;
  Func_02005f94(123);
  Actor_ResetAndSetPositionInMode2(gAdventure.currentOwner, 312, 792);
  FieldEvent_Wait(5);
  Func_02005efc(15);
  Func_02005e1c();
}
