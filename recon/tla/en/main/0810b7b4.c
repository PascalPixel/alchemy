// Draft: picks a party member and then one of their items (Func_0810b9c0),
// four members a page, returning the unit and slot through the pointers (both
// -1 when cancelled). As ☀️'s Shop_PickUnitItem (games/tbs/src/shop/shop.c),
// with ⚓️'s cursor work (Func_08108148, Func_0810824c), its page cursor helper
// (Func_08108690) and the item list's mode stored at 0x50b.
// Draft, differ 169 of 233 (2026-10-19), the shape matching throughout: the
// native gives the result r6 and uses it as the zero argument of the cursor
// icon call, where unitId's r9 serves here, which moves the shop work's offsets
// into other low registers; the same A and B test pattern as 08109ad8.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "render_input.h"

struct ShopCursor {
  struct WindowSprite *anchor;
  u16 x;
  u16 y;
  s16 targetX;
  s16 targetY;
  s8 active;
  s8 kind;
};

struct ShopRuntime {
  u8 unk000[0x24];
  void *itemWindow;
  u8 unk028[0x4a2];
  s16 partyMemberIds[8];
  u8 unk4da[2];
  struct ShopCursor cursor;
  u16 cursorIcon;
  u8 unk4ee[0x16];
  s8 partyMemberCount;
  u8 mode;
  u8 unk506[5];
  u8 itemMode;
  u8 unk50c[0x7dc];
  void *listWindow;
};

void Func_08108148(void);
void Func_0810824c(void);
void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_08108aa8(struct ShopCursor *cursor, s32 x, s32 y);
void Func_080f8060(void *window, s32 a, s32 b);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_080f8058(s32 first);
void wait(s32 frames);
void Func_0810928c(void *window, s32 memberIndex, s32 item);
void Shop_DrawItemCount(void *window, s32 unitId);
s32 Inventory_CountFar(s32 unitId);
void Sound_PlayFar(s32 cue);
void WindowSprite_PrepareForRedrawFar(void *window);
s32 Func_0810b9c0(s32 unitId);
s32 Func_08108690(s32 *cursor, s32 count, s32 columns);
void Menu_ReleaseEntryObjectsFar(void);
void Window_CloseFar(void *window, s32 style);

s32 Func_0810b7b4(s32 *outUnit, s32 *outSlot, s32 itemMode) {
  struct ShopRuntime *shop;
  s32 memberIndex = 0;
  s32 redraw = 1;
  s32 unitId = 0;
  void *listWindow;
  void *iconWindow;
  struct WindowSprite *icon;
  s32 slot;
  s32 result;

  Func_08108148();
  shop = Ram_HeapSlots->menuRuntime;
  shop->itemMode = itemMode;
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, 2);
  listWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->listWindow = listWindow;
  iconWindow = Window_CreateFar(30, 0, 1, 1, 2);
  icon = WindowSprite_CreateFar(shop->cursorIcon, 0x40000000,
                                (struct RenderInput *)iconWindow, 0, 0);
  icon->active = 4;
  icon->kind = 0;
  Func_08108aa8(&shop->cursor, -32, 112);
  shop->cursor.anchor = icon;
  shop->mode = 1;
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
    wait(1);
    if (gInput.pressed & 1) {
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(113);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      Sound_PlayFar(112);
      slot = Func_0810b9c0(unitId);
      if (slot == -1) {
        shop->cursor.anchor->active = 4;
        shop->mode = 12;
        redraw = 2;
        continue;
      }
      *outUnit = unitId;
      *outSlot = slot;
      result = 0;
      break;
    } else if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      *outUnit = -1;
      *outSlot = -1;
      result = -1;
      break;
    } else {
      redraw = Func_08108690(&memberIndex, shop->partyMemberCount, 4);
    }
  }
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(iconWindow, 2);
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  wait(1);
  Func_0810824c();
  return result;
}
