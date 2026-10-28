#include "irqlock.h"
#include "types.h"
#include "io_reg.h"
#include "task.h"
#include "string.h"

void Task_ResetAll(void) {
  struct Task *task;
  s32 remaining;
  u32 zero;

  gTasksInitialized = 0;
  task = gTasks;
  gTasksRunning = 0;
  zero = 0;
  remaining = 23;
  do {
    task->func = zero;
    task->priority |= 0xffff;
    task->flags = zero;
    task++;
    remaining--;
  } while (remaining >= 0);
  gTasksInitialized = 1;
}

void Task_CopyWords(u32 *destination, u32 *source, u32 byte_count) {
  u32 index;
  byte_count >>= 2;
  for (index = 0; index < byte_count; index++)
    *destination++ = *source++;
}

void Task_Sort(void) {
  struct Task saved;
  struct Task *base = gTasks;
  struct Task *task;
  s32 pass = 23;
  s32 remaining;
  goto sort_pass;
next_pass:
  base = gTasks;
sort_pass:
  task = base;
  if (pass <= 0)
    goto finish_pass;
  remaining = pass;
next_task:
  if ((s16)task[1].priority > (s16)task->priority) {
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

s32 Task_Find(TaskFunc func) {
  s32 result;
  struct Task *task;
  u32 saved_interrupt_master;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (task->func == (u32)func) {
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Task_Add(TaskFunc func, s32 order) {
  u32 saved_interrupt_state;
  s32 index;
  struct Task *task;
  s32 i;

  task = gTasks;
  (void)gTasksRunning;
  index = -1;
  IRQ_LOCK(saved_interrupt_state);
  i = 0;
  if (task->func == (u32)func) {
    task->priority = order;
    index = 0;
  } else {
  find_existing:
    i++;
    task++;
    if (i <= 23) {
      if (task->func == (u32)func) {
        task->priority = order;
        index = i;
      } else {
        goto find_existing;
      }
    }
  }
  task = gTasks;
  if (index == -1) {
    i = 0;
    if (task->func == 0) {
      task->func = (u32)func;
      task->priority = order;
      task->flags = 0;
      index = 0;
    } else {
    find_empty:
      i++;
      task++;
      if (i <= 23) {
        if (task->func == 0) {
          task->func = (u32)func;
          task->priority = order;
          task->flags = 0;
          index = i;
        } else {
          goto find_empty;
        }
      }
    }
  }
  Task_Sort();
  IRQ_UNLOCK(saved_interrupt_state);
  return index;
}

void Task_Idle(void) {}

void Task_Empty(void) {}

s32 Task_Remove(TaskFunc func) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (task->func == (u32)func) {
      task->func = 0;
      task->priority = 0x7fff;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Task_SuspendAll(TaskFunc func) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (func != 0) {
      if (task->func != (u32)func)
        continue;
    }
    TASK_STATE_HIGH(task) |= 1;
    /* FAKEMATCH: the result copy follows the priority's high byte store, as in
     * Task_SuspendOverlayTasks. */
    __asm__ volatile("");
    result = i;
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Task_SuspendOverlayTasks(void) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if ((task->func >> 24) == 2 && (task->flags & 1) == 0) {
      TASK_STATE_HIGH(task) |= 1;
      /* FAKEMATCH: under the ARM9TDMI load schedule the result copy fills the
       * priority byte's load delay; native stores the byte first. */
      __asm__ volatile("");
      result = i;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->priority)[1])
extern volatile u8 gTasksRunning;
extern u8 gTasksInitialized;
extern struct Task gTasks[24];

s32 Task_SetFlags(TaskFunc func, u32 mask) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
  /* FAKEMATCH: ⚓️ loads the task table before it builds the -1 result */
  __asm__ volatile("" : "+l"(task));
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (task->func == (u32)func) {
      task->flags = mask;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}
