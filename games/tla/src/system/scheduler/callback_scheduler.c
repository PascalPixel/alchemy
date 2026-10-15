#include "irqlock.h"
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "string.h"

void Scheduler_ResetTaskTable(void) {
  struct SchedulerTask *task;
  s32 remaining;
  u32 zero;

  gSchedulerReady = 0;
  task = gSchedulerTaskTable;
  gSchedulerStatus = 0;
  zero = 0;
  remaining = 23;
  do {
    task->callback = zero;
    task->state |= 0xffff;
    task->mask = zero;
    task++;
    remaining--;
  } while (remaining >= 0);
  gSchedulerReady = 1;
}

void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count) {
  u32 index;
  byte_count >>= 2;
  for (index = 0; index < byte_count; index++)
    *destination++ = *source++;
}

void Scheduler_SortTasks(void) {
  struct SchedulerTask saved;
  struct SchedulerTask *base = gSchedulerTaskTable;
  struct SchedulerTask *task;
  s32 pass = 23;
  s32 remaining;
  goto sort_pass;
next_pass:
  base = gSchedulerTaskTable;
sort_pass:
  task = base;
  if (pass <= 0)
    goto finish_pass;
  remaining = pass;
next_task:
  if ((s16)task[1].state > (s16)task->state) {
    memcpy(&saved, task, sizeof(saved));
    base = task;
    task++;
    memcpy(base, task, sizeof(saved));
    memcpy(task, &saved, sizeof(saved));
  } else {
    task++;
  }
  if (--remaining != 0)
    goto next_task;
finish_pass:
  if (--pass > 1)
    goto next_pass;
}

s32 Scheduler_FindCallback(SchedulerCallback callback) {
  s32 result;
  struct SchedulerTask *task;
  u32 saved_interrupt_master;
  s32 i;

  task = gSchedulerTaskTable;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (task->callback == (u32)callback) {
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order) {
  u32 saved_interrupt_state;
  s32 index;
  struct SchedulerTask *task;
  s32 i;

  task = gSchedulerTaskTable;
  (void)gSchedulerStatus;
  index = -1;
  IRQ_LOCK(saved_interrupt_state);
  i = 0;
  if (task->callback == (u32)callback) {
    task->state = order;
    index = 0;
  } else {
  find_existing:
    i++;
    task++;
    if (i <= 23) {
      if (task->callback == (u32)callback) {
        task->state = order;
        index = i;
      } else {
        goto find_existing;
      }
    }
  }
  task = gSchedulerTaskTable;
  if (index == -1) {
    i = 0;
    if (task->callback == 0) {
      task->callback = (u32)callback;
      task->state = order;
      task->mask = 0;
      index = 0;
    } else {
    find_empty:
      i++;
      task++;
      if (i <= 23) {
        if (task->callback == 0) {
          task->callback = (u32)callback;
          task->state = order;
          task->mask = 0;
          index = i;
        } else {
          goto find_empty;
        }
      }
    }
  }
  Scheduler_SortTasks();
  IRQ_UNLOCK(saved_interrupt_state);
  return index;
}

void Scheduler_Idle(void) {}

void Scheduler_EmptyCallback(void) {}

s32 Scheduler_RemoveCallback(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gSchedulerTaskTable;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (task->callback == (u32)callback) {
      task->callback = 0;
      task->state = 0x7fff;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Scheduler_EnableCallbacks(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gSchedulerTaskTable;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (callback != 0) {
      if (task->callback != (u32)callback)
        continue;
    }
    TASK_STATE_HIGH(task) |= 1;
    /* FAKEMATCH: the result copy follows the state byte store, as in
     * Scheduler_EnableUnmaskedOverlayCallbacks. */
    __asm__ volatile("");
    result = i;
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Scheduler_EnableUnmaskedOverlayCallbacks(void) {
  struct SchedulerTask *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gSchedulerTaskTable;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
      TASK_STATE_HIGH(task) |= 1;
      /* FAKEMATCH: under the ARM9TDMI load schedule the result copy fills the
       * state byte's load delay; native stores the byte first. */
      __asm__ volatile("");
      result = i;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerReady;
extern struct SchedulerTask gSchedulerTaskTable[24];

s32 Scheduler_SetCallbackMask(SchedulerCallback callback, u32 mask) {
  struct SchedulerTask *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gSchedulerTaskTable;
  /* FAKEMATCH: ⚓️ loads the task table before it builds the -1 result */
  __asm__ volatile("" : "+l"(task));
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (task->callback == (u32)callback) {
      task->mask = mask;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}
