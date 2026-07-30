#include "types.h"

struct DmaChannel_080aad10 {
    const void *source;
    void *destination;
    u32 control;
};

void Func_080a10d0(void *state, s32, s32, s32, s32, s32);
void Func_080030f8(u32 frames);
void Func_080153d8(s32 destination);
s32 Func_080045e8(void);
void Func_080aac84(s32, s32, s32, s32);
void Func_080aafb8(s32 state);

void Func_080aad10(void)
{
    u8 *scene = *(u8 **)0x03001f2c;
    s32 graphics = *(s32 *)(scene + 0x184);
    void (*copy)(s32, s32, s32) =
        (void (*)(s32, s32, s32))0x03001388;
    void (*fill)(s32, s32, s32) =
        (void (*)(s32, s32, s32))0x03000168;
    volatile struct DmaChannel_080aad10 *dma =
        (volatile struct DmaChannel_080aad10 *)0x040000d4;
    u16 background_color;

    Func_080a10d0(scene + 0x30, 0, 5, 0x1e, 0x0f, 2);
    Func_080030f8(1);
    copy(graphics + 0xa8, 0x06004000, 0x2000);
    copy(graphics + 0x20a8, 0x05000080, 0x80);
    fill(0x06004000, 0x2000, 0x33333333);
    fill(0x05000080, 0x80, 0x55555555);
    Func_080153d8(0x06005000);
    copy(0x060052c0, 0x080af26c, 0x20);

    dma->source = (const void *)Func_080045e8();
    dma->destination = (void *)0x050000a0;
    dma->control = 0x80000010;
    background_color = *(volatile u16 *)0x050001e8;
    *(volatile u16 *)0x050000bc = background_color;

    dma->source = (const void *)0x050001e0;
    dma->destination = (void *)0x050000e0;
    dma->control = 0x80000010;
    Func_080aac84(8, 0x050000e0, 0x80000010, 0x040000d4);
    *(volatile u16 *)0x050000e8 = background_color;
    *(volatile u16 *)0x050000c8 = background_color;
    Func_080aafb8(graphics);
}
