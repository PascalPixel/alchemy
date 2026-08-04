#include "types.h"

extern s32 Data_0200adb8;
extern s32 Data_02009f00[];
extern s32 Func_03000380(s32 value, s32 divisor);
extern u32 Func_080000f8(void);

/* Fade a captured RGB555 palette through the current three-channel profile. */
void Func_02001244(void)
{
    u8 *workspace = *(u8 **)0x03001ebc;
    u16 *captured = *(u16 **)0x03001ed0 + 16;
    volatile u16 *palette = (volatile u16 *)0x05000020;
    s32 profile = Data_0200adb8;
    s32 redDelta = Data_02009f00[profile];
    s32 greenDelta = Data_02009f00[profile + 1];
    s32 blueDelta = Data_02009f00[profile + 2];
    s32 index;

    if (*(s16 *)(workspace + 382) != 0 || (*(volatile u32 *)0x03001e40 & 31) != 0)
        return;

    for (index = 0; index <= 62; index++) {
        s32 red;
        s32 green;
        s32 blue;
        u16 color;

        if (index > 47) {
            redDelta -= redDelta / 2 + Func_03000380(redDelta, 3);
            greenDelta -= greenDelta / 2 + Func_03000380(greenDelta, 3);
            blueDelta -= blueDelta / 2 + Func_03000380(blueDelta, 3);
        } else if (index > 31) {
            redDelta -= redDelta / 4 + Func_03000380(redDelta, 3);
            greenDelta -= greenDelta / 4 + Func_03000380(greenDelta, 3);
            blueDelta -= blueDelta / 4 + Func_03000380(blueDelta, 3);
        } else if (index > 15) {
            redDelta -= redDelta / 4 + Func_03000380(redDelta, 5);
            greenDelta -= greenDelta / 4 + Func_03000380(greenDelta, 5);
            blueDelta -= blueDelta / 4 + Func_03000380(blueDelta, 5);
        }

        color = captured[index];
        red = (color & 31) + redDelta;
        green = ((color >> 5) & 31) + greenDelta;
        blue = ((color >> 10) & 31) + blueDelta;
        if (red > 31) red = 31;
        if (green > 31) green = 31;
        if (blue > 31) blue = 31;
        if (red < 0) red = 0;
        if (green < 0) green = 0;
        if (blue < 0) blue = 0;
        palette[index] = (u16)(red | (green << 5) | (blue << 10));
    }

    Data_0200adb8 += (Func_080000f8() & 7) * 3;
    if (Data_02009f00[Data_0200adb8] == 99)
        Data_0200adb8 = 0;
}
