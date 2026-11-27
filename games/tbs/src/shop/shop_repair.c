// Repairing broken items. Part of shop.c.

#include "shop_internal.h"

// Returns what a repair costs: a quarter of the price for a broken item, nothing for one that is
// not broken.
Sint32 mnShopRepairPrice(Sint32 inventoryEntry)
{
    Sint32 repairCost = gmItemGetDirectStub(inventoryEntry)->price / 4;

    if ((inventoryEntry & INVENTORY_BROKEN) == 0)
    {
        repairCost = 0;
    }
    return repairCost;
}

// Runs the repair counter's party member choice; always returns 0.
Sint32 mnShopRunRepairMemberChoice(void)
{
    mnShopChooseMember();
    return 0;
}

// Lets the player choose items to repair from the party member's grid, one at a time, with each
// one's repair price, until B or the inventory is empty. Returns -1 when cancelled, 0 when the
// inventory ran out.
Sint32 mnShopSelectRepair(Sint32 unitId)
{
    SHOP_STATE *shop;
    Sint32      inventoryCount;
    WINDOW     *descriptionWindow;
    WINDOW     *priceWindow;
    UNIT       *unit;
    Sint32      slot;
    Bool        redraw;
    Sint32      result;
    Sint32      itemId;
    Sint32      repairCost;
    WINDOW     *window;
    Sint32      x;
    Sint32      y;

    shop           = MenuState;
    unit           = gmUnitGetStub(unitId);
    inventoryCount = 1;
    priceWindow =
        mnWindowCreateStub(SHOP_SELL_PRICE_X, 8, SHOP_SELL_PRICE_WIDTH, 4, WINDOW_FLAG_INSTANT);
    slot = 0;

    for (;;)
    {
        descriptionWindow           = mnWindowCreateStub(0, 5, 30, 3, WINDOW_FLAG_INSTANT);
        shop->cursor.anchor->motion = WINDOW_SPRITE_MOTION_HOP_DOWN_LEFT;
        shop->cursorFrame           = SHOP_CURSOR_ENTER_TICKS;
        redraw                      = TRUE;

        while (1)
        {
            if (redraw != 0)
            {
                redraw         = FALSE;
                inventoryCount = gmInventoryCountStub(unitId);
                if (slot > inventoryCount - 1)
                    slot = inventoryCount - 1;
                itemId = INVENTORY_ITEM_ID_MASK & unit->inventory[slot];
                window = shop->itemWindow;
                x      = slot % SHOP_INVENTORY_COLUMNS * 16;
                y      = slot / SHOP_INVENTORY_COLUMNS * 16 + 8;
                mnCursorPlace(window, x, y);
                shop->cursorFrame = 3;
                repairCost        = mnShopRepairPrice(unit->inventory[slot]);
                mnShopDrawItemPrice(priceWindow, itemId, repairCost, SHOP_PRICE_KIND_REPAIR);
                mnShopDrawMessage(descriptionWindow, itemId + MsgItemPlainName);
            }
            if ((PerPush & KEY_A) != 0)
            {
                sySoundPlayStub(SE_MENU_CONFIRM);
                result = 0;
                break;
            }
            if ((PerPush & KEY_B) != 0)
            {
                sySoundPlayStub(SE_MENU_CANCEL);
                result = -1;
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
            sySynch(1);
        }

        mnWindowCloseStub(descriptionWindow, 2);
        sySynch(1);
        if (result != 0)
            break;

        mnShopRepairItem(unitId, slot);
        // "Anything else need repairing?"
        mnShopShowKeeperMessage(MsgRepairAnythingElse);
        if (gmInventoryCountStub(unitId) == 0)
            break;
    }

    mnWindowCloseStub(priceWindow, 2);
    return result;
}

// Repairs the item in `slot`: refuses one that cannot break, is not broken or is equipped and
// cursed on, or that the coins do not cover; asks, then takes the item away while three clangs
// sound, gives it back mended, charges the price and offers to equip it.
void mnShopRepairItem(Sint32 unitId, Sint32 slot)
{
    SHOP_STATE      *shop       = MenuState;
    UNIT            *unit       = gmUnitGetStub(unitId);
    Sint32           itemId     = unit->inventory[slot] & INVENTORY_ITEM_ID_MASK;
    ITEM_DEFINITION *item       = gmItemGetDirectStub(itemId);
    Sint32           equipped   = gmInventoryFindEquippedStub(unitId, item->type);
    Uint32           repairCost = mnShopRepairPrice(unit->inventory[slot]);
    Sint32           msgBase;
    Uint8            useType;
    Uint32           savedEntry;

    useType = item->useType;
    if (useType != ITEM_USE_MAY_BREAK)
    {
        mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
        // "Sorry, but we don't handle (item)s."
        mnShopShowKeeperMessageHidingCursor(MsgCannotRepairKind);
        return;
    }
    if (!(unit->inventory[slot] & INVENTORY_BROKEN))
    {
        mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
        // "Huh? That (item) isn't damaged at all."
        mnShopShowKeeperMessageHidingCursor(MsgNotDamaged);
        return;
    }
    // useType is ITEM_USE_MAY_BREAK here, so this tests ITEM_CANNOT_UNEQUIP.
    if ((unit->inventory[slot] & INVENTORY_EQUIPPED) && (useType & item->flags))
    {
        mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
        // "This (item) won't come off. Bring it back after you get it off."
        mnShopShowKeeperMessageHidingCursor(MsgCursedWontComeOff);
        return;
    }
    if (repairCost > MainRoutineStateWork.holdings.coins)
    {
        // "Uh-oh, you don't seem to have enough money."
        mnShopShowKeeperMessageHidingCursor(MsgNotEnoughMoney);
        return;
    }
    mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
    mnMessageArgumentPushStub(repairCost, MESSAGE_ARGUMENT_NUMBER);
    msgBase = MsgShopRepairConfirm;
    // "I'll fix your (item) for (price) coins, OK?"
    mnShopShowKeeperMessageHidingCursor(msgBase);
    if (mnShopAskYesNo(0) != 0)
    {
        // "Changed your mind? Whatever you say!"
        mnShopShowKeeperMessageHidingCursor(msgBase + 1);
        return;
    }
    // Hide the item while it is repaired, without compacting its slot.
    savedEntry            = unit->inventory[slot];
    unit->inventory[slot] = 0;
    mnShopDrawMemberItems(shop->itemWindow, unitId);
    mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
    // "Yes, sir! I'll take your (item), then."
    mnShopShowKeeperMessageHidingCursor(msgBase + 2);
    mnMessageCloseFinishedWindowsStub();
    // Six seconds of work: three clangs 110 ticks apart, then the menu's confirm sound.
    sySynch(10);
    sySoundPlayStub(SE_BELL);
    sySynch(110);
    sySoundPlayStub(SE_BELL);
    sySynch(110);
    sySoundPlayStub(SE_BELL);
    sySynch(110);
    sySoundPlayStub(SE_MENU_CONFIRM);
    sySynch(20);
    unit->inventory[slot] = savedEntry;
    gmInventoryRepairStub(unitId, slot);
    gmPartyAdjustCoinsStub(-repairCost);
    mnShopDrawMoney();
    mnShopDrawMemberItems(shop->itemWindow, unitId);
    mnMessageArgumentPushStub(itemId, MESSAGE_ARGUMENT_ITEM);
    // "I've repaired your (item). Here it is."
    mnShopShowKeeperMessageHidingCursor(msgBase + 3);
    if (mnShopConfirmEquip(unitId, slot))
        mnShopSellOld(unitId, equipped);
}
