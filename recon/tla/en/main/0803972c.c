/* Near miss: UiWork_CopyParamsToRenderWork, ☀️'s twin through ⚓️'s heap
   slot, differ 8 (with UiWork_ResetChannelTransition after it, which
   matches). The native loads the work pointer before building 0xf3e and
   each channel field one instruction earlier than this draft; a 250-step
   device search reached differ 6 only with a bare scheduling barrier,
   not kept. Adopt both into RENDER3.C, dropping the padding before them,
   on a match. */
#include "WINDOW.H"
#include "RAM_BUFFER.H"

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
