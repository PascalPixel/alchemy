#include "window.h"
#include "ram_buffer.h"

void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;

    work->colour = channel->colour;
    work->outline = channel->outline;
    work->line_spacing = channel->line_spacing;
}

void UiWork_ResetChannelTransition(struct UiChannelSlot *channel)
{
    channel->transition = 2;
}
