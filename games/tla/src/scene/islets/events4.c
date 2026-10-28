#include "types.h"
#include "actor.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020034fc();
void Task_Add();
void Task_RemoveFar();

void Func_020034d8(void) {
  Value2((s32 (*)())Task_Add, &Func_020034fc, 0x480);
  return;
}

void Func_020034ec(void) {
  Call1((void (*)())Task_RemoveFar, &Func_020034fc);
  return;
}
