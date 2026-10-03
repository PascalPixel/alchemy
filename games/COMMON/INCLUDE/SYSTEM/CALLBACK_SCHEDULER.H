#ifndef ALCHEMY_CALLBACK_SCHEDULER_H
#define ALCHEMY_CALLBACK_SCHEDULER_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* TLA expanded the same task record to twenty-four entries. */
enum SchedulerTaskLimits {
    SCHEDULER_TBS_TASK_COUNT = 20,
    SCHEDULER_TLA_TASK_COUNT = 24
};

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

extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[];

/* Render entries have different tails; only their first-word link is shared. */
void Runtime_PushSlotEntry(void *entry, s32 priority);

void Runtime_InvokeCallbacksByKey(s32 key);
void Runtime_CallWorkEntryWithArgs(s32 arg0, s32 arg1);
void Scheduler_ScheduleCallbackBAfterFrames(void);

void Scheduler_ResetTaskTable(void);
void Scheduler_CopyWords(u32 *destination, u32 *source, u32 byte_count);
void Scheduler_SortTasks(void);
s32 Scheduler_FindCallback(u32 callback);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
void Scheduler_Idle(void);
void Scheduler_EmptyCallback(void);
s32 Scheduler_RemoveCallback(u32 callback);
s32 Scheduler_EnableCallbacks(u32 callback);
s32 Scheduler_EnableUnmaskedOverlayCallbacks(void);
s32 Scheduler_SetCallbackMask(u32 callback, u32 mask);
s32 Scheduler_DisableCallbacks(u32 callback);
s32 Scheduler_DisableOverlayCallbacks(void);
s32 Scheduler_DisableOverlayCallbacksWithFlags(void);
s32 Scheduler_EnableOverlayCallbacksWithFlags(void);

void Scheduler_RunCallbacksByKey(s32 key);
s32 Scheduler_CountCallbacksByKey(s32 key);
void Scheduler_RunCallbacksBeforeBoundary(s32 key);

#endif
