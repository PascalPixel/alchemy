// The item picker: which party member's item to drop when every pack is full. Part of shop.c.

#include "shop_internal.h"

// Lets the player choose a party member, then one of their items that may be dropped, in the
// selling screen without its shopkeeper. Returns 0 with the choice in selectedUnit and
// selectedSlot, or -1 for B with both -1.
Sint32 mnItemPickerRun(Sint32 *selectedUnit, Sint32 *selectedSlot)
{
    SHOP_STATE *shop;
    WINDOW_SPR *cursorAnchor;
    WINDOW     *partyWindow;
    Sint32      memberIndex = 0;
    Bool        redraw      = TRUE;
    Sint32      unitId      = 0;
    Sint32      itemSlot;
    Sint32      result = 0;

    mnShopInitState();
    shop             = MenuState;
    shop->itemWindow = mnWindowCreateStub(16, 12, 14, 8, WINDOW_FLAG_INSTANT);
    partyWindow      = mnWindowCreateStub(0, 14, 13, 3, WINDOW_FLAG_INSTANT);
    cursorAnchor =
        mnWindowSpriteCreateStub(shop->cursorIcon, WINDOW_SPRITE_SIZE_16X16, partyWindow, 0, 0);
    cursorAnchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
    cursorAnchor->kind   = WINDOW_SPRITE_PLAIN;
    mnCursorSetPosImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursorAnchor;
    shop->cursorFrame   = SHOP_CURSOR_ENTER_TICKS;
    mnCreatePartyFiguresStub(partyWindow, 2, 0, 8, 0);

    while (1)
    {
        if (redraw != 0)
        {
            redraw      = FALSE;
            memberIndex = (memberIndex + shop->partyMemberCount) % shop->partyMemberCount;
            unitId      = shop->partyMemberIds[memberIndex];
            mnCursorPlace(partyWindow, memberIndex * 24 - 12, 0);
            shop->cursorFrame = 3;
            mnShopDrawParty(partyWindow, memberIndex, 0);
            mnShopDrawMemberItems(shop->itemWindow, unitId);
        }

        sySynch(1);
        if ((PerPush & KEY_A) != 0)
        {
            if (gmInventoryCountStub(unitId) == 0)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                continue;
            }

            sySoundPlayStub(SE_MENU_CONFIRM);
            itemSlot = mnItemPickerSelectItem(unitId);
            if (itemSlot == -1)
            {
                shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_WOBBLE;
                shop->cursorFrame           = SHOP_CURSOR_ENTER_TICKS;
                redraw                      = TRUE;
                continue;
            }
            *selectedUnit = unitId;
            *selectedSlot = itemSlot;
            result        = 0;
            break;
        }

        if ((PerPush & KEY_B) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            *selectedUnit = -1;
            *selectedSlot = -1;
            result        = -1;
            break;
        }

        if ((PerRepeat & KEY_LEFT) != 0)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            memberIndex--;
            redraw = TRUE;
        }
        if ((PerRepeat & KEY_RIGHT) != 0)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            memberIndex++;
            redraw = TRUE;
        }
    }

    mnReleasePartyFiguresStub();
    mnWindowCloseStub(partyWindow, 2);
    mnWindowCloseStub(shop->itemWindow, 2);
    sySynch(1);
    mnShopReleaseState();
    return result;
}

// Where the line under the item draws "Price", the stack's sale value and the coins word, in
// pixels, and how far back from MsgPriceHeading the coins word is: MsgCoins, or in Japanese the
// message just before.
#define ITEM_PICKER_PRICE_LABEL_X        GERMAN_OR(0, 8)
#define ITEM_PICKER_PRICE_NUMBER_X       BY_LANGUAGE(40, 40, 32, 40, 32, 40)
#define ITEM_PICKER_COINS_LABEL_X        BY_LANGUAGE(80, 80, 72, 88, 72, 80)
#define ITEM_PICKER_COINS_MESSAGE_OFFSET JAPANESE_OR(1, 5)

// Lets the player choose one of the party member's items to drop, refusing an indispensable one
// with a message and an equipped cursed one with only the cancel sound (the game's bug; BUGFIX
// shows its message, see SHOP_CANNOT_REMOVE_STATUS). Returns the slot, or -1 when cancelled.
Sint32 mnItemPickerSelectItem(Sint32 unitId)
{
    SHOP_STATE *shop = MenuState;
    WINDOW     *descriptionWindow;
    WINDOW     *priceWindow;
    UNIT       *unit;
    Sint32      slot;
    Bool        redraw;
    Sint32      inventoryCount;
    Sint32      selectionResult;
    Sint32      discardStatus;
    Sint32      itemId;
    WINDOW     *window;
    Sint32      x;
    Sint32      y;

    unit            = gmUnitGetStub(unitId);
    inventoryCount  = 1;
    selectionResult = 0;
    redraw          = TRUE;
    priceWindow     = mnWindowCreateStub(
        ITEM_PICKER_WINDOW_X, 8, ITEM_PICKER_WINDOW_WIDTH, 4, WINDOW_FLAG_INSTANT);
    descriptionWindow           = mnWindowCreateStub(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
    shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
    shop->cursorFrame           = SHOP_CURSOR_ENTER_TICKS;
    slot                        = 0;

    while (1)
    {
        if (redraw != 0)
        {
            redraw         = FALSE;
            inventoryCount = gmInventoryCountStub(unitId);
            if (slot > inventoryCount - 1)
                slot = inventoryCount - 1;

            itemId = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
            window = shop->itemWindow;
            x      = slot % SHOP_INVENTORY_COLUMNS << 4;
            y      = (slot / SHOP_INVENTORY_COLUMNS << 4) + 8;
            mnCursorPlace(window, x, y);
            shop->cursorFrame = 3;
            mnItemPickerDrawItem(priceWindow, unitId, slot);
            mnShopDrawMessage(descriptionWindow, itemId + MsgItemPlainName);
        }

        sySynch(1);

        if ((PerPush & KEY_A) != 0)
        {
            discardStatus = gmInventoryCheckDiscardStub(unitId, slot);
            if (discardStatus == INVENTORY_DISCARD_ALLOWED)
            {
                sySoundPlayStub(SE_MENU_CONFIRM);
                selectionResult = slot;
                break;
            }
            // BUG: default code selects the error using selectionResult, still zero.
            if (discardStatus == INVENTORY_DISCARD_INDISPENSABLE)
            {
                // "You can't drop it!"
                mnMessageOpenWindowStub(MsgCannotDropIt, 8, 1, MESSAGE_WINDOW_UNREAD);
            }
            else if (SHOP_CANNOT_REMOVE_STATUS(discardStatus, selectionResult)
                     == INVENTORY_DISCARD_STUCK)
            {
                // "You can't remove it!"
                mnMessageOpenWindowStub(MsgCannotRemoveIt, 8, 1, MESSAGE_WINDOW_UNREAD);
            }
            sySoundPlayStub(SE_MENU_CANCEL);
            while (mnMessageAllTypedStub() == 0)
                sySynch(1);
            mnMessageCloseFinishedWindowsStub();
            continue;
        }

        if ((PerPush & KEY_B) != 0)
        {
            sySoundPlayStub(SE_MENU_CANCEL);
            selectionResult = -1;
            break;
        }

        if ((PerRepeat & KEY_LEFT) != 0)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            slot--;
            slot   = (slot + inventoryCount) % inventoryCount;
            redraw = TRUE;
        }
        if ((PerRepeat & KEY_RIGHT) != 0)
        {
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            slot++;
            slot   = (slot + inventoryCount) % inventoryCount;
            redraw = TRUE;
        }
        if ((PerRepeat & KEY_UP) != 0)
        {
            slot -= SHOP_INVENTORY_COLUMNS;
            if (slot < 0)
                slot += INVENTORY_SLOTS;
            while (slot >= inventoryCount)
                slot -= SHOP_INVENTORY_COLUMNS;
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            redraw = TRUE;
        }
        if ((PerRepeat & KEY_DOWN) != 0)
        {
            slot += SHOP_INVENTORY_COLUMNS;
            if (slot >= inventoryCount)
                slot -= INVENTORY_SLOTS;
            while (slot < 0)
                slot += SHOP_INVENTORY_COLUMNS;
            sySoundPlayStub(SE_MENU_CURSOR_MOVE);
            redraw = TRUE;
        }
    }

    mnWindowCloseStub(descriptionWindow, 2);
    mnWindowCloseStub(priceWindow, 2);
    sySynch(1);
    return selectionResult;
}

// Draws the slot's item name and, below it, why it cannot be dropped or what the whole stack would
// sell for.
void mnItemPickerDrawItem(WINDOW *window, Sint32 unitId, Sint32 slot)
{
    UNIT  *unit           = gmUnitGetStub(unitId);
    Sint32 itemId         = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    Uint32 inventoryEntry = unit->inventory[slot];
    Sint32 quantity       = (inventoryEntry >> INVENTORY_QUANTITY_SHIFT) + 1;

    if (window != 0)
    {
        Sint32 discardStatus;

        mnWindowSpriteRedrawSavedRectStub(window);
        mnTextDrawMessageAsTilesStub(itemId + MsgItemName, window, 0, 0);

        discardStatus = gmInventoryCheckDiscardStub(unitId, slot);
        if (discardStatus == INVENTORY_DISCARD_INDISPENSABLE)
        {
            mnTextDrawMessageAsTilesStub(MsgCannotDrop, window, SHOP_ITEM_NOTE_X, 8);
        }
        else if (discardStatus == INVENTORY_DISCARD_STUCK)
        {
            mnTextDrawMessageAsTilesStub(MsgCannotRemove, window, SHOP_ITEM_NOTE_X, 8);
        }
        else
        {
            Sint32 unitSalePrice;
            Sint32 stackSalePrice;

            unitSalePrice  = mnShopSalePrice(unit->inventory[slot]);
            stackSalePrice = quantity * unitSalePrice;

            mnTextDrawMessageAsTilesStub(MsgPriceHeading, window, ITEM_PICKER_PRICE_LABEL_X, 8);
            mnTextDrawNumberInWindowStub(stackSalePrice, 5, window, ITEM_PICKER_PRICE_NUMBER_X, 8);
            mnTextDrawMessageAsTilesStub(MsgPriceHeading - ITEM_PICKER_COINS_MESSAGE_OFFSET,
                                         window,
                                         ITEM_PICKER_COINS_LABEL_X,
                                         8);
        }
    }
}

#undef ITEM_PICKER_PRICE_LABEL_X
#undef ITEM_PICKER_PRICE_NUMBER_X
#undef ITEM_PICKER_COINS_LABEL_X
#undef ITEM_PICKER_COINS_MESSAGE_OFFSET
