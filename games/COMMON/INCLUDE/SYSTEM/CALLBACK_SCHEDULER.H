#ifndef ALCHEMY_CALLBACK_SCHEDULER_H
#define ALCHEMY_CALLBACK_SCHEDULER_H

#include "TYPES.H"

struct SchedulerTask {
    u32 callback;
    u16 state;
    u8 mask;
    u8 reserved;
};

s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 order);
s32 Scheduler_RemoveCallback(u32 callback);

#endif
