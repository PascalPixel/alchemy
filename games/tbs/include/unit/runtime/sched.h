#ifndef ALCHEMY_UNIT_RUNTIME_SCHED_H
#define ALCHEMY_UNIT_RUNTIME_SCHED_H

#include "types.h"
#include "callback_scheduler.h"
#include "irq.h"

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
void Runtime_InvokeCallbacksByKey(s32 key);

#endif
