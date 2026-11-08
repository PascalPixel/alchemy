#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "edition.h"

// The Japanese money window is a tile narrower, as in shop/f108b70.c.
#if LANGUAGE == LANGUAGE_JAPANESE
#define MONEY_WINDOW_WIDTH 11
#else
#define MONEY_WINDOW_WIDTH 12
#endif

struct ShopCursorAnchor {
  u8 unk00[5];
  u8 active;
};

struct ShopRuntime {
  u8 unk000[0x0c];
  void *moneyWindow;
  u8 unk010[0x14];
  void *itemWindow;
  u8 unk028[0x4a2];
  s16 partyMemberIds[8];
  u8 unk4da[2];
  struct ShopCursorAnchor *cursorAnchor;
  u8 unk4e0[0x24];
  s8 partyMemberCount;
  u8 mode;
  u8 unk506;
  s8 partyAction;
  u8 unk508[0x7e0];
  void *listWindow;
};

void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_08109188(void);
void Func_080f8060(void *window, s32 a, s32 b);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_080f8058(s32 first);
void wait(s32 frames);
void Func_0810928c(void *window, s32 memberIndex, s32 item);
void Shop_DrawItemCount(void *window, s32 unitId);
s32 Inventory_CountFar(s32 unitId);
void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedrawFar(void *window);
void Func_08109cac(s32 unitId);
void Func_0810a2d0(s32 unitId);
s32 Func_0810b6c4(s32 unitId);
s32 Func_08108690(s32 *cursor, s32 count, s32 columns);
void Func_0810bea8(s32 active);
void Menu_ReleaseEntryObjectsFar(void);
void Window_CloseFar(void *window, s32 style);

s32 Func_08109ad8(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 unitId = 0;
  s32 memberIndex = 0;
  void *listWindow;
  s32 redraw = 1;
  s32 result;

  shop->moneyWindow = Window_CreateFar(0, 9, MONEY_WINDOW_WIDTH, 4, 2);
  Func_08109188();
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, 2);
  listWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->listWindow = listWindow;
  shop->cursorAnchor->active = 4;
  shop->mode = 12;
  Func_080f8060(listWindow, 2, 0);
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        wait(1);
      }
      Func_0810928c(listWindow, memberIndex, 0);
      Shop_DrawItemCount(shop->itemWindow, unitId);
      redraw = 0;
    }
    if (gInput.pressed & 1) {
      wait(1);
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(113);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      Sound_PlayFar(112);
      if (shop->partyAction == 1) {
        Func_08109cac(unitId);
      } else if (shop->partyAction == 3) {
        Func_0810a2d0(unitId);
      } else {
        result = Func_0810b6c4(unitId);
        if (result == 0)
          break;
      }
      shop->cursorAnchor->active = 4;
      shop->mode = 12;
      redraw = 2;
    } else if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else {
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
      wait(1);
    }
  }
  /* FAKEMATCH(barrier): an empty asm clobbering r8 at the loop exit, which
   * keeps listWindow (live here) out of r8 and leaves it to unitId (dead
   * here); as no asm, 24 of 205 instructions differ in tla-en. */
  __asm__("" : : : "r8");
  Func_0810bea8(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return result;
}
