#ifndef ALCHEMY_CALLBACK_SCHEDULER_H
#define ALCHEMY_CALLBACK_SCHEDULER_H

/// @file
/// The task scheduler: tasks, their order and the scheduler's state.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

enum SchedulerTaskLimits {
  SCHEDULER_TBS_TASK_COUNT = 20,
  SCHEDULER_TLA_TASK_COUNT = 24
};

typedef void (*SchedulerCallback)(void);

/// A scheduled task: its routine's address, its order (see SCHEDULER_KEY_) and
/// its flags, where bit 0 keeps it running while overlay tasks are suspended.
struct SchedulerTask {
  u32 callback;
  u16 state;
  u8 mask;
  u8 reserved;
};

STATIC_ASSERT(sizeof(struct SchedulerTask) == 8);
STATIC_ASSERT(offsetof(struct SchedulerTask, state) == 4);
STATIC_ASSERT(offsetof(struct SchedulerTask, mask) == 6);

/// A task's order: the high byte picks the pass that runs it, the low byte
/// ranks it within the pass (higher runs first). An odd high byte means the
/// task is suspended, so no pass matches it.
#define SCHEDULER_KEY_VBLANK 0x480
#define SCHEDULER_KEY_MAIN_LOOP 0xc80

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

/// 1 while WaitFrames runs the main-loop tasks.
/* FAKEMATCH(volatile): the read Scheduler_AddOrUpdateCallback discards stays a
 * load; as plain u8, 8 of 61 instructions differ in tbs-en. */
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerReady; ///< 1 once the task table is set up
extern struct SchedulerTask gSchedulerTaskTable[];

void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1);

void Scheduler_RunCallbacksByKey(s32 key);
s32 Scheduler_CountCallbacksByKey(s32 key);
void Scheduler_RunCallbacksBeforeBoundary(s32 key);

#endif
