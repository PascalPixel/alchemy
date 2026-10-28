#include "local.h"

void Func_02004fb4(void) {
  struct Actor **slot = &gData_020062ac;
  /* FAKEMATCH: the native keeps one zero in r7 for the three byte stores below;
   * plain zeros are built in r2 where they are stored (differ 22). */
  s32 zero = 0;
  struct Actor *object =
      Func_02005140(0x1e9, 0x1980000, 0x280000, 0x1580000);

  *slot = object;
  if (object) {
    struct FieldSprite *sprite = object->animation;
    u8 *buffer;

    sprite->fullColor = 0;
    sprite->palette = 0;
    object->motionFlags = zero;
    object->unk5c[0] = 1;
    sprite->nextFrame = zero;
    sprite->unk1b[0] = zero;
    buffer = HeapSlot_AllocFar(68, 0x608);
    Func_020051b0(198);
    VramCache_Load(sprite->vramBlock, 128, buffer + 0x400);
    HeapSlot_Release(68);
  }
}

void Func_02005028(void) {
  Func_020051c0();
  Func_02005370(0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_WaitFrames(20);
  Func_020053d8(121);
  Func_02005170(41, 0, 20, 0, 1, 1);
  FieldEvent_WaitFrames(10);
  Func_02005170(41, 1, 20, 1, 1, 1);
  FieldEvent_WaitFrames(10);
  Func_02005170(41, 2, 20, 2, 1, 1);
  FieldEvent_WaitFrames(10);
  Func_02005170(41, 3, 20, 3, 1, 1);
  FieldEvent_WaitFrames(10);
  GameFlag_SetBit(0x92d);
  Func_020051c8();
}
