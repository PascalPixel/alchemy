#include "scene.h"
#include "abi/event/wait_value_1c8_frames.h"
#include "event_runtime.h"

s32 WaitFrames(s32);

void Event_WaitValue1c8Frames(void)
{
    WaitFrames(gWork->value_1c8);
}
