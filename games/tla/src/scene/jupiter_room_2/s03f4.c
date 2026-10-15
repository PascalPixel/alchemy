#include "types.h"
#include "ram_buffer.h"

struct EventWork {
    u8 unk000[0x16a];
    u16 unk16a;
};

void Func_020003f4(void)
{
    struct EventWork *event = Ram_HeapSlots->eventWork;
    u8 *effect = Ram_HeapSlots->effectWork;

    event->unk16a = 30;
    effect[52] = 1;
}

void Func_02000410(void)
{
    struct EventWork *event = Ram_HeapSlots->eventWork;
    u8 *effect = Ram_HeapSlots->effectWork;

    event->unk16a = 31;
    effect[52] = 1;
}
