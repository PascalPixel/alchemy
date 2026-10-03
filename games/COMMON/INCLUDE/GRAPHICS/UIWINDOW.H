#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

#include "RENDER_INPUT.H"

/* Window geometry is in tiles; output owns its linked sprite records. */
struct UiWindow {
    struct RenderOutputList output;
    u16 width;                  /* 0x08 */
    u16 height;                 /* 0x0a */
    u16 x;                      /* 0x0c */
    u16 y;                      /* 0x0e */
    u16 unknown_10;
    u16 unknown_12;
    u16 state;                  /* 0x14 */
    u16 flags;                  /* 0x16 */
    u16 frame;                  /* 0x18 */
    s16 duration;               /* 0x1a */
    u16 previous_x;             /* 0x1c */
    u16 previous_y;             /* 0x1e */
    u16 previous_width;         /* 0x20 */
    u16 previous_height;        /* 0x22 */
};

LAYOUT_SIZE_GUARD(UiWindow_Size, struct UiWindow, 0x24);
LAYOUT_OFFSET_GUARD(UiWindow_Width, struct UiWindow, width, 8);
LAYOUT_OFFSET_GUARD(UiWindow_Frame, struct UiWindow, frame, 0x18);
LAYOUT_OFFSET_GUARD(UiWindow_PreviousHeight, struct UiWindow, previous_height, 0x22);

/* One of the three window text channels. Positions are 8.8 fixed point;
   entry names a built text entry, and colours hold its four palette values. */
struct UiChannelSlot {
    struct UiWindow *work;
    u16 x;
    u16 y;
    u16 colours[4];
    u16 unknown_10;
    u16 entry;
    u16 countdown;
    u16 colour;
    u16 outline;
    u16 line_spacing;
    u16 transition;
    u16 start_x;
    u16 unknown_20;
    u16 unknown_22;
    u16 flags;
    u16 unknown_26;
};

LAYOUT_SIZE_GUARD(UiChannelSlot_Size, struct UiChannelSlot, 0x28);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Entry, struct UiChannelSlot, entry, 0x12);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Countdown, struct UiChannelSlot, countdown, 0x14);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Transition, struct UiChannelSlot, transition, 0x1c);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Flags, struct UiChannelSlot, flags, 0x24);

#endif
