#include "types.h"
#include "field_event.h"
#include "flag_call.h"

void Func_02001bd4(s32 a0);
void Func_02001aec(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void WaitFrames(s32 a0);
void Func_02001ae4(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
struct FieldActor *Actor_GetById(s32 id);

void Func_02000060(s32 animate) {
  if (animate == 1) {
    Func_02001bd4(133);
    Func_02001aec(81, 20, 2, 3, 79, 29);
    Func_02001aec(81, 20, 2, 3, 79, 37);
    WaitFrames(6);
  }
  Func_02001aec(84, 20, 2, 3, 79, 29);
  Func_02001aec(84, 20, 2, 3, 79, 37);
  Func_02001ae4(20, 22, 2, 1, 15, 31);
  Func_02001ae4(20, 22, 2, 1, 15, 39);
}

void Func_020000ec(void) {
  Func_02001bd4(134);
  Func_02001aec(81, 20, 2, 3, 79, 29);
  Func_02001aec(81, 20, 2, 3, 79, 37);
  WaitFrames(6);
  Func_02001aec(87, 20, 2, 3, 79, 29);
  Func_02001aec(87, 20, 2, 3, 79, 37);
  Func_02001ae4(23, 22, 2, 1, 15, 31);
  Func_02001ae4(23, 22, 2, 1, 15, 39);
}

void Func_02000178(void) {
  struct FieldActor *object = Actor_GetById(10);

  if (!Flag_IsSet(0xa01) && object->x >> 20 == 17 && object->z >> 20 == 33) {
    Flag_Set(0xa01);
    Func_02000060(1);
  }
}

void Func_020001b4(void) {
  if (!Flag_IsSet(0x200)) {
    Flag_Set(0x200);
    Func_02000060(1);
  }
}

void Func_020001d4(void) {
  if (Flag_IsSet(0x200) == 1) {
    Flag_Clear(0x200);
    Func_020000ec();
  }
  Func_02000178();
}
