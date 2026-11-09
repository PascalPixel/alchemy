#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_020034fc();

void Func_020034d8(void) {
  Task_AddFrame(&Func_020034fc, 0x480);
  return;
}

void Func_020034ec(void) {
  FieldTask_Remove(Func_020034fc);
  return;
}
