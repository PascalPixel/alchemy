#include "window.h"
#include "system.h"
#include "ram_buffer.h"

void UiWork_ResetCounters(void);
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWork_WaitWindowDuration(void *work);

struct UiWindow *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 attrs)
{
    struct UiWindow *slot;
    struct UiWindow *found;
    s32 i;

    slot = ((struct UiRenderWork *)Ram_HeapSlots->window_tiles)->windows;
    found = 0;
    i = 0;
    while ((slot->flags & 1) != 0 || slot->duration != 0) {
        i++;
        slot++;
        if (i == UI_WINDOW_COUNT) {
            goto done;
        }
    }
    found = slot;
done:
    if (found != 0) {
        found->y = y;
        found->width = width;
        found->height = height;
        found->x = x;
        found->output.head = NULL;
        found->state = 0;
        found->output.tail_link = &slot->output.head;
        found->unknown_10 = 1;
        found->flags = 1;
        UiWork_ResetCounters();
        if (attrs & 8) {
            found->flags |= 8;
        }
        if (attrs & 32) {
            found->flags |= 32;
        }
        if (attrs & 64) {
            found->flags |= 64;
        }
        if (attrs & 128) {
            found->flags |= 128;
        }
        if (attrs & 0x100) {
            found->flags |= 0x100;
        }
        if (attrs & 2) {
            found->duration = 1;
            found->flags |= 2;
            found->frame = 0;
            UiWork_DrawByAttributes(found);
        } else {
            found->duration = 8;
            found->frame = 7;
            UiWork_WaitWindowDuration(found);
            WaitFrames(1);
        }
    }
    return found;
}
