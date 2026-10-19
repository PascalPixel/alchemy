#ifndef ALCHEMY_CALLBACK_SCHEDULER_H
#define ALCHEMY_CALLBACK_SCHEDULER_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

enum SchedulerTaskLimits {
  SCHEDULER_TBS_TASK_COUNT = 20,
  SCHEDULER_TLA_TASK_COUNT = 24
};

typedef void (*SchedulerCallback)(void);

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

/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it Scheduler_AddOrUpdateCallback: differ 14 in tbs-en, 59
 * instructions for 61. */
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerReady; // 1 once the task table is set up
extern struct SchedulerTask gSchedulerTaskTable[];

void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1);

void Scheduler_RunCallbacksByKey(s32 key);
s32 Scheduler_CountCallbacksByKey(s32 key);
void Scheduler_RunCallbacksBeforeBoundary(s32 key);

#endif
