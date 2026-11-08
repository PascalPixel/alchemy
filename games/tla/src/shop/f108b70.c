#include "types.h"
#include "ram_buffer.h"
#include "edition.h"
#include "version.h"
#include "input.h"

// The Select window is wider outside English and narrower in Japanese, the
// price window wider in German, French and Italian and narrower in Japanese;
// the Japanese money window is narrower too.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define HELD_WINDOW_X 10
#define HELD_WINDOW_WIDTH 20
#elif LANGUAGE == LANGUAGE_JAPANESE
#define HELD_WINDOW_X 14
#define HELD_WINDOW_WIDTH 13
#else
#define HELD_WINDOW_X 13
#define HELD_WINDOW_WIDTH 17
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) ||          \
    (LANGUAGE == LANGUAGE_ITALIAN)
#define PRICE_WINDOW_WIDTH 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#define PRICE_WINDOW_WIDTH 14
#else
#define PRICE_WINDOW_WIDTH 15
#endif
#if LANGUAGE == LANGUAGE_JAPANESE
#define MONEY_WINDOW_X 19
#define MONEY_WINDOW_WIDTH 11
#else
#define MONEY_WINDOW_X 18
#define MONEY_WINDOW_WIDTH 12
#endif

struct ShopCursor {
  struct ShopCursorSprite *anchor;
};

struct ShopCursorSprite {
  u8 unk00[5];
  u8 active;
};

struct ShopRuntime {
  u8 unk000[0x0c];
  void *moneyWindow;
  u8 unk010[0x24 - 0x10];
  void *itemWindow;
  u8 unk028[0x270 - 0x28];
  s16 stockItemIds[0x12d];
  s16 partyMemberIds[8];
  u8 unk4da[2];
  struct ShopCursor cursor;
  u8 unk4e0[0x4fc - 0x4e0];
  u16 selectedItem;
  u8 unk4fe[4];
  s16 stockCount;
  s8 partyMemberCount;
  u8 mode;
  u8 unk506;
  s8 partyAction;
};

struct ItemDefinition {
  u16 price;
};

struct PartyCoins {
  u8 unk00[0x10];
  u32 coins;
};

extern struct PartyCoins gMainRoutineWork;
extern char MsgItemPlainName[], MsgWhoGetsThis[], MsgPackFull[],
    MsgCannotCarryItem[], MsgCannotEquipStillWant[], MsgBuyAnythingElse[],
    MsgBuyNotEnoughMoney[];

s32 Window_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Window_CloseFar(void *window, s32 mode);
void WindowSprite_RedrawSavedRectFar(void *window);
void WindowSprite_PrepareForRedrawFar(void *window);
struct ItemDefinition *Item_Get(s32 item);
s32 Item_GetEquipmentGroupFar(s32 item);
s32 Item_IsCompatibleWithOwnerFar(s32 unit, s32 item);
void Item_AdjustCounterFar(s32 item, s32 delta);
s32 PartyInventory_CountItemFar(s32 item);
s32 Inventory_AddItemFar(s32 unit, s32 item);
void Inventory_RemoveFar(s32 unit, s32 slot);
s32 Inventory_CountFar(s32 unit);
void Shop_PlaceCursor(void *window, s32 x, s32 y);
void Shop_DrawMsg(void *window, s32 message);
s32 Shop_SelectBuyCount(s32 unit, s32 item);
void Shop_BuyDone(s32 unit, s32 item, s32 quantity);
void MessageArgument_PushFar(s32 value, s32 slot);
void Func_08109188(void);
void Func_08109068(void *window, s32 selected);
void Func_081091cc(void *window, s32 item, s32 price, s32 mode);
s32 Func_08108690(s32 *cursor, s32 count, s32 columns);
void Func_080f8040(void *window, s32 item, s32 count);
void Func_080f8058(s32 first);
void Func_080f8060(void *window, s32 a, s32 b);
void Func_0810928c(void *window, s32 selected, s32 item);
void Func_081095b0(void *window, s32 unit, s32 item);
void Func_081093a4(void *window, s32 unit, s32 item);
void Func_0810a490(void *partyWindow, void *itemWindow);
void Func_0810bea8(s32 mode);
s32 Func_081080a8(void);
void Func_081084f4(s32 message);
void Func_0810857c(s32 message);
s32 Func_08108630(s32 mode);
void Menu_ReleaseEntryObjectsFar(void);
void Sound_PlayFar(s32 cue);
void wait(s32 frames);

s32 Func_08108b70(void) {
  struct ShopRuntime *shop = Ram_HeapSlots->menuRuntime;
  void *priceWindow = 0;
  void *stockWindow;
  void *messageWindow;
  void *partyWindow;
  s32 selected = 0;
  s32 quantity;
  s16 *stockIds;
  struct ItemDefinition *heldDefinition;
  struct ItemDefinition *selectedDefinition;
  s32 repromptRecipient;
  void *itemWindow;
  s32 redraw;
  s32 selectionIndex;
  s32 stockCount;
  s32 itemId;
  struct ItemDefinition *stockDefinition;
  s32 selectionResult;
  s32 purchaseResult;
  s32 unitId;
  s32 capacitySlot;
  s32 removedCount;
  s32 key;

  /* FAKEMATCH(dead): both A tests store into key, never read; as plain
     `if (gInput.pressed & 1)`, 8 of 540 instructions differ in tla-en. */
  // tla-ja still differs in 14 of 531 instructions: the stack slots of
  // stockIds, stockCount and selectedDefinition.
  shop->itemWindow = 0;
  shop->moneyWindow =
      Window_CreateFar(MONEY_WINDOW_X, 7, MONEY_WINDOW_WIDTH, 4, 2);
  Func_08109188();
  priceWindow = Window_CreateFar(0, 8, PRICE_WINDOW_WIDTH, 4, 2);

next_page:
  selectionIndex = selected;
  stockWindow = Window_CreateFar(0, 12, 30, 4, 2);
  shop->cursor.anchor->active = 18;
  shop->mode = 12;
  messageWindow = Window_CreateFar(0, 17, 30, 3, 2);
  redraw = 1;
  stockIds = shop->stockItemIds;

  while (1) {
    stockCount = shop->stockCount;
    if (redraw != 0) {
      itemId = stockIds[selectionIndex];
      stockDefinition = Item_Get(itemId);
      Shop_PlaceCursor(stockWindow, selectionIndex % 7 * 32 - 8, 8);
      shop->mode = 4;
      Func_08109068(stockWindow, selectionIndex);
      Shop_DrawMsg(messageWindow, itemId + (s32)MsgItemPlainName);
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Func_081091cc(priceWindow, itemId, stockDefinition->price, 0);
      redraw = 0;
    }
    if (key = gInput.pressed & 1) {
      selected = selectionIndex;
      Sound_PlayFar(112);
      selectionResult = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      selectionResult = -1;
      break;
    }
    if (gInput.held & 4) {
      s32 held;
      void *window;
#if !TLA_PATCH_1
      s32 heldItem;
#endif

#if TLA_PATCH_1
      stockCount = stockIds[selectionIndex];
      held = PartyInventory_CountItemFar(stockCount);
      heldDefinition = Item_Get(stockCount);
#else
      heldItem = stockIds[selectionIndex];
      held = PartyInventory_CountItemFar(heldItem);
#endif
      Sound_PlayFar(126);
      window = Window_CreateFar(HELD_WINDOW_X, 3, HELD_WINDOW_WIDTH, 10, 2);
#if TLA_PATCH_1
      Func_080f8040(window, stockCount, held);
#else
      Func_080f8040(window, heldItem, held);
#endif
      while (gInput.held & 4)
        wait(1);
      Window_CloseFar(window, 2);
      Func_08109068(stockWindow, selectionIndex);
      Func_08109188();
#if TLA_PATCH_1
      // Japanese does not redraw the price window here.
      WindowSprite_RedrawSavedRectFar(priceWindow);
      Func_081091cc(priceWindow, stockCount, heldDefinition->price, 0);
#endif
    } else {
      redraw = Func_08108690(&selectionIndex, stockCount, 7);
    }
    wait(1);
  }

  Window_CloseFar(messageWindow, 2);
  Window_CloseFar(stockWindow, 2);
  wait(1);
  if (selectionResult == -1)
    goto close;

  shop->selectedItem = shop->stockItemIds[selected];
  Func_081084f4((s32)MsgWhoGetsThis);
  selectedDefinition = Item_Get(shop->selectedItem);
  quantity = 1;
  unitId = 0;
  repromptRecipient = 0;
  selectionIndex = 0;
  partyWindow = Window_CreateFar(0, 14, 13, 3, 2);
  shop->cursor.anchor->active = 4;
  shop->mode = 12;
  Func_080f8060(partyWindow, 2, 0);
  itemWindow = Window_CreateFar(16, 11, 14, 9, 2);
  redraw = 2;

  while (1) {
    if (repromptRecipient != 0) {
      repromptRecipient = 0;
      Func_081084f4((s32)MsgWhoGetsThis);
      redraw = 1;
    }
    if (redraw != 0) {
      unitId = shop->partyMemberIds[selectionIndex];
      Shop_PlaceCursor(partyWindow, selectionIndex % 4 * 24 - 12, 0);
      shop->mode = 3;
      if (redraw == 2) {
        Func_080f8058(selectionIndex / 4 * 4);
        wait(1);
      }
      Func_0810928c(partyWindow, selectionIndex, shop->selectedItem);
      if (Item_GetEquipmentGroupFar(shop->selectedItem) == 0)
        Func_081095b0(itemWindow, unitId, shop->selectedItem);
      else
        Func_081093a4(itemWindow, unitId, shop->selectedItem);
    }
    if (key = gInput.pressed & 1) {
      redraw = 2;
      WindowSprite_PrepareForRedrawFar(partyWindow);
      capacitySlot = Inventory_AddItemFar(unitId, shop->selectedItem);
      if (capacitySlot < 0) {
        Sound_PlayFar(113);
        MessageArgument_PushFar(unitId, 1);
        MessageArgument_PushFar(shop->selectedItem, 2);
        if (Inventory_CountFar(unitId) == 15)
          Func_081084f4((s32)MsgPackFull);
        else
          Func_081084f4((s32)MsgCannotCarryItem);
        continue;
      }
      Inventory_RemoveFar(unitId, capacitySlot);
      if (selectedDefinition->price > gMainRoutineWork.coins) {
        Sound_PlayFar(113);
        Func_0810857c((s32)MsgBuyNotEnoughMoney);
        purchaseResult = -1;
        break;
      }
      if (Item_IsCompatibleWithOwnerFar(unitId, shop->selectedItem) == 0) {
        MessageArgument_PushFar(unitId, 1);
        Func_081084f4((s32)MsgCannotEquipStillWant);
        if (Func_08108630(0) != 0) {
          repromptRecipient = 1;
          continue;
        }
      }
      Sound_PlayFar(112);
      wait(1);
      quantity = Shop_SelectBuyCount(unitId, shop->selectedItem);
      repromptRecipient = 1;
      if (quantity == -1)
        continue;
      Shop_BuyDone(unitId, shop->selectedItem, quantity);
      Func_0810a490(partyWindow, itemWindow);
      purchaseResult = 0;
      break;
    }
    if (gInput.pressed & 2) {
      Sound_PlayFar(113);
      purchaseResult = -1;
      break;
    }
    redraw = Func_08108690(&selectionIndex, shop->partyMemberCount, 4);
    wait(1);
  }

  Func_0810bea8(0);
  Menu_ReleaseEntryObjectsFar();
  Window_CloseFar(itemWindow, 2);
  Window_CloseFar(partyWindow, 2);
  wait(1);
  if (purchaseResult == 0 && shop->partyAction == 2) {
    for (removedCount = 0; removedCount < quantity; removedCount++)
      Item_AdjustCounterFar(shop->selectedItem, -1);
    if (Func_081080a8() == 0)
      goto close;
    if (selected > shop->stockCount - 1)
      selected = shop->stockCount - 1;
  }
  Func_081084f4((s32)MsgBuyAnythingElse);
  goto next_page;

close:
  Window_CloseFar(priceWindow, 2);
  Window_CloseFar(shop->moneyWindow, 2);
  wait(1);
  return 0;
}
