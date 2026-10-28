/// @file
/// Tasks: per-frame and VBlank routines in priority order.
// Part of runtime.c.

#include "../runtime_internal.h"

#ifndef UBFIX
typedef s32 (*TaskIntFunc)(void);
#endif

// Tasks: up to 20 routines run each frame by wait() or each VBlank, kept
// sorted by their priority (see TASK_PASS_FRAME).

/// Empties the task table: every slot gets no routine, priority
/// TASK_PRIORITY_EMPTY and no flags. Clears gTasksInitialized while it works
/// and sets it at the end, so neither pass runs a half-cleared table. Called by
/// System_Initialize and by the battle, Lucky Dice and other modes that start
/// from a clean table.
void Task_ResetAll(void) {
  struct Task *task = gTasks;
  s32 remaining;

  gTasksInitialized = 0;
  gTasksRunning = 0;
  remaining = TASK_COUNT - 1;
  do {
    task->func = 0;
    task->priority = TASK_PRIORITY_EMPTY;
    task->flags = 0;
    task++;
    remaining--;
  } while (remaining >= 0);
  gTasksInitialized = 1;
}

/// Unused: copies `byteCount` bytes a word at a time. Nothing in ☀️ calls it.
UNUSED void Task_CopyWords(u32 *destination, u32 *source, u32 byteCount) {
  u32 index;
  byteCount >>= 2;
  for (index = 0; index < byteCount; index++)
    *destination++ = *source++;
}

/// Bubble-sorts the task table by priority, highest first, so each pass runs
/// its tasks from the highest priority down; removed tasks
/// (TASK_PRIORITY_REMOVED) rise to the front. Called by Task_Add with
/// interrupts disabled.
void Task_Sort(void) {
  struct Task saved;
  // DEAD: every pass sets task again; this initializer is the table's load
  // before the loop.
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

/// Unused: returns the table index of the task running `func`, or -1.
/// Nothing in ☀️ calls it.
UNUSED s32 Task_Find(TaskFunc func) {
  s32 result;
  struct Task *task;
  u32 savedInterruptMaster;
  s32 i;

  result = -1;
  task = gTasks;
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

/// Adds a task running `func` with priority `order`, or gives a task
/// already running it the new priority, and re-sorts the table. Writes the
/// task's routine, priority and flags (cleared for a new task) with interrupts
/// disabled. Returns the task's index, or -1 when all 20 slots are taken.
/// Called throughout the game to start per-frame and VBlank work; `order` is
/// TASK_PASS_FRAME or TASK_PASS_VBLANK's high byte with a rank in
/// the low byte. The index is the slot written before this call's own sort,
/// which may already have moved the task; a few callers return it, but nothing
/// in ☀️ tests it. A task stays until Task_Remove or Task_ResetAll.
s32 Task_Add(TaskFunc func, s32 order) {
  u32 savedInterruptState;
  s32 index;
  struct Task *task;
  s32 i;

  // Adding re-sorts the table, which would shift the tasks after the new one
  // if it came during a pass; it never does. In a call graph of the English
  // build, main image and overlays, no task reaches Task_Add, and neither does
  // any function whose address is taken except main-thread entries (far-call
  // entries, a scene's entry table and events, start-up).
  index = -1;
  task = gTasks;
  // DEAD: nothing uses the status read here; its load is in the ROM.
  (void)gTasksRunning;
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

/// Unused: an empty routine; nothing in ☀️ calls it.
UNUSED void Task_Idle(void) {}

/// Unused: an empty routine; nothing in ☀️ calls it.
UNUSED void Task_Empty(void) {}

/// Removes the task running `func`: clears its routine and gives it
/// priority TASK_PRIORITY_REMOVED, which no pass matches. Returns its old
/// index, or -1. Called throughout the game when the work a task did is
/// finished.
s32 Task_Remove(TaskFunc func) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gTasks;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (task->func == (u32)func) {
      task->func = 0;
      task->priority = TASK_PRIORITY_REMOVED;
      result = i;
      break;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Suspends the task running `func`, or every task when `func` is
/// null, by setting bit 0 of its priority's high byte, which then matches
/// neither pass. Returns the last index changed, or -1. Called by the map
/// animation and battle code, through the far-call table, to pause tasks
/// Task_ResumeAll restarts later.
s32 Task_SuspendAll(TaskFunc func) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gTasks;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (func != 0) {
      if (task->func != (u32)func)
        continue;
    }
    TASK_STATE_HIGH(task) |= 1;
    result = i;
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Suspends every task whose routine is in an overlay (EWRAM, 0x02xxxxxx)
/// unless its flag bit 0 asks to keep it running, as Task_SuspendAll
/// does. Called by the menus' shared code while a menu is open, and by the
/// world map's visibility transition. Returns the last index changed, or -1.
s32 Task_SuspendOverlayTasks(void) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gTasks;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    // Overlays run from EWRAM, so a routine at 0x02xxxxxx is an overlay's.
    // An empty slot (routine 0) never matches.
    if ((task->func >> 24) == 2 && (task->flags & 1) == 0) {
      TASK_STATE_HIGH(task) |= 1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Stores `mask` as the flags of the task running `func`; bit 0 keeps it
/// running while the overlay tasks are suspended. Called by the Battle Arena
/// and Colosso scenes for the tasks that must keep running under a menu.
/// Returns its index, or -1.
s32 Task_SetFlags(TaskFunc func, u32 mask) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gTasks;
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

/// Resumes the task running `func`, or every task when `func` is
/// null, by clearing bit 0 of its priority's high byte. Called by the map
/// animation and actor code. Returns the last index changed, or -1.
s32 Task_ResumeAll(TaskFunc func) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gTasks;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if (func == 0 || task->func == (u32)func) {
      TASK_STATE_HIGH(task) &= ~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Resumes every task whose routine is in an overlay, flagged or not. Called
/// by the menus' shared code when a menu closes, and by the world map's
/// visibility transition. Returns the last index changed, or -1.
s32 Task_ResumeOverlayTasks(void) {
  struct Task *task;
  u32 savedInterruptMaster;
  s32 result;
  s32 i;

  result = -1;
  task = gTasks;
  IRQ_LOCK(savedInterruptMaster);
  for (i = 0; i < TASK_COUNT; i++, task++) {
    if ((task->func >> 24) == 2) {
      TASK_STATE_HIGH(task) &= ~1;
      result = i;
    }
  }
  IRQ_UNLOCK(savedInterruptMaster);
  return result;
}

/// Runs one pass of the tasks: every task whose priority's high byte matches
/// `pass`'s (TASK_PASS_FRAME or TASK_PASS_VBLANK), in table order.
/// Called once a frame by wait() and once a VBlank by
/// System_VBlankHandler. Nothing runs until Task_ResetAll has set
/// gTasksInitialized.
void Task_RunPass(s32 pass) {
  struct Task *task = gTasks;
  s32 remaining;

  // A pass matches the high byte only; the low byte has already ranked the
  // tasks through the sort, and a suspended task's odd high byte matches no
  // pass
  pass >>= 8;
  if (gTasksInitialized == 1) {
    remaining = TASK_COUNT + 1;
    task--;
    for (;;) {
      if (--remaining == 0)
        return;
      task++;
      if (TASK_STATE_HIGH(task) == pass) {
        // UB: tasks return void, and the stored word is not a pointer
        // object. The native integer-return call uses r0; a void call uses
        // r3 instead (two instructions and the call relocation differ).
#ifdef UBFIX
        TaskFunc func = (TaskFunc)task->func;
#else
        TaskIntFunc func = *(TaskIntFunc *)&task->func;
#endif

        func();
      }
    }
  }
}
