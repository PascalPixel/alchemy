#include "types.h"
#include "ram_buffer.h"

s32 Func_02000280(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x100;
    return 0;
}
