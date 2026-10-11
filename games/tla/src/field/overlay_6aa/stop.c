#include "types.h"
#include "ram_buffer.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_020047b8(void);

void Func_02004718(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 256;
    Scheduler_RemoveCallbackFar(Func_020047b8);
}
