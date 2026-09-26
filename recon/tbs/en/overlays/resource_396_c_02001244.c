/* NONMATCHING: complete 480-byte owner, candidate 464, 222 differing
 * halfwords, 175 aligned edits (2026-09-26). Reproducible single-overlay
 * unit binds the two imports and four data symbols. ENTER_ROOM installs
 * this palette callback at runtime address 0x02009245.
 * Remaining: hand-written from the disassembly; the goto loop keeps the clamp
 * zero and 31 unhoisted as the reference does. The reference allocates the
 * red tint to r4 with caller-saves around each divide and holds the red
 * half-sum (r5) across the first divide, i.e. evaluates red/2 before the
 * call; here preexpand_calls puts the divide first for all three and the
 * tints take r5-r7, so 16 bytes are missing. A half temporary before the call
 * reaches 480 bytes but takes a fourth high register.
 * Bounded negative result: sharing the red output local as the attenuation
 * accumulator gave 488 bytes, 239 differing halfwords and 180 edits. It
 * kept the partial sums in r5 but introduced fp, moved blue to r8 and kept
 * the shifts before the divide. The better direct-expression draft stays;
 * the next hypothesis must explain the unshifted numerator across calls. */
#include "TYPES.H"
#include "FIELD_EVENT.H"


extern u8 *Data_03001ebc[];
extern u32 Data_03001e40;
/* Index of the current tint in Data_02009f00. */
extern s32 Data_0200adb8;
/* Red, green and blue tints in threes; a red of 99 ends the list. */
extern s32 Data_02009f00[];

/* Every 32 frames, tints the background palette from the next random entry
 * of the tint list, weaker for the later colours. */
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
    u16 color;

    src = (u16 *)Data_03001ebc[5];
    if (*(s16 *)(Data_03001ebc[0] + 0x17e) != 0)
        return;
    if ((Data_03001e40 & 31) != 0)
        return;
    src += 16;
    dst = (u16 *)0x05000020;
    i = 0;
loop:
    {
        r = Data_02009f00[Data_0200adb8];
        g = Data_02009f00[Data_0200adb8 + 1];
        b = Data_02009f00[Data_0200adb8 + 2];
        if (i > 47) {
            r -= r / 2 + Engine_MathDivide(r, 3);
            g -= g / 2 + Engine_MathDivide(g, 3);
            b -= b / 2 + Engine_MathDivide(b, 3);
        } else if (i > 31) {
            r -= Engine_MathDivide(r, 3) + r / 4;
            g -= Engine_MathDivide(g, 3) + g / 4;
            b -= Engine_MathDivide(b, 3) + b / 4;
        } else if (i > 15) {
            r -= r / 4 + Engine_MathDivide(r, 5);
            g -= g / 4 + Engine_MathDivide(g, 5);
            b -= b / 4 + Engine_MathDivide(b, 5);
        }
        color = *src;
        red = (color & 31) + r;
        green = ((color >> 5) & 31) + g;
        blue = ((color >> 10) & 31) + b;
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
        *dst = (blue << 10) | (green << 5) | red;
        i++;
        dst++;
        src++;
    }
    if (i <= 62)
        goto loop;
    Data_0200adb8 += (Engine_RandomNext() & 7) * 3;
    if (Data_02009f00[Data_0200adb8] == 99)
        Data_0200adb8 = 0;
}
