#include "islets.h"

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct ObjectRuntime *owner = Object_GetById(gAdventure.currentOwner);
  struct ObjectRuntime *object = Object_GetById(9);

  Func_02003eec(9, gAdventure.currentOwner);
  Func_02003edc(10, 0x1110000, 0x2080000);
  WaitFrames(1);
  object->angle = 0xc000;
  owner->update = (void (*)(void))Func_02001278;
  Object_SetPartAttribute(Object_GetById(gAdventure.currentOwner), 15);
  Object_SetModeById(9, 3);
  WaitFrames(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 9, 0x9999, 0x4ccc);
  Call3((void (*)())ObjectMotion_SetSpeedParameters, 10, 0x9999, 0x4ccc);
  Object_GetById(10)->update = (void (*)(void))Func_02000df8;
  object->update = (void (*)(void))Func_02000df8;
  Call3((void (*)())ObjectMotion_ResetAndSetPositionInMode2, 10, 280, 468);
  Call3((void (*)())ObjectMotion_SetPositionAndCommit, 9, 296, 452);
  Object_SetModeById(10, 1);
  owner->update = 0;
  object->update = 0;
  Object_GetById(10)->update = 0;
  Func_02003fac(gAdventure.currentOwner, 9);
  Func_02003f24((s32)MsgHereWeAre);
  Func_02003f2c(9, 0);
  Object_SetModeById(9, 1);
  ObjectMotion_SetSpeedParameters(gAdventure.currentOwner, 0x19999, 0xcccc);
  Object_SetPartAttribute(Object_GetById(gAdventure.currentOwner), 0);
  ObjectMotion_Launch(gAdventure.currentOwner, 6, 0);
  ObjectMotion_SetPositionAndCommit(gAdventure.currentOwner, 312, 440);
  Call2((void (*)())Func_02003f3c, gAdventure.currentOwner, 0x6000);
  Func_02003f2c(9, 0);
  Func_02003fb4();
  Call2((void (*)())Func_02003f3c, 9, 0x4000);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}
