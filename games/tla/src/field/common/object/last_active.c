#include "types.h"
#include "ram_buffer.h"
#include "event_work.h"

s32 ObjectTable_FindLastActiveId(void)
{
    struct EventWork *work = Ram_HeapSlots->eventWork;
    s32 last = 7;
    s32 i;

    for (i = 8; i <= 63; i++) {
        if (work->objects[i] != NULL)
            last = i;
    }
    last++;
    if (last == 64)
        last = -1;
    return last;
}
