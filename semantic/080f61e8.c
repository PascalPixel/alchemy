#include "types.h"

struct DmaChannel_080f61e8 {
    const void *source;
    void *destination;
    u32 control;
};

const u16 *Func_08002f40(s32 resource_id);

void Func_080f61e8(s32 resource_id)
{
    volatile struct DmaChannel_080f61e8 *dma =
        (volatile struct DmaChannel_080f61e8 *)0x040000d4;
    volatile u16 *palette = (volatile u16 *)0x05000000;
    u16 target[64];
    s32 index;

    dma->source = Func_08002f40(resource_id);
    dma->destination = target;
    dma->control = 0x84000020;

    for (index = 0; index < 64; index++) {
        u16 current = palette[index];
        u16 wanted = target[index];
        s32 red = current & 0x1f;
        s32 green = (current >> 5) & 0x1f;
        s32 blue = (current >> 10) & 0x1f;
        s32 wanted_red = wanted & 0x1f;
        s32 wanted_green = (wanted >> 5) & 0x1f;
        s32 wanted_blue = (wanted >> 10) & 0x1f;

        if (red < wanted_red)
            red++;
        else if (red > wanted_red)
            red--;

        if (green < wanted_green)
            green++;
        else if (green > wanted_green)
            green--;

        if (blue < wanted_blue)
            blue++;
        else if (blue > wanted_blue)
            blue--;

        target[index] = (blue << 10) | (green << 5) | red;
    }

    /*
     * Palette entry zero is transparent and remains untouched.  Transfer the
     * other 63 interpolated colours back to palette memory.
     */
    dma->source = target + 1;
    dma->destination = (void *)0x05000002;
    dma->control = 0x8000003f;
}
