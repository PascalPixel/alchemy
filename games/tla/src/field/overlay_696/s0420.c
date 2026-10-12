#include "ov696.h"

void Func_02000420(void)
{
    s32 *view = (s32 *)((u8 *)Ram_HeapSlots->map_work + 376);

    view[2] -= 0x20000;
    do { } while (0); /* FAKEMATCH: the loop note ends the scheduling block: the native loads the second word before it builds its step (2 differ without it). */
    view[3] += 0x20000;
    if (view[3] > 0x4100000) {
        view[2] = 0;
        view[3] = 0x4000000;
    }
}
