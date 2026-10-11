#include "ov694.h"

void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Func_020001f8(void);

void Func_02000158(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    work->screen_effect = 256;
    Scheduler_RemoveCallbackFar(Func_020001f8);
}
