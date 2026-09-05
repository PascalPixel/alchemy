#include "event_runtime.h"

s32 WaitFrames(s32);

void Event_WaitValue1c8Frames(void)
{
    WaitFrames(Data_03001ebc->value_1c8);
}
