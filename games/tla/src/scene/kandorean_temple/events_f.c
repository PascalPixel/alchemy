#include "local.h"
#include "macros.h"

void Func_02000b5c(void) {
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner),
                                     1);
}

void Func_02000b78(s32 id) {
  struct Actor *object = Actor_GetById(id);

  *((u8 *)object + 98) = 1;
  object->priorityFlags |= 2;
  Actor_SetModeById(id, 1);
  Actor_SetSpritePriority(id, 3);
  Func_020052c0(id);
}

void Func_02000bac(s32 id) {
  *((u8 *)Actor_GetById(id) + 98) = 1;
  Actor_SetSpritePriority(id, 2);
  Func_020052c0(id);
}

void Func_02000bcc(void) {
  Func_02005348(133, 1);
  Func_02005350(12, 10);
  Func_02005368();
  Field_DispatchTypeHandler(1);
  Func_02005358();
  Func_02005360();
}

void Func_02000bf4(void) {
  s32 position[3];

  if (gData_02006298 != 0 && gFrameCount % 9 == 0) {
    SET_VECTOR(position, 0x1280000, 0, 0xb00000);
    Func_0200098c(position, 0x20000);
  }
}

void Func_02000c34(void) {
  s32 position[3];

  if (gData_0200629c != 0 && gFrameCount % 5 == 0) {
    SET_VECTOR(position, 0x1580000, 0, 0xa00000);
    Func_0200098c(position, 0x40000);
  }
}

void Func_02000c74(void) {
  if (gFrameCount % 5 == 0)
    Func_02000adc();
}

void Func_02000c90(s32 id) {
  Actor_GetById(id)->scaleY = 0x800;
  Actor_SetPositionAndResetMotion_(Actor_GetById(id), PIXELS(216), 0, PIXELS(216));
}

void Func_02000cb4(s32 id) {
  Actor_GetById(id)->scaleY = 0x800;
  Actor_SetPositionAndResetMotion_(Actor_GetById(id), PIXELS(376), 0, PIXELS(248));
}

void Func_02000cd8(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY += scale;
  object->y += dy;
  object->z -= dy;
}

void Func_02000cf8(s32 id, s32 dy, s32 scale) {
  struct Actor *object = Actor_GetById(id);

  object->scaleY -= scale;
  object->y -= dy;
  object->z += dy;
}

s32 Func_02000d18(void) {
  if (GameFlag_Test(0x20a) || GameFlag_Test(0x300))
    return 1;
  return 0;
}

s32 Func_02000d3c(void) {
  if (GameFlag_Test(0x20a) || GameFlag_Test(0x301))
    return 1;
  return 0;
}

void Func_02000d64(void) {
  Actor_SetModeById_(11, 2);
  Actor_SetModeById_(13, 3);
  Actor_SetModeById_(14, 3);
  Actor_SetModeById_(15, 3);
  Actor_SetModeById_(16, 3);
  Actor_SetModeById_(9, 0);
  Actor_SetSpritePriority_(9, 2);
  Actor_GetById_(9)->priorityFlags &= ~2;
  Actor_SetPositionAndResetMotion_(Actor_GetById_(9), PIXELS(216), PIXELS(8),
                                    PIXELS(216));
  if (GameFlag_Test_(0x208)) {
    Actor_SetModeById_(9, 4);
    Actor_SetPositionAndResetMotion_(Actor_GetById(gMainRoutineWork.currentOwner),
                                      PIXELS(216), PIXELS(12), PIXELS(216));
    Actor_SetSingleChildFlags_(
        Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
  Actor_GetById_(11)->scaleY = 0x10000;
  Actor_SetPositionAndResetMotion_(Actor_GetById_(11), PIXELS(216), 0, PIXELS(216));
  Func_02000c90(13);
  Func_02000c90(14);
  if (GameFlag_Test_(0x301)) {
    Func_02000c90(15);
    Func_02000c90(16);
  }
  gData_02006298 = 1;
  GameFlag_SetBit_(0x20c);
}

void Func_02000e60(void) {
  Actor_SetModeById_(12, 2);
  Actor_SetModeById_(17, 3);
  Actor_SetModeById_(18, 3);
  Actor_SetModeById_(19, 3);
  Actor_SetModeById_(20, 3);
  Actor_SetModeById_(10, 0);
  Actor_SetSpritePriority_(10, 2);
  Actor_GetById_(10)->priorityFlags &= ~2;
  Actor_SetPositionAndResetMotion_(Actor_GetById_(10), PIXELS(376), PIXELS(8),
                                    PIXELS(248));
  if (GameFlag_Test_(0x209)) {
    Actor_SetModeById_(10, 4);
    Actor_SetPositionAndResetMotion_(Actor_GetById(gMainRoutineWork.currentOwner),
                                      PIXELS(376), PIXELS(12), PIXELS(248));
    Actor_SetSingleChildFlags_(
        Actor_GetById(gMainRoutineWork.currentOwner), 0);
  }
  Actor_GetById_(12)->scaleY = 0x10000;
  Actor_SetPositionAndResetMotion_(Actor_GetById_(12), PIXELS(376), 0,
                                    PIXELS(248));
  Func_02000cb4(17);
  Func_02000cb4(18);
  if (GameFlag_Test_(0x300)) {
    Func_02000cb4(19);
    Func_02000cb4(20);
  }
  gData_0200629c = 1;
  GameFlag_SetBit_(0x20d);
}
