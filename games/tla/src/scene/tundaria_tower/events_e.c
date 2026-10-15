#include "tundaria_tower.h"

void Func_02001868(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    /* FAKEMATCH: a one-pass block loads the event work before its offset is built (one reordered row). */
    do {
    } while (0);
    if (work->action_counter != 0) {
        Call1((void (*)())GameFlag_SetBit, 0x106);
        Func_02003100();
        Func_02003140();
        Call1((void (*)())GameFlag_ClearBit, 0x106);
    }
}
