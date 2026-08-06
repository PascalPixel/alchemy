#include "types.h"

/*
 * Build the five-entry display strip for the current overlay palette.  A
 * two-step phase controls DMA initialization, palette-channel modulation,
 * sparse-buffer carry-forward, and the final display-command emission.
 */









static s32 ClampChannel(s32 value)
{
    return (u32)value <= 31 ? value : 31;
}

extern s32 Func_02003624(s32 flag);
extern void Func_02003606(s32 slot);
extern u8 * Func_0200363e(s32 bytes);
extern s32 Func_02003660(s32 value, s32 divisor);
extern s32 Func_02003688(s32 value, s32 divisor);
extern s32 Func_020036aa(s32 value, s32 divisor);
extern void Func_02003752(void *destination, u32 value);
extern void Func_0200375e(void *destination, u32 value);
extern void Func_02003766(void *destination, u32 value);
extern s32 Func_02003724(s32 value, s32 divisor);
extern void Func_0200384a(s32 slot, s32 bytes, void *source);
extern void Func_020038a4(void *command, s32 opacity);
extern void Func_02003896(void *buffer);
void Func_020018a4(void)
{
    s16 *selected_slot = (s16 *)0x0200a6d0;
    s16 *phase = (s16 *)0x0200a6be;
    s16 *pulse = (s16 *)0x0200a6bc;
    s16 *decay = (s16 *)0x0200a6c0;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u16 tile = *(u16 *)(0x03001b10 + *selected_slot * 4 + 2) >> 5;
    s16 *active = (s16 *)0x0200b030;
    u8 *work;
    u32 index;

    if (*active != 0) {
    } else if (Func_02003624(0x104) != 0) {
        *phase = 2;
        if (*phase > 0)
            *phase = (s16)(*phase - 1);
    } else if (*phase <= 1) {
        *phase = (s16)(*phase + 1);
        if (*phase == 1) {
            dma3[0] = 0x02009f80;
            dma3[1] = 0x050003c0;
            dma3[2] = 0x80000010;
        }
    }

    if (*phase == 0) {
        Func_02003606(*selected_slot);
        return;
    }

    {
        u8 *object_table = *(u8 **)0x03001ecc;
        if (object_table != 0) {
            u8 selector = object_table[0x539];
            u8 *entry = object_table + selector * 644 + 38;
            u16 value = (u16)(*phase << 3);

            for (index = 0; index <= 143; index++)
                *(u16 *)(entry + index * 4) = value;
        }
    }

    work = Func_0200363e(0x900);
    dma3[2] = 0x80000010;
    dma3[0] = 0x02009f80;
    dma3[1] = (u32)work;

    for (index = 0; index <= 11; index++) {
        u16 color = *(u16 *)(work + 12 + index * 2);
        s32 red = color & 31;
        s32 green = (color >> 5) & 31;
        s32 blue = (color >> 10) & 31;

        red += Func_02003660(*pulse, 3);
        blue -= Func_02003688(*pulse, 6);
        if (*pulse > 60 && (*(volatile u32 *)0x03001e40 & 1) != 0)
            green += Func_020036aa(*pulse << 6, 120) - 32;

        red = ClampChannel(red);
        green = ClampChannel(green);
        blue = ClampChannel(blue);
        *(u16 *)(work + 12 + index * 2) =
            (u16)((blue << 10) | (green << 5) | red);
    }

    Func_02003752((void *)0x050003cc, *(u32 *)(work + 12));
    Func_0200375e((void *)0x050003d0, *(u32 *)(work + 16));
    Func_02003766((void *)0x050003d4, *(u32 *)(work + 20));

    *pulse = (s16)Func_02003724(
        *(s16 *)(0x02000240 + 0x232) * 120,
        *(s16 *)(0x02000240 + 0x22c));

    if (*pulse > 119)
        *decay = 119;
    if (*decay != 0) {
        *pulse = *decay;
        *decay = (s16)(*decay - 8);
        if (*decay <= 0)
            *decay = 0;
    }

    dma3[1] = (u32)work;
    dma3[2] = 0x84000240;
    dma3[0] = 0x0200a730;

    if (*decay <= 118) {
        s32 limit = 128 - *pulse;
        u8 *cursor = work + 80;

        for (index = 12; index < limit; index++) {
            *(u32 *)(cursor + 32) = 0xeeeeeeee;
            *(u32 *)cursor = 0xeeeeeeee;
            cursor += 4;
            if ((index & 7) == 7)
                cursor += 32;
        }
        *(u32 *)cursor = *(u32 *)work;
        *(u32 *)(cursor + 32) = *(u32 *)(work + 32);
    }

    for (index = 0; index <= 0x47f; index++) {
        u8 value = work[0x480 + index];
        if (value != 0)
            work[index] = value;
    }

    Func_0200384a(*selected_slot, 0x480, work);

    for (index = 0; index <= 4; index++) {
        u32 *command = (u32 *)(0x0200a6e0 + index * 12);
        u32 flags = index == 4 ? 0x80000000 : 0x80008000;
        u32 angle = (u32)((*phase * 8 - 16) & 0x1ff);

        command[0] = 0;
        command[1] = flags | (angle << 16) | (u32)(index * 32 + 8);
        command[2] = 0xe400 | tile;
        Func_020038a4(command, 255);
        tile = (u16)(tile + 8);
    }

    Func_02003896(work);
}
