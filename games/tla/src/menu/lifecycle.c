#define OWN_Window_CreateFar
//--------------------------------------------------------------------------
// lifecycle.c
// Windows: open and close windows held by handles.
//--------------------------------------------------------------------------

#include "uiwindow.h"

void Window_CloseFar();
struct Window *Window_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void WindowSprite_RedrawSavedRectFar(struct Window *window);

/// Creates *handle's window if absent and returns 1. Otherwise returns 0,
/// redrawing its saved rectangle unless flags has 0x100 set.
/// flags: the low 8 bits select the window type.
s32 Window_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h,
                            s32 flags) {
  s32 cur;
  s32 style;

  cur = *handle;
  if (cur != 0) {
    if (0x100 & flags) {
      return 0;
    }
    WindowSprite_RedrawSavedRectFar((struct Window *)cur);
    return 0;
  }
  style = flags & 0xff;
  *handle = (s32)Window_CreateFar(x, y, w, h, style);
  return 1;
}

/// Closes *handle's window if present, then sets the handle to NULL.
/// Although declared with one argument, the caller's second argument stays in r1
/// and reaches Window_CloseFar.
void Window_CloseIfOpen(void **handle) {
  if (*handle != NULL) {
    Window_CloseFar(*handle);
    *handle = NULL;
  }
}
