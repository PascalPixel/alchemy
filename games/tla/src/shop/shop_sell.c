/// @file
/// Selling: choosing a party member, the item grid and the sale.
// Part of shop.c.

#include "shop_internal.h"
#include "sounds.h"
#include "gamepak/far_call/game.h"
#include "gamepak/far_call/menu.h"
#include "shop/shop.h"

/// Lets the player choose whose items to sell, repair or forge, by
/// shop->action, until B or, at the forge, an item is taken.
s32 Shop_ChooseMember(void) {
  struct ShopState *shop = Ram_HeapSlots->menuState;
  s32 unitId = 0;
  s32 memberIndex = 0;
  struct Window *listWindow;
  s32 redraw = 1;
  s32 result;

  shop->moneyWindow =
      Window_CreateFar(0, 9, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
  Shop_DrawMoney();
  shop->itemWindow = Window_CreateFar(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
  listWindow = Window_CreateFar(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
  shop->listWindow = listWindow;
  shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
  shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
  Menu_ShowPartyFiguresFar(listWindow, 2, 0);
  while (1) {
    /* FAKEMATCH(dead): updates of unitId that cancel out. */
    unitId++;
    unitId--;
    if (redraw != 0) {
      unitId = shop->partyMemberIds[memberIndex];
      MenuCursor_Place(listWindow,
                       (memberIndex % SHOP_PARTY_PAGE_MEMBERS) * 24 - 12, 0);
      shop->cursorFrames = 3;
      if (redraw == MENU_GRID_CHANGED_PAGE) {
        Menu_CreatePartyFiguresFar(memberIndex / SHOP_PARTY_PAGE_MEMBERS *
                                   SHOP_PARTY_PAGE_MEMBERS);
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
      if (shop->action == SHOP_ACTION_SELL) {
        Shop_SelectSell(unitId);
      } else if (shop->action == SHOP_ACTION_REPAIR) {
        Shop_SelectRepair(unitId);
      } else {
        result = Forge_SelectItem(unitId);
        if (result == 0)
          break;
      }
      shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
      shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
      redraw = MENU_GRID_CHANGED_PAGE;
    } else if (gInput.pressed & KEY_B) {
      Sound_PlayFar(SE_MENU_CANCEL);
      result = -1;
      break;
    } else {
      redraw = MenuGrid_MoveCursor(&memberIndex, shop->partyMemberCount,
                                   SHOP_PARTY_PAGE_MEMBERS);
      wait(1);
    }
  }
  ItemIconGrid_SetVisible(0);
  Menu_ReleasePartyFiguresFar();
  Window_CloseFar(listWindow, 2);
  Window_CloseFar(shop->itemWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return result;
}

/// The price window and the item details window are sized to their text.
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

/// Lets the player choose items to sell, repair or forge from the party
/// member's grid, one at a time, with each one's price and description, until
/// B or the inventory is empty. Returns -1 when cancelled, 0 when the
/// inventory ran out or, at the forge, an item was taken.
s32 Shop_SelectSell(s32 unitId) {
  struct ShopState *shop;
  struct Window *descriptionWindow;
  struct Window *priceWindow;
  struct Unit *unit;
  s32 inventoryCount;
  s32 slot;
  struct Window *window;
  s32 redraw;
  s32 itemId;
  s32 result;
  s32 count;

  shop = Ram_HeapSlots->menuState;
  unit = Unit_Get(unitId);
  slot = 0;
  inventoryCount = 1;
  priceWindow = Window_CreateFar(PRICE_WINDOW_X, 8, PRICE_WINDOW_WIDTH, 4,
                                 WINDOW_FLAG_INSTANT);
  do {
    descriptionWindow = Window_CreateFar(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
    shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
    redraw = 1;
    while (1) {
      if (redraw != 0) {
        redraw = 0;
        inventoryCount = Inventory_CountFar(unitId);
        if (slot > inventoryCount - 1)
          slot = inventoryCount - 1;
        itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
        MenuCursor_Place(shop->itemWindow, slot % SHOP_INVENTORY_COLUMNS * 16,
                         slot / SHOP_INVENTORY_COLUMNS * 16 + 8);
        shop->cursorFrames = 3;
        if (shop->action == SHOP_ACTION_SELL)
          Shop_DrawItemPrice(priceWindow, itemId,
                             Shop_SalePrice(unit->inventory[slot]),
                             SHOP_PRICE_KIND_SELL);
        else if (shop->action == SHOP_ACTION_REPAIR)
          Shop_DrawItemPrice(priceWindow, itemId,
                             Shop_RepairPrice(unit->inventory[slot]),
                             SHOP_PRICE_KIND_REPAIR);
        else
          Shop_DrawItemPrice(priceWindow, itemId,
                             Forge_GetItemCondition(itemId),
                             SHOP_PRICE_KIND_FORGE);
        Shop_DrawMessage(descriptionWindow, itemId + (s32)MsgItemDescription);
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
        u32 inventoryEntry = unit->inventory[slot];

        Sound_PlayFar(SE_RECOVERY);
        window = Window_CreateFar(0, 9, DETAILS_WINDOW_WIDTH, 10,
                                  WINDOW_FLAG_INSTANT);
        ItemMenu_DrawItemDetailsFar(window, inventoryEntry);
        *(u8 *)&shop->memberSpritesShown = 0;
        while (gInput.held & KEY_SELECT)
          wait(1);
        shop->memberSpritesShown = 1;
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
          slot -= SHOP_INVENTORY_COLUMNS;
          if (slot < 0)
            slot += INVENTORY_SLOTS;
          while (slot >= inventoryCount)
            slot -= SHOP_INVENTORY_COLUMNS;
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
        }
        if (gInput.repeat & KEY_DOWN) {
          slot += SHOP_INVENTORY_COLUMNS;
          if (slot >= inventoryCount)
            slot -= INVENTORY_SLOTS;
          while (slot < 0)
            slot += SHOP_INVENTORY_COLUMNS;
          Sound_PlayFar(SE_MENU_CURSOR_MOVE);
          redraw = 1;
        }
        wait(1);
      }
    }
    Window_CloseFar(descriptionWindow, 2);
    wait(1);
    if (result != 0)
      break;
    if (shop->action == SHOP_ACTION_SELL) {
      count = Shop_SelectSellCount(unitId, slot);
      if (count != -1)
        Shop_SellItem(unitId, slot, count);
      Shop_ShowKeeperMessage((s32)MsgSellAnythingElse);
    } else if (shop->action == SHOP_ACTION_REPAIR) {
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

/// Where the item grid writes MsgNoItems for a party member with no items, in
/// pixels.
#if LANGUAGE == LANGUAGE_ITALIAN
#define SHOP_EMPTY_GRID_X 20
#define SHOP_EMPTY_GRID_Y 12
#else
#define SHOP_EMPTY_GRID_X 8
#define SHOP_EMPTY_GRID_Y 20
#endif

/// Draws the party member's items as icons in rows of five, or a line saying it
/// has none.
void Shop_DrawMemberItems(struct Window *window, s32 unitId) {
  if (window != NULL) {
    WindowSprite_PrepareForRedrawFar(window);
    ItemIconGrid_ShowInventory(window, 8, 0, unitId);
    if (Inventory_CountFar(unitId) == 0)
      Text_DrawMessageFar((s32)MsgNoItems, (s32)window, SHOP_EMPTY_GRID_X,
                          SHOP_EMPTY_GRID_Y);
  }
}

#undef SHOP_EMPTY_GRID_X
#undef SHOP_EMPTY_GRID_Y

/// Asks how many of a stack to sell, when the party member holds more than
/// one, and returns the count, -1 when cancelled; 1 otherwise.
s32 Shop_SelectSellCount(s32 unitId, s32 slot) {
  s32 result;
  s16 savedX;
  s16 savedY;
  s32 price;
  s32 count;
  s32 selection;
  struct ItemDefinition *item;
  struct ShopState *shop;
  struct Unit *unit;

  shop = Ram_HeapSlots->menuState;
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
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
    shop->cursorFrames = SHOP_CURSOR_ENTER_FRAMES;
    MenuCursor_Place(NULL, SHOP_QUANTITY_CURSOR_X, 48);
    result = Shop_SelectQuantity(0, selection, price);
    wait(1);
    Icon_ShowFar(shop->cursor.anchor);
    MenuCursor_Place(NULL, savedX, savedY);
  }
  return result;
}

/// Sells `count` of the item in `slot` once the player accepts the
/// shopkeeper's offer, worded for a broken item, for several or for a rare
/// one. A `count` of -1 is the offer for the equipment a purchase replaced,
/// and sells one. The shop refuses an item it pays nothing for and one its
/// wearer cannot take off.
void Shop_SellItem(s32 unitId, s32 slot, s32 count) {
  struct ShopState *shop;

  shop = Ram_HeapSlots->menuState;
  {
    struct Unit *unit = Unit_Get(unitId);
    s32 itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    struct ItemDefinition *item = Item_GetDirectFar(itemId);
    u8 rare = item->flags & ITEM_RARE;
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
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgWeDoNotBuy);
      return;
    }
    if ((unit->inventory[slot] & INVENTORY_EQUIPPED) &&
        (item->flags & ITEM_CANNOT_UNEQUIP)) {
      MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
      Shop_ShowKeeperMessageHidingCursor((s32)MsgCannotSellEquipped);
      return;
    }
    if (all)
      message = (s32)MsgOldItemSellOffer;
    else if (unit->inventory[slot] & INVENTORY_BROKEN)
      message = (s32)MsgDamagedSellOffer;
    else if (count > 1)
      message = (s32)MsgSellTotal;
    else if (rare)
      message = (s32)MsgRareSellOffer;
    else
      message = (s32)MsgSellOffer;
    MessageArgument_PushFar(itemId, MESSAGE_ARGUMENT_ITEM);
    MessageArgument_PushFar(total, MESSAGE_ARGUMENT_NUMBER);
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
