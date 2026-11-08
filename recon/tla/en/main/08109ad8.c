// Differ 24 of 205 (tla-en, 2026-11-08, -mtune=gba): r8 and r10 swapped, as below.
// Round 7 (2026-11-08), all 24 unless given: unitId as s16 or register, the
// action call in its if, !Inventory_CountFar, an else in place of the continue;
// for (;;) 113, declarations reversed 43, Func_080f8060 reading shop->listWindow 47;
// 8 permute minutes (nearly every candidate failed to compile). greg ranks
// listWindow (14 refs over 120 insns) above unitId (19 over 270), so unitId
// needs about 24 refs or listWindow a life over about 150 insns to take r8.
// Twin of ☀️'s Shop_ChooseMember (games/tbs/src/shop/shop/sell.c): picks the
// party member whose items the shop works on (sell, repair or the third
// action), four members a page, with ⚓️'s party cursor helper (Func_08108690)
// in place of the left and right tests. Taken from the twin: redraw starts at
// 1 where it is declared.
// 2026-11-08: the window style local and the input pointer are gone (plain
// constant and gInput.pressed measure the same 24). Re-tried without gain: the
// window read back through the struct at any one use (30 to 101), unitId
// declared or initialised elsewhere (43), memberIndex = unitId = 0 (24),
// redraw as u8 (35), an action local, nested else, goto for the cancel, and
// 8 permute minutes (no candidate beat 24).
// Left: only unitId and listWindow are swapped, r10 and r8 here against r8 and
// r10 in the native; all 24 differing rows are those two. Tried before: every
// order of the declarations and of the opening assignments, for and do loops,
// the twin's continue, a chained store of the window, chained zero
// initializers (three instructions longer).
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

  shop->moneyWindow = Window_CreateFar(0, 9, 12, 4, 2);
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
  Func_0810bea8(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return result;
}
