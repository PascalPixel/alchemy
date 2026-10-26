// Draft: picks items from one unit's five-column inventory grid for the shop's
// item action (sell, repair or forge) and runs it, until B or the inventory is
// empty. As ☀️'s Shop_SelectSell (games/tbs/src/shop/shop.c), with ⚓️'s
// three actions: the price panel (Func_081091cc) shows the sell price, the
// repair price or the forge chance; Shop_SellItem (Func_0810a108, after the
// quantity, Func_0810a03c), Shop_RepairItem (Func_0810a2d8) or
// Forge_OfferItem (Func_0810b6cc), each followed by its "anything else?"
// message. SELECT held shows the item's details until it is released.
// Near miss, differ 6 of 374. The locals declared in reverse of their native
// stack slots give those slots (84 to 46); the plain-name message as a
// symbol, which the native loads from the pool (32); slot set before the
// count gives their constants' registers (24); the item window read in
// place keeps the inventory offset in r7 (10); a barrier before the shop
// work's load keeps it after sub sp (6). Left: the native stores the name
// window one row after its call returns (a barrier there stores it at once:
// 8), and loads the input's address one row later at the loop's top.
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
  u8 unk506;
  s8 partyAction;
  u8 unk508;
  u8 detailsHidden;
  u8 unk50a[0x7de];
  void *listWindow;
};

struct ShopUnit {
  u8 unk00[216];
  u16 inventory[15];
};

// Unnamed in the catalog yet: 4699, 4723 and 4862 take placeholders here.
extern u8 Msg04699[], Msg04723[], Msg04862[];

struct ShopUnit *Unit_Get(s32 unitId);
void *UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
s32 Inventory_CountFar(s32 unitId);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
s32 Shop_GetSellPrice(s32 item);
s32 Shop_GetItemPrice(s32 item);
s32 Forge_GetMaterialChance(s32 item);
void Func_081091cc(void *window, s32 itemId, s32 value, s32 kind);
void Shop_DrawMsg(void *window, s32 message);
void Audio_PlayCue(s32 cue);
void ItemMenu_DrawItemDetailsFar(void *window, s32 item);
void WaitFrames(s32 frames);
void UiWork_FinalizeFar(void *window, s32 style);
void Func_08109188(void);
void RenderOutput_PrepareForRedrawFar(void *window);
s32 Func_0810a03c(s32 unitId, s32 slot);
void Func_0810a108(s32 unitId, s32 slot, s32 count);
void Func_0810a2d8(s32 unitId, s32 slot);
s32 Func_0810b6cc(s32 unitId, s32 slot);
void Func_081084f4(s32 message);

s32 Func_08109cac(s32 unitId) {
  struct ShopRuntime *shop;
  void *nameWindow;
  void *priceWindow;
  struct ShopUnit *unit;
  s32 inventoryCount;
  s32 slot;
  void *window;
  s32 redraw;
  s32 itemId;
  s32 result;
  s32 count;

  /* FAKEMATCH(barrier): the shop work loads after sub sp and the unit's
   * spill; as its declaration's initializer, 4 of 374 instructions differ
   * in tla-en. */
  __asm__ volatile("");
  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  slot = 0;
  inventoryCount = 1;
  priceWindow = UiWindow_CreateFar(15, 8, 15, 4, 2);
  do {
    nameWindow = UiWindow_CreateFar(0, 5, 30, 3, 2);
    shop->cursorAnchor->active = 18;
    shop->mode = 12;
    redraw = 1;
    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = 0x1ff & unit->inventory[slot];
        Shop_PlaceCursor(shop->itemWindow, slot % 5 * 16, slot / 5 * 16 + 8);
        shop->mode = 3;
        if (shop->partyAction == 1)
          Func_081091cc(priceWindow, itemId,
                        Shop_GetSellPrice(unit->inventory[slot]), 1);
        else if (shop->partyAction == 3)
          Func_081091cc(priceWindow, itemId,
                        Shop_GetItemPrice(unit->inventory[slot]), 2);
        else
          Func_081091cc(priceWindow, itemId, Forge_GetMaterialChance(itemId),
                        3);
        Shop_DrawMsg(nameWindow, itemId + (s32)MsgItemPlainName);
      }
      if (gInput.pressed & 1) {
        Audio_PlayCue(112);
        result = 0;
        break;
      }
      if (gInput.pressed & 2) {
        Audio_PlayCue(113);
        result = -1;
        break;
      }
      if (gInput.held & 4) {
        u32 item = unit->inventory[slot];

        Audio_PlayCue(126);
        window = UiWindow_CreateFar(0, 9, 16, 10, 2);
        ItemMenu_DrawItemDetailsFar(window, item);
        shop->detailsHidden = 0;
        while (gInput.held & 4)
          WaitFrames(1);
        shop->detailsHidden = 1;
        UiWork_FinalizeFar(window, 2);
        Func_08109188();
        RenderOutput_PrepareForRedrawFar(shop->listWindow);
        WaitFrames(1);
        redraw = 1;
        continue;
      }
      if (gInput.repeat & 32) {
        Audio_PlayCue(111);
        slot--;
        slot = (inventoryCount + slot) % inventoryCount;
        redraw = 1;
      }
      if (gInput.repeat & 16) {
        Audio_PlayCue(111);
        slot++;
        slot = (inventoryCount + slot) % inventoryCount;
        redraw = 1;
      }
      if (gInput.repeat & 64) {
        slot -= 5;
        if (slot < 0)
          slot += 15;
        while (slot >= inventoryCount)
          slot -= 5;
        Audio_PlayCue(111);
        redraw = 1;
      }
      if (gInput.repeat & 128) {
        slot += 5;
        if (slot >= inventoryCount)
          slot -= 15;
        while (slot < 0)
          slot += 5;
        Audio_PlayCue(111);
        redraw = 1;
      }
      WaitFrames(1);
    }
    UiWork_FinalizeFar(nameWindow, 2);
    WaitFrames(1);
    if (result != 0)
      break;
    if (shop->partyAction == 1) {
      count = Func_0810a03c(unitId, slot);
      if (count != -1)
        Func_0810a108(unitId, slot, count);
      Func_081084f4((s32)Msg04699);
    } else if (shop->partyAction == 3) {
      Func_0810a2d8(unitId, slot);
      Func_081084f4((s32)Msg04723);
    } else {
      result = Func_0810b6cc(unitId, slot);
      if (result == 0)
        break;
      Func_081084f4((s32)Msg04862);
    }
  } while (Inventory_CountFar(unitId) != 0);
  UiWork_FinalizeFar(priceWindow, 2);
  return result;
}
