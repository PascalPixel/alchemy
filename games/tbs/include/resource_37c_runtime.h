#ifndef ALCHEMY_RESOURCE_37C_RUNTIME_H
#define ALCHEMY_RESOURCE_37C_RUNTIME_H

#include "layout_guard.h"
extern struct EventWork *gEventWork;

struct Resource37cRuntime
{
    u8  unk000[0x1c0];
    s32 setupRequest1c0;
};

LAYOUT_OFFSET_GUARD(
    Resource37cRuntime_SetupRequestOffset, struct Resource37cRuntime, setupRequest1c0, 0x1c0);

#define RESOURCE37C_RUNTIME ((struct Resource37cRuntime *)*(void **)&gEventWork)

void Resource37c_QueueSoundCue(s32);
void Resource37c_FinalizeSetup(void);

#endif
