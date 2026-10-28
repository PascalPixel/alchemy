#include "kibombo.h"

void Func_0200489c(void) {
  Func_02004524();
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_SetPartAttribute(Actor_GetById(12), 0);
  Actor_SetPartAttribute(Actor_GetById(13), 0);
  wait(8);
  Func_02005b94(17, 0x100, 20);
  Func_02005b4c((s32)MsgLookLookThe);
  Func_02005b64(17, 0);
  Func_02005c9c(0x241);
  Actor_SetMovementTypeById(12, gData_0200614c);
  Actor_SetMovementTypeAndWaitById(13, gData_0200614c);
  Actor_SetMovementTypeById(12, gData_0200613c);
  Actor_SetMovementTypeById(13, gData_0200613c);
  Actor_SetPartAttribute(Actor_GetById(12), 12);
  Actor_SetPartAttribute(Actor_GetById(13), 12);
  FieldEvent_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Func_02005bc4(12);
}

void Func_02004978(void) {
  Func_02004524();
  Kibombo_SePlay(0x240);
  Actor_SetPartAttribute(Actor_GetById(12), 7);
  Actor_SetPartAttribute(Actor_GetById(13), 7);
  wait(8);
  Actor_GetById(12)->update = Func_020000a0;
  Actor_GetById(13)->update = Func_020000a0;
  Func_02005b94(17, 0x100, 20);
  Func_02005b4c((s32)MsgLookTheGreat);
  Func_02005b5c(17, 0, 20);
  Kibombo_SePlay(0x240);
  Func_020036e0(1, 0);
  FieldEvent_Wait(20);
  Func_02005c9c(162);
  Task_Add(Func_02002f34, 0x480);
  FieldEvent_Wait(120);
  Func_020045d0();
  GameFlag_SetBit(0x8fc);
  GameFlag_SetBit(265);
  Func_02005bc4(12);
}
