#include "mars_lighthouse_room_m.h"
#include "event_inline.h"

void Func_0200030c(void) {
  struct MarsLighthouseRoomMEventWork *work;

  Func_020031e0();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplyTarget(0x7fff, 0);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  Event_PlaySound(190);
  switch (gMainRoutineWork.entrance) {
  case 1:
    Event_SetFlag(0xa32);
    break;
  case 2:
    Event_SetFlag(0xa33);
    break;
  case 3:
    Event_SetFlag(0xa34);
    break;
  case 4:
    Event_SetFlag(0xa35);
    break;
  }
  work = Ram_HeapSlots->eventWork;
  work->field166 = 0;
  FieldEvent_Wait(30);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(30);
  FieldEvent_Wait(30);
  FieldEvent_Wait(10);
  Func_020031e8();
}
