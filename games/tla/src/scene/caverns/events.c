#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02001094();
void Func_020010ac();
void Func_020010dc();
void Func_020010e4();
void Func_0200113c();
void GameFlag_SetBit();

void FieldEvent_Wait();
void Func_02000d68();
void Actor_CommitPositionAndActivate();
void Actor_ArmCallback();
void Func_02000d28();
void GameFlag_SetBit();
void Func_02001094();
void Func_020010ac();
void GameFlag_Test();

void Func_02000e20(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 16, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  ((void (*)())Func_02000d68)();
  return;
}

void Func_02000e44(void) {
  Call3((void (*)())Actor_CommitPositionAndActivate, 4, 0, -14);
  Value3((s32 (*)())Actor_CommitPositionAndActivate, 4, -16, 0);
  Value1((s32 (*)())FieldEvent_Wait, 10);
  Value0((s32 (*)())Func_02000d68);
  return;
}

void Func_02000e6c(void) {
  Call3((void (*)())Actor_ArmCallback, 4, 0xc000, 0);
  Call1((void (*)())FieldEvent_Wait, 10);
  ((void (*)())Func_02000d68)();
  return;
}

void Func_02000e88(s32 arg0) {
  if (arg0 != 1)
    goto L_02008e9c;
  Value0((s32 (*)())Func_02000d28);
  Value1((s32 (*)())GameFlag_SetBit, 0x9af);
L_02008e9c:
  return;
}

void Func_02000ea0(void) {
  s32 result0;

  result0 = Value1((s32 (*)())GameFlag_Test, 0x917);
  if (result0 == 0)
    goto L_02008eee;
  Call6((void (*)())Func_020010ac, 31, 43, 9, 12, 48, 47);
  Call6((void (*)())Func_02001094, 30, 110, 48, 110, 9, 13);
  Call6((void (*)())Func_020010ac, 52, 110, 1, 1, 52, 111);
L_02008eee:
  return;
}

void Func_02000ef4(void) {
  ((void (*)())Func_020010dc)();
  Call1((void (*)())Func_0200113c, 0);
  Call6((void (*)())Func_020010ac, 31, 43, 9, 11, 48, 47);
  Call6((void (*)())Func_02001094, 30, 110, 48, 110, 9, 13);
  Call6((void (*)())Func_020010ac, 52, 110, 1, 1, 52, 111);
  Call1((void (*)())GameFlag_SetBit, 0x917);
  ((void (*)())Func_020010e4)();
  return;
}
