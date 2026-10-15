#include "mars_star.h"

void Func_02002d04(struct ObjectRuntime *object) {
  s32 angle;
  s32 turn;

  if (object->action != 0) {
    object->action--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (GameFlag_Test(0x106))
    angle = -1;
  else
    angle = gData_020047fc[(gInput >> 4) & 15];
  if (angle == -1) {
    Func_020041cc(object, 9);
    return;
  }
  turn = (s16)(angle - object->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->angle += turn;
  Func_020041cc(object, 2);
  ObjectDispatch_ApplyValueToChildren(object, 48);
}

void Func_02002d90(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  /* FAKEMATCH: a one-pass block loads the event work before its offset is built
   * (one reordered row). */
  do {
  } while (0);
  if (work->actionCounter != 0) {
    Call1((void (*)())GameFlag_SetBit, 0x106);
    Func_020043f4();
    Func_02004444();
    Call1((void (*)())GameFlag_ClearBit, 0x106);
  }
}
