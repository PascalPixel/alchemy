#include "kibombo.h"

void Func_02003610(void) {
  Task_RemoveFar(Func_02002ccc);
  wait(1);
  Func_02005bb4();
  FieldEvent_Wait(40);
  if (gData_02007950 != 0)
    Func_02003504();
}

void Func_02003640(s32 open, s32 hide) {
  if (open != 0) {
    Kibombo_MapCopy(49, 37, 79, 15, 3, 2);
    Kibombo_ObjSetPos(Actor_GetById(12), 0xfd0000, 0x200000, 0x1210000);
    Kibombo_ObjSetPos(Actor_GetById(13), 0x1130000, 0x200000, 0x1210000);
    if (hide != 0) {
      Actor_SetModeById(12, 0);
      Actor_SetModeById(13, 0);
    } else {
      Actor_SetModeById(12, 1);
      Actor_SetModeById(13, 1);
    }
  } else {
    Kibombo_MapCopy(45, 37, 79, 15, 3, 2);
    Func_02005b0c(12, 0, 0);
    Func_02005b0c(13, 0, 0);
  }
}

void Func_020036e0(s32 open, s32 mode) {
  if (open != 0) {
    Kibombo_MapCopy(41, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(12, 21, 1, 1, 16, 22);
    if (mode != 0)
      Kibombo_ObjSetPos(Actor_GetById(14), 0x1080000, 0x200000, 0x1340000);
    switch (mode) {
    case 0:
      Func_02005b0c(14, 0, 0);
      break;
    case 1:
      Actor_SetModeById(14, 0);
      break;
    case 2:
      Actor_SetModeById(14, 1);
      break;
    case 3:
      Actor_SetModeById(14, 2);
      break;
    case 4:
      Actor_SetModeById(14, 3);
      break;
    case 5:
      Actor_SetModeById(14, 6);
      Actor_GetById(14)->y -= 0x220000;
      break;
    case 6:
      Actor_SetModeById(14, 7);
      Actor_GetById(14)->y -= 0x220000;
      break;
    }
  } else {
    Kibombo_MapCopy(45, 38, 79, 16, 3, 3);
    Kibombo_BlendStart(17, 22, 1, 1, 16, 22);
    Func_02005b0c(14, 0, 0);
  }
}
