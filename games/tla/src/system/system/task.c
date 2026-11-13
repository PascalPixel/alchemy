/// @file
/// Tasks: per-frame and VBlank routines in priority order.
// Part of system.c.

#include "../system_internal.h"

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
  // DEAD: nothing uses the status read here; its load is in the ROM.
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
    /* FAKEMATCH(dead): an empty once-only block after the priority's high byte
     * store, so the result's copy follows it; without it, 2 of 27 instructions
     * differ in tla-en. */
    do {
    } while (0);
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
      /* FAKEMATCH(dead): an empty once-only block after the priority's high
       * byte store, so the result's copy follows it; without it, 2 of 31
       * instructions differ in tla-en. */
      do {
      } while (0);
      result = i;
    }
  }
  IRQ_UNLOCK(saved_interrupt_master);
  return result;
}

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->priority)[1])
extern struct Task gTasks[24];

s32 Task_SetFlags(TaskFunc func, u32 mask) {
  struct Task *task;
  u32 saved_interrupt_master;
  s32 result;
  s32 i;

  task = gTasks;
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
  // The loop is written with goto: as for (i = 0; i < 24; i++, task++), the
  // compiler tests it at the bottom and 24 instructions differ from the 22 in
  // tla-en. The cursor is a u32, so it can start one task before gTasks
  // without forming that pointer.
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
