#include "types.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"

void Func_081963ec(s32 slot, s32 size);

void Func_08144aac(s32 mode, void **slots)
{
    /* FAKEMATCH: an unused 4-byte local explains the native 4-byte frame. */
    s32 unused[1];

    if (mode == 0) {
        Func_081963ec(104, 19);
        slots[0] = Ram_HeapSlots->unknown_64[1];
        Func_081963ec(188, 3);
    } else {
        Func_081963ec(104, 23);
        slots[0] = Ram_HeapSlots->unknown_64[1];
        Func_081963ec(188, 7);
    }
    slots[1] = Ram_HeapSlots->unknown_b8[1];
}
