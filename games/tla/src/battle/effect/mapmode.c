#include "types.h"
#include "ram_buffer.h"

void Func_080dc954(void)
{
    struct HeapSlots *slots = Ram_HeapSlots;
    s32 mode = *(s8 *)((u8 *)slots->event_work + 0x18a);

    if (mode == 3)
        *((u8 *)slots->map_work + 0x970) = mode;
}
