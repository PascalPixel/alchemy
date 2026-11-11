#include "local.h"

void Func_020010d0(struct Actor *object) {
  s32 frames = 60;

  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
}

void Func_020010ec(void) {
  Func_02003d7c();
  FieldEvent_ResolvePendingActions(0);
  Message_ShowCentered(MsgCheckedThePsynergy, 1);
  Event_PlaySound(126);
  Func_02003eb4(999, 0);
  FieldEvent_Wait(10);
  Event_ClearFlag(0x142);
  Message_ShowCentered(MsgCheckedThePsynergy + 1, 1);
  Func_02003d84();
}

void Func_02001138(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  MapGrid_CopyCellAttributes(17, 39, 2, 1, 20, 39);
  MapGrid_CopyCellAttributes(23, 39, 1, 1, object->x >> 20, 39);
}

void Func_02001170(void) {
  s32 i;
  s32 result;

  if (Event_GetFlag(0x24b)) {
    /* FAKEMATCH(signature): these two calls go through a void function type, so
     * r0 is set before r1, as the ROM does; as declared (value-returning, as
     * the third), r1 loads first and 4 instructions differ. */
    ((void (*)(s32, s32))Func_02000b24)(12, 38);
    ((void (*)(s32, s32))Func_02000b24)(13, 39);
    Func_02000b24(15, 37);
    Func_02003628();
    Func_02003688(38, 2, 19, 1);
    for (i = 8; i >= 0; i--)
      result = Func_020036b8(33, 55);
    Event_SetFlag(0x24a);
    if (result == 1)
      Event_SetFlag(0xa65);
  }
}
