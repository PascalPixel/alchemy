#include "window.h"
#include "ram_buffer.h"

void UiWork_CopyParamsToRenderWork(struct UiChannelSlot *channel)
{
    struct UiRenderWork *work = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

    work->colour = channel->colour;
    work->outline = channel->outline;
    work->lineSpacing = channel->lineSpacing;
}

void UiWork_ResetChannelTransition(struct UiChannelSlot *channel)
{
    channel->transition = 2;
}
