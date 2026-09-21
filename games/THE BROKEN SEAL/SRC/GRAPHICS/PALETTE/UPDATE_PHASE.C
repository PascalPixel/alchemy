#include "TYPES.H"

typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

static __inline__ void CopyPalette(WordCopy copy, void *destination,
                                   const void *source, s32 size)
{
    copy(destination, source, size);
}

s32 Func_08002322(s32 angle);
#define Engine_MathSin Func_08002322

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
    red_offset = (Engine_MathSin(phase + red_phase) * 16) >> 15;
    green_offset = (Engine_MathSin(phase + green_phase) * 16) >> 15;
    blue_offset = (Engine_MathSin(phase + blue_phase) * 16) >> 15;

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

    copy = (WordCopy)0x03001388;
    CopyPalette(copy, (void *)0x05000002, palette, sizeof(palette));
}
