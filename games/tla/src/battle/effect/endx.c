#include "types.h"
#include "ram_buffer.h"
#include "resource.h"
#include "callback_scheduler.h"

void Func_080e2c38(void);

void BattleFx_EndX(void)
{
    u8 *work = Ram_HeapSlots->battleFxWork[0];

    /* FAKEMATCH: the loop note ends the scheduling block: the native loads the effect work before the callback address (2 differ without it). */
    do
    {
    } while (0);
    Scheduler_RemoveCallback((u32)Func_080e2c38);
    Resource_ResetEntry(*(s16 *)(work + 0x1b00));
}
