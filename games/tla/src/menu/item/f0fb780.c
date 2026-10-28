#include "types.h"
#include "system.h"
#include "input.h"
#include "ram_buffer.h"
#include "edition.h"

// The Details window's width in tiles: Japanese opens it over the left part
// only, beside the item window, the other editions over the whole top half.
#if LANGUAGE == LANGUAGE_JAPANESE
#define DETAILS_WINDOW_WIDTH 13
#else
#define DETAILS_WINDOW_WIDTH 30
#endif

struct ItemMenuIcon {
  u8 unk00[5];
  u8 motion;
};

struct ItemDetailsMenu {
  u8 unk000[0x10];
  s32 mainWindow;
  u8 unk014[0x15c];
  struct ItemMenuIcon *cursor;
  u8 unk174[0xa4];
  struct ItemMenuIcon *selectedItemIcon;
  u16 unk21c; ///< bit 1 read by Func_080fd968; Japanese's Details sets 1
};

void Menu_UpdateEntryObjectTransforms(void);
void Task_Remove(void (*callback)(void));
s32 Task_Add(void (*callback)(void), s32 order);
void Palette_CopyObjectBankToBackground14(void);
s32 GameFlag_Test(s32);
s32 Window_CreateFar(s32, s32, s32, s32, s32);
void ItemMenu_DrawItemDetails(s32, s32);
s32 WindowSprite_RedrawSavedRectFar(s32);
void Window_CloseFar(s32, s32);
void Window_DrawDividerLineFar(s32, s32, s32, s32, s32);
void Palette_LightenHighlight(s32);
void Window_DrawBorderFar(s32, s32, s32, s32);

/// The item menu's Details screen: describes the item entry value in a window
/// over the top half (DETAILS_WINDOW_WIDTH) and waits for A or B. Up and Down
/// move an unused counter from 0 to 4. Returns the counter after A or Start, or
/// -1 after B. The twin of The Broken Seal's ItemMenu_ShowDetails.
s32 Func_080fb780(s32 value) {
  struct ItemDetailsMenu *menu = Ram_HeapSlots->menuRuntime;
  s32 changed = 1;
#if LANGUAGE != LANGUAGE_JAPANESE
  struct ItemMenuIcon *itemIcon = menu->selectedItemIcon;
#endif
  s32 window;
  s32 counter = 0;

#if LANGUAGE != LANGUAGE_JAPANESE
  itemIcon->motion = 13;
#endif
  window = Window_CreateFar(0, 0, DETAILS_WINDOW_WIDTH, 10, 2);
  Task_Remove(Menu_UpdateEntryObjectTransforms);

  {
    struct ItemMenuIcon *cursor = menu->cursor;
    cursor->motion = 13;
  }
  Palette_CopyObjectBankToBackground14();
  WaitFrames(1);

  for (;;) {
    if (GameFlag_Test(336) != 0)
      break;

    if (changed != 0) {
      changed = 0;
      counter = (counter + 5) % 5;
      ItemMenu_DrawItemDetails(window, value);
    }

    if (gInput.pressed & 1)
      break;
    if (gInput.pressed & 2) {
      counter = -1;
      break;
    }

    if (gInput.repeat & 0x40) {
      counter -= 1;
      changed = 1;
    }
    if (gInput.repeat & 0x80) {
      counter += 1;
      changed = 1;
    }
    WaitFrames(1);
  }

  WindowSprite_RedrawSavedRectFar(window);
  WaitFrames(1);
  Window_CloseFar(window, 1);
  WindowSprite_RedrawSavedRectFar(menu->mainWindow);
  Window_DrawDividerLineFar(menu->mainWindow, 0, 3, 12, 3);
  Palette_LightenHighlight(14);
  {
    s32 priority = 0x480;

    Task_Add(Menu_UpdateEntryObjectTransforms, priority);
  }

  {
    struct ItemMenuIcon *cursor = menu->cursor;
    cursor->motion = 1;
  }
#if LANGUAGE == LANGUAGE_JAPANESE
  menu->unk21c = 1;
#else
  Window_DrawBorderFar(13, 0, 17, 10);
#endif

  return counter;
}
