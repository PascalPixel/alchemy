#include "uiwindow.h"
#include "system.h"
#include "window.h"
#include "ram_buffer.h"

#define UI_WORK ((struct WindowWork *)Ram_HeapSlots->windowTiles)

void Text_ResetStyle(s32 x);
void Window_DrawOpened(struct Window *window);
void Window_WaitZoom(void *work);

/// Opens the first free window at x, y with width and height; attrs bits 8,
/// 32, 64, 128 and 0x100 become window flags, and bit 2 draws it at once
/// instead of waiting for it to open.
struct Window *Window_Create(s32 x, s32 y, s32 width, s32 height,
                                 s32 attrs) {
  struct Window *slot;
  struct Window *found;
  s32 i;

  slot = UI_WORK->windows;
  found = 0;
  for (i = 0; i != UI_WINDOW_COUNT; i++, slot++) {
    if ((slot->flags & 1) == 0 && slot->duration == 0) {
      found = slot;
      break;
    }
  }
  if (found != 0) {
    found->y = y;
    found->width = width;
    found->height = height;
    found->x = x;
    found->output.head = NULL;
    found->state = 0;
    found->output.tailLink = &slot->output.head;
    found->unk10 = 1;
    found->flags = 1;
    Text_ResetStyle(x);
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
      found->flags |= 2;
      found->frame = 0;
      found->duration = 1;
      Window_DrawOpened(found);
    } else {
      found->duration = 8;
      found->frame = 7;
      Window_WaitZoom(found);
      wait(1);
    }
  }
  return found;
}

void Window_WaitZoom(void *work) {
  if (!(2 & ((struct Window *)work)->flags) &&
      (((struct Window *)work)->duration != 0)) {
    do {
      wait(1);
    } while (((struct Window *)work)->duration != 0);
  }
}
