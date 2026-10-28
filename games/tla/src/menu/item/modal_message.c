#include "types.h"
#include "edition.h"
#include "input.h"
#include "io_reg.h"
#include "flag_call.h"
#include "render_input.h"
#include "ram_buffer.h"

struct InventoryMenuState {
  u8 unk000[20];
  struct WindowSprite *paneIcons[2];
  u8 unk01c[0x14];
  struct RenderInput *infoWindow;
  u8 unk034[0xc];
  struct RenderInput *modalWindow;
  u8 unk044[0x1da];
  u16 completionFlag;
};

void Text_GetMessageSizeFar(s32 message, s32 *left, s32 *top,
                                     s32 *width, s32 *height);
s32 Window_UpdateOrCreate(struct RenderInput **handle, s32 x, s32 y, s32 w,
                            s32 h, s32 mode);
void Window_SetBounds(struct RenderInput *window, s32 x, s32 y, s32 width,
                        s32 height);
void WindowSprite_RedrawSavedRectFar(struct RenderInput *window);
void WindowSprite_ClearListFar(struct RenderInput *window);
void Text_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                     s32 x, s32 y);
void Text_DrawMessageFar(s32 message, struct RenderInput *window, s32 x,
                            s32 y);
void Window_CloseIfOpen(struct RenderInput **handle, s32 mode);
void wait(s32 frames);

/* FAKEMATCH(volatile): a volatile view of gInput makes each of the three tests
 * load the keys afresh; as gInput.pressed & KEY_A and so on, 76 of 127
 * instructions differ in tla-en. */
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
    Text_GetMessageSizeFar(message, &left, &top, &width, &height);
    if (Window_UpdateOrCreate(&menu->modalWindow, x, y, width, height,
                                0x102) == 0)
      Window_SetBounds(menu->modalWindow, x, y, width, height);
    window = menu->modalWindow;
  } else {
    window = menu->infoWindow;
  }
  WindowSprite_RedrawSavedRectFar(window);
  WindowSprite_ClearListFar(window);
#if LANGUAGE == LANGUAGE_JAPANESE
  Text_DrawMessageFar(message, window, 0, 0);
#else
  if (y == -1)
    Text_DrawCharacterAtOffsetFar(message, window, 0, 0);
  else
    Text_DrawMessageFar(message, window, 0, 0);
#endif
  if (x != -1) {
    wait(1);
    do {
      wait(1);
    } while (!(INPUT->pressed & KEY_A) && !(INPUT->pressed & KEY_B) &&
             !(INPUT->pressed & KEY_START));
    WindowSprite_ClearListFar(window);
  } else {
    GameFlag_SetBit(337);
  }
  menu->completionFlag = 1;
  menu->paneIcons[0]->active = 1;
#if LANGUAGE != LANGUAGE_JAPANESE
  if (y != -1)
    Window_CloseIfOpen(&menu->modalWindow, 1);
#endif
}
