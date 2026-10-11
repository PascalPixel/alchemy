#include "ov6ae.h"

void Func_02005010(void)
{
    *(s32 *)((u8 *)Ram_HeapSlots->event_work + 428) = 256;
    Scheduler_RemoveCallbackFar(Func_020050b0);
}
