#ifndef ALCHEMY_TASK_H
#define ALCHEMY_TASK_H

/// @file
/// Tasks, their priorities and the task runner's state.

#include "edition.h"
#include "static_assert.h"
#include "types.h"
#include "system/io_reg.h"

/// The most tasks that can run at once.
enum TaskLimits { TASK_COUNT = 20 };

typedef void (*TaskFunc)(void);

/// A task: its routine's address, its priority (see TASK_PASS_) and its
/// flags, where TASK_FLAG_KEEP_RUNNING keeps it running while overlay tasks are
/// suspended.
struct Task {
  u32 func;
  u16 priority;
  u8 flags;
  u8 reserved;
};

STATIC_ASSERT(sizeof(struct Task) == 8);
STATIC_ASSERT(offsetof(struct Task, priority) == 4);
STATIC_ASSERT(offsetof(struct Task, flags) == 6);

/// A task's priority: the high byte picks the pass that runs it, the low byte
/// ranks it within the pass (higher runs first). An odd high byte means the
/// task is suspended, so no pass matches it.
///
/// The two passes: the VBlank handler runs TASK_PASS_VBLANK's tasks, and
/// wait() runs TASK_PASS_FRAME's once a frame. Each ranks 0x80, the middle of
/// the low byte, so a task added at TASK_PASS_FRAME + 10 runs before those
/// added at TASK_PASS_FRAME.
#define TASK_PASS_VBLANK 0x480
#define TASK_PASS_FRAME 0xc80
/// An empty slot's priority, sorted last: the sort reads priorities signed.
#define TASK_PRIORITY_EMPTY -1
/// A removed task's priority: its odd high byte matches no pass, and as the
/// highest priority it sorts to the front of the table.
#define TASK_PRIORITY_REMOVED 0x7fff

/// A task's priority's high byte: the pass it runs in.
#define TASK_STATE_HIGH(task) (((u8 *)&(task)->priority)[1])
/// The bit of a task's high priority byte (TASK_STATE_HIGH) that suspends it.
#define TASK_SUSPENDED 1
/// The bit of a task's flags that keeps it running while
/// Task_SuspendOverlayTasks suspends the others.
#define TASK_FLAG_KEEP_RUNNING 1
/// Whether the task's routine is in EWRAM, where a scene's overlay runs.
#define TASK_IN_OVERLAY(task) (((task)->func >> 24) == EWRAM_START >> 24)

/// 1 while wait() runs the frame pass's tasks (TASK_PASS_FRAME).
extern volatile u8 gTasksRunning;
extern u8 gTasksInitialized; ///< 1 once the task table is set up
/// The task table, run in priority order by each pass.
extern struct Task gTasks[];

#endif
