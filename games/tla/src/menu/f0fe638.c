#include "types.h"
#include "input.h"
#include "ram_buffer.h"

struct MenuIcon {
  u8 unk00[5];
  u8 active;
};

struct WindowWork {
  u8 unk00[3];
  u8 menuBusy;
};

struct DjinnMenuWork {
  u8 unk000[0x14];
  struct MenuIcon *cursor;
  u8 unk018[0x10];
  s32 listWindow;
  u8 unk02c[0xc4];
  s32 messageWindow;
  u8 unk0f4[0x34];
  u16 ownerY[4];
  u8 unk130[0x40];
  struct MenuIcon *selector;
  u8 unk174[4];
  struct MenuIcon *arrows[2];
  u8 unk180[0xb0];
  u16 rowX[4];
  u16 rowY[4];
};

extern u8 MsgButtonReturn[], MsgCurrentDjinn[], MsgDjinnName[];

void Sound_PlayFar(s32 cue);
void WindowSprite_RedrawSavedRectFar(s32 window);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Text_DrawMessageFar(u8 *message, s32 window, s32 x, s32 y);
void MenuIcon_SetActive(s32 active);
void wait(s32 frames);
s32 GameFlag_Test(s32 flag);
void Window_DrawDividerLineFar(s32 window, s32 x1, s32 y1, s32 x2, s32 y2);
void Menu_DrawPageIndicator(s32 window, s32 count, s32 pageSize, s32 page,
                            s32 style);
void Window_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y,
                                 s32 palette);
void Link_DrawShiftedTilePairFar(void *tiles);
void Input_ResetKeyRepeat(void);
void WindowSprite_ClearListFar(s32 window);

void Func_080fe638(void) {
  struct DjinnMenuWork *menu = Ram_HeapSlots->menuRuntime;
  s32 page;
  s32 redraw;
  s32 window;
  s32 element;
  s32 row;
  s32 djinn;
  s32 i;
  s32 pages;

  page = 0;
  pages = 2;
  Sound_PlayFar(112);
  redraw = 1;
  WindowSprite_RedrawSavedRectFar(menu->messageWindow);
  Text_DrawCharacterAtOffsetFar(MsgButtonReturn, menu->messageWindow, 0, 16);
  Text_DrawMessageFar(MsgCurrentDjinn, menu->messageWindow, -96, 132);
  menu->arrows[0]->active = 13;
  menu->arrows[1]->active = 13;
  MenuIcon_SetActive(0);
  menu->cursor->active = 13;
  menu->selector->active = 13;
  for (i = 0; i < 4; i++) {
    menu->rowX[i] = 32 + i * 56;
    menu->rowY[i] = 56;
    menu->ownerY[i] = 26;
  }
  wait(1);
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      window = menu->listWindow;
      WindowSprite_RedrawSavedRectFar(window);
      wait(1);
      Window_DrawDividerLineFar(window, 0, 11, 28, 11);
      Menu_DrawPageIndicator(window, pages, 1, page, 28);
      for (element = 0; element < 4; element++) {
        for (row = 0; row <= 8; row++) {
          djinn = element * 20 + (row + (page * 8 + page) + 7) % 18;
          if (GameFlag_Test(djinn + 48)) {
            Window_SetTilemapEntryFar(window, element + 0x1001,
                                        element * 7 + 1, row + 1, 0);
            Text_DrawCharacterAtOffsetFar(djinn + MsgDjinnName, window,
                                            element * 56 + 16, row * 8 + 8);
          }
        }
      }
      ((struct WindowWork *)Ram_HeapSlots->windowTiles)->menuBusy = 1;
      redraw = 0;
    }
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    wait(1);
    if (gInput.pressed & 7)
      break;
    if (gInput.repeat & 0x30) {
      if (gInput.repeat & 0x10)
        page++;
      else
        page--;
      page = (page + pages) % pages;
      Sound_PlayFar(111);
      Input_ResetKeyRepeat();
      redraw = 1;
    }
  }
  WindowSprite_RedrawSavedRectFar(menu->listWindow);
  WindowSprite_ClearListFar(menu->messageWindow);
  for (i = 0; i < 4; i++) {
    menu->rowX[i] = 130 + i * 32;
    menu->rowY[i] = 128;
  }
  menu->cursor->active = 1;
  menu->selector->active = 1;
  MenuIcon_SetActive(1);
  Sound_PlayFar(113);
}
