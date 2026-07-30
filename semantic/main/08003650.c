#include "types.h"

struct DmaChannel_08003650 {
    const void *source;
    void *destination;
    u32 control;
};

s16 Func_08006088(void *state, void *work);
void Func_0800655c(void);
void Func_080f9018(void);
s32 Func_08003adc(void);
s32 Func_08003a7c(
    const void *source,
    void *destination,
    u32 control,
    volatile struct DmaChannel_08003650 *dma);
void Func_080072ec(s32 context, volatile s32 *pending, s32 callback);
void Func_08004420(s32 sound);
void Func_080006fc(void);

void Func_08003650(void)
{
    volatile struct DmaChannel_08003650 *dma =
        (volatile struct DmaChannel_08003650 *)0x040000d4;
    volatile u16 *dma1_control = (volatile u16 *)0x040000ba;
    s32 context;
    s32 keys;
    s32 newly_pressed;

    *dma1_control &= 0xc5ff;
    *dma1_control &= 0x7fff;

    if (*(volatile u16 *)0x03001cb0 != 0) {
        *(s16 *)0x03001f64 =
            Func_08006088((void *)0x02002220, (void *)0x02002020);
        Func_0800655c();
    }

    Func_080f9018();
    context = Func_08003adc();
    if (*(u8 *)0x03001e44 != 0) {
        if (*(u8 *)0x03001d18 != 0) {
            dma->source = *(const void **)0x03001f20;
            dma->destination = (void *)0x07000000;
            dma->control = 0x84000100;
        }
        dma->source = (const void *)0x03001ad0;
        dma->destination = (void *)0x04000010;
        dma->control = 0x84000004;
        context = Func_08003a7c(
            (const void *)0x03001ad0,
            (void *)0x04000010,
            0x84000004,
            dma);
        *(u8 *)0x03001e44 = 0;
    }

    if (*(volatile s32 *)0x03001cfc != 0) {
        s32 callback = *(volatile s32 *)0x03001cfc;

        *(volatile s32 *)0x03001cfc = 0;
        Func_080072ec(context, (volatile s32 *)0x03001cfc, callback);
    }

    Func_08004420(0x480);
    keys = 0x3ff ^ *(volatile u16 *)0x04000130;
    newly_pressed = keys & ~*(s32 *)0x03001ae8;
    *(s32 *)0x03001c94 = newly_pressed;
    *(s32 *)0x03001af8 |= newly_pressed;
    *(s32 *)0x03001ae8 = keys;

    if (keys == 0) {
        *(s32 *)0x03001b00 = 0x13;
        *(s32 *)0x03001b04 = 0;
    } else if (keys & (*(s32 *)0x03001d0c ^ 0xffff)) {
        *(s32 *)0x03001b00 = -1;
        *(s32 *)0x03001b04 = keys;
    } else if (*(s32 *)0x03001b00 > 0) {
        (*(s32 *)0x03001b00)--;
    }

    *(s32 *)0x03001d0c = keys;
    (*(s32 *)0x03001800)++;
    (*(u16 *)0x03001ccc)++;
    *(u16 *)0x03001d28 = 1;
    Func_080006fc();
}
