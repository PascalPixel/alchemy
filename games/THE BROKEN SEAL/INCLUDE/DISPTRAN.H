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

/* The transition's fields in the map work, including the IRQ's latched lines. */
struct DisplayTransitionWindow {
    u32 unknown_000[5];
    u16 dispcnt;
    u8 unknown_016[0xea];
    u16 first_line;
    u16 second_line;
    u16 irq_first_line;
    u16 irq_second_line;
    u16 irq_phase;
};

/* The window/tile transition view of the 0x540-byte block. */
struct DisplayTransitionState {
    u8 data[0x528];
    s16 mode;
    s16 value;
    u8 unknown_52c[8];
    s16 mask;
    s16 active;
    u8 unknown_538[2];
    s8 start;
    s8 end;
    s8 duration;
    s8 step;
};

LAYOUT_OFFSET_GUARD(DisplayTransitionFrame_Value, struct DisplayTransitionFrame, value, 0x52a);
LAYOUT_OFFSET_GUARD(DisplayTransitionFrame_Start, struct DisplayTransitionFrame, start, 0x53a);
LAYOUT_SIZE_GUARD(DisplayTransitionFrame_Size, struct DisplayTransitionFrame, 0x540);
LAYOUT_OFFSET_GUARD(DisplayTransitionWindow_Control, struct DisplayTransitionWindow, dispcnt, 0x14);
LAYOUT_OFFSET_GUARD(DisplayTransitionWindow_FirstLine, struct DisplayTransitionWindow, first_line, 0x100);
LAYOUT_OFFSET_GUARD(DisplayTransitionWindow_IrqPhase, struct DisplayTransitionWindow, irq_phase, 0x108);
LAYOUT_SIZE_GUARD(DisplayTransitionWindow_PrefixSize, struct DisplayTransitionWindow, 0x10c);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Mode, struct DisplayTransitionState, mode, 0x528);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Mask, struct DisplayTransitionState, mask, 0x534);
LAYOUT_OFFSET_GUARD(DisplayTransitionState_Duration, struct DisplayTransitionState, duration, 0x53c);
LAYOUT_SIZE_GUARD(DisplayTransitionState_Size, struct DisplayTransitionState, 0x540);

#endif
