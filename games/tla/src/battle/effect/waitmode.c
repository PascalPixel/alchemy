#include "types.h"
#include "input.h"
#include "ram_buffer.h"

extern u8 gDebugMode;

struct EventWaitWork {
    u8 unknown_000[0x1b8];
    s32 wait_mode;
};

void EventRuntime_UpdateWaitMode(void)
{
    struct EventWaitWork *work = Ram_HeapSlots->event_work;

    if (gDebugMode != 0) {
        if (((struct InputState *)&gInput)->pressed & 0x200) {
            work->wait_mode = 0;
        }
        if (((volatile struct InputState *)&gInput)->pressed & 0x100) {
            work->wait_mode = -1;
        }
    }
}
