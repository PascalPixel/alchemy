#include "types.h"
#include "ram_buffer.h"

struct EventWork {
    u8 unknown_000[0x16a];
    u16 unknown_16a;
};

void Func_020003f4(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    u8 *effect = Ram_HeapSlots->effect_work;

    event->unknown_16a = 30;
    effect[52] = 1;
}

void Func_02000410(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;
    u8 *effect = Ram_HeapSlots->effect_work;

    event->unknown_16a = 31;
    effect[52] = 1;
}
