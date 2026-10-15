#include "types.h"
#include "ram_buffer.h"
#include "window.h"
#include "uiwindow.h"

void Func_08039f94(struct UiWindow *window, s32 save_position);
void UiWork_DrawByAttributes(struct UiWindow *window);
void UiWindow_EraseBorderRect(s32 x, s32 y, s32 width, s32 height);

// Steps each window's open or close animation: an open window moves toward
// its geometry and is drawn when it arrives; a closing one is erased frame
// by frame and cleared when its duration is reached.
void UiWork_ProcessDirectWork(void)
{
    struct UiRenderWork *state = (struct UiRenderWork *)Ram_HeapSlots->window_tiles;
    struct UiWindow *work = state->windows;
    s32 index = 0;
    u8 dirty;

    do
    {
        if (work->flags != 0)
        {
            if ((s16)work->frame != 0)
            {
                Func_08039f94(work, 0);
                work->frame = (s16)work->frame - 1;
            }
            else if (work->duration != 0)
            {
                UiWork_DrawByAttributes(work);
            }
        }
        else if (work->duration != 0)
        {
            if ((s16)work->frame != work->duration)
            {
                UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
                    (s16)work->previous_width, (s16)work->previous_height);
                Func_08039f94(work, 1);
                work->frame++;
                dirty = 1;
                state->dirty = dirty;
            }
            else
            {
                UiWindow_EraseBorderRect((s16)work->previous_x, (s16)work->previous_y,
                    (s16)work->previous_width, (s16)work->previous_height);
                work->output.head = NULL;
                work->output.tail_link = NULL;
                work->width = 0;
                work->height = 0;
                work->x = 0;
                work->y = 0;
                work->unknown_10 = 0;
                work->effect = 0;
                work->state = 0;
                work->flags = 0;
                work->frame = 0;
                work->duration = 0;
                work->previous_x = 0;
                work->previous_y = 0;
                work->previous_width = 0;
                work->previous_height = 0;
                state->dirty = 1;
            }
        }
        index++;
        work++;
    } while (index != UI_WINDOW_COUNT);
}
