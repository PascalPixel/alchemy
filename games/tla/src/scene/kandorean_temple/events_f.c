#include "local.h"
#include "macros.h"

void Func_02000b5c(void) {
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     1);
}

void Func_02000b78(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  *((u8 *)object + 98) = 1;
  object->priorityFlags |= 2;
  Object_SetModeById(id, 1);
  ObjectMotion_SetActionVariant(id, 3);
  Func_020052c0(id);
}

void Func_02000bac(s32 id) {
  *((u8 *)Object_GetById(id) + 98) = 1;
  ObjectMotion_SetActionVariant(id, 2);
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
  Object_GetById(id)->scaleY = 0x800;
  Object_SetPositionAndResetMotion_(Object_GetById(id), 0xd80000, 0, 0xd80000);
}

void Func_02000cb4(s32 id) {
  Object_GetById(id)->scaleY = 0x800;
  Object_SetPositionAndResetMotion_(Object_GetById(id), 0x1780000, 0, 0xf80000);
}

void Func_02000cd8(s32 id, s32 dy, s32 scale) {
  struct ObjectRuntime *object = Object_GetById(id);

  object->scaleY += scale;
  object->y += dy;
  object->z -= dy;
}

void Func_02000cf8(s32 id, s32 dy, s32 scale) {
  struct ObjectRuntime *object = Object_GetById(id);

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
  Object_SetModeById_(11, 2);
  Object_SetModeById_(13, 3);
  Object_SetModeById_(14, 3);
  Object_SetModeById_(15, 3);
  Object_SetModeById_(16, 3);
  Object_SetModeById_(9, 0);
  ObjectMotion_SetActionVariant_(9, 2);
  Object_GetById_(9)->priorityFlags &= ~2;
  Object_SetPositionAndResetMotion_(Object_GetById_(9), 0xd80000, 0x80000,
                                    0xd80000);
  if (GameFlag_Test_(0x208)) {
    Object_SetModeById_(9, 4);
    Object_SetPositionAndResetMotion_(Object_GetById(gPartyState.currentOwner),
                                      0xd80000, 0xc0000, 0xd80000);
    ObjectDispatch_SetSingleChildFlags_(
        Object_GetById(gPartyState.currentOwner), 0);
  }
  Object_GetById_(11)->scaleY = 0x10000;
  Object_SetPositionAndResetMotion_(Object_GetById_(11), 0xd80000, 0, 0xd80000);
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
  Object_SetModeById_(12, 2);
  Object_SetModeById_(17, 3);
  Object_SetModeById_(18, 3);
  Object_SetModeById_(19, 3);
  Object_SetModeById_(20, 3);
  Object_SetModeById_(10, 0);
  ObjectMotion_SetActionVariant_(10, 2);
  Object_GetById_(10)->priorityFlags &= ~2;
  Object_SetPositionAndResetMotion_(Object_GetById_(10), 0x1780000, 0x80000,
                                    0xf80000);
  if (GameFlag_Test_(0x209)) {
    Object_SetModeById_(10, 4);
    Object_SetPositionAndResetMotion_(Object_GetById(gPartyState.currentOwner),
                                      0x1780000, 0xc0000, 0xf80000);
    ObjectDispatch_SetSingleChildFlags_(
        Object_GetById(gPartyState.currentOwner), 0);
  }
  Object_GetById_(12)->scaleY = 0x10000;
  Object_SetPositionAndResetMotion_(Object_GetById_(12), 0x1780000, 0,
                                    0xf80000);
  Func_02000cb4(17);
  Func_02000cb4(18);
  if (GameFlag_Test_(0x300)) {
    Func_02000cb4(19);
    Func_02000cb4(20);
  }
  gData_0200629c = 1;
  GameFlag_SetBit_(0x20d);
}
