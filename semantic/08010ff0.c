#include "types.h"

struct Runtime_08010ff0 {
    u8 *graphics;
    u8 *state;
};

void Func_08010ff0(void)
{
    struct Runtime_08010ff0 *runtime = (struct Runtime_08010ff0 *)0x03001E6C;
    u8 *graphics = runtime->graphics + 0xC80;
    u16 *state = (u16 *)runtime->state;
    volatile u16 *dmaControl = (volatile u16 *)0x040000BA;
    u16 displayMode;

    *dmaControl &= 0xC5FF;
    *dmaControl &= 0x7FFF;

    if (graphics != 0) {
        u32 *source = (u32 *)(graphics +
            ((*(u32 *)0x03001E40 & 1) * 0x1400));
        volatile u32 *registers = (volatile u32 *)0x04000020;
        volatile u32 *dma = (volatile u32 *)0x040000B0;

        registers[0] = source[0];
        registers[1] = source[1];
        registers[2] = source[2];
        registers[3] = source[3];
        registers[4] = source[4];
        registers[5] = source[5];
        registers[6] = source[6];
        registers[7] = source[7];
        dma[0] = (u32)(source + 8);
        dma[1] = 0x04000020;
        dma[2] = 0xA6600008;
    }

    state[0x104 / 2] = state[0x100 / 2];
    state[0x106 / 2] = state[0x102 / 2];

    displayMode = 0;
    if (state[0x104 / 2] <= 199) {
        displayMode = state[0x102 / 2] != 0 ? 2 : 0;
        if (state[0x104 / 2] <= state[0x102 / 2]) {
            displayMode = state[0x104 / 2] == 0 ? 2 : 0;
        }
    }
    *(volatile u16 *)0x04000000 =
        (*(volatile u16 *)0x04000000 & 0xFFF8) | displayMode;
    state[0x108 / 2] = 0;
}
