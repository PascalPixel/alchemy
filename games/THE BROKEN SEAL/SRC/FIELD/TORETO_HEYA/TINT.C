#include "TYPES.H"
#include "FIELD_EVENT.H"

/* FAKEMATCH: array view of distinct linker-adjacent pointer cells preserves
 * the shared address base; it does not establish one source array. */
extern u8 *Data_03001ebc[];
/* Current entry in the RGB tint list; a red of 99 ends the list. */
extern s32 ToretoHeya_TintIndex;
extern s32 ToretoHeya_TintSteps[];

/* Every 32 frames, tint the background palette, weaker for later colours. */
void ToretoPalette_ApplyTint(void)
{
    u16 *src;
    u16 *dst;
    u32 i;
    s32 r;
    s32 g;
    s32 b;
    s32 red;
    s32 green;
    s32 blue;
    u32 color;
    u8 *event;

    event = Data_03001ebc[0];
    src = (u16 *)Data_03001ebc[5];
    if (*(s16 *)(event + 0x17e) != 0)
        return;
    if ((gFrameCount & 31) != 0)
        return;
    src += 16;
    dst = (u16 *)0x05000020;
    i = 0;
    for (; i <= 62; i++, src++) {
        r = ToretoHeya_TintSteps[ToretoHeya_TintIndex];
        g = ToretoHeya_TintSteps[ToretoHeya_TintIndex + 1];
        b = ToretoHeya_TintSteps[ToretoHeya_TintIndex + 2];
        if (i > 47) {
            r -= r / 2 + r / 3;
            g -= g / 2 + g / 3;
            b -= b / 2 + b / 3;
        } else if (i > 31) {
            r -= r / 3 + r / 4;
            g -= g / 3 + g / 4;
            b -= b / 3 + b / 4;
        } else if (i > 15) {
            r -= r / 4 + r / 5;
            g -= g / 4 + g / 5;
            b -= b / 4 + b / 5;
        }
        color = *src;
        red = color & 31;
        green = (color >> 5) & 31;
        blue = (color >> 10) & 31;
        red += r;
        green += g;
        blue += b;
        if (red > 31)
            red = 31;
        if (green > 31)
            green = 31;
        if (blue > 31)
            blue = 31;
        if (red < 0)
            red = 0;
        if (green < 0)
            green = 0;
        if (blue < 0)
            blue = 0;
        *dst++ = (blue << 10) | (green << 5) | red;
    }
    ToretoHeya_TintIndex += (Engine_RandomNext() & 7) * 3;
    if (ToretoHeya_TintSteps[ToretoHeya_TintIndex] == 99)
        ToretoHeya_TintIndex = 0;
}
