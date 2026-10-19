#include "types.h"
#include "edition.h"
#include "input.h"
#include "io_reg.h"
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

/* FAKEMATCH(volatile): the keys read through a volatile view of gInput, so
 * that each of the three tests loads them afresh from its base; as
 * gInput.pressed & KEY_A and so on, 76 of 127 instructions differ in tla-en
 * (the three tests fold into one against 11). */
#define INPUT ((volatile struct InputState *)&gInput)

/// Shows message in the item menu, with the hand cursor hidden meanwhile.
///
/// With y == -1 the message goes into the info window; otherwise a modal
/// window sized to the message opens at (x, y). With x == -1 the message stays
/// up and flag 337 records that the info window holds it; otherwise it waits
/// for A, B or Start and clears it again.
// As The Broken Seal's InventoryMenu_ShowModalMessage: in Japanese its first
// form, elsewhere its TBS_PATCH_1 form less the redraw after the wait.
void InventoryMenu_ShowModalMessage(s32 message, s32 x, s32 y) {
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
#if LANGUAGE == LANGUAGE_JAPANESE
  UiText_DrawResourceFar(message, window, 0, 0);
#else
  if (y == -1)
    UiText_DrawCharacterAtOffsetFar(message, window, 0, 0);
  else
    UiText_DrawResourceFar(message, window, 0, 0);
#endif
  if (x != -1) {
    WaitFrames(1);
    do {
      WaitFrames(1);
    } while (!(INPUT->pressed & KEY_A) && !(INPUT->pressed & KEY_B) &&
             !(INPUT->pressed & KEY_START));
    RenderOutput_ClearListFar(window);
  } else {
    GameFlag_SetBit(337);
  }
  menu->completionFlag = 1;
  menu->paneIcons[0]->active = 1;
#if LANGUAGE != LANGUAGE_JAPANESE
  if (y != -1)
    UiWindow_CloseIfOpen(&menu->modalWindow, 1);
#endif
}
