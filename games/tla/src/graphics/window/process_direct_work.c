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
void UiWork_ProcessDirectWork(void) {
  struct UiRenderWork *state =
      (struct UiRenderWork *)Ram_HeapSlots->windowTiles;
  struct UiWindow *work = state->windows;
  s32 index = 0;
  u8 dirty;

  do {
    if (work->flags != 0) {
      if ((s16)work->frame != 0) {
        Func_08039f94(work, 0);
        work->frame = (s16)work->frame - 1;
      } else if (work->duration != 0) {
        UiWork_DrawByAttributes(work);
      }
    } else if (work->duration != 0) {
      if ((s16)work->frame != work->duration) {
        UiWindow_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                                 (s16)work->previousWidth,
                                 (s16)work->previousHeight);
        Func_08039f94(work, 1);
        work->frame++;
        dirty = 1;
        state->dirty = dirty;
      } else {
        UiWindow_EraseBorderRect((s16)work->previousX, (s16)work->previousY,
                                 (s16)work->previousWidth,
                                 (s16)work->previousHeight);
        work->output.head = NULL;
        work->output.tailLink = NULL;
        work->width = 0;
        work->height = 0;
        work->x = 0;
        work->y = 0;
        work->unk10 = 0;
        work->effect = 0;
        work->state = 0;
        work->flags = 0;
        work->frame = 0;
        work->duration = 0;
        work->previousX = 0;
        work->previousY = 0;
        work->previousWidth = 0;
        work->previousHeight = 0;
        state->dirty = 1;
      }
    }
    index++;
    work++;
  } while (index != UI_WINDOW_COUNT);
}
