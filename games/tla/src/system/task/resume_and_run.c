#include "types.h"
#include "io_reg.h"
#include "task.h"
#include "system.h"
#include "irqlock.h"

/// Keyed callbacks return a value (the target address is left in r0).
typedef s32 (*TaskIntFunc)(void);

s32 Task_ResumeAll(TaskFunc func) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if (func == 0 || task->func == (u32)func) {
      TASK_STATE_HIGH(task) &= (u8)~1;
      result = i;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

s32 Task_ResumeOverlayTasks(void) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(saved_interrupt_master);
  for (i = 0; i <= 23; i++, task++) {
    if ((task->func >> 24) == 2) {
      TASK_STATE_HIGH(task) &= (u8)~1;
      result = i;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

void Task_RunPass(s32 pass) {
  struct Task *task = gTasks;
  s32 remaining;

  pass >>= 8;
  if (gTasksInitialized == 1) {
    remaining = TASK_COUNT + 1;
    task--;
  next_task:
    remaining--;
    if (remaining != 0) {
      task++;
      if (TASK_STATE_HIGH(task) == pass) {
        TaskIntFunc func = *(TaskIntFunc *)&task->func;

        func();
      }
      goto next_task;
    }
  }
}

s32 Task_CountInPass(s32 pass) {
  /* FAKEMATCH: plain typed scans use bottom-tested loops. The native
   * preincrement countdown uses an unsigned cursor so no before-array C pointer
   * is formed. */
  u32 cursor;
  struct Task *task;
  s32 count;
  s32 i;
  u8 *status = &gTasksInitialized;
  u32 observed;

  count = 0;
  observed = *status;
  cursor = (u32)gTasks;
  pass >>= 8;
  if (observed == 1) {
    i = 25;
    cursor -= sizeof(*task);
  next_task:
    i--;
    if (i != 0) {
      goto count_task;
    }
    pass = count;
    goto finish;
  count_task:
    {
      cursor += sizeof(*task);
      task = (struct Task *)cursor;
      if (TASK_STATE_HIGH(task) == pass) {
        count++;
      }
      goto next_task;
    }
  }
finish:
  return pass;
}

void Task_RunPassBeforeVBlankTasks(s32 pass) {
  struct Task *task = gTasks;
  s32 remaining;

  pass >>= 8;
  if (gTasksInitialized == 1) {
    remaining = TASK_COUNT + 1;
    task--;
  next_task:
    remaining--;
    if (remaining != 0) {
      task++;
      if (TASK_STATE_HIGH(task) == 4) {
        return;
      }
      if (TASK_STATE_HIGH(task) == pass) {
        TaskIntFunc func = *(TaskIntFunc *)&task->func;

        func();
      }
      goto next_task;
    }
  }
}

u32 Random16(void) {
  u32 value;

  value = gRandomState * 0x41c64e6dU + 0x3039U;
  gRandomState = value;
  return (value << 8) >> 16;
}
