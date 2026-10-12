#include "types.h"
#include "ram_buffer.h"

extern s32 Data_020024b4;

void Func_02000258(void)
{
    s32 *layer = (s32 *)((u8 *)Ram_HeapSlots->map_work + 0x108);
    s32 phase = Data_020024b4;

    layer[2] = phase;
    phase += 0x8000;
    Data_020024b4 = phase;
    if (phase == 0x200000)
        Data_020024b4 = 0;
}
