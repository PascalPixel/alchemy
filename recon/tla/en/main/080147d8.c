// Adopted since (2026-10-18): Scheduler_RunCallbacksByKey is C in
// games/tla/src/system/scheduler/disable_callbacks.c; this trial remains.
// Pin-free trial (tla-en): listing 22 instructions, draft 22, common 20,
// differ 4. Earlier register-allocation notes describe the prior pinned form.

// Trial: an s32 callback return type gives differ 0 but conflicts with the
// SchedulerCallback void return contract, so it is rejected. A register hint,
// an unconstrained target barrier and a typed inline helper all leave differ 4.
// An unprototyped void callback also leaves differ 4.

#include "types.h"
#include "callback_scheduler.h"

void Scheduler_RunCallbacksByKey(s32 arg) {
  s32 key;
  s32 held;
  /* FAKEMATCH: the native preincrement countdown uses an unsigned cursor so no
   * before-array C pointer is formed. */
  u32 cursor = (u32)gSchedulerTaskTable;
  struct SchedulerTask *task;
  s32 i;
  u8 *status = &gSchedulerReady;

  /* FAKEMATCH: retain the native address/key/status order. */
  asm("" : "=r"(held), "+r"(status) : "0"(arg));
  key = held;
  key >>= 8;
  if (*status == 1) {
    i = 25;
    cursor -= sizeof(*task);
  next_task:
    i--;
    if (i != 0) {
      cursor += sizeof(*task);
      task = (struct SchedulerTask *)cursor;
      if (TASK_STATE_HIGH(task) == key) {
        void (*call)(void) = (void (*)(void))task->callback;
        call();
      }
      goto next_task;
    }
  }
}
