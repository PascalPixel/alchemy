#include "TYPES.H"
#include "WINDOW.H"
#include "RAM_BUFFER.H"

/* 1 when a window has stopped moving and has no timer left; a missing
   window counts as idle. */
s32 UiWork_IsIdle(struct UiWindow *window)
{
    if (window == NULL)
        return 1;
    if (window->flags == 0 && window->duration == 0)
        return 1;
    return 0;
}

/* Sets the text pen back to white, ten-pixel lines and no outline. */
void UiWork_ResetCounters(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;

    state->colour = 15;
    state->line_spacing = 10;
    state->glyph_spacing = 9;
    state->outline = 0;
    state->unknown_after_line_spacing = 1;
}
