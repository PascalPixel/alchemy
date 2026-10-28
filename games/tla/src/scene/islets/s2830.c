#include "islets.h"

void Func_02002830(void) {
  struct IsletsEventWork *work;
  struct Actor *owner = Actor_GetById(gAdventure.currentOwner);
  struct Actor *object = Actor_GetById(9);

  Func_02003eec(9, gAdventure.currentOwner);
  Func_02003edc(10, 0x1110000, 0x2080000);
  wait(1);
  object->angle = 0xc000;
  owner->update = (void (*)(void))Func_02001278;
  Actor_SetPartAttribute(Actor_GetById(gAdventure.currentOwner), 15);
  Actor_SetModeById(9, 3);
  wait(1);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x201;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Call3((void (*)())Actor_SetSpeedParameters, 9, 0x9999, 0x4ccc);
  Call3((void (*)())Actor_SetSpeedParameters, 10, 0x9999, 0x4ccc);
  Actor_GetById(10)->update = (void (*)(void))Func_02000df8;
  object->update = (void (*)(void))Func_02000df8;
  Call3((void (*)())Actor_ResetAndSetPositionInMode2, 10, 280, 468);
  Call3((void (*)())Actor_SetPositionAndCommit, 9, 296, 452);
  Actor_SetModeById(10, 1);
  owner->update = 0;
  object->update = 0;
  Actor_GetById(10)->update = 0;
  Func_02003fac(gAdventure.currentOwner, 9);
  Func_02003f24((s32)MsgHereWeAre);
  Func_02003f2c(9, 0);
  Actor_SetModeById(9, 1);
  Actor_SetSpeedParameters(gAdventure.currentOwner, 0x19999, 0xcccc);
  Actor_SetPartAttribute(Actor_GetById(gAdventure.currentOwner), 0);
  Actor_Jump(gAdventure.currentOwner, 6, 0);
  Actor_SetPositionAndCommit(gAdventure.currentOwner, 312, 440);
  Call2((void (*)())Func_02003f3c, gAdventure.currentOwner, 0x6000);
  Func_02003f2c(9, 0);
  Func_02003fb4();
  Call2((void (*)())Func_02003f3c, 9, 0x4000);
  work = Ram_HeapSlots->eventWork;
  work->field1ac = 0x100;
  Func_020034d8();
}
