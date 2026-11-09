#include "local.h"

void Func_02004d40(void);
void Func_02004d48(void);

void Func_020023e8(void) {
  s32 width = 1;
  s32 height = 2;

  Func_02004d40();
  FieldEvent_ResolvePendingActions(0);
  Event_PlaySound(188);
  MapGrid_CopyCellsTo(72, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(73, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(74, 67, 15, 19, width, height);
  FieldEvent_Wait(8);
  MapGrid_CopyCellsTo(75, 67, 15, 19, width, height);
  FieldEvent_Wait(15);
  Actor_Get(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Actor_Get(gMainRoutineWork.currentOwner)->speedLimit =
      Actor_Get(gMainRoutineWork.currentOwner)->acceleration = 0x8000;
  Event_PlaySound(123);
  Actor_ResetAndSetPositionInMode2_(gMainRoutineWork.currentOwner, 248, 328);
  FieldEvent_Wait(5);
  FieldEvent_SetExitRequest(1);
  Func_02004d48();
}
