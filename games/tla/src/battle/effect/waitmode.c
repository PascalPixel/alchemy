#include "types.h"
#include "input.h"
#include "ram_buffer.h"

extern u8 gDebugMode;

struct EventWaitWork
{
    u8  unk000[0x1b8];
    s32 waitMode;
};

void EventRuntime_UpdateWaitMode(void)
{
    struct EventWaitWork *work = Ram_HeapSlots->eventWork;

    if (gDebugMode != 0)
    {
        if (((struct InputState *)&gInput)->pressed & 0x200)
        {
            work->waitMode = 0;
        }
        if (((volatile struct InputState *)&gInput)->pressed & 0x100)
        {
            work->waitMode = -1;
        }
    }
}
