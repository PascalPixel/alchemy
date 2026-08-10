#include "types.h"

#define M2C_FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct DmaTransfer_08010ff0 {
    u32 source;
    u32 destination;
    u32 control;
};

void Func_08010ff0(void)
{
    u8 **runtime = (u8 **)0x03001E6C;
    u8 *graphics;
    u16 *state;
    s16 display;
    u16 displayMode;
    void *temp_dma;

    graphics = *runtime++ + 0xC80;
    display = *(volatile u16 *)0x04000000;
    state = (u16 *)*runtime;
    display &= 0xFFF8;

    temp_dma = (void *)0x040000B0;
    M2C_FIELD(temp_dma, volatile u16 *, 0xA) =
        (u16)(0xC5FF & M2C_FIELD(temp_dma, volatile u16 *, 0xA));
    M2C_FIELD(temp_dma, volatile u16 *, 0xA) =
        (u16)(0x7FFF & M2C_FIELD(temp_dma, volatile u16 *, 0xA));
    (void)M2C_FIELD(temp_dma, volatile u16 *, 0xA);

    if (graphics != 0) {
        u32 *source = (u32 *)(graphics +
            ((*(u32 *)0x03001E40 & 1) * 0x1400));
        volatile u32 *registers = (volatile u32 *)0x04000020;
        volatile struct DmaTransfer_08010ff0 *dma =
            (volatile struct DmaTransfer_08010ff0 *)0x040000B0;

        *registers = *source++;
        registers++;
        *registers++ = *source++;
        *registers++ = *source++;
        *registers++ = *source++;
        *registers++ = *source++;
        *registers++ = *source++;
        *registers++ = *source++;
        *registers = *source++;
        dma->source = (u32)source;
        dma->destination = 0x04000020;
        dma->control = 0xA6600008;
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
    *(volatile u16 *)0x04000000 = display | displayMode;
    state[0x108 / 2] = 0;
}
