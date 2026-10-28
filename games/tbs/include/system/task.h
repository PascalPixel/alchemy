#ifndef ALCHEMY_TASK_H
#define ALCHEMY_TASK_H

/// @file
/// Tasks, their priorities and the task runner's state.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

enum TaskLimits { TASK_COUNT = 20, TASK_COUNT_TLA = 24 };

typedef void (*TaskFunc)(void);

/// A task: its routine's address, its priority (see TASK_PRIORITY_) and
/// its flags, where bit 0 keeps it running while overlay tasks are suspended.
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
/// wait() runs TASK_PASS_FRAME's once a frame, the field's and every
/// scene's own per-frame tasks among them.
#define TASK_PASS_VBLANK 0x480
#define TASK_PASS_FRAME 0xc80
/// An empty slot's priority, sorted last.
#define TASK_PRIORITY_EMPTY 0xffff
/// A removed task's priority: its odd high byte matches no pass, and it sorts
/// to the front of the table.
#define TASK_PRIORITY_REMOVED 0x7fff

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->priority)[1])

/// 1 while wait() runs the frame pass's tasks (TASK_PASS_FRAME).
/* FAKEMATCH(volatile): the read Task_Add discards stays a
 * load; as plain u8, 8 of 61 instructions differ in tbs-en. */
extern volatile u8 gTasksRunning;
extern u8 gTasksInitialized; ///< 1 once the task table is set up
extern struct Task gTasks[];

#endif
