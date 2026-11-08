#include "../../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_020010d0(struct Actor *object) {
  s32 frames = 60;

  while (frames != 0) {
    wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
}

void Func_020010ec(void) {
  Func_02003d7c();
  Func_02003eac(0);
  FieldMessage_Show(MsgCheckedThePsynergy, 1);
  Func_02003ef4(126);
  Func_02003eb4(SPLIT_CONSTANT(0x2e8, 255), 0);
  FieldEvent_Wait(10);
  GameFlag_ClearBit(0x142);
  FieldMessage_Show(MsgCheckedThePsynergy + 1, 1);
  Func_02003d84();
}

void Func_02001138(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  Func_02003d1c(17, 39, 2, 1, 20, 39);
  Func_02003d1c(23, 39, 1, 1, object->x >> 20, 39);
}

void Func_02001170(void) {
  s32 i;
  s32 result;

  if (GameFlag_Test(SPLIT_CONSTANT(0x14c, 255))) {
    /* FAKEMATCH: called as void through a cast, these two calls set r0 before
     * r1 as the native does; called as declared (value-returning, as the third)
     * they load r1 first (differ 4). */
    ((void (*)(s32, s32))Func_02000b24)(12, 38);
    ((void (*)(s32, s32))Func_02000b24)(13, 39);
    Func_02000b24(15, 37);
    Func_02003628();
    Func_02003688(38, 2, 19, 1);
    for (i = 8; i >= 0; i--)
      result = Func_020036b8(33, 55);
    GameFlag_SetBit(SPLIT_CONSTANT(0x200, 74));
    if (result == 1)
      GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 101));
  }
}
