#include "types.h"
#include "ram_buffer.h"

void Func_0814cd48(s32 id, s32 a, s32 b, s32 c, s32 d);

void Func_081434f8(void)
{
    u8 *base;
    s32 i;
    s32 index;

    base = Ram_HeapSlots->battle_fx_work[0];
    i = 0;
    do {
        if (base[0x78dc + i] != 0) {
            if ((base[0x78dc + i] = base[0x78dc + i] - 1) == 0) {
                index = i * 2 + 36;
                Func_0814cd48(
                    *(s16 *)(*(u8 **)(base + 0x78f0) + index),
                    0, -1, -1, 0);
            }
        }
        i += 1;
    } while (i != 8);
}
