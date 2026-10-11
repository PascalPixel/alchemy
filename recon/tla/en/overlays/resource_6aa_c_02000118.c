/* Draft of overlay 6aa Func_02000118, the twin of overlay 6ab's
 * Func_02000060 (mark.c). differ 2 of 10: the listing loads the event work
 * before it builds the offset 0xd38; this draft starts the offset first.
 * The 6ab twin holds the load first with an asm barrier. */
#include "types.h"
#include "ram_buffer.h"

struct EventWork {
    u8 unknown_000[0xd38];
    u8 unknown_d38;
};

void Func_02000118(void)
{
    struct EventWork *event = Ram_HeapSlots->event_work;

    event->unknown_d38 = 1;
}
