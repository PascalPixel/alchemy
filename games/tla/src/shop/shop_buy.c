// Buying: the stock list, the money and party windows, quantities and prices. Part of shop.c.

#include "field/stubs.h"

// The Select window is wider outside English and narrower in Japanese, the price window wider in
// German, French and Italian and narrower in Japanese.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH)                                \
    || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#    define HELD_WINDOW_X     10
#    define HELD_WINDOW_WIDTH 20
#elif LANGUAGE == LANGUAGE_JAPANESE
#    define HELD_WINDOW_X     14
#    define HELD_WINDOW_WIDTH 13
#else
#    define HELD_WINDOW_X     13
#    define HELD_WINDOW_WIDTH 17
#endif
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#    define BUY_PRICE_WINDOW_WIDTH 16
#elif LANGUAGE == LANGUAGE_JAPANESE
#    define BUY_PRICE_WINDOW_WIDTH 14
#else
#    define BUY_PRICE_WINDOW_WIDTH 15
#endif

// Runs the Buy and Artifacts counters: the player browses the stock seven items a page, picks one,
// chooses who gets it and how many, and pays. Checks room in the bag and coins first, and warns
// when the party member cannot equip it. An artifact bought takes its stock counter down, and the
// list is rebuilt. Holding Select shows the item's details. Repeats until B.
Sint32 mnShopSelectBuy(void)
{
    SHOP_STATE      *shop        = RamHeapSlots->menuState;
    WINDOW          *priceWindow = 0;
    WINDOW          *stockWindow;
    WINDOW          *descriptionWindow;
    WINDOW          *partyWindow;
    Sint32           selected = 0;
    Sint32           quantity;
    Sint32           stockCount;
    Sint16          *stockIds;
    ITEM_DEFINITION *heldDefinition;
    ITEM_DEFINITION *selectedDefinition;
    Bool             repromptRecipient;
    WINDOW          *itemWindow;
    Sint32           redraw;
    Sint32           selectionIndex;
    Sint32           itemId;
    ITEM_DEFINITION *stockDefinition;
    Sint32           selectionResult;
    Sint32           purchaseResult;
    Sint32           unitId;
    Sint32           capacitySlot;
    Sint32           removedCount;
    // FAKEMATCH(dead): both A-button tests also store into key.
    Sint32           key;

    shop->itemWindow  = 0;
    shop->moneyWindow = mnWindowCreateStub(
        30 - SHOP_COIN_WINDOW_WIDTH, 7, SHOP_COIN_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
    mnShopDrawMoney();
    priceWindow = mnWindowCreateStub(0, 8, BUY_PRICE_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);

next_page:
    selectionIndex              = selected;
    stockWindow                 = mnWindowCreateStub(0, 12, 30, 4, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
    shop->cursorFrame           = SHOP_CURSOR_ENTER_TICKS;
    descriptionWindow           = mnWindowCreateStub(0, 17, 30, 3, WINDOW_FLAG_INSTANT);
    redraw                      = 1;
    stockIds                    = shop->stockItemIds;

    while (1)
    {
        stockCount = shop->stockCount;
        if (redraw != 0)
        {
            itemId          = stockIds[selectionIndex];
            stockDefinition = gmItemGetDirectStub(itemId);
            mnCursorPlace(stockWindow, selectionIndex % SHOP_STOCK_PAGE_ITEMS * 32 - 8, 8);
            shop->cursorFrame = 4;
            mnShopDrawStock(stockWindow, selectionIndex);
            mnShopDrawMessage(descriptionWindow, itemId + (Sint32)MsgItemDescription);
            mnWindowSpriteRedrawSavedRectStub(priceWindow);
            mnShopDrawItemPrice(
                priceWindow, itemId, (Uint16)stockDefinition->price, SHOP_PRICE_KIND_BUY);
            redraw = 0;
        }
        if (key = PerWork.push & KEY_A)
        {
            selected = selectionIndex;
            sySoundPlayStub(SE_MENU_CONFIRM);
            selectionResult = 0;
            break;
        }
        if (PerWork.push & KEY_B)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            selectionResult = -1;
            break;
        }
        if (PerWork.data & KEY_SELECT)
        {
            Sint32  held;
            WINDOW *window;
#if LANGUAGE == LANGUAGE_JAPANESE
            Sint32 heldItem;
#endif

#if LANGUAGE != LANGUAGE_JAPANESE
            // stockCount, which the loop's top reloads, holds the item shown.
            stockCount     = stockIds[selectionIndex];
            held           = gmPartyInventoryCountItemStub(stockCount);
            heldDefinition = gmItemGetDirectStub(stockCount);
#else
            heldItem = stockIds[selectionIndex];
            held     = gmPartyInventoryCountItemStub(heldItem);
#endif
            sySoundPlayStub(SE_RECOVERY);
            window =
                mnWindowCreateStub(HELD_WINDOW_X, 3, HELD_WINDOW_WIDTH, 10, WINDOW_FLAG_INSTANT);
#if LANGUAGE != LANGUAGE_JAPANESE
            mnItemMenuDrawItemDetailsWithCountStub(window, stockCount, held);
#else
            mnItemMenuDrawItemDetailsWithCountStub(window, heldItem, held);
#endif
            while (PerWork.data & KEY_SELECT)
                sySynch(1);
            mnWindowCloseStub(window, 2);
            mnShopDrawStock(stockWindow, selectionIndex);
            mnShopDrawMoney();
#if LANGUAGE != LANGUAGE_JAPANESE
            // Japanese does not redraw the price window here.
            mnWindowSpriteRedrawSavedRectStub(priceWindow);
            mnShopDrawItemPrice(
                priceWindow, stockCount, (Uint16)heldDefinition->price, SHOP_PRICE_KIND_BUY);
#endif
        }
        else
        {
            redraw = mnGridMoveCursor(&selectionIndex, stockCount, SHOP_STOCK_PAGE_ITEMS);
        }
        sySynch(1);
    }

    mnWindowCloseStub(descriptionWindow, 2);
    mnWindowCloseStub(stockWindow, 2);
    sySynch(1);
    if (selectionResult == -1)
        goto close;

    shop->selectedItem = shop->stockItemIds[selected];
    mnShopShowKeeperMessage((Sint32)MsgWhoGetsThis);
    selectedDefinition          = gmItemGetDirectStub(shop->selectedItem);
    quantity                    = 1;
    unitId                      = 0;
    repromptRecipient           = FALSE;
    selectionIndex              = 0;
    partyWindow                 = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
    shop->cursorFrame           = SHOP_CURSOR_ENTER_TICKS;
    mnShowPartyFiguresStub(partyWindow, 2, 0);
    itemWindow = mnWindowCreateStub(16, 11, 14, 9, WINDOW_FLAG_INSTANT);
    redraw     = MENU_GRID_CHANGED_PAGE;

    while (1)
    {
        if (repromptRecipient != 0)
        {
            repromptRecipient = FALSE;
            mnShopShowKeeperMessage((Sint32)MsgWhoGetsThis);
            redraw = 1;
        }
        if (redraw != 0)
        {
            unitId = shop->partyMemberIds[selectionIndex];
            mnCursorPlace(partyWindow, selectionIndex % SHOP_PARTY_PAGE_MEMBERS * 24 - 12, 0);
            shop->cursorFrame = 3;
            if (redraw == MENU_GRID_CHANGED_PAGE)
            {
                mnCreatePartyFiguresStub(selectionIndex / SHOP_PARTY_PAGE_MEMBERS
                                         * SHOP_PARTY_PAGE_MEMBERS);
                sySynch(1);
            }
            mnShopDrawParty(partyWindow, selectionIndex, shop->selectedItem);
            if (gmItemGetEquipmentGroupStub(shop->selectedItem) == EQUIPMENT_GROUP_NONE)
                mnShopDrawCarriedItem(itemWindow, unitId, shop->selectedItem);
            else
                mnShopDrawEquipComparison(itemWindow, unitId, shop->selectedItem);
        }
        if (key = PerWork.push & KEY_A)
        {
            redraw = MENU_GRID_CHANGED_PAGE;
            mnWindowSpritePrepareForRedrawStub(partyWindow);
            capacitySlot = gmInventoryAddItemStub(unitId, shop->selectedItem);
            if (capacitySlot < 0)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
                mnMessageArgumentPushStub(shop->selectedItem, MESSAGE_ARGUMENT_ITEM);
                if (gmInventoryCountStub(unitId) == INVENTORY_SLOTS)
                    mnShopShowKeeperMessage((Sint32)MsgPackFull);
                else
                    mnShopShowKeeperMessage((Sint32)MsgCannotCarryItem);
                continue;
            }
            gmInventoryRemoveStub(unitId, capacitySlot);
            if ((Uint16)selectedDefinition->price > MainRoutineStateWork.coins)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                mnShopShowKeeperMessageHidingCursor((Sint32)MsgBuyNotEnoughMoney);
                purchaseResult = -1;
                break;
            }
            if (gmItemIsCompatibleWithUnitStub(unitId, shop->selectedItem) == 0)
            {
                mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
                mnShopShowKeeperMessage((Sint32)MsgCannotEquipStillWant);
                if (mnShopAskYesNo(0) != 0)
                {
                    repromptRecipient = TRUE;
                    continue;
                }
            }
            sySoundPlayStub(SE_MENU_CONFIRM);
            sySynch(1);
            quantity          = mnShopSelectBuyCount(unitId, shop->selectedItem);
            repromptRecipient = TRUE;
            if (quantity == -1)
                continue;
            mnShopBuyDone(unitId, shop->selectedItem, quantity);
            mnShopGiveGameTicket(partyWindow, itemWindow);
            purchaseResult = 0;
            break;
        }
        if (PerWork.push & KEY_B)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            purchaseResult = -1;
            break;
        }
        redraw = mnGridMoveCursor(&selectionIndex, shop->partyMemberCount, SHOP_PARTY_PAGE_MEMBERS);
        sySynch(1);
    }

    mnItemIconGridSetVisible(0);
    mnReleasePartyFiguresStub();
    mnWindowCloseStub(itemWindow, 2);
    mnWindowCloseStub(partyWindow, 2);
    sySynch(1);
    if (purchaseResult == 0 && shop->action == SHOP_ACTION_ARTIFACTS)
    {
        for (removedCount = 0; removedCount < quantity; removedCount++)
            gmItemAdjustRareItemStockStub(shop->selectedItem, -1);
        if (mnShopListAvailableArtifacts() == 0)
            goto close;
        if (selected > shop->stockCount - 1)
            selected = shop->stockCount - 1;
    }
    mnShopShowKeeperMessage((Sint32)MsgBuyAnythingElse);
    goto next_page;

close:
    mnWindowCloseStub(priceWindow, 2);
    mnWindowCloseStub(shop->moneyWindow, 2);
    sySynch(1);
    return 0;
}

#undef HELD_WINDOW_X
#undef HELD_WINDOW_WIDTH
#undef BUY_PRICE_WINDOW_WIDTH

// Draws a page of seven stock items with their prices, the page arrows, and the chosen item lifted
// and pulsing.
void mnShopDrawStock(WINDOW *window, Sint32 selected)
{
    SHOP_STATE      *shop;
    Sint16          *itemIds;
    Sint32           itemCount;
    Sint32           stockIndex;
    Sint32           pageSlot;
    Sint16           itemId;
    ITEM_DEFINITION *item;
    WINDOW_SPR      *icon;

    shop       = RamHeapSlots->menuState;
    itemIds    = shop->stockItemIds;
    itemCount  = shop->stockCount;
    stockIndex = selected / SHOP_STOCK_PAGE_ITEMS * SHOP_STOCK_PAGE_ITEMS;
    if (window != 0)
    {
        mnWindowSpritePrepareForRedrawStub(window);
        if (stockIndex != 0)
        {
            icon = mnWindowSpriteCreateStub(
                shop->prevPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 216, -16);
            icon->kind          = WINDOW_SPRITE_PLAIN;
            icon->motion        = WINDOW_SPRITE_MOTION_HOP_UP;
            icon->animationStep = 0;
        }
        if (stockIndex + SHOP_STOCK_PAGE_ITEMS < itemCount)
        {
            icon = mnWindowSpriteCreateStub(
                shop->nextPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 216, 24);
            icon->kind          = WINDOW_SPRITE_PLAIN;
            icon->motion        = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
            icon->animationStep = 0;
        }
        pageSlot = 0;
        if (stockIndex < (Uint32)itemCount)
        {
            // Each price sits 16 pixels right of its item.
            for (; (Uint32)pageSlot < SHOP_STOCK_PAGE_ITEMS && stockIndex < (Uint32)itemCount;
                 pageSlot++, stockIndex++)
            {
                itemId      = itemIds[stockIndex];
                item        = gmItemGetDirectStub(itemId);
                icon        = mnWindowSpriteCreateLoadedStub(itemId, 1, window, pageSlot * 32, 0);
                icon->depth = OAM_DEPTH_FRONT - 3;
                if (stockIndex == selected)
                {
                    icon->motion        = WINDOW_SPRITE_MOTION_SWELL;
                    icon->animationStep = 10;
                    icon->depth         = OAM_DEPTH_FRONT - 2;
                }
                icon = mnShopCreatePriceSprite((Uint16)item->price, window, pageSlot * 32 + 16, 0);
                icon->depth = OAM_DEPTH_FRONT - 4;
            }
        }
    }
}

// Draws the party's coins in the coins window, in the edition's layout.
void mnShopDrawMoney(void)
{
    WINDOW *window = ((SHOP_STATE *)RamHeapSlots->menuState)->moneyWindow;
#if LANGUAGE == LANGUAGE_JAPANESE
    Sint32 msg;
#endif

    if (window != NULL)
    {
        mnWindowSpritePrepareForRedrawStub(window);
#if LANGUAGE == LANGUAGE_JAPANESE
        // "Coins" (MsgCoins) after the count, two messages before MsgYourCoins.
        msg = (Sint32)MsgYourCoins;
        mnTextDrawMessageAsTilesStub(msg, window, 0, 0);
        msg -= 2;
        mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins, 6, (Sint32)window, 0, 8);
        mnTextDrawMessageAsTilesStub(msg, window, 48, 8);
#else
        mnTextDrawMessageAsTilesStub((Sint32)MsgYourCoins, window, 0, 0);
        mnTextDrawNumberInWindowStub(MainRoutineStateWork.coins, 6, (Sint32)window, 32, 8);
#endif
    }
}

// The price line reads "Price", the price, then "Coins". Japanese indents the line and its notes by
// 8 pixels, French draws the price 8 pixels nearer its label, Spanish and Italian the coins label 8
// pixels further right.
#define SHOP_ITEM_NOTE_X    JAPANESE_OR(8, 0)
#define SHOP_PRICE_NUMBER_X FRENCH_OR(24, 32)
#define SHOP_PRICE_LABEL_X  BY_LANGUAGE(72, 72, 72, 80, 72, 80)

// Draws the item's name and below it its price. With a price of 0 it says instead that the shop
// does not buy it (SHOP_PRICE_KIND_SELL) or that it is not broken (SHOP_PRICE_KIND_REPAIR); at the
// forge (SHOP_PRICE_KIND_FORGE) `price` is the item's condition, drawn as its word.
void mnShopDrawItemPrice(WINDOW *window, Sint32 itemId, Sint32 price, Sint32 priceKind)
{
    Sint32 labelMsg;

    if (window == 0)
        return;
    goto draw;
cannot_buy:
    mnTextDrawMessageAsTilesStub((Sint32)MsgCannotBuyThat, window, SHOP_ITEM_NOTE_X, 8);
    return;
not_broken:
    mnTextDrawMessageAsTilesStub((Sint32)MsgNotBroken, window, SHOP_ITEM_NOTE_X, 8);
    return;
show_price:
    labelMsg = (Sint32)MsgPrice;
    mnTextDrawMessageAsTilesStub(labelMsg, window, SHOP_ITEM_NOTE_X, 8);
    // "Coins", three messages before "Price".
    labelMsg -= 3;
    mnTextDrawNumberInWindowStub(price, 5, (Sint32)window, SHOP_PRICE_NUMBER_X, 8);
    mnTextDrawMessageAsTilesStub(labelMsg, window, SHOP_PRICE_LABEL_X, 8);
    return;
draw:
    mnWindowSpriteRedrawSavedRectStub(window);
    mnTextDrawMessageAsTilesStub(itemId + (Sint32)MsgItemName, window, 0, 0);
    if (price == 0)
    {
        switch (priceKind)
        {
            case SHOP_PRICE_KIND_SELL  : goto cannot_buy;
            case SHOP_PRICE_KIND_REPAIR: goto not_broken;
        }
    }
    // MsgItemCondition starts a list: "----------", "Processed", "Rusted".
    if (priceKind == SHOP_PRICE_KIND_FORGE)
        mnTextDrawMessageAsTilesStub(price + (Sint32)MsgItemCondition, window, 8, 8);
    else
        goto show_price;
}

#undef SHOP_ITEM_NOTE_X
#undef SHOP_PRICE_NUMBER_X
#undef SHOP_PRICE_LABEL_X

void mnWindowSpriteClearListStub(void *window);

// Shows `msg`, an item's description, in the help window; a NULL window is ignored. The Japanese
// release clears the window and waits a tick; the others redraw the window's saved rectangle.
void mnShopDrawMessage(WINDOW *window, Sint32 msg)
{
    if (window != 0)
    {
#if LANGUAGE != LANGUAGE_JAPANESE
        mnWindowSpriteRedrawSavedRectStub(window);
#else
        mnWindowSpriteClearListStub(window);
        sySynch(1);
#endif
#if (LANGUAGE != LANGUAGE_JAPANESE)
        mnTextDrawMessageAsTilesStub(msg, window, 0, 0);
#else
        mnTextDrawMessageStub(msg, (Sint32)window, 0, 0);
#endif
    }
}

// Shows the chosen party member running in place and the others standing, four a page with its
// arrows, and draws at four fifths of their size the party members who cannot equip `itemId`.
void mnShopDrawParty(WINDOW *window, Sint32 selected, Sint32 itemId)
{
    SHOP_STATE *shop  = RamHeapSlots->menuState;
    Sint32      page  = selected / SHOP_PARTY_PAGE_MEMBERS;
    Sint32      first = page * SHOP_PARTY_PAGE_MEMBERS;
    Sint32      count = shop->partyMemberCount;
    WINDOW_SPR *icon;
    Sint32      i;

    if (window != 0)
    {
        mnWindowSpritePrepareForRedrawStub(window);
        if (first != 0)
        {
            icon = mnWindowSpriteCreateStub(
                shop->prevPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 88, -12);
            icon->kind          = WINDOW_SPRITE_PLAIN;
            icon->motion        = WINDOW_SPRITE_MOTION_HOP_UP;
            icon->animationStep = 0;
        }
        if (first + SHOP_PARTY_PAGE_MEMBERS < count)
        {
            icon = mnWindowSpriteCreateStub(
                shop->nextPageIcon, WINDOW_SPRITE_SIZE_16X16, window, 88, 0);
            icon->kind          = WINDOW_SPRITE_PLAIN;
            icon->motion        = WINDOW_SPRITE_MOTION_HOP_DOWN_NEXT_PAGE;
            icon->animationStep = 0;
        }
        for (i = 0; i < SHOP_PARTY_PAGE_MEMBERS; i++)
        {
            Sint32 unitId = shop->partyMemberIds[first + i];
            void  *figure = shop->partyMemberIcons[i];

            if (figure != NULL)
            {
                if (first + i == selected)
                    fdSpriteStartAnimationStub(figure, ANIM_PARTY_RUN);
                else
                    fdSpriteStartAnimationStub(figure, ANIM_STAND);
                shop->partyMemberScale[i] = toFIXED(1);
                if (!gmItemIsCompatibleWithUnitStub(unitId, itemId))
                    shop->partyMemberScale[i] = toFIXED(4) / 5;
            }
        }
    }
}

// Shows how Attack, Defense and Agility would change if the party member wore the item: equips it
// in place of what it would replace, recomputes, puts the slot back and recomputes again, then
// draws both values with an arrow and the name of the item it would replace. Called by
// mnShopSelectBuy for weapons and armor.
void mnShopDrawEquipComparison(WINDOW *window, Sint32 unitId, Sint32 itemId)
{
    SHOP_STATE      *shop           = RamHeapSlots->menuState;
    UNIT            *unit           = syUnitGet(unitId);
    ITEM_DEFINITION *item           = gmItemGetDirectStub(itemId);
    Sint32           replacedItemId = -1;
    Sint32           previewSlot;
    Sint32           savedEntry;
    Sint32           index;
    Sint32           baseStats[4];
    Sint32           previewStats[4];

    if (window == 0)
        return;
    mnWindowSpritePrepareForRedrawStub(window);
    if (!gmItemCanUnitEquipDirectStub(unitId, itemId))
    {
        mnTextDrawMessageStub(MsgShopCannotEquip, window, 8, 24);
        return;
    }
    previewSlot = gmInventoryFindEquippedStub(unitId, item->type);
    // With nothing of the type equipped, borrow the first slot that is not equipped, else the first
    // Psynergy item's, else slot 0.
    if (previewSlot == -1)
    {
        for (index = 0; index < INVENTORY_SLOTS; index++)
        {
            if (!(unit->inventory[index] & INVENTORY_EQUIPPED))
                break;
        }
        if (index == INVENTORY_SLOTS)
        {
            for (index = 0; index < INVENTORY_SLOTS; index++)
            {
                if (gmItemGetDirectStub(unit->inventory[index])->type == ITEM_TYPE_PSYNERGY)
                    break;
            }
            if (index == INVENTORY_SLOTS)
                index = 0;
        }
        previewSlot = index;
    }
    else
    {
        replacedItemId = unit->inventory[previewSlot] & INVENTORY_ITEM_ID_MASK;
    }
    // Borrow a live slot so gmUnitCalculate includes every equipment effect; restore both the slot
    // and the stats before returning to the counter.
    savedEntry                   = unit->inventory[previewSlot];
    unit->inventory[previewSlot] = itemId | INVENTORY_EQUIPPED;
    gmUnitCalculateStub(unitId);
    previewStats[0]              = unit->attack;
    previewStats[1]              = unit->defense;
    previewStats[2]              = unit->agility;
    previewStats[3]              = unit->luck;
    unit->inventory[previewSlot] = savedEntry;
    gmUnitCalculateStub(unitId);
    baseStats[0] = unit->attack;
    baseStats[1] = unit->defense;
    baseStats[2] = unit->agility;
    baseStats[3] = unit->luck;
    for (index = 0; index <= 2; index++)
    {
        if (baseStats[index] > previewStats[index])
            mnWindowSpriteCreateStub(
                shop->statDownIcon, WINDOW_SPRITE_SIZE_16X16, window, 56, index * 16 - 4)
                ->kind = WINDOW_SPRITE_PLAIN;
        else if (baseStats[index] < previewStats[index])
            mnWindowSpriteCreateStub(
                shop->statUpIcon, WINDOW_SPRITE_SIZE_16X16, window, 56, index * 16 - 4)
                ->kind = WINDOW_SPRITE_PLAIN;
        mnTextDrawNumberInWindowStub(baseStats[index], 3, window, 32, index * 16);
        if (baseStats[index] != previewStats[index])
            mnTextDrawNumberInWindowStub(previewStats[index], 3, window, 72, index * 16);
        mnTextDrawMessageAsTilesStub(MsgShopStatLabel + index, window, 0, index * 16);
        mnWindowDrawDividerLineStub(window, 0, index * 2 + 2, 13, index * 2 + 2);
    }
    if (replacedItemId != -1)
        mnTextDrawMessageAsTilesStub(replacedItemId + MsgItemName, window, 0, 48);
}

// Shows how many of the item the party member already carries and the party member's inventory
// icons.
void mnShopDrawCarriedItem(WINDOW *window, Sint32 unitId, Sint32 itemId)
{
    UNIT  *unit;
    Sint32 slot;

    unit = syUnitGet(unitId);
    if (window != 0)
    {
        mnWindowSpritePrepareForRedrawStub(window);
        slot = gmInventoryFindStub(unitId, itemId);
        if (slot != -1)
        {
            mnMessageArgumentPushStub((unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1,
                                      MESSAGE_ARGUMENT_NUMBER);
            mnTextDrawMessageAsTilesStub((Sint32)MsgYouHave, window, 0, 0);
        }
        else
        {
            mnTextDrawMessageAsTilesStub((Sint32)MsgShopNoneInStock, window, 0, 0);
        }
        mnItemIconGridShowInventory(window, 8, 8, unitId);
    }
}

// Asks how many to buy when the item stacks, limited by what the coins buy, the artifact's stock
// and a stack of 30 less what the party member carries. Returns the count (1 for other items), or
// -1 when cancelled.
Sint32 mnShopSelectBuyCount(Sint32 unitId, Sint32 itemId)
{
    SHOP_STATE      *shop;
    UNIT            *unit;
    ITEM_DEFINITION *item;
    Sint32           carriedQuantity;
    Sint32           quantityLimit;
    Sint32           slot;
    Sint32           artifactsInStock;
    Sint32           result;

    shop   = RamHeapSlots->menuState;
    unit   = syUnitGet(unitId);
    item   = gmItemGetDirectStub(itemId);
    result = 1;
    if (item->flags & ITEM_STACKABLE)
    {
        mnShopShowKeeperMessage((Sint32)MsgHowManyToBuy);
        slot = gmInventoryFindStub(unitId, itemId);
        if (slot != -1)
            carriedQuantity = (unit->inventory[slot] >> INVENTORY_QUANTITY_SHIFT) + 1;
        else
            carriedQuantity = 0;

        quantityLimit = SHOP_STACK_LIMIT;
        if ((Uint16)item->price != 0)
            quantityLimit = MainRoutineStateWork.coins / (Uint16)item->price;

        if (shop->action == SHOP_ACTION_ARTIFACTS)
        {
            artifactsInStock = gmItemAdjustRareItemStockStub(itemId, 0);
            if (quantityLimit > artifactsInStock)
                artifactsInStock = gmItemAdjustRareItemStockStub(itemId, 0);
            else
                artifactsInStock = quantityLimit;
            quantityLimit = artifactsInStock;
        }

        quantityLimit += carriedQuantity;
        if (quantityLimit > SHOP_STACK_LIMIT)
            quantityLimit = SHOP_STACK_LIMIT;

        shop->cursorFrame = SHOP_CURSOR_ENTER_TICKS;
        mnCursorPlace(0, SHOP_QUANTITY_CURSOR_X, 48);
        result = mnShopSelectQuantity(carriedQuantity, quantityLimit, (Uint16)item->price);
    }
    return result;
}

// The quantity window: outside the English edition the count sits further left and the price and
// coins label move with their words; the Spanish, French and Italian windows are wider; Japanese
// draws the counter コ after the count.
#if LANGUAGE == LANGUAGE_ENGLISH
#    define QUANTITY_WINDOW_X     7
#    define QUANTITY_WINDOW_WIDTH 23
#    define QUANTITY_COUNT_X      72
#    define QUANTITY_PRICE_X      88
#    define QUANTITY_COINS_X      136
#elif LANGUAGE == LANGUAGE_JAPANESE
#    define QUANTITY_WINDOW_X     7
#    define QUANTITY_WINDOW_WIDTH 23
#    define QUANTITY_COUNT_X      64
#    define QUANTITY_PIECE_X      80
#    define QUANTITY_PRICE_X      96
#    define QUANTITY_COINS_X      144
#elif LANGUAGE == LANGUAGE_GERMAN
#    define QUANTITY_WINDOW_X     7
#    define QUANTITY_WINDOW_WIDTH 23
#    define QUANTITY_COUNT_X      64
#    define QUANTITY_PRICE_X      80
#    define QUANTITY_COINS_X      128
#elif LANGUAGE == LANGUAGE_SPANISH
#    define QUANTITY_WINDOW_X     5
#    define QUANTITY_WINDOW_WIDTH 25
#    define QUANTITY_COUNT_X      64
#    define QUANTITY_PRICE_X      104
#    define QUANTITY_COINS_X      160
#else
#    define QUANTITY_WINDOW_X     5
#    define QUANTITY_WINDOW_WIDTH 25
#    define QUANTITY_COUNT_X      64
#    define QUANTITY_PRICE_X      88
#    define QUANTITY_COINS_X      144
#endif

// Lets the player pick a quantity with left and right in a small window that shows it and its price
// in coins: a bar of 30 segments, the ones already carried in one colour and the ones chosen in
// another. Returns the count above `minimum`, the items already carried, or -1 when B cancels.
Sint32 mnShopSelectQuantity(Sint32 minimum, Sint32 maximum, Sint32 price)
{
    Uint8      *tileBuf;
    WINDOW     *window;
    Bool        changed;
    Sint32      resourceSlot;
    Sint32      count;
    Sint32      result;
    WINDOW_SPR *rightHalf;
    Sint32      selected;

    tileBuf  = syHeapSlotAllocateEWRAM(HEAP_SLOT_TILE_SCRATCH, 1024);
    changed  = TRUE;
    maximum -= minimum;
    result   = -1;
    count    = 0;
    window =
        mnWindowCreateStub(QUANTITY_WINDOW_X, 4, QUANTITY_WINDOW_WIDTH, 3, WINDOW_FLAG_INSTANT);
    if (window != 0)
    {
        resourceSlot = syVRAMCacheFindFreeEntry();
        if (resourceSlot != VRAM_CACHE_ENTRY_COUNT)
        {
            syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, 0);
            // The bar is two 32x8 sprites, the right one four tiles on.
            mnWindowSpriteCreateStub(resourceSlot, WINDOW_SPRITE_SIZE_32X8, window, 0, 0);
            rightHalf =
                mnWindowSpriteCreateStub(resourceSlot, WINDOW_SPRITE_SIZE_32X8, window, 32, 0);
            rightHalf->sprite.oam.index += 4;
            for (;;)
            {
                if (PerWork.push & KEY_A)
                {
                    sySoundPlayStub(SE_MENU_CONFIRM);
                    result = count + 1;
                    break;
                }
                if (PerWork.push & KEY_B)
                {
                    sySoundPlayStub(SE_MENU_CANCEL);
                    result = -1;
                    break;
                }
                if (PerWork.repeat & KEY_LEFT)
                {
                    sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                    changed = TRUE;
                    count--;
                }
                if (PerWork.repeat & KEY_RIGHT)
                {
                    sySoundPlayStub(SE_MENU_CURSOR_MOVE);
                    changed = TRUE;
                    count++;
                }
                if (changed)
                {
                    changed = FALSE;
                    count   = (count + maximum) % maximum;
                    syDMAXCopy(ShopQuantityTiles,
                               tileBuf,
                               DMA_COPY_WORDS(8 * TILE_SIZE_4BPP / 4),
                               REG_DMA3);
                    // Paint capacity, available stock, selection and carried items in that order;
                    // each shorter prefix overwrites the previous colour.
                    mnQuantityBarFill(SHOP_STACK_LIMIT, QUANTITY_BAR_OUT_OF_REACH, tileBuf);
                    mnQuantityBarFill(minimum + maximum, QUANTITY_BAR_OPEN, tileBuf);
                    mnQuantityBarFill(minimum + count + 1, QUANTITY_BAR_CHOSEN, tileBuf);
                    selected = count + 1;
                    mnQuantityBarFill(minimum, QUANTITY_BAR_HELD, tileBuf);
                    syVRAMCacheLoad(resourceSlot, 8 * TILE_SIZE_4BPP, tileBuf);
                    result = selected;
                    mnTextDrawNumberInWindowStub(result, 2, (Sint32)window, QUANTITY_COUNT_X, 0);
#if LANGUAGE == LANGUAGE_JAPANESE
                    // The counter コ after the count: MsgPiece, which the ROM
                    // reaches from MsgCoins, the message before it.
                    mnTextDrawMessageAsTilesStub((Sint32)MsgCoins + 1, window, QUANTITY_PIECE_X, 0);
#endif
                    mnTextDrawNumberInWindowStub(
                        price * result, 6, (Sint32)window, QUANTITY_PRICE_X, 0);
                    mnTextDrawMessageAsTilesStub((Sint32)MsgCoins, window, QUANTITY_COINS_X, 0);
                }
                sySynch(1);
            }
            sySynch(1);
            mnWindowCloseStub(window, 2);
        }
    }
    syHeapSlotRelease(HEAP_SLOT_TILE_SCRATCH);
    return result;
}

#undef QUANTITY_WINDOW_X
#undef QUANTITY_WINDOW_WIDTH
#undef QUANTITY_COUNT_X
#undef QUANTITY_PIECE_X
#undef QUANTITY_PRICE_X
#undef QUANTITY_COINS_X

// Gives the party member each item bought, one at a time, taking its price and counting it toward
// the next Game Ticket; then offers to equip it and to sell what it replaced.
void mnShopBuyDone(Sint32 unitId, Sint32 itemId, Sint32 quantity)
{
    Sint32           replacedSlot;
    Sint32           i;
    Sint32           lastAddedSlot;
    ITEM_DEFINITION *item;

    item          = gmItemGetDirectStub(itemId);
    lastAddedSlot = 0;
    replacedSlot  = gmInventoryFindEquippedStub(unitId, item->type);
    sySoundPlayStub(SE_SHOP_PURCHASE);
    for (i = 0; i < quantity; i++)
    {
        lastAddedSlot = gmInventoryAddItemStub(unitId, itemId);
        gmPartyAdjustCoinsStub(0 - item->price);
        gmPartyAdjustShopCreditStub(item->price);
        mnShopDrawMoney();
    }
    mnShopShowKeeperMessageHidingCursor(MsgHereYouGo);
    if (mnShopConfirmEquip(unitId, lastAddedSlot) != 0)
    {
        mnShopSellOld(unitId, replacedSlot);
    }
}

// Offers to equip the item just bought, and warns of a curse. Returns 1 when it was equipped.
Bool mnShopConfirmEquip(Sint32 unitId, Sint32 slot)
{
    SHOP_STATE      *shop   = RamHeapSlots->menuState;
    UNIT            *unit   = syUnitGet(unitId);
    Sint32           itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    ITEM_DEFINITION *item   = gmItemGetDirectStub(itemId);
    Sint32           replacedSlot;
    WINDOW          *itemWindow;

    if (unit->inventory[slot] & INVENTORY_EQUIPPED)
        return FALSE;

    if (gmItemCanUnitEquipDirectStub(unitId, itemId) == 0)
        return FALSE;

    replacedSlot = gmInventoryFindEquippedStub(unitId, item->type);
    if (replacedSlot != -1)
    {
        ITEM_DEFINITION *oldInfo = gmItemGetDirectStub(unit->inventory[replacedSlot]);

        if (oldInfo->flags & ITEM_CANNOT_UNEQUIP)
            return FALSE;
    }

    mnMessageArgumentPushStub(unitId, MESSAGE_ARGUMENT_ACTOR);
    mnShopShowKeeperMessage(MsgEquipNowPrompt);
    if (mnShopAskYesNo(0) != 0)
        return FALSE;

    gmInventoryEquipStub(unitId, slot);
    itemWindow = shop->itemWindow;
    if (itemWindow != 0)
        mnShopDrawMemberItems(itemWindow, unitId);

    if (item->flags & ITEM_CURSED)
    {
        sySoundPlayStub(SE_CURSE);
        mnMessageCloseFinishedWindowsStub();
        mnMessageOpenWindowStub(MsgBecameCursed, 8, 4, MESSAGE_WINDOW_UNREAD);
        while (mnMessageAllTypedStub() == 0)
        {
            sySynch(1);
        }
    }

    mnShopShowKeeperMessageHidingCursor(MsgLookBolder);
    return TRUE;
}

// Offers to sell the item the new one replaced, unless there is none or it is a Psynergy item or
// indispensable. Returns 1 when it offered.
Bool mnShopSellOld(Sint32 unitId, Sint32 slot)
{
    UNIT  *unit;
    Sint32 itemId;

    unit = syUnitGet(unitId);
    if (slot == -1)
    {
        return FALSE;
    }
    itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    if (gmItemGetDirectStub(itemId)->type == ITEM_TYPE_PSYNERGY)
    {
        return FALSE;
    }
    if (gmItemGetDirectStub(itemId)->flags & ITEM_INDISPENSABLE)
    {
        return FALSE;
    }
    mnShopSellItem(unitId, slot, -1);
    return TRUE;
}

// Returns what the shops pay for an inventory entry: nothing for an indispensable item, half the
// price for a broken one, three quarters otherwise.
Sint32 mnShopSalePrice(Sint32 inventoryEntry)
{
    Sint32 salePrice;

    salePrice = gmItemGetDirectStub(inventoryEntry)->price;

    if (gmItemGetDirectStub(inventoryEntry)->flags & ITEM_INDISPENSABLE)
    {
        salePrice = 0;
    }
    else if (inventoryEntry & INVENTORY_BROKEN)
    {
        salePrice = salePrice / 2;
    }
    else
    {
        salePrice = salePrice * 3 / 4;
    }
    return salePrice;
}
