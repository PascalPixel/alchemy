#include "scene.h"

void Func_020005e8(s32 actor);
void Func_0200067c(s32 value);
void Func_02003544(s32 actor, s32 value, s32 frames);
void Func_0200351c(s32 actor, s32 value, s32 frames);

void Func_02000978(s32 actor) {
  Func_0200345c();
  Func_020035b4(0);
  Func_0200350c((s32)MsgAhHaveYou);
  Func_020005e8(actor);
  Func_02003534_(8, 0x3000);
  Actor_SetModeById(8, 7);
  FieldEvent_Wait(30);
  Func_02003544(8, 258, 50);
  Actor_SetModeById(8, 1);
  FieldEvent_Wait(15);
  Func_0200351c(8, 0, 5);
  Actor_SetModeById(8, 7);
  FieldEvent_Wait(30);
  Func_02003544(8, 264, 50);
  Actor_SetModeById(8, 1);
  FieldEvent_Wait(15);
  Func_0200351c(8, 0, 5);
  Func_0200067c(0);
  Func_02003534(8, 0x8000);
  FieldEvent_Wait(10);
  Func_0200351c(8, 0, 5);
  Func_02003534_(8, 0x5000);
  Func_02003464();
}
