// Matches: differ 0 (tla-en, 2026-11-08)
// Twin of ☀️'s Shop_ChooseMember (games/tbs/src/shop/shop/sell.c): picks the
// party member whose items the shop works on (sell, repair or the third
// action), four members a page, with ⚓️'s party cursor helper (Func_08108690)
// in place of the left and right tests. Taken from the twin: redraw starts at
// 1 where it is declared.
// Every edition is differ 0: tla-en, tla-fr and tla-it at this address;
// tla-ja, tla-de and tla-es measured with --symbol Func_08109ad8. Japanese
// needs the money window a tile narrower (MONEY_WINDOW_WIDTH by LANGUAGE, as
// games/tla/src/shop/f108b70.c has it). Before adopting, confirm with
// make compare-editions.
// Matched with one tagged barrier, an r8 clobber at the loop exit. Without
// it the same C is differ 24 of 205: unitId and listWindow swap r8 and r10,
// and nothing else differs. Why no plain form reached it: unitId = 0 and
// redraw = 1 both carry a constant REG_EQUAL note, so local-alloc doubles their
// live lengths (unitId 135 to 270, redraw 85 to 170), and greg ranks shop
// 0.59, result 0.57, redraw 0.40, listWindow 0.35 (14 refs over 120), unitId
// 0.28 (19 over 270), &shop->mode 0.14. Three back edges reach the redraw test,
// so the redraw blocks weigh 4, the A test 3 and the A path 2. unitId would
// need 24 to 26 weighted refs, or listWindow 11 or fewer, and no form that
// keeps these instructions gives either; without the doubling unitId (0.56)
// passes redraw and takes r7. The clobber keeps listWindow, live after the
// loop, out of r8, and unitId, dead there, takes it. No milder device was
// found: copies merge in CSE, inline parameters substitute directly, and dead
// reads are deleted in flow before the counts are taken.
// Round 8 (2026-11-08), not kept: the action tail written in each arm for
// cross-jumping (114: it is not merged), the window read back right after its
// store (24: CSE drops the load), asm "+r", "+g", "+X" or "+rm" on unitId
// before Inventory_CountFar (100, four instructions longer).
// Round 7 and earlier, all 24 unless given: unitId as s16 or register, the
// action call in its if, !Inventory_CountFar, an else in place of the
// continue, for (;;) 113, declarations reversed 43, Func_080f8060 reading
// shop->listWindow 47, the window style local and the input pointer (gone,
// same 24), the window read back through the struct at any one use (30 to
// 101), unitId declared or initialised elsewhere (43), memberIndex = unitId =
// 0, redraw as u8 (35), an action local, nested else, goto for the cancel,
// every order of the declarations and of the opening assignments, for and do
// loops, the twin's continue, a chained store of the window, chained zero
// initializers (three instructions longer), and 16 permute minutes.
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
