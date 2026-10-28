// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 6 of 254 in tla-en. First difference: this issues `movs r1, #139` earlier
// (row 12) than the native does (row 16). An empty asm after `selection =
// menu->tabIndex[1];` measured 4. No single empty asm or one-pass block reaches
// 0.
// First draft of Func_080fd2e8: a second member row of the Psynergy menu,
// with a cursor icon over the members. With browseOnly 0 each move redraws
// the member's status and Psynergy; A returns the chosen member's id, B -1.
// The selection moves through Menu_MoveGridCursor and stays on the stack.
// Differ 6 of 254 (2026-10-19): only the stack frame's sub sp, which the native
// issues right after the menu load and agscc after the count's first move
// (sched2 priority 80 against 82). Reading the count first, a barrier after
// the menu load or reordering the locals is no nearer (8 to 12).
// MsgAbilityDescription is message 2181 in English (the description of
// ability 0, as The Broken Seal names it); it needs its name in the six
// catalogs before this links.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"

// The pressed keys, read again at each test.
#define KEYS_PRESSED (((volatile struct InputState *)&gInput)->pressed)

struct SpriteAttr {
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
};

struct MenuIcon {
  u8 unk00[5];
  u8 active;
  u16 x;
  u8 unk08[0x0c];
  struct SpriteAttr oam;
};

struct MenuWindow {
  u8 unk00[0x0c];
  u16 x;
};

struct PsynergyMenuState {
  u8 unk000[8];
  s32 selectedOwner;
  u8 unk00c[4];
  struct MenuWindow *rowWindow;
  u8 unk014[4];
  struct MenuIcon *rowCursor;
  s8 tabIndex[2];
  s8 tabCount;
  u8 unk01f[9];
  s32 statusWindow;
  u8 unk02c[4];
  s32 infoWindow;
  u8 unk034[0x138];
  u16 infoMessage;
  u8 unk16e[0x0a];
  struct MenuIcon *paneIcon[2];
  u8 unk180[0x84];
  u16 ownerIds[8];
  u8 unk214;
  u8 ownerCount;
  u8 unk216;
  u8 ownerId;
};

extern u8 MsgAbilityDescription[];

void *Unit_Get(s32 owner);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080f8ab4(s32 x, s32 y);
void Func_080f8170(s32 window, s32 ownerId, s32 x, s32 y);
void PsynergyMenu_CallIconRoutineWithValue(struct PsynergyMenuState *menu,
                                           s32 ownerId);
void Func_08104ef8(s32 first);
void WaitFrames(s32 frames);
void Menu_HighlightPageEntry(struct MenuWindow *window, s32 selection, s32 count);
void WindowSprite_RedrawSavedRectFar(s32 window);
void Text_DrawCharacterAtOffsetFar(u8 *message, s32 window, s32 x, s32 y);
void Func_080f8a44(s32 x, s32 y);
void Sound_PlayFar(s32 cue);
s32 Menu_MoveGridCursor(s32 *selection, s32 count, s32 columns);
void Icon_PrepareActor(struct MenuIcon *icon);

s32 Func_080fd2e8(s32 browseOnly) {
  struct PsynergyMenuState *menu;
  s32 selection;
  s32 count;
  s32 redraw;
  s32 result;
  s32 shown;
  struct MenuIcon *icon;
  s32 column;

  menu = Ram_HeapSlots->menuRuntime;
  selection = menu->tabIndex[1];
  count = menu->ownerCount;
  result = 0;
  shown = 0;
  Unit_Get(menu->ownerIds[menu->tabIndex[0]]);
  redraw = 2;
  Func_080f8ab4(selection * 24 - 10, 16);
  while (!GameFlag_Test(0x150)) {
    if (redraw) {
      Unit_Get(menu->ownerIds[selection]);
      icon = menu->rowCursor;
      column = (selection % 4) * 3;
      icon->oam.x = icon->x = (menu->rowWindow->x + column) * 8 - 2;
      if (browseOnly == 0) {
        Func_080f8170(menu->statusWindow, menu->ownerIds[selection], 0, 0);
        PsynergyMenu_CallIconRoutineWithValue(menu, menu->ownerIds[selection]);
        if (redraw == 2) {
          Func_08104ef8(selection / 4 * 4);
          WaitFrames(1);
        }
        Menu_HighlightPageEntry(menu->rowWindow, selection, count);
        if (!GameFlag_Test(0x151) && !shown) {
          WindowSprite_RedrawSavedRectFar(menu->infoWindow);
          Text_DrawCharacterAtOffsetFar((menu->infoMessage & 0x3fff) +
                                              MsgAbilityDescription,
                                          menu->infoWindow, 0, 0);
          shown = 1;
        } else {
          GameFlag_ClearBit(0x151);
        }
      }
    }
    Func_080f8a44((selection % 4) * 24 - 10, 16);
    WaitFrames(1);
    if (KEYS_PRESSED & 1) {
      Sound_PlayFar(112);
      result = menu->ownerIds[selection];
      break;
    }
    if (KEYS_PRESSED & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    redraw = Menu_MoveGridCursor(&selection, count, 4);
  }
  icon = menu->rowCursor;
  Icon_PrepareActor(icon);
  icon->active = 13;
  WaitFrames(1);
  menu->tabIndex[1] = selection;
  menu->selectedOwner = menu->ownerIds[selection];
  menu->ownerId = menu->ownerIds[selection];
  selection = menu->tabIndex[0];
  count = menu->tabCount;
  Func_08104ef8(selection / 4 * 4);
  Menu_HighlightPageEntry(menu->rowWindow, selection, count);
  menu->paneIcon[0]->active = 13;
  menu->paneIcon[1]->active = 13;
  return result;
}
