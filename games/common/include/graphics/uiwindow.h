#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

#include "render_input.h"

struct UiWindow
{
    struct RenderOutputList output;
    u16                     width;
    u16                     height;
    u16                     x;
    u16                     y;
    u16                     unk10;
    u16                     effect;
    u16                     state;
    u16                     flags;
    u16                     frame;
    s16                     duration;
    u16                     previousX;
    u16                     previousY;
    u16                     previousWidth;
    u16                     previousHeight;
};

LAYOUT_SIZE_GUARD(UiWindow_Size, struct UiWindow, 0x24);
LAYOUT_OFFSET_GUARD(UiWindow_Width, struct UiWindow, width, 8);
LAYOUT_OFFSET_GUARD(UiWindow_Frame, struct UiWindow, frame, 0x18);
LAYOUT_OFFSET_GUARD(UiWindow_PreviousHeight, struct UiWindow, previousHeight, 0x22);

struct UiChannelSlot
{
    struct UiWindow *work;
    u16              x;
    u16              y;
    u16              spacing[4];
    u16              line;
    u16              entry;
    u16              countdown;
    u16              colour;
    u16              outline;
    u16              lineSpacing;
    u16              transition;
    u16              startX;
    u16              ended;
    u16              glyphDelay;
    u16              flags;
    u16              unk26;
};

LAYOUT_SIZE_GUARD(UiChannelSlot_Size, struct UiChannelSlot, 0x28);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Entry, struct UiChannelSlot, entry, 0x12);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Countdown, struct UiChannelSlot, countdown, 0x14);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Transition, struct UiChannelSlot, transition, 0x1c);
LAYOUT_OFFSET_GUARD(UiChannelSlot_Flags, struct UiChannelSlot, flags, 0x24);

// 窓の行 y のあるタイルマップの帯 (4 行) を転送させる
/* A statement macro (S2: a layer, 10 sites): the dirty-row store stays where it is written at every use; as a plain expression, tbs-en differs. */
#define UiRender_MarkRow(render, y)              \
    do                                           \
    {                                            \
        (render)->dirty |= 2 << ((u32)(y) >> 2); \
    } while (0)

#endif
