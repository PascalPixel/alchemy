#include "types.h"
#include "ram_buffer.h"

s32 ColorBuffer_ScaleThreeQuarters(void *source, void *destination, s32 size);

s32 Func_08127038(void)
{
    void *source = Ram_HeapSlots->battle_fx_work[1];
    u8 *context = Ram_HeapSlots->battle_fx_work[0];

    if (source != 0) {
        u32 *active = (u32 *)(context + 0x13c0);

        if (*active != 0) {
            *active = 0;
            return ColorBuffer_ScaleThreeQuarters(source, (void *)0x06004000, 0x4000);
        }
    }
    return (s32)source;
}
