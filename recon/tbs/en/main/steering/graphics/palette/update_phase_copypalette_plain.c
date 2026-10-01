/* NONMATCHING: 2026-10-01 brief Wave2 CopyPalette plain-source attempt.
 * Removing this one source device changes Graphics_UpdatePhasePalette.
 * First remaining difference: Graphics_UpdatePhasePalette: mov	r6, r8 => mov	r6, r9 (99/103 assembly lines).
 * Measured with the existing TBS agscc option set, EN edition; no option changes.
 * This reduced draft preserves the affected function and its declarations.
 * Production retains the measured device with its FAKEMATCH reason.
 */
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"

typedef s32 (*WordCopy)(void *destination, const void *source, s32 size);


void Graphics_UpdatePhasePalette(s32 frame, s32 red_phase, s32 green_phase, s32 blue_phase)
{
    u16 palette[64];
    u16 *output;
    s32 phase;
    s32 red_offset;
    s32 green_offset;
    s32 blue_offset;
    s32 index;
    WordCopy copy;

    phase = frame * 0x400;
    red_offset = (Trig_Sin(phase + red_phase) * 16) >> 15;
    green_offset = (Trig_Sin(phase + green_phase) * 16) >> 15;
    blue_offset = (Trig_Sin(phase + blue_phase) * 16) >> 15;

    palette[0] = 0;
    index = 1;
    output = &palette[1];
    do {
        s32 red;
        s32 green;
        s32 blue;

        red = (index + red_offset) / 2;
        green = (index + green_offset) / 2;
        blue = (index + blue_offset) / 2;

        if (red < 0)
            red = 0;
        if (red > 31)
            red = 31;
        if (green < 0)
            green = 0;
        if (green > 31)
            green = 31;
        if (blue < 0)
            blue = 0;
        if (blue > 31)
            blue = 31;

        *output++ = (blue << 10) | (green << 5) | red;
        index++;
    } while (index != 64);

    copy = Iwram_CopyWords;
    copy((void *)0x05000002, palette, sizeof(palette));
}
