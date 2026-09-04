#include "event_runtime.h"

#define Event_WaitValue1c8Frames Func_08091e20

s32 WaitFrames(s32);

void Event_WaitValue1c8Frames(void)
{
    WaitFrames(Data_03001ebc->value_1c8);
}
