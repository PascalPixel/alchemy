#include "types.h"
#include "window.h"
#include "ram_buffer.h"

s32 UiWork_IsIdle(struct UiWindow *window)
{
    if (window == NULL)
        return 1;
    if (window->flags == 0 && window->duration == 0)
        return 1;
    return 0;
}

void UiWork_ResetCounters(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)Ram_HeapSlots->windowTiles;

    state->colour = 15;
    state->lineSpacing = 10;
    state->glyphSpacing = 9;
    state->outline = 0;
    state->unknownAfterLineSpacing = 1;
}
