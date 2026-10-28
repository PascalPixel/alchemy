// Draft: picks one of a unit's items in the five-column inventory grid for
// the sell and drop lists, with the item's panel (Func_0810bca0) and plain
// name drawn as the cursor moves. A refuses an item the shop's check
// (Func_080ad268, or Func_080ad270 in the drop mode) flags, with message 4676
// or 4675; SELECT held shows the item's details until it is released. As ☀️'s
// Shop_SelectSell (games/tbs/src/shop/shop.c) inner loop. Returns the slot,
// or -1 when cancelled.
// Draft, differ 270 of 321 (2026-10-19): the shape follows the listing, but
// here slot and redraw spill to the stack where the native keeps them in r7
// and r10 (the count in r11); the native also keeps gInput's address across
// the calls (saved at sp + 4) and the item window in a stack slot.
#include "types.h"
#include "input.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

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
extern u8 Msg04676[], Msg04675[];

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
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  struct ShopUnit *unit = Unit_Get(unitId);
  s32 inventoryCount = 1;
  s32 redraw = 1;
  void *priceWindow = Window_CreateFar(14, 8, 16, 4, 2);
  void *nameWindow = Window_CreateFar(0, 5, 30, 3, 2);
  s32 slot;
  s32 itemId;
  s32 flags;
  s32 result;
  void *window;

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
      Shop_PlaceCursor(window, slot % 5 * 16, slot / 5 * 16 + 8);
      shop->mode = 3;
      Func_0810bca0(priceWindow, unitId, slot);
      Shop_DrawMsg(nameWindow, itemId + MSGNO(MsgItemPlainName));
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
        Message_OpenWindowFar((s32)Msg04676, 8, 1, 0);
      else if (flags & 2)
        Message_OpenWindowFar((s32)Msg04675, 8, 1, 0);
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

      Sound_PlayFar(126);
      window = Window_CreateFar(0, 9, 16, 10, 2);
      ItemMenu_DrawItemDetailsFar(window, item);
      shop->detailsHidden = 0;
      while (gInput.held & 4)
        wait(1);
      shop->detailsHidden = 1;
      Window_CloseFar(window, 2);
      WindowSprite_PrepareForRedrawFar(shop->listWindow);
      wait(1);
      redraw = 1;
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
