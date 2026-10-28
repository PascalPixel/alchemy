// Draft: picks the party member whose items the shop works on (sell, repair or
// the third action), four members a page, as ☀️'s Shop_PickUnit
// (games/tbs/src/shop/shop.c) with ⚓️'s party cursor helper (Func_08108690)
// in place of the left and right tests. Draft, differ 77 of 205 (2026-10-19):
// the native keeps redraw in r7 apart from the result in r5, which leaves the
// list window in r10 and the mode's address in r9; here redraw shares r5
// with the result and the window takes r7. The A and B tests also differ: the
// native ands into the pressed word's register and reloads it for B. Tried:
// the style 2 as literals, the input through a pointer; five minutes of
// alchemy-devtools permute found nothing better.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"

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
void WaitFrames(s32 frames);
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
  s32 memberIndex = 0;
  s32 result = 2;
  s32 unitId = 0;
  void *listWindow;
  s32 redraw;

  shop->moneyWindow = Window_CreateFar(0, 9, 12, 4, result);
  Func_08109188();
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, result);
  listWindow = Window_CreateFar(0, 14, 13, 3, result);
  shop->listWindow = listWindow;
  shop->cursorAnchor->active = 4;
  shop->mode = 12;
  Func_080f8060(listWindow, 2, 0);
  redraw = 1;
  while (1) {
    struct InputState *input;

    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      Shop_PlaceCursor(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(memberIndex / 4 * 4);
        WaitFrames(1);
      }
      Func_0810928c(listWindow, memberIndex, 0);
      Shop_DrawItemCount(shop->itemWindow, unitId);
      redraw = 0;
    }
    input = &gInput;
    if (input->pressed & 1) {
      WaitFrames(1);
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
    } else if (input->pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    } else {
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
      WaitFrames(1);
    }
  }
  Func_0810bea8(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  WaitFrames(1);
  return result;
}
