#include "types.h"
#include "ram_buffer.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_0200099c(void);

void Func_020008fc(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 256;
    Scheduler_RemoveCallbackFar(Func_0200099c);
}
