#include "islets.h"
#include "scene_tables.h"

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct Actor *owner = Actor_GetById(gMainRoutineWork.currentOwner);
  struct Actor *object = Actor_GetById(9);

  Func_02003eec(9, gMainRoutineWork.currentOwner);
  Actor_SetHorizontalPositionWithTerrain(10, PIXELS(273), PIXELS(520));
  wait(1);
  object->angle = FACING_NORTH;
  owner->update = (void (*)(void))Func_02001278;
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 15);
  Actor_SetModeById(9, 3);
  wait(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x9999, 0x4ccc);
  Actor_SetSpeedParameters(10, 0x9999, 0x4ccc);
  Actor_GetById(10)->update = (void (*)(void))Func_02000df8;
  object->update = (void (*)(void))Func_02000df8;
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 10, 280, 468);
  Call3((void (*)())Actor_SetPositionAndCommit, 9, 296, 452);
  Actor_SetModeById(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_GetById(10)->update = 0;
  Func_080dc978(gMainRoutineWork.currentOwner, 9);
  Event_SetMessageId((s32)MsgHereWeAre);
  Event_ShowMessageAndWait(9, 0);
  Actor_SetModeById(9, 1);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x19999, 0xcccc);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_Jump(gMainRoutineWork.currentOwner, 6, 0);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 312, 440);
  Actor_ArmCallbackAndRefresh(gMainRoutineWork.currentOwner, FACING_SOUTHWEST);
  Event_ShowMessageAndWait(9, 0);
  Func_02003fb4();
  Call2((void (*)())Actor_ArmCallbackAndRefresh, 9, FACING_SOUTH);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}
