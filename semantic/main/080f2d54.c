#include "types.h"

struct DmaTransfer_080f2d54 {
    const void *source;
    void *destination;
    u32 control;
};

const void *Func_08002f40(s32 resource);
u32 Func_08005340(const void *source, void *destination);
void Func_080040e8(void);
void Func_08003b70(s32 duration);
void Func_08004760(void);
void Func_080030f8(s32 frames);
void Func_0800479c(void);
void Func_08003c3c(s32 duration);
void Func_08003ce0(void);

s32 Func_080f2d54(void)
{
    volatile struct DmaTransfer_080f2d54 *dma =
        (volatile struct DmaTransfer_080f2d54 *)0x040000d4;
    u8 *decoded = (u8 *)0x02010000;
    u8 *dynamicTiles;
    volatile u16 *scroll = (volatile u16 *)0x03001ad0;
    s32 index;
    u32 waits;

    *(volatile u8 *)0x03001d18 = 1;
    Func_080040e8();
    Func_08003b70(1);
    Func_08004760();
    Func_080030f8(1);

    *(volatile u16 *)0x0400000c = 0x18;
    *(volatile u16 *)0x04000000 = 0x685;
    scroll[5] = 0;

    Func_08005340(Func_08002f40(0x19), decoded);

    dma->source = decoded;
    dma->destination = (void *)0x05000000;
    dma->control = 0x84000070;

    dma->source = decoded + 0x1c0;
    dma->destination = (void *)0x06003000;
    dma->control = 0x84000200;

    dma->source = decoded + 0x9c0;
    dma->destination = (void *)0x06004000;
    dma->control = 0x84001000;
    dynamicTiles = decoded + 0x49c0;

    for (index = 0; index < 4; index++) {
        scroll[index * 2] = 0;
        scroll[index * 2 + 1] = 0;
    }

    dma->source = (const void *)0x03001ad0;
    dma->destination = (void *)0x04000010;
    dma->control = 0x84000004;

    Func_0800479c();
    Func_08004760();
    Func_08003c3c(1);
    Func_08003ce0();
    *(volatile u16 *)0x04000000 = 0x1540;

    waits = 0;
    for (;;) {
        u32 frame =
            ((*(volatile u32 *)0x03001e40 >> 3) & 3) << 10;

        dma->source = dynamicTiles + frame;
        dma->destination = (void *)0x06004100;
        dma->control = 0x840000d0;

        if ((*(volatile u32 *)0x03001c94 & 9) != 0)
            break;

        Func_080030f8(1);
        waits++;
        if (waits > 119)
            break;
    }

    return 0;
}
