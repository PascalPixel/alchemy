#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "edition.h"
#include "version.h"

// The price window and the item details window fit their edition's text.
#if LANGUAGE == LANGUAGE_JAPANESE
#define PRICE_WINDOW_X 16
#define PRICE_WINDOW_WIDTH 14
#define DETAILS_WINDOW_WIDTH 13
#else
#define PRICE_WINDOW_X 14
#define PRICE_WINDOW_WIDTH 16
#define DETAILS_WINDOW_WIDTH 16
#endif

struct ShopCursorAnchor {
  u8 unk00[5];
  u8 active;
};

struct ShopRuntime {
  u8 unk000[0x24];
  void *itemWindow;
  u8 unk028[0x4b4];
  struct ShopCursorAnchor *cursorAnchor;
  u8 unk4e0[0x25];
  u8 mode;
  u8 unk506[3];
  u8 detailsHidden;
  u8 unk50a;
  s8 dropMode;
  u8 unk50c[0x7dc];
  void *listWindow;
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

// Unnamed in the catalog yet: 4676 and 4675 take placeholders here.
extern u8 MsgCannotRemoveIt[], MsgCannotDropIt[], MsgItemPlainName[];

struct ShopUnit *Unit_Get(s32 unitId);
void *Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 Inventory_CountFar(s32 unitId);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Func_0810bca0(void *window, s32 unitId, s32 slot);
void Shop_DrawMsg(void *window, s32 message);
void wait(s32 frames);
s32 Func_080ad268(s32 unitId, s32 slot);
s32 Func_080ad270(s32 unitId, s32 slot);
void Sound_PlayFar(s32 cue);
s32 Message_OpenWindowFar(s32 message, s32 x, s32 y, s32 mode);
s32 Message_AllTypedFar(void);
void Message_CloseFinishedWindowsFar(void);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);
void Window_CloseFar(void *window, s32 style);
void WindowSprite_PrepareForRedrawFar(void *window);

s32 Func_0810b9c0(s32 unitId) {
  void *nameWindow;
  struct ShopRuntime *shop;
  void *priceWindow;
  struct ShopUnit *unit;
  s32 slot;
  s32 inventoryCount;
  s32 redraw;
  s32 result;
  s32 itemId;
  s32 flags;
  void *window;
  s32 x;
  s32 y;
  do {
  } while (0);

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  inventoryCount = 1;
  redraw = 1;
  priceWindow = Window_CreateFar(PRICE_WINDOW_X, 8, PRICE_WINDOW_WIDTH, 4, 2);
  nameWindow = Window_CreateFar(0, 5, 30, 3, 2);

  shop->cursorAnchor->active = 18;
  shop->mode = 12;
  slot = 0;
  while (1) {
    if (redraw != 0) {
      redraw = 0;
      inventoryCount = Inventory_CountFar(unitId);
      if (slot > inventoryCount - 1)
        slot = inventoryCount - 1;
      itemId = 0x1ff & unit->inventory[slot];
      window = shop->itemWindow;
      x = slot % 5;
      y = slot / 5;
      Shop_PlaceCursor(window, x * 16, y * 16 + 8);
      shop->mode = 3;
      Func_0810bca0(priceWindow, unitId, slot);
      Shop_DrawMsg(nameWindow, itemId + (s32)MsgItemPlainName);
    }
    wait(1);
    if (gInput.pressed & 1) {
      wait(1);
      if (shop->dropMode == 0)
        flags = Func_080ad268(unitId, slot);
      else
        flags = Func_080ad270(unitId, slot);
      if (flags == 0) {
        Sound_PlayFar(112);
        result = slot;
        break;
      }
      if (flags & 1)
        Message_OpenWindowFar((s32)MsgCannotRemoveIt, 8, 1, 0);
      else if (flags & 2)
        Message_OpenWindowFar((s32)MsgCannotDropIt, 8, 1, 0);
      Sound_PlayFar(113);
      while (Message_AllTypedFar() == 0)
        wait(1);
      Message_CloseFinishedWindowsFar();
      continue;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      result = -1;
      break;
    }
    if (gInput.held & 4) {
      u32 item = unit->inventory[slot];
      void *details;

      Sound_PlayFar(126);
      details = Window_CreateFar(0, 9, DETAILS_WINDOW_WIDTH, 10, 2);
      ItemMenu_DrawItemDetailsFar(details, item);
      shop->detailsHidden = 0;
      /* FAKEMATCH(barrier): an empty asm between `shop->detailsHidden = 0;` and
       * `while (gInput.held & 4)`, which keeps their instructions apart; as
       * plain statements, 4 of 321 instructions differ in tla-en. */
      asm("");
      while (gInput.held & 4)
        wait(1);
      shop->detailsHidden = 1;
      Window_CloseFar(details, 2);
      WindowSprite_PrepareForRedrawFar(shop->listWindow);
      wait(1);
      // Japanese does not redraw the cursor or the two windows here.
#if TLA_PATCH_1
      redraw = 1;
#endif
      continue;
    }
    if (gInput.repeat & 32) {
      Sound_PlayFar(111);
      slot--;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if (gInput.repeat & 16) {
      Sound_PlayFar(111);
      slot++;
      slot = (slot + inventoryCount) % inventoryCount;
      redraw = 1;
    }
    if (gInput.repeat & 64) {
      slot -= 5;
      if (slot < 0)
        slot += 15;
      while (slot >= inventoryCount)
        slot -= 5;
      Sound_PlayFar(111);
      redraw = 1;
    }
    if (gInput.repeat & 128) {
      slot += 5;
      if (slot >= inventoryCount)
        slot -= 15;
      while (slot < 0)
        slot += 5;
      Sound_PlayFar(111);
      redraw = 1;
    }
  }
  Window_CloseFar(nameWindow, 2);
  Window_CloseFar(priceWindow, 2);
  wait(1);
  return result;
}
