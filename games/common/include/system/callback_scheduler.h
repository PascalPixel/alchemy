#ifndef ALCHEMY_CALLBACK_SCHEDULER_H
#define ALCHEMY_CALLBACK_SCHEDULER_H

#include "edition.h"
#include "types.h"
#include "layout_guard.h"

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

LAYOUT_SIZE_GUARD(SchedulerTask_Size, struct SchedulerTask, 8);
LAYOUT_OFFSET_GUARD(SchedulerTask_State, struct SchedulerTask, state, 4);
LAYOUT_OFFSET_GUARD(SchedulerTask_Mask, struct SchedulerTask, mask, 6);

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

/* FAKEMATCH: volatile makes every access to this cell a load or store of its
 * own; without it Scheduler_AddOrUpdateCallback: differ 14 in tbs-en, 59
 * instructions for 61. */
extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[];

#if GAME_TLA
void Runtime_PushSlotEntry(void *entry, s32 priority);

void Runtime_InvokeCallbacksByKey(s32 key);
#endif
void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1);
#if GAME_TLA
void Scheduler_ScheduleCallbackBAfterFrames(void);

void Scheduler_ResetTaskTable(void);
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count);
void Scheduler_SortTasks(void);
s32 Scheduler_FindCallback(SchedulerCallback callback);
s32 Scheduler_AddOrUpdateCallback(SchedulerCallback callback, s32 order);
void Scheduler_Idle(void);
void Scheduler_EmptyCallback(void);
s32 Scheduler_RemoveCallback(SchedulerCallback callback);
s32 Scheduler_EnableCallbacks(SchedulerCallback callback);
s32 Scheduler_EnableUnmaskedOverlayCallbacks(void);
s32 Scheduler_SetCallbackMask(SchedulerCallback callback, u32 mask);
s32 Scheduler_DisableCallbacks(SchedulerCallback callback);
s32 Scheduler_DisableOverlayCallbacks(void);
s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);
#endif

void Scheduler_RunCallbacksByKey(s32 key);
s32 Scheduler_CountCallbacksByKey(s32 key);
void Scheduler_RunCallbacksBeforeBoundary(s32 key);

#endif
