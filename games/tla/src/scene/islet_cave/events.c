#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Actor_SetModeById();
void Func_020007dc(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02000848(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Func_020008b4(s32 arg0) {
  Call2((void (*)())Actor_SetModeById, arg0, 1);
  Call2((void (*)())Actor_SetModeById, arg0, 2);
  return;
}

void Func_020008c8(void) {
  ((void (*)())Func_02000848)(8, 27, 0, 136, 0x1f8, 20, 0x280);
  ((void (*)())Func_02000848)(9, 28, 0, 136, 232, 21, 0x281);
  ((void (*)())Func_02000848)(10, 29, 0, 0x358, 152, 22, 0x282);
  ((void (*)())Func_02000848)(11, 30, 0, 0x318, 152, 23, 0x283);
  ((void (*)())Func_02000848)(12, 31, 0, 0x358, 0x178, 24, 0x284);
  ((void (*)())Func_02000848)(13, 32, 0, 0x318, 0x178, 25, 0x285);
  ((void (*)())Func_020007dc)(14, 33, 0, 0x338, 120, 26, 0x286);
  return;
}

void Func_020009a4(void) {
  Call1((void (*)())Func_020008b4, 8);
  return;
}

void Func_020009b0(void) {
  Call1((void (*)())Func_020008b4, 9);
  return;
}

void Func_020009bc(void) {
  Call1((void (*)())Func_020008b4, 10);
  return;
}

void Func_020009c8(void) {
  Call1((void (*)())Func_020008b4, 11);
  return;
}

void Func_020009d4(void) {
  Call1((void (*)())Func_020008b4, 12);
  return;
}

void Func_020009e0(void) {
  Call1((void (*)())Func_020008b4, 13);
  return;
}

void Func_020009ec(void) {
  Call1((void (*)())Func_020008b4, 14);
  return;
}
