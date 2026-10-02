#ifndef ALCHEMY_CALLBACK_SCHEDULER_H
#define ALCHEMY_CALLBACK_SCHEDULER_H

#include "TYPES.H"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

#define TASK_STATE_HIGH(task) (((u8 *)&(task)->state)[1])

extern volatile u8 gSchedulerStatus;
extern u8 gSchedulerTaskCount;
extern struct SchedulerTask gSchedulerTaskTable[];

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
