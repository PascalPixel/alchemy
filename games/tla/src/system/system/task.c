/// @file
/// Tasks: per-frame and VBlank routines in priority order.
// Part of system.c.

#include "../system_internal.h"

void Task_ResetAll(void) {
  struct Task *task = gTasks;
  s32 i;

  gTasksInitialized = 0;
  gTasksRunning = 0;
  for (i = 0; i < TASK_COUNT; i++) {
    task->func = 0;
    task->priority |= 0xffff;
    task->flags = 0;
    task++;
  }
  gTasksInitialized = 1;
}

void Task_CopyWords(u32 *destination, u32 *source, u32 byteCount) {
  u32 index;
  byteCount >>= 2;
  for (index = 0; index < byteCount; index++)
    *destination++ = *source++;
}

/// Bubble-sorts the task table by priority, highest first, so each pass runs
/// its tasks from the highest priority down; removed tasks rise to the front.
void Task_Sort(void) {
  struct Task saved;
  struct Task *task = gTasks;
  s32 pass;
  s32 index;

  // BUG: the last pass, which would settle the first two tasks, never runs,
  // so the tasks in slots 0 and 1 can stay out of order
#ifdef BUGFIX
  for (pass = TASK_COUNT - 1; pass > 0; pass--) {
#else
  for (pass = TASK_COUNT - 1; pass > 1; pass--) {
#endif
    task = gTasks;
    for (index = 0; index < pass; index++) {
      if ((s16)task[1].priority > (s16)task->priority) {
        memcpy(&saved, task, sizeof(saved));
        memcpy(task, task + 1, sizeof(saved));
        memcpy(task + 1, &saved, sizeof(saved));
      }
      task++;
    }
  }
}

s32 Task_Find(TaskFunc func) {
  s32 result;
  struct Task *task;
  u32 savedInterruptMaster;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->func == (u32)func) {
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

s32 Task_Add(TaskFunc func, s32 order) {
  u32 savedInterruptState;
  s32 index;
  struct Task *task;
  s32 i;

  task = gTasks;
  // DEAD: nothing uses the status read here; the ROM keeps the load.
  (void)gTasksRunning;
  index = -1;
  IRQ_LOCK(savedInterruptState);
  // Update an existing task before looking for an unused task slot.
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->func == (u32)func) {
      task->priority = order;
      index = i;
      break;
    }
  }
  task = gTasks;
  if (index == -1) {
    for (i = 0; i < TASK_COUNT; i++, task++) {
      if (task->func == 0) {
        task->func = (u32)func;
        task->priority = order;
        task->flags = 0;
        index = i;
        break;
      }
    }
  }
  Task_Sort();
  IRQ_UNLOCK(savedInterruptState);
  return index;
}

void Task_Idle(void) {}

void Task_Empty(void) {}

s32 Task_Remove(TaskFunc func) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->func == (u32)func) {
      task->func = 0;
      task->priority = 0x7fff;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

s32 Task_SuspendAll(TaskFunc func) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (func != 0) {
      if (task->func != (u32)func)
        continue;
    }
    TASK_STATE_HIGH(task) |= 1;
    /* FAKEMATCH(dead): an empty once-only block after the priority's high byte
     * store. */
    do {
    } while (0);
    result = i;
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

s32 Task_SuspendOverlayTasks(void) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if ((task->func >> 24) == 2 && (task->flags & 1) == 0) {
      TASK_STATE_HIGH(task) |= 1;
      /* FAKEMATCH(dead): an empty once-only block after the priority's high
       * byte store. */
      do {
      } while (0);
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->priority)[1])
extern struct Task gTasks[TASK_COUNT];

s32 Task_SetFlags(TaskFunc func, u32 mask) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->func == (u32)func) {
      task->flags = mask;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

s32 Task_ResumeAll(TaskFunc func) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (func == 0 || task->func == (u32)func) {
      TASK_STATE_HIGH(task) &= (u8)~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

s32 Task_ResumeOverlayTasks(void) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  task = gTasks;
  result = -1;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if ((task->func >> 24) == 2) {
      TASK_STATE_HIGH(task) &= (u8)~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

void Task_RunPass(s32 pass) {
  struct Task *task = gTasks;
  s32 remaining;

  pass >>= 8;
  if (gTasksInitialized == 1) {
    remaining = TASK_COUNT + 1;
    task--;
    for (;;) {
      if (--remaining == 0)
        return;
      task++;
      if (TASK_STATE_HIGH(task) == pass) {
        TaskIntFunc func = *(TaskIntFunc *)&task->func;

        func();
      }
    }
  }
}

s32 Task_CountInPass(s32 pass) {
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
    i = TASK_COUNT + 1;
    cursor -= sizeof(*task);
    for (;;) {
      if (--i == 0)
        return count;
      cursor += sizeof(*task);
      task = (struct Task *)cursor;
      if (TASK_STATE_HIGH(task) == pass)
        count++;
    }
  }
  return pass;
}

void Task_RunPassBeforeVBlankTasks(s32 pass) {
  struct Task *task = gTasks;
  s32 remaining;

  pass >>= 8;
  if (gTasksInitialized == 1) {
    remaining = TASK_COUNT + 1;
    task--;
    for (;;) {
      if (--remaining == 0)
        return;
      task++;
      if (TASK_STATE_HIGH(task) == 4)
        return;
      if (TASK_STATE_HIGH(task) == pass) {
        TaskIntFunc func = *(TaskIntFunc *)&task->func;

        func();
      }
    }
  }
}
