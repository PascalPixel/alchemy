#include "local.h"
#include "event_work.h"

void Func_0200173c(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    do {
    } while (0);
    if (work->action_counter != 0) {
        Flag_Set(0x106);
        Func_02002118();
        Func_02002148();
        Flag_Clear(0x106);
    }
}
