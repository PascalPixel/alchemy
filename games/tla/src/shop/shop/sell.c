/// @file
/// Selling: choosing a party member, the item grid and the sale.
// Part of shop.c.

#include "../shop_internal.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/menu.h"
#include "module/shop/shop.h"

// The Japanese money window is a tile narrower, as in Shop_SelectBuy.
#if LANGUAGE == LANGUAGE_JAPANESE
#define MONEY_WINDOW_WIDTH 11
#else
#define MONEY_WINDOW_WIDTH 12
#endif

s32 Shop_ChooseMember(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  s32 unitId = 0;
  s32 memberIndex = 0;
  void *listWindow;
  s32 redraw = 1;
  s32 result;

  shop->moneyWindow = Window_CreateFar(0, 9, MONEY_WINDOW_WIDTH, 4, 2);
  Shop_DrawMoney();
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, 2);
  listWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->listWindow = listWindow;
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  Menu_PlaceOwnerEntryObjectsFar(listWindow, 2, 0);
  while (1) {
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(listWindow, (memberIndex % 4) * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Menu_CreateOwnerEntryObjectsFar(memberIndex / 4 * 4);
        wait(1);
      }
      Shop_DrawParty(listWindow, memberIndex, 0);
      Shop_DrawMemberItems(shop->itemWindow, unitId);
      redraw = 0;
    }
    if (gInput.pressed & KEY_A) {
      wait(1);
      if (Inventory_CountFar(unitId) == 0) {
        Sound_PlayFar(SE_MENU_CANCEL);
        continue;
      }
      WindowSprite_PrepareForRedrawFar(listWindow);
      Sound_PlayFar(SE_MENU_CONFIRM);
      if (shop->partyAction == 1) {
        Shop_SelectSell(unitId);
      } else if (shop->partyAction == 3) {
        Shop_SelectRepair(unitId);
      } else {
        result = Forge_SelectItem(unitId);
        if (result == 0)
          break;
      }
      shop->cursor.anchor->active = 4;
      shop->mode = 12;
      redraw = 2;
    } else if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else {
      redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount, 4);
      wait(1);
    }
  }
  /* FAKEMATCH(barrier): an empty asm clobbering r8 at the loop exit. */
  __asm__("" : : : "r8");
  ItemIconGrid_SetVisible(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return result;
}

#undef MONEY_WINDOW_WIDTH

// The price window and the item details window are sized to their text.
#if (LANGUAGE == LANGUAGE_JAPANESE)
#define PRICE_WINDOW_X 16
#define PRICE_WINDOW_WIDTH 14
#define DETAILS_WINDOW_WIDTH 13
#elif (LANGUAGE == LANGUAGE_ENGLISH)
#define PRICE_WINDOW_X 15
#define PRICE_WINDOW_WIDTH 15
#define DETAILS_WINDOW_WIDTH 16
#elif (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH)
#define PRICE_WINDOW_X 14
#define PRICE_WINDOW_WIDTH 16
#define DETAILS_WINDOW_WIDTH 18
#else
#define PRICE_WINDOW_X 15
#define PRICE_WINDOW_WIDTH 15
#define DETAILS_WINDOW_WIDTH 18
#endif

s32 Shop_SelectSell(s32 unitId) {
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

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  slot = 0;
  inventoryCount = 1;
  priceWindow = Window_CreateFar(PRICE_WINDOW_X, 8, PRICE_WINDOW_WIDTH, 4, 2);
  do {
    nameWindow = Window_CreateFar(0, 5, 30, 3, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    redraw = 1;
    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = 0x1ff & unit->inventory[slot];
        MenuCursor_Place(shop->itemWindow, slot % 5 * 16, slot / 5 * 16 + 8);
        shop->mode = 3;
        if (shop->partyAction == 1)
          Shop_DrawItemPrice(priceWindow, itemId,
                             Shop_SalePrice(unit->inventory[slot]), 1);
        else if (shop->partyAction == 3)
          Shop_DrawItemPrice(priceWindow, itemId,
                             Shop_RepairPrice(unit->inventory[slot]), 2);
        else
          Shop_DrawItemPrice(priceWindow, itemId,
                             Forge_GetMaterialChance(itemId), 3);
        Shop_DrawMessage(nameWindow, itemId + (s32)MsgItemPlainName);
      }
      if (gInput.pressed & KEY_A) {
        Sound_PlayFar(SE_MENU_CONFIRM);
        result = 0;
        break;
      }
      if (gInput.pressed & KEY_B) {
        Sound_PlayFar(SE_MENU_CANCEL);
        result = -1;
        break;
      }
      if (gInput.held & KEY_SELECT) {
        u32 item = unit->inventory[slot];

        Sound_PlayFar(SE_RECOVERY);
        window = Window_CreateFar(0, 9, DETAILS_WINDOW_WIDTH, 10, 2);
        ItemMenu_DrawItemDetailsFar(window, item);
        *(u8 *)&shop->detailsHidden = 0;
        while (gInput.held & KEY_SELECT)
          wait(1);
        shop->detailsHidden = 1;
        Window_CloseFar(window, 2);
        Shop_DrawMoney();
        WindowSprite_PrepareForRedrawFar(shop->listWindow);
#if TLA_PATCH_2
        Shop_DrawMemberItems(shop->itemWindow, unitId);
#endif
        wait(1);
        // The Japanese release goes on to the next frame without redrawing
        // the cursor, price and name.
#if TLA_PATCH_1
        redraw = 1;
        continue;
#endif
      } else {
        if (gInput.repeat & KEY_LEFT) {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          slot--;
          slot = (inventoryCount + slot) % inventoryCount;
          redraw = 1;
        }
        if (gInput.repeat & KEY_RIGHT) {
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          slot++;
          slot = (inventoryCount + slot) % inventoryCount;
          redraw = 1;
        }
        if (gInput.repeat & KEY_UP) {
          slot -= 5;
          if (slot < 0)
            slot += 15;
          while (slot >= inventoryCount)
            slot -= 5;
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
        }
        if (gInput.repeat & KEY_DOWN) {
          slot += 5;
          if (slot >= inventoryCount)
            slot -= 15;
          while (slot < 0)
            slot += 5;
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
        }
        wait(1);
      }
    }
    Window_CloseFar(nameWindow, 2);
    wait(1);
    if (result != 0)
      break;
    if (shop->partyAction == 1) {
      count = Shop_SelectSellCount(unitId, slot);
      if (count != -1)
        Shop_SellItem(unitId, slot, count);
      Shop_ShowKeeperMessage((s32)MsgSellAnythingElse);
    } else if (shop->partyAction == 3) {
      Shop_RepairItem(unitId, slot);
      Shop_ShowKeeperMessage((s32)MsgRepairAnythingElse);
    } else {
      result = Forge_OfferItem(unitId, slot);
      if (result == 0)
        break;
      Shop_ShowKeeperMessage((s32)MsgWhichItemNeedsAttention);
    }
  } while (Inventory_CountFar(unitId) != 0);
  Window_CloseFar(priceWindow, 2);
  return result;
}

#undef PRICE_WINDOW_X
#undef PRICE_WINDOW_WIDTH
#undef DETAILS_WINDOW_WIDTH

void Shop_DrawMemberItems(void *window, s32 item) {
  if (window != NULL) {
    WindowSprite_PrepareForRedrawFar(window);
    ItemIconGrid_ShowInventory(window, 8, 0, item);
    if (Inventory_CountFar(item) == 0)
#if (LANGUAGE == LANGUAGE_ITALIAN)
      Text_DrawMessageFar((s32)MsgNoItems, window, 20, 12);
#else
      Text_DrawMessageFar((s32)MsgNoItems, window, 8, 20);
#endif
  }
}

#if (LANGUAGE == LANGUAGE_GERMAN)
#define QUANTITY_CURSOR_X 120
#elif (LANGUAGE == LANGUAGE_SPANISH) || (LANGUAGE == LANGUAGE_FRENCH) ||       \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define QUANTITY_CURSOR_X 104
#else
#define QUANTITY_CURSOR_X 128
#endif

/// Asks how many of a stackable item to sell, when the unit holds more than
/// one, and returns the count; 1 otherwise.
// The Broken Seal's Shop_SelectSellCount.
s32 Shop_SelectSellCount(s32 unitId, s32 slot) {
  s32 result;
  s16 savedX;
  s16 savedY;
  s32 price;
  s32 count;
  s32 selection;
  struct ItemDefinition *item;
  struct ShopSellWork *shop;
  struct ShopUnit *unit;

  shop = Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(unitId);
  item = Item_GetDirectFar(unit->inventory[slot]);
  result = 1;
  price = Shop_SalePrice(unit->inventory[slot]);
  count = Inventory_GetQuantityFar(unitId, slot);
  selection = count;
  if ((item->flags & ITEM_STACKABLE) && count > 1) {
    Shop_ShowKeeperMessage((s32)MsgHowManyToSell);
    savedX = shop->cursor.targetX;
    savedY = shop->cursor.targetY;
    shop->cursor.anchor->active = 4;
    shop->mode = 12;
    MenuCursor_Place(NULL, QUANTITY_CURSOR_X, 0x30);
    result = Shop_SelectQuantity(0, selection, price);
    wait(1);
    Icon_PrepareActorFar(shop->cursor.anchor);
    MenuCursor_Place(NULL, savedX, savedY);
  }
  return result;
}

#undef QUANTITY_CURSOR_X

void Shop_SellItem(s32 unitId, s32 slot, s32 count) {
  struct ShopRuntime *shop;

  shop = Ram_HeapSlots->menuRuntime;
  // The inner block gives its locals their pseudos after the item id's
  // offset, which puts the stack slots in the ROM's order.
  {
    struct BattleUnit *unit = Unit_Get(unitId);
    s32 itemId = unit->inventory[slot] & 0x1ff;
    struct ItemDefinition *item = Item_GetDirectFar(itemId);
    u8 rare = item->flags & 4;
    s32 all = 0;
    s32 total;
    s32 message;
    s32 i;

    if (count == -1) {
      all = 1;
      count = 1;
    }
    total = Shop_SalePrice(unit->inventory[slot]) * count;
    if (total == 0) {
      MessageArgument_PushFar(itemId, 2);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgWeDoNotBuy);
      return;
    }
    if ((unit->inventory[slot] & 0x200) && (item->flags & 2)) {
      MessageArgument_PushFar(itemId, 2);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgCannotSellEquipped);
      return;
    }
    if (all)
      message = (s32)MsgOldItemSellOffer;
    else if (unit->inventory[slot] & 0x400)
      message = (s32)MsgDamagedSellOffer;
    else if (count > 1)
      message = (s32)MsgSellTotal;
    else if (rare)
      message = (s32)MsgRareSellOffer;
    else
      message = (s32)MsgSellOffer;
    MessageArgument_PushFar(itemId, 2);
    MessageArgument_PushFar(total, 5);
    Shop_ShowKeeperMessageHidingCursor(message);
    if (Shop_AskYesNo(0) != 0) {
      if (rare || all)
        message = (s32)MsgKeepIt;
      else
        message = (s32)MsgChangedMind;
      Shop_ShowKeeperMessageHidingCursor(message);
      return;
    }
    Sound_PlayFar(SE_SHOP_SELL);
    for (i = 0; i < count; i++)
      Inventory_DiscardFar(unitId, slot);
    Unit_RefreshPsynergyFar(unitId);
    Unit_CalculateFar(unitId);
    Party_AdjustCoinsFar(total);
    Shop_DrawMoney();
    Shop_DrawMemberItems(shop->itemWindow, unitId);
    if (rare || all)
      message = (s32)MsgSold;
    else
      message = (s32)MsgDeal;
    Shop_ShowKeeperMessageHidingCursor(message);
  }
}
