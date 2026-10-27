#include "mars_lighthouse_room_m.h"

void Func_0200030c(void) {
  struct MarsLighthouseRoomMEventWork *work;

  Func_020031e0();
  Func_02003280(0);
  Call2((void (*)())Func_02003248, 0x7fff, 0);
  Func_02003258(30);
  FieldEvent_WaitTicks(30);
  Func_020032b8(190);
  switch (gPartyState.entrance) {
  case 1:
    GameFlag_SetBit(0xa32);
    break;
  case 2:
    GameFlag_SetBit(0xa33);
    break;
  case 3:
    GameFlag_SetBit(0xa34);
    break;
  case 4:
    GameFlag_SetBit(0xa35);
    break;
  }
  work = Ram_HeapSlots->eventWork;
  work->field166 = 0;
  FieldEvent_WaitTicks(30);
  Call2((void (*)())Func_02003248, 0x10000, 0);
  Func_02003258(30);
  FieldEvent_WaitTicks(30);
  FieldEvent_WaitTicks(10);
  Func_020031e8();
}
