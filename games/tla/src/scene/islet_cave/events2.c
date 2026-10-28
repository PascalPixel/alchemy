#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Actor_SetSingleChildFlags();

void Func_020012dc();

void Func_020003b0(void) {
  Call2((void (*)())Func_020012dc, 15, 12);
  return;
}

void Func_020003bc(void) {
  Call2((void (*)())Func_020012dc, 9, 37);
  return;
}

s32 Func_020003c8(s32 arg0) {
  Call2((void (*)())Actor_SetSingleChildFlags, arg0, 0);
  return 0;
}
