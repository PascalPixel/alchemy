#include "TYPES.H"

struct ChannelWork {
    u8 reserved_00[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
    u16 reserved_10;
    u16 transition;
};

struct RenderChannel {
    struct ChannelWork *work;
    u16 field_04;
    u16 field_06;
    u16 values[4];
    u16 field_10;
    u16 field_12;
    u16 countdown;
};

void UiWindow_DrawFrame(s32 x, s32 y, s32 width, s32 height);
void UiWindow_EraseBorderRect(s32 x, s32 y, u32 width, u32 height);


/* Runs transition 4 of a render channel's work: each call draws a border one
 * tile outside the window and counts down; when the count reaches zero it
 * ends the transition, erases the outer border and redraws the window's own
 * border. */
void UiWork_AdvanceChannelTransition(struct RenderChannel *channel)
{
    struct ChannelWork *work = channel->work;
    s32 transition = work->transition;
    s32 x = work->x;
    s32 y = work->y;
    s32 width = work->width;
    s32 height = work->height;

    if (transition != 4)
        return;

    UiWindow_DrawFrame(x - 1, y - 1, width + 2, height + 2);
    channel->countdown--;
    if (channel->countdown != 0)
        return;

    channel->work->transition = 0;
    UiWindow_EraseBorderRect(x - 1, y - 1, width + 2, height + 2);
    UiWindow_DrawFrame(x, y, width, height);
}
