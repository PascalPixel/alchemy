#include "types.h"

extern u8 Value_0000fff1;
extern u8 Data_02010000;
extern u8 Data_02010082;
extern u8 Data_0200fffa;
extern u8 Value_02010082;

void Func_080d66cc(void)
{
    const volatile u16 *source = (const volatile u16 *)&Data_02010000;
    s32 sentinel = (s32)&Value_0000fff1;
    s32 index = 0;
    volatile u16 *destination = (volatile u16 *)&Data_02010082;
    const u8 *offset = (const u8 *)&Data_0200fffa;

    do {
        if ((u32)(index - 8) <= 127) {
            s32 value = *source - *offset;
            if (value < 0)
                value = 0;
            if (value > 240)
                value = 240;
            *destination = (u16)value;
        } else {
            *destination = sentinel;
        }
        index++;
        destination++;
        offset++;
    } while (index != 160);

    {
        volatile u32 *dma = (volatile u32 *)0x040000B0;
        *(volatile u16 *)((u8 *)dma + 10) &= 0xC5FF;
        *(volatile u16 *)((u8 *)dma + 10) &= 0x7FFF;
        dma[0] = (u32)&Value_02010082;
        dma[1] = 0x04000040;
        dma[2] = 0xA2600001;
    }
}
