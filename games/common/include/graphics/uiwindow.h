#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

#include "render_input.h"

struct UiWindow {
    struct RenderOutputList output;
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 unknown_10;
    u16 effect;
    u16 state;
    u16 flags;
    u16 frame;
    s16 duration;
    u16 previous_x;
    u16 previous_y;
    u16 previous_width;
    u16 previous_height;
};

LAYOUT_SIZE_GUARD(UiWindow_Size, struct UiWindow, 0x24);
LAYOUT_OFFSET_GUARD(UiWindow_Width, struct UiWindow, width, 8);
LAYOUT_OFFSET_GUARD(UiWindow_Frame, struct UiWindow, frame, 0x18);
LAYOUT_OFFSET_GUARD(UiWindow_PreviousHeight, struct UiWindow, previous_height, 0x22);

struct UiChannelSlot {
    struct UiWindow *work;
    u16 x;
    u16 y;
    u16 spacing[4];
    u16 line;
    u16 entry;
    u16 countdown;
    u16 colour;
    u16 outline;
    u16 line_spacing;
    u16 transition;
    u16 start_x;
    u16 ended;
    u16 glyph_delay;
    u16 flags;
    u16 unknown_26;
};

LAYOUT_SIZE_GUARD(UiChannelSlot_Size, struct UiChannelSlot, 0x28);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Entry, struct UiChannelSlot, entry, 0x12);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Countdown, struct UiChannelSlot, countdown, 0x14);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Transition, struct UiChannelSlot, transition, 0x1c);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Flags, struct UiChannelSlot, flags, 0x24);

#endif
