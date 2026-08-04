#include "types.h"

void *Func_08002f40(s32);
void Func_080030f8(s32);
void Func_08003b70(s32);
void Func_08003bb4(s32);
void Func_08003ce0(void);
void Func_080040e8(void);
void Func_08004760(void);
void Func_0800479c(void);
void Func_08005340(const void *, void *);
void Func_080f9010(s32);

static s32 InputPressed(void)
{
    return *(volatile u32 *)0x03001C94 & 9;
}

s32 Func_080f2b70(s32 mode)
{
    volatile u32 *dma;
    volatile u16 *tilemap;
    volatile u16 *pairs;
    u8 *resource;
    u16 tile;
    u32 row;
    u32 column;
    u32 frames;
    s32 pressed;
    s32 result;

    result = 0;
    Func_080f9010(0x6E);
    *(volatile u8 *)0x03001D18 = 1;
    Func_080040e8();
    Func_08003b70(1);
    Func_08004760();
    Func_080030f8(1);

    *(volatile u16 *)0x0400000C = 0x0681;
    *(volatile u16 *)0x04000000 = 0x1440;
    *(volatile u16 *)0x03001ADA = 0;

    resource = Func_08002f40(0x18);
    dma = (volatile u32 *)0x040000D4;
    dma[0] = (u32)resource;
    dma[1] = 0x05000000;
    dma[2] = 0x84000070;

    Func_08005340(resource + 0x1C0, (void *)0x02010000);
    dma[0] = 0x02010000;
    dma[1] = 0x06004000;
    dma[2] = 0x84002580;

    tilemap = (volatile u16 *)0x06003000;
    tile = 0x0100;
    for (row = 0; row < 20; row++) {
        for (column = 0; column < 30; column++)
            *tilemap++ = tile++;
        *tilemap++ = 0x01FF;
        *tilemap++ = 0x01FF;
    }

    pairs = (volatile u16 *)0x03001AD0;
    for (row = 0; row < 4; row++) {
        *pairs++ = 0;
        *pairs++ = 0;
    }

    dma[0] = 0x03001AD0;
    dma[1] = 0x04000010;
    dma[2] = 0x84000004;
    Func_0800479c();
    Func_08004760();
    *(volatile u16 *)0x04000000 = 0x1540;

    if (mode == 0) {
        Func_08003bb4(1);
        Func_08003ce0();

        pressed = InputPressed();
        frames = 0;
        while (!pressed) {
            frames++;
            Func_080030f8(1);
            if (frames > 119)
                break;
            pressed = InputPressed();
        }
        if (pressed)
            result = -1;
    } else {
        pressed = InputPressed();
        frames = 0;
        while (!pressed) {
            frames++;
            Func_080030f8(1);
            if (frames > 59)
                break;
            pressed = InputPressed();
        }
        if (pressed)
            result = -1;

        if (result != 0)
            Func_08003bb4(8);
        else
            Func_08003bb4(60);
        Func_08003ce0();

        if (result == 0) {
            pressed = InputPressed();
            frames = 0;
            while (!pressed) {
                frames++;
                Func_080030f8(1);
                if (frames > 179)
                    break;
                pressed = InputPressed();
            }
            if (pressed)
                result = -1;
        }

        if (result != 0)
            Func_08003b70(8);
        else
            Func_08003b70(60);
        Func_08003ce0();
    }

    return result;
}
