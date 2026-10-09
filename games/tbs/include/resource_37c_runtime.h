#ifndef ALCHEMY_RESOURCE_37C_RUNTIME_H
#define ALCHEMY_RESOURCE_37C_RUNTIME_H

#include "layout_guard.h"
extern struct EventWork *gEventWork;

struct Resource37cRuntime
{
	u8	unknown_000[0x1c0];
	s32 setup_request_1c0;
};

LAYOUT_OFFSET_GUARD(
	Resource37cRuntime_SetupRequestOffset, struct Resource37cRuntime, setup_request_1c0, 0x1c0);

#define RESOURCE37C_RUNTIME ((struct Resource37cRuntime *)*(void **)&gEventWork)

s32	 Resource37c_TestSetupGate(s32);
void Resource37c_SetSetupFlag(s32);
void Resource37c_SetViewScale(s32, s32, s32);
void Resource37c_QueueSoundCue(s32);
void Resource37c_FinalizeSetup(void);

#endif
