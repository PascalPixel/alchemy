#include "types.h"

/*
 * resource_3bc per-frame chain renderer at 0x020040d0, complete 964-byte
 * owner and eight-word pool.  This is the overlay-local sibling of the
 * independently reviewed resource_3bb:3638 and resource_3ba:33a0 owners.
 */











extern u8 Data_0200cd60[];
extern u8 Data_0200cd80[];

extern s32 Func_02008ad4();
extern s32 Func_020089f4();
extern void Func_02008a0e();
extern void Func_02008a34();
extern void Func_02008a1a();
extern void Func_02008a46();
extern void Func_02008abc();
extern void Func_02008afe();
extern void Func_02008b3e();
extern void Func_02008b6e();
extern void Func_02008bb4();
extern void Func_02008c0a();
extern u8 * Func_02008eca();
extern s32 Func_02008bac();
extern s32 Func_02008bc2();
extern void Func_02008ca6();
extern u8 * Func_02008f52();
extern s32 Func_02008c34();
extern s32 Func_02008c4a();
extern void Func_02008d28();
void Func_020040d0(void)
{
    u8 *workspace;
    u32 *out;
    s32 tile;
    s32 count;
    s32 state;
    s32 column;
    s32 index;
    s32 handle;
    u16 stepped;
    u8 *marker;
    s32 screen_y;
    s32 screen_x;

    count = *(s16 *)(workspace + 230);
    workspace = *(u8 **)0x03001f3c;
    tile = *(u16 *)(0x03001b10 + (*(s16 *)(workspace + 216) << 2) + 2) >> 5;

    if (*(s16 *)(workspace + 220) != 0) {
        *(u16 *)(workspace + 218) = 2;
    } else if (Func_02008ad4(0x106) != 0) {
        if (*(s16 *)(workspace + 218) > 0)
            *(u16 *)(workspace + 218) =
                (u16)(*(u16 *)(workspace + 218) - 1);
    } else if (*(s16 *)(workspace + 218) <= 1) {
        stepped = (u16)(*(u16 *)(workspace + 218) + 1);
        *(u16 *)(workspace + 218) = stepped;

        if ((stepped << 16) == 0x10000) {
            *(volatile u32 *)0x040000d4 = (u32)Data_0200cd60;
            *(volatile u32 *)0x040000d8 = 0x050003c0;
            *(volatile u32 *)0x040000dc = 0x80000010;

            handle = Func_020089f4(0x200);
            Func_02008a0e(Data_0200cd80, handle);
            Func_02008a34(*(s16 *)(workspace + 216), 0x200, handle);
            Func_02008a1a(handle);
        }
    }

    state = *(s16 *)(workspace + 218);
    if (state == 0) {
        Func_02008a46(*(s16 *)(workspace + 216));
        return;
    }

    column = (state * 6 - 8) & 0xff;
    out = (u32 *)workspace;

    out[2] = tile | 0xe400;
    out[0] = 0;
    out[1] = ((104 - (count << 4)) << 16) | column | 0x8000;
    Func_02008abc(out, 255, 12);
    out += 3;

    for (index = 0; (u32)index < (u32)count; index++) {
        out[0] = 0;
        out[1] = ((96 - (index << 4)) << 16) | column | 0x40000000;
        out[2] = (tile + 2) | 0xe400;
        Func_02008afe(out, 255, 12);
        out += 3;
    }

    out[0] = 0;
    out[2] = (tile + 6) | 0xe400;
    Func_02008b3e(out, 255, 12);
    out += 3;
    out[1] = 0x700000 | column | 0x8000;

    out[1] = 0x780000 | column | 0x8000 | 0x10000000;
    out[2] = (tile + 6) | 0xe400;
    Func_02008b6e(out, 255, 12);
    out[0] = 0;
    out += 3;

    for (index = 0; (u32)index < (u32)count; index++) {
        out[0] = 0;
        out[1] = column | (0x800000 + (index << 20)) |
                 0x40000000 | 0x10000000;
        out[2] = (tile + 2) | 0xe400;
        Func_02008bb4(out, 255, 12);
        out += 3;
    }

    out[2] = tile | 0xe400;
    out[0] = 0;
    out[1] = column | (((count << 4) + 128) << 16) |
             0x8000 | 0x10000000;
    Func_02008c0a(out, 255, 12);
    out += 3;

    if ((*(s32 *)0x03001e40 & 15) <= 4)
        return;

    marker = Func_02008eca(*(s16 *)(workspace + 224));
    if (marker != 0) {
        screen_y = Func_02008bac(
            *(s32 *)(marker + 8) - *(s32 *)(workspace + 232), 0xe0000) + 112;
        screen_x = Func_02008bc2(
            *(s32 *)(marker + 16) - *(s32 *)(workspace + 236), 0xe0000);
        screen_x = (screen_x + *(s16 *)(workspace + 218) * 6 - 4) & 0xff;

        out[0] = 0;
        out[1] = screen_x | (screen_y << 16) | 0x40000000;
        out[2] = (tile + 12) | 0xe400;
        Func_02008ca6(out, 255, 12);
        out += 3;
    }

    marker = Func_02008f52(*(s16 *)(workspace + 222));
    if (marker == 0)
        return;

    screen_y = Func_02008c34(
        *(s32 *)(marker + 8) - *(s32 *)(workspace + 232), 0xe0000) + 112;
    screen_x = Func_02008c4a(
        *(s32 *)(marker + 16) - *(s32 *)(workspace + 236), 0xe0000);
    screen_x = (screen_x + *(s16 *)(workspace + 218) * 6 - 4) & 0xff;

    out[0] = 0;
    out[1] = screen_x | (screen_y << 16) | 0x40000000;
    out[2] = (tile + 8) | 0xe400;
    Func_02008d28(out, 255);
}
