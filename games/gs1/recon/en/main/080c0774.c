#include "types.h"

#define BattlePresentation_ConfigurePaletteFade Func_080c0774

struct DmaChannel3 {
    const void *source;
    void *destination;
    u32 control;
};

void BattlePresentation_ConfigurePaletteFade(s32 mode, u16 value, s32 fade)
{
    s32 *transition = *(s32 **)0x03001f00;
    volatile struct DmaChannel3 *dma =
        (volatile struct DmaChannel3 *)0x040000d4;
    u8 *battle;

    (void)value;
    if (transition[2] == 0) {
        Func_080041d8(0x080c0131, 0x4ff);
    }
    transition[2] = mode;

    if (mode == 1) {
        u16 interrupt_enable = *(u16 *)0x04000208;
        u16 *event_count = (u16 *)0x02002090;

        *(u16 *)0x04000208 = 0x0208;
        if (*event_count <= 31) {
            u32 *event = (u32 *)((u8 *)event_count + *event_count * 12 + 4);

            (*event_count)++;
            event[0] = 0x1f83;
            event[1] = 0x0400000a;
            event[2] = 0x20000;
        }
        *(u16 *)0x04000208 = interrupt_enable;
    }

    dma->source = (void *)0x05000200;
    dma->destination = (void *)0x050000a0;
    dma->control = 0x80000010;
    *(u16 *)0x050000bc = *(u16 *)0x050001e8;

    if (fade == 0x80) {
        battle = *(u8 **)0x03001e74;
        dma->source = battle + 0x544;
        dma->destination = (void *)0x050000c0;
        dma->control = 0x80000080;
    } else if (fade != 0) {
        u16 *source;
        u16 *destination = (u16 *)0x050000c0;
        s32 index;

        battle = *(u8 **)0x03001e74;
        source = (u16 *)(battle + 0x544);
        for (index = 0; index < 128; index++) {
            u16 color = source[index];
            s32 red = color & 31;
            s32 green = (color >> 5) & 31;
            s32 blue = (color >> 10) & 31;

            red = red > fade ? red - fade : 0;
            green = green > fade ? green - fade : 0;
            blue = blue > fade ? blue - fade : 0;
            destination[index] = (blue << 10) | (green << 5) | red;
        }
    }

    Func_080c0098((void *)0x06003800);
    Func_080c00d8((void *)0x0600f800);
}
