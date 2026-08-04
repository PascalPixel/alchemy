#include "types.h"

struct Dma_080170c4 {
    const void *source;
    void *destination;
    u32 control;
};

u16 *Func_080170c4(u16 *destination, u16 value, s32 count)
{
    u16 fill;

    if (count > 0) {
        struct Dma_080170c4 *dma = (struct Dma_080170c4 *)0x040000D4;
        fill = value;
        dma->source = &fill;
        dma->destination = destination;
        dma->control = 0x81000000 | count;
        destination += count;
    }
    return destination;
}
