/// @file
/// The task scheduler: per-frame and VBlank routines in order.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

#ifndef UBFIX
typedef s32 (*KeyCallbackFn)(void);
#endif

// The task scheduler: up to 20 routines run each frame by WaitFrames or each
// VBlank, kept sorted by their order value (see SCHEDULER_KEY_MAIN_LOOP).

/// Empties the task table.
void Scheduler_ResetTaskTable(void) {
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 remaining;

  gSchedulerReady = 0;
  gSchedulerStatus = 0;
  remaining = SCHEDULER_TBS_TASK_COUNT - 1;
  do {
    task->callback = 0;
    task->state = 0xffff;
    task->mask = 0;
    task++;
    remaining--;
  } while (remaining >= 0);
  gSchedulerReady = 1;
}

/// Copies `byteCount` bytes a word at a time.
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byteCount) {
  u32 index;
  byteCount >>= 2;
  for (index = 0; index < byteCount; index++)
    *destination++ = *source++;
}

/// Bubble-sorts the task table by order, highest first, so each pass runs its
/// tasks from the highest order down.
void Scheduler_SortTasks(void) {
  /* FAKEMATCH: the declaration initializer repeats the first pass's assignment;
   * the native loads the table before the loop and again at the top of every
   * pass. Without it the table address is hoisted into r5 (push r7): differ 28.
   * Reusing the swap cursor, a leading pass test or the reload at the end of a
   * pass: differ 11 to 26. Ordinary struct assignment for all three copies
   * differs 60 (33 instructions for 39); without the initializer as well,
   * it differs 64 (31 for 39), measured in tbs-en. */
  struct SchedulerTask saved;
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 pass;
  s32 index;

  for (pass = SCHEDULER_TBS_TASK_COUNT - 1; pass > 1; pass--) {
    task = gSchedulerTaskTable;
    for (index = 0; index < pass; index++) {
      if ((s16)task[1].state > (s16)task->state) {
        memcpy(&saved, task, sizeof(saved));
        memcpy(task, task + 1, sizeof(saved));
        memcpy(task + 1, &saved, sizeof(saved));
      }
      task++;
    }
  }
}

/// Returns the table index of the task running `callback`, or -1.
s32 Scheduler_FindCallback(SchedulerCallback callback) {
  s32 result;
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Schedules `callback` with `order`, or gives a task already running it the
/// new order, and re-sorts the table. Returns the task's index, or -1 when
/// the table is full.
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order) {
  u32 savedInterruptState;
  s32 index;
  struct SchedulerTask *task;
  s32 i;

  index = -1;
  task = gSchedulerTaskTable;
  (void)gSchedulerStatus;
  IRQ_LOCK(savedInterruptState);
  // Update an existing callback before looking for an unused task slot.
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      task->state = order;
      index = i;
      break;
    }
  }
  task = gSchedulerTaskTable;
  if (index == -1) {
    for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
      if (task->callback == 0) {
        task->callback = (u32)callback;
        task->state = order;
        task->mask = 0;
        index = i;
        break;
      }
    }
  }
  Scheduler_SortTasks();
  IRQ_UNLOCK(savedInterruptState);
  return index;
}

/// Empty scheduler callback; it leaves all runtime state unchanged.
void Scheduler_Idle(void) {}

/// Alternate empty scheduler callback with the same no-op behavior.
void Scheduler_EmptyCallback(void) {}

/// Unschedules the task running `callback`. Returns its old index, or -1.
s32 Scheduler_RemoveCallback(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      task->callback = 0;
      task->state = 0x7fff;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Suspends the task running `callback`, or every task when `callback` is null,
/// by making the high byte of its order odd. Returns the last index changed,
/// or -1.
s32 Scheduler_SuspendTasks(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (callback != 0) {
      if (task->callback != (u32)callback)
        continue;
    }
    TASK_STATE_HIGH(task) |= 1;
    result = i;
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Suspends every task whose routine is in an overlay (EWRAM, 0x02xxxxxx)
/// unless the task is marked to keep running (flag bit 0). The menus do this
/// while they are open. Returns the last index changed, or -1.
s32 Scheduler_SuspendOverlayTasks(void) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if ((task->callback >> 24) == 2 && (task->mask & 1) == 0) {
      TASK_STATE_HIGH(task) |= 1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Sets the flags of the task running `callback`; bit 0 keeps it running while
/// the overlay tasks are suspended. Returns its index, or -1.
s32 Scheduler_SetTaskFlags(SchedulerCallback callback, u32 mask) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (task->callback == (u32)callback) {
      task->mask = mask;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Resumes the task running `callback`, or every task when `callback` is null.
/// Returns the last index changed, or -1.
s32 Scheduler_ResumeTasks(SchedulerCallback callback) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if (callback == 0 || task->callback == (u32)callback) {
      TASK_STATE_HIGH(task) &= ~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Resumes every task whose routine is in an overlay. Returns the last index
/// changed, or -1.
s32 Scheduler_ResumeOverlayTasks(void) {
  struct SchedulerTask *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gSchedulerTaskTable;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < SCHEDULER_TBS_TASK_COUNT; i++, task++) {
    if ((task->callback >> 24) == 2) {
      TASK_STATE_HIGH(task) &= ~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Runs one pass of the scheduler: every task whose order's high byte matches
/// `key`'s (SCHEDULER_KEY_MAIN_LOOP or SCHEDULER_KEY_VBLANK), in table order.
/// Nothing runs until Scheduler_ResetTaskTable has set the table up.
void Scheduler_RunPass(s32 key) {
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 remaining;

  key >>= 8;
  if (gSchedulerReady == 1) {
    remaining = SCHEDULER_TBS_TASK_COUNT + 1;
    task--;
    for (;;) {
      if (--remaining == 0)
        return;
      task++;
      if (TASK_STATE_HIGH(task) == key) {
        // UB: callbacks return void, and the stored word is not a pointer
        // object. The native integer-return call uses r0; a void call uses
        // r3 instead (two instructions and the call relocation differ).
#ifdef UBFIX
        SchedulerCallback callback = (SchedulerCallback)task->callback;
#else
        KeyCallbackFn callback = *(KeyCallbackFn *)&task->callback;
#endif

        callback();
      }
    }
  }
}
