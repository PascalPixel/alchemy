#ifndef ALCHEMY_DISPTRAN_H
#define ALCHEMY_DISPTRAN_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

/* Palette wipes and scanline splits share the same frame interpolation. */
struct DisplayTransitionFrame {
    u8 unknown_000[0x508];
    u8 palette_nibbles[0x22];
    u16 value;
    u8 unknown_52c[13];
    u8 dither_toggle;
    s8 start;
    s8 end;
    s8 duration;
    s8 step;
};

struct DisplayTransitionWindow {
    u8 unknown_000[0x100];
    u16 first_line;
    u16 second_line;
};

LAYOUT_OFFSET_GUARD(DisplayTransitionFrame_Value, struct DisplayTransitionFrame, value, 0x52a);
LAYOUT_OFFSET_GUARD(DisplayTransitionFrame_Start, struct DisplayTransitionFrame, start, 0x53a);
LAYOUT_SIZE_GUARD(DisplayTransitionFrame_Size, struct DisplayTransitionFrame, 0x540);
LAYOUT_SIZE_GUARD(DisplayTransitionWindow_Size, struct DisplayTransitionWindow, 0x104);

#endif
