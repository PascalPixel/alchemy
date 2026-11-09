#include "kibombo.h"

void Func_0200489c(void) {
  Func_02004524();
  Sound_Play(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookLookThe);
  Event_ShowMessageAndWait(17, 0);
  Sound_Play(0x241);
  Actor_SetMovementTypeById(12, gData_0200614c);
  Actor_SetMovementTypeAndWaitById(13, gData_0200614c);
  Actor_SetMovementTypeById(12, gData_0200613c);
  Actor_SetMovementTypeById(13, gData_0200613c);
  Actor_SetPartAttribute(Actor_GetById(12), 12);
  Actor_SetPartAttribute(Actor_GetById(13), 12);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}

void Func_02004978(void) {
  Func_02004524();
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_GetById(12)->update = Func_020000a0;
  Actor_GetById(13)->update = Func_020000a0;
  Func_080d47b4(17, 0x100, 20);
  Event_SetMessageId((s32)MsgLookTheGreat);
  Event_ShowMessageThenWait(17, 0, 20);
  Sound_Play(0x240);
  Func_020036e0(1, 0);
  Event_Wait(20);
  Sound_Play(162);
  Task_Add(Func_02002f34, 0x480);
  Event_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Import_EventSetExitRequest(12);
}
