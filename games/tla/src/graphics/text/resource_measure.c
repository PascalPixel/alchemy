#include "types.h"
#include "ram_buffer.h"
#include "window.h"

s32 UiText_BuildRenderEntries(s32, s32);
void UiWindow_FitOnScreen(s32 idx, s32 *x, s32 *y, u32 *width, u32 *height, s32 arg5, s32 flags);

void UiText_MeasureResourceEntries(s32 no, s32 *x, s32 *y)
{
    UiText_MeasureEntryDimensions(UiText_BuildRenderEntries(no, 0), x, y, 0);
}

// Builds the render entries of message no and fits its window on screen,
// passing flags to UiWindow_FitOnScreen; returns 0 when the message is empty.
#define DEFINE_GET_RESOURCE_DIMENSIONS(name, flags)                                  \
    s32 name(s32 no, s32 *x, s32 *y, u32 *width, u32 *height)                        \
    {                                                                                \
        struct UiRenderWork *work;                                                   \
        s32 idx;                                                                     \
                                                                                     \
        work = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;                   \
        idx = UiText_BuildRenderEntries(no, 0);                                      \
        if (work->entries[idx] == 0)                                                 \
        {                                                                            \
            return 0;                                                                \
        }                                                                            \
        UiWindow_FitOnScreen(idx, x, y, width, height, 0, flags);                    \
        return 1;                                                                    \
    }

DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensions, 0)
DEFINE_GET_RESOURCE_DIMENSIONS(UiText_GetResourceDimensionsAlt, 1)
