#include "mars_lighthouse_room_m.h"

void Func_0200030c(void) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 22 of 74 instructions
   * differ in tla-en. */
  struct MarsLighthouseRoomMEventWork *work;

  Func_020031e0();
  Func_02003280(0);
  Func_02003248(SPLIT_CONSTANT(0x7f00, 255), 0);
  Func_02003258(30);
  FieldEvent_Wait(30);
  Func_020032b8(190);
  switch (gMainRoutineWork.entrance) {
  case 1:
    GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 50));
    break;
  case 2:
    GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 51));
    break;
  case 3:
    GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 52));
    break;
  case 4:
    GameFlag_SetBit(SPLIT_CONSTANT(0xa00, 53));
    break;
  }
  work = Ram_HeapSlots->eventWork;
  work->field166 = 0;
  FieldEvent_Wait(30);
  Func_02003248(0x10000, 0);
  Func_02003258(30);
  FieldEvent_Wait(30);
  FieldEvent_Wait(10);
  Func_020031e8();
}
