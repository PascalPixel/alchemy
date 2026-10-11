#include "types.h"
#include "ram_buffer.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_020001f8(void);

void Func_02000158(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 0x1ac) = 0x100;
    Scheduler_RemoveCallbackFar(Func_020001f8);
}
