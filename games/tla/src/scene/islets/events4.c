#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_020034fc();
void Scheduler_AddOrUpdateCallback();
void Scheduler_RemoveCallbackFar();

void Func_020034d8(void) {
  Value2((s32 (*)())Scheduler_AddOrUpdateCallback, &Func_020034fc, 0x480);
  return;
}

void Func_020034ec(void) {
  Call1((void (*)())Scheduler_RemoveCallbackFar, &Func_020034fc);
  return;
}
