#define OWN_UiWindow_CreateFar
//--------------------------------------------------------------------------
// lifecycle.c
// Windows: open and close windows held by handles.
//--------------------------------------------------------------------------

#include "uiwindow.h"

void UiWork_FinalizeFar();
struct UiWindow *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height,
                                    s32 style);
void RenderOutput_RedrawSavedRectFar(struct UiWindow *window);

//--------------------------------------------------------------------------
// Create *handle's window if absent and return 1.
// Otherwise return 0; redraw its saved rectangle unless flags has 0x100 set.
// flags: the low 8 bits select the window type.
//--------------------------------------------------------------------------
s32 UiWindow_UpdateOrCreate(s32 *handle, s32 x, s32 y, s32 w, s32 h,
                            s32 flags) {
  s32 cur;
  s32 style;

  cur = *handle;
  if (cur != 0) {
    if (0x100 & flags) {
      return 0;
    }
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)cur);
    return 0;
  }
  style = flags & 0xff;
  *handle = (s32)UiWindow_CreateFar(x, y, w, h, style);
  return 1;
}

//--------------------------------------------------------------------------
// Close *handle's window if present, then set the handle to NULL.
// Although declared with one argument, the caller's second argument stays in r1
// and reaches UiWork_FinalizeFar.
//--------------------------------------------------------------------------
void UiWindow_CloseIfOpen(void **handle) {
  if (*handle != NULL) {
    UiWork_FinalizeFar(*handle);
    *handle = NULL;
  }
}
