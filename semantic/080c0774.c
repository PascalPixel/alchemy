#include "types.h"

struct DmaChannel_080c0774 {
    const void *source;
    void *destination;
    u32 control;
};

struct DeferredWrite_080c0774 {
    u32 value;
    u32 address;
    u32 mask;
};

void Func_080041d8(s32 message, s32 code);
void Func_080c0098(void *destination);
void Func_080c00d8(void *destination);

void Func_080c0774(s32 mode, s32 unused, s32 fade)
{
    u8 *state = *(u8 **)0x03001f00;
    volatile struct DmaChannel_080c0774 *dma =
        (volatile struct DmaChannel_080c0774 *)0x040000d4;
    s32 index;

    (void)unused;
    if (*(s32 *)(state + 8) == 0)
        Func_080041d8(0x080c0131, 0x4ff);
    *(s32 *)(state + 8) = mode;

    if (mode == 1) {
        volatile u16 *interrupt_master = (volatile u16 *)0x04000208;
        u16 saved = *interrupt_master;
        volatile u16 *count = (volatile u16 *)0x02002090;

        *interrupt_master = (u16)(u32)interrupt_master;
        if (*count <= 31) {
            struct DeferredWrite_080c0774 *write =
                (struct DeferredWrite_080c0774 *)
                    (0x02002090 + *count * sizeof(*write));

            (*count)++;
            write->value = 0x1f83;
            write->address = 0x0400000a;
            write->mask = 0x00020000;
        }
        *interrupt_master = saved;
    }

    dma->source = (const void *)0x05000200;
    dma->destination = (void *)0x050000a0;
    dma->control = 0x80000010;
    *(volatile u16 *)0x050000bc = *(volatile u16 *)0x050001e8;

    if (fade == 128) {
        dma->source = state + 0x544;
        dma->destination = (void *)0x050000c0;
        dma->control = 0x80000080;
    } else if (fade != 0) {
        const u16 *source = (const u16 *)(state + 0x544);
        volatile u16 *destination = (volatile u16 *)0x050000c0;

        for (index = 0; index < 128; index++) {
            u16 color = source[index];
            s32 red = color & 0x1f;
            s32 green = (color >> 5) & 0x1f;
            s32 blue = (color >> 10) & 0x1f;

            red = red > fade ? red - fade : 0;
            green = green > fade ? green - fade : 0;
            blue = blue > fade ? blue - fade : 0;
            destination[index] = (blue << 10) | (green << 5) | red;
        }
    }

    Func_080c0098((void *)0x06003800);
    Func_080c00d8((void *)0x0600f800);
}
