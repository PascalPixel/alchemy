// First draft of Func_080f8ce8: shows a message in the item menu with the hand
// cursor hidden meanwhile, the twin of ☀️'s InventoryMenu_ShowModalMessage
// (games/tbs/src/menu/menu.c) in its TBS_PATCH_1 form, less the redraw after
// the wait: with y == -1 the message goes into the info window, otherwise into
// a modal window sized to it at (x, y); with x == -1 it stays and flag 337 is
// set, otherwise it waits for A, B or Start and clears it. Differ 76 of 127
// (draft 115). Left: the native reads gInput.pressed afresh for each of the
// three key tests, as a volatile read would, where this draft tests the word
// once against 11 (separate if-breaks keep three tests but one load, differ
// 119); and register choice from the prologue on: the native keeps message,
// x and y in r7, r10 and r9 and the window in r6, saving r8 to r10.
#include "types.h"
#include "input.h"
#include "flag_call.h"
#include "render_input.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[20];
  struct RenderOutput *paneIcons[2];
  u8 unk01c[0x14];
  struct RenderInput *infoWindow;
  u8 unk034[0xc];
  struct RenderInput *modalWindow;
  u8 unk044[0x1da];
  u16 completionFlag;
};

void UiText_GetResourceDimensionsFar(s32 message, s32 *left, s32 *top,
                                     s32 *width, s32 *height);
s32 UiWindow_UpdateOrCreate(struct RenderInput **handle, s32 x, s32 y, s32 w,
                            s32 h, s32 mode);
void UiWindow_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width,
                        s32 height);
void RenderOutput_RedrawSavedRectFar(struct RenderInput *window);
void RenderOutput_ClearListFar(struct RenderInput *window);
void UiText_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                     s32 x, s32 y);
void UiText_DrawResourceFar(s32 message, struct RenderInput *window, s32 x,
                            s32 y);
void UiWindow_CloseIfOpen(struct RenderInput **handle, s32 mode);
void WaitFrames(s32 frames);

void Func_080f8ce8(s32 message, s32 x, s32 y) {
  struct InventoryMenuState *menu =
      (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  struct RenderInput *window;
  s32 height;
  s32 width;
  s32 top;
  s32 left;

  menu->paneIcons[0]->active = 13;
  if (y != -1) {
    UiText_GetResourceDimensionsFar(message, &left, &top, &width, &height);
    if (UiWindow_UpdateOrCreate(&menu->modalWindow, x, y, width, height,
                                0x102) == 0)
      UiWindow_SetBounds(menu->modalWindow, x, y, width, height);
    window = menu->modalWindow;
  } else {
    window = menu->infoWindow;
  }
  RenderOutput_RedrawSavedRectFar(window);
  RenderOutput_ClearListFar(window);
  if (y == -1)
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
  else
    UiText_DrawResourceFar(message, window, 0, 0);
  if (x != -1) {
    WaitFrames(1);
    do {
      WaitFrames(1);
    } while (!(gInput.pressed & 1) && !(gInput.pressed & 2) &&
             !(gInput.pressed & 8));
    RenderOutput_ClearListFar(window);
  } else {
    GameFlag_SetBit(337);
  }
  menu->completionFlag = 1;
  menu->paneIcons[0]->active = 1;
  if (y != -1)
    UiWindow_CloseIfOpen(&menu->modalWindow, 1);
}
