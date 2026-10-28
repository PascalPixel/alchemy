#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "uiflow.h"

void WindowSprite_PrepareForRedraw(struct Window *work);
void WindowSprite_RedrawSavedRect(struct Window *window);
void Window_DrawBorder(s16 x, s16 y, u16 width, u16 height);

void Window_Close(struct Window *work, s32 release) {
  u16 zero;

  if (work == 0)
    return;

  WindowSprite_PrepareForRedraw(work);
  zero = 0;
  work->flags = zero;
  work->previousX = work->x;
  work->previousY = work->y;
  work->previousWidth = work->width;
  work->previousHeight = work->height;

  if (release != 0) {
    Window_EraseBorderRect((s16)work->x, (s16)work->y, work->width,
                             work->height);
    work->output.head = NULL;
    work->output.tailLink = NULL;
    work->width = zero;
    work->height = zero;
    work->x = zero;
    work->y = zero;
    work->unk10 = zero;
    work->effect = zero;
    work->state = zero;
    work->flags = zero;
    work->frame = zero;
    work->duration = zero;
    work->previousX = zero;
    work->previousY = zero;
    work->previousWidth = zero;
    work->previousHeight = zero;
  } else {
    work->frame = release;
    work->duration = 4;
  }
}

void WindowSprite_PrepareForRedraw(struct Window *work) {
  if (!(8 & work->flags)) {
    WindowSprite_RedrawSavedRect(work);
    WindowSprite_ClearList(work);
  }
}

void WindowSprite_RedrawSavedRect(struct Window *window) {
  Window_DrawBorder(window->x, window->y, window->width, window->height);
}
