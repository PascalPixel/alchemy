/// @file
/// The task scheduler: per-frame and VBlank routines in order.
/// Part of runtime.c, the runtime object, compiled only through it.

#include "../runtime_internal.h"

#ifndef UBFIX
typedef s32 (*KeyCallbackFn)(void);
#endif

// The task scheduler: up to 20 routines run each frame by WaitFrames or each
// VBlank, kept sorted by their order value (see SCHEDULER_KEY_MAIN_LOOP).

/// Empties the task table: every slot gets no routine, order 0xffff (sorted
/// last) and no flags. Clears gSchedulerReady while it works and sets it at
/// the end, so neither pass runs a half-cleared table. Called by
/// System_Initialize and by the battle, Lucky Dice and other modes that start
/// from a clean table.
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

/// Copies `byteCount` bytes a word at a time. Nothing in ☀️ calls it.
UNUSED void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byteCount) {
  u32 index;
  byteCount >>= 2;
  for (index = 0; index < byteCount; index++)
    *destination++ = *source++;
}

/// Bubble-sorts the task table by order, highest first, so each pass runs its
/// tasks from the highest order down; removed tasks (order 0x7fff) rise to the
/// front. Called by Scheduler_AddOrUpdateCallback with interrupts disabled.
void Scheduler_SortTasks(void) {
  struct SchedulerTask saved;
  // DEAD: every pass sets task again; this initializer is the table's load
  // before the loop.
  struct SchedulerTask *task = gSchedulerTaskTable;
  s32 pass;
  s32 index;

  // BUG: the last pass, which would settle the first two tasks, never runs,
  // so the tasks in slots 0 and 1 can stay out of order
#ifdef BUGFIX
  for (pass = SCHEDULER_TBS_TASK_COUNT - 1; pass > 0; pass--) {
#else
  for (pass = SCHEDULER_TBS_TASK_COUNT - 1; pass > 1; pass--) {
#endif
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

/// Returns the table index of the task running `callback`, or -1. Nothing in
/// ☀️ calls it.
UNUSED s32 Scheduler_FindCallback(SchedulerCallback callback) {
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
/// new order, and re-sorts the table. Writes the task's routine, order and
/// flags (cleared for a new task) with interrupts disabled. Returns the task's
/// index, or -1 when all 20 slots are taken. Called throughout the game to
/// start per-frame and VBlank work; `order` is SCHEDULER_KEY_MAIN_LOOP or
/// SCHEDULER_KEY_VBLANK's high byte with a priority in the low byte.
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

/// An empty routine; nothing in ☀️ calls it.
UNUSED void Scheduler_Idle(void) {}

/// An empty routine; nothing in ☀️ calls it.
UNUSED void Scheduler_EmptyCallback(void) {}

/// Unschedules the task running `callback`: clears its routine and gives it
/// order 0x7fff, which no pass matches. Returns its old index, or -1. Called
/// throughout the game when the work a task did is finished.
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

/// Suspends the task running `callback`, or every task when `callback` is
/// null, by setting bit 0 of its order's high byte, which then matches neither
/// pass key. Returns the last index changed, or -1. Called by the map
/// animation and battle code, through the far-call table, to pause tasks
/// Scheduler_ResumeTasks restarts later.
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
/// unless its flag bit 0 asks to keep it running, as Scheduler_SuspendTasks
/// does. Called by the menus' common code while a menu is open, and by the
/// world map's visibility transition. Returns the last index changed, or -1.
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

/// Stores `mask` as the flags of the task running `callback`; bit 0 keeps it
/// running while the overlay tasks are suspended. Called by the link lobby
/// and Colosso scenes for the tasks that must keep running under a menu.
/// Returns its index, or -1.
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

/// Resumes the task running `callback`, or every task when `callback` is
/// null, by clearing bit 0 of its order's high byte. Called by the map
/// animation and field object code. Returns the last index changed, or -1.
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

/// Resumes every task whose routine is in an overlay, flagged or not. Called
/// by the menus' common code when a menu closes, and by the world map's
/// visibility transition. Returns the last index changed, or -1.
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
/// Called once a frame by WaitFrames and once a VBlank by
/// System_VBlankHandler. Nothing runs until Scheduler_ResetTaskTable has set
/// gSchedulerReady.
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
