#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SYSTEM.H"
#include "UI.H"
#include "NODE_CHAIN.H"
#include "DMA.H"
#include "RESOURCE.H"
#include "SHOP.H"
#include "IO_REG.H"
#include "GAME_STATE.H"
#include "SOUND_IDS.H"
#include "TBS_EDITION.H"
#include "ITEM.H"
#include "text/MSG_IDS.H"
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)

#define BattleFx_GetResourceIdFar ((s32 (*)(s32))BattleFx_GetResourceIdFar)
#define UiIcon_PrepareObjectFar ((void (*)(void*))UiIcon_PrepareObjectFar)
#define Shop_BuySpecialItem ((void (*)(s32,s32))Shop_BuySpecialItem)
#define Item_AdjustCounterFar ((void (*)(s32,s32))Item_AdjustCounterFar)

void UiWork_FinalizePendingCoreFar(void);

struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

extern u8 gEventWork[];

struct SpriteAttr {
    unsigned y : 8;
    unsigned affine : 2;
    unsigned blend_mode : 2;
    unsigned mosaic : 1;
    unsigned full_color : 1;
    unsigned shape : 2;
    unsigned x : 9;
    unsigned affine_index : 5;
    unsigned size : 2;
};

struct SpriteAttr2 {
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

struct ShopCursorSprite3 {
    struct RenderOutput *next;
    u8 unknown_04[2];
    u16 x;
    u16 y;
    u8 unknown_0a[0x0a];
    struct SpriteAttr2 oam;
};

extern struct ShopRuntime *gMenuWork;

void UiMessage_ShowAndWait(s32 arg0)
{
    struct ShopRuntime *state = gMenuWork;
    s32 value = BattleFx_GetResourceIdFar(state->keeper_resource);
    s32 result = arg0;
    s8 mode;

    UiWork_FinalizePendingCoreFar();
    mode = state->shop_type;
    if (mode == 2)
        result += MsgArmorShopWelcome - MsgWeaponShopWelcome;
    if (mode == 0)
        result += MsgItemShopWelcome - MsgWeaponShopWelcome;
    if ((s8)state->warrior_shop != 0)
        result += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
    UiText_OpenMessageWindowFar(result, 5, 0, (value << 16) | 0x22);
    while (UiWork_IsCompleteFar() == 0)
        WaitFrames(1);
    WaitFrames(1);
}

void UiMessage_ShowAndRestoreState(s32 message_id)
{
    s32 variant;
    s32 no;
    s8 mode;
    s8 flag;
    u8 saved;
    struct ShopRuntime *state;
    struct RenderOutput **slot;

    state = gMenuWork;
    slot = &state->cursor.anchor;
    saved = (*slot)->active;
    no = message_id;
    variant = BattleFx_GetResourceIdFar(state->keeper_resource);
    mode = state->shop_type;
    if (mode == 2) {
        no += MsgArmorShopWelcome - MsgWeaponShopWelcome;
    }
    if (mode == 0) {
        no += MsgItemShopWelcome - MsgWeaponShopWelcome;
    }
    flag = state->warrior_shop;
    if (flag != 0) {
        no += MsgWarriorShopWelcome - MsgWeaponShopWelcome;
    }
    (*slot)->active = 0xDU;
    UiWork_FinalizePendingCoreFar();
    UiText_OpenMessageWindowFar(no, 5, 0, (variant << 0x10) | 0x22);
    while (UiWork_IsCompleteFar() == 0) {
        WaitFrames(1U);
    }
    WaitFrames(1U);
    state->cursor.anchor->active = saved;
}

s32 UiMessage_ShowChoice(s32 arg0)
{
    struct RenderOutput **slot = &gMenuWork->cursor.anchor;
    u8 saved = (*slot)->active;
    UiIcon_PrepareObjectFar(*slot);
#if defined(TBS_EDITION_DE)
    arg0 = Menu_RunConfirmSelectionAtFar(6, 5, arg0);
#else
    arg0 = Menu_RunConfirmSelectionAtFar(7, 5, arg0);
#endif
    (*slot)->active = saved;
    return arg0;
}

s32 UiMessage_ShowChoiceVariant(s32 arg0)
{
    struct RenderOutput **slot = &gMenuWork->cursor.anchor;
    u8 saved = (*slot)->active;
    UiIcon_PrepareObjectFar(*slot);
    arg0 = Menu_RunConfirmSelectionAtFar(7, 7, arg0);
    (*slot)->active = saved;
    return arg0;
}

struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}

void Shop_FillSelector(s32 count, s32 selector, u8 *base)
{
    u32 shifted = selector << 4;
    u16 *offset;

    selector = shifted + 1;

    if (count > 0) {
        offset = Shop_SelectorTileOffsets;
        do {
            struct Record_080b06c0 *record = (struct Record_080b06c0 *)(base + *offset++);
            record->values[0] = selector;
            record->values[4] = selector;
            record->values[8] = selector;
            record->values[12] = selector;
            record->values[16] = selector;
            record->values[20] = selector;
            count--;
        } while (count != 0);
    }
}

void Shop_CopyGlyphs(s32 arg0, u8 *arg1, u32 arg2)
{
    u8 *src = Shop_GlyphBytes + (arg0 << 5);
    u8 *dst =
        arg1 + Shop_GlyphOffsets[arg2] + 2;
    s32 count = 3;

    do {
        if (*src != 0) {
            dst[0] = *src++;
            if (*src != 0) {
                dst[1] = *src++;
                if (*src != 0) {
                    dst[30] = *src++;
                    if (*src != 0) dst[31] = *src++;
                }
            }
        }
        dst += 4;
        count--;
    } while (count >= 0);
}

struct RenderOutput *Shop_CreatePriceSprite(s32 value, s32 window, s32 x, s32 y)
{
    u8 *buf;
    s32 slot;
    struct RenderOutput *sprite;

    buf = Runtime_AllocateBlock(14, 0x400);
    sprite = 0;
    Dma_Set(Shop_PriceTiles, buf, 0x84000040, REG_DMA3);
    Shop_CopyGlyphs(value % 10, buf, 0);
    value = value / 10;
    if (value != 0) {
        Shop_CopyGlyphs(value % 10, buf, 1);
        value = value / 10;
        if (value != 0) {
            Shop_CopyGlyphs(value % 10, buf, 2);
            value = value / 10;
            if (value != 0) {
                s32 last;

                Shop_CopyGlyphs(value % 10, buf, 3);
                last = value / 10;
                if (last != 0)
                    Shop_CopyGlyphs(last % 10, buf, 4);
            }
        }
    }
    slot = Resource_FindFreeEntry();
    if (slot != 96) {
        VramBlock_LoadCached(slot, 0x100, buf);
        sprite = RenderOutput_CreateFar(slot, 0x80008000, (struct RenderInput *)window, x, y);
    }
    Runtime_ReleaseHeapBlock(14);
    return sprite;
}

void Shop_RestoreSceneTiles(s32 layer)
{
    u32 *scene = (u32 *)gEventWork;
    u8 *work = (u8 *)scene[5];
    u8 *map = (u8 *)scene[0];
    u8 *saved = work + 0xe00;

    Dma_Set(saved, map + 0x236, 0x84000150, REG_DMA3);
    Dma_Set(saved, work + 0x380, 0x840002a0, REG_DMA3);
    BattleFx_ApplyColorToTargetBufferFar(layer, 1);
    BattleFx_StartBufferInterpolationFar(16);
}

void Shop_InitEffect(void)
{
    BattleFx_ApplyColorToTargetBufferFar(*(s32 *)gEventWork + 0x236, 1);
    BattleFx_StartBufferInterpolationFar(0x10);
}

void ShopCursor_Advance(struct ShopCursor *cursor)
{
    struct RenderOutput *sprite;
    s32 kind;
    s32 step;
    s32 x;
    s32 y;

    if (cursor == NULL)
        return;
    kind = cursor->kind;
    if (kind == 0)
        return;
    sprite = cursor->anchor;
    step = ++cursor->active;
    x = cursor->x + (cursor->target_x - (s16)cursor->x) * step / kind;
    sprite->x = x;
    ((struct SpriteAttr *)&sprite->packed)->x = x;
    y = cursor->y + (cursor->target_y - (s16)cursor->y) * step / kind;
    sprite->y = y;
    ((struct SpriteAttr *)&sprite->packed)->y = y;
    if (step == kind) {
        cursor->kind = 0;
        cursor->active = 0;
    }
}

void ShopCursor_MoveTowardTarget(struct ShopCursor *cursor)
{
    struct RenderOutput *sprite;
    s32 delta;
    s32 step;

    sprite = cursor->anchor;
    if (sprite != NULL) {
        delta = (u16)sprite->x - cursor->target_x;
        step = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0) {
            if (step != 0)
                sprite->x = sprite->x - step;
            else
                sprite->x = (u16)sprite->x - 1;
        } else {
            if (delta >= 0)
                goto move_y;
            if (step != 0)
                sprite->x = sprite->x + step;
            else
                sprite->x = sprite->x + 1;
        }
        ((struct SpriteAttr2 *)&sprite->packed)->x = sprite->x;

move_y:
        delta = (u16)sprite->y - cursor->target_y;
        step = delta / 4;
        if (step < 0)
            step = -step;
        if (delta > 0) {
            if (step != 0)
                sprite->y = sprite->y - step;
            else
                sprite->y = (u16)sprite->y - 1;
        } else {
            if (delta >= 0)
                return;
            if (step != 0)
                sprite->y = sprite->y + step;
            else
                sprite->y = sprite->y + 1;
        }
        *(u8 *)&sprite->packed = sprite->y;
    }
}

void Shop_SetCursor(
    struct ShopCursor *cursor,
    s32 target_x,
    s32 target_y,
    s8 kind)
{
    struct RenderOutput *anchor = cursor->anchor;

    cursor->x = anchor->x;
    cursor->y = anchor->y;
    cursor->target_x = target_x;
    cursor->target_y = target_y;
    cursor->kind = kind;
    cursor->active = 0;
}

void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 x, s32 y)
{

    {
        struct ShopCursorSprite3 *sprite = (struct ShopCursorSprite3 *)cursor->anchor;

        cursor->target_x = x;
        cursor->kind = 1;
        cursor->active = 0;
        cursor->x = x;
        sprite->oam.x = sprite->x = x;
    }
    {
        struct ShopCursorSprite3 *sprite;

        cursor->target_y = y;
        cursor->y = y;
        sprite = (struct ShopCursorSprite3 *)cursor->anchor;
        sprite->oam.y = sprite->y = y;
    }
}

void Shop_PlaceCursor(void *window, s32 x, s32 y)
{
    s32 cursor_x;
    s32 cursor_y;
    struct ShopRuntime *shop;

    cursor_x = x;
    cursor_y = y;
    shop = gMenuWork;
    if (window != NULL) {
        cursor_x = cursor_x + (((struct RenderInput *)window)->x * 8) + 8;
        cursor_y = cursor_y + (((struct RenderInput *)window)->y * 8) + 8;
    }
    Shop_SetCursor(
        &shop->cursor,
        cursor_x,
        cursor_y,
        shop->mode);
}

void UiWork_FinalizeFar(s32 window, s32 style);
void UiWork_PushValueSlotFar(s32 value, s32 slot);
void PsynergyMenu_InitializeEntryObjectsFar(s32 window, s32 column, s32 row, s32 height, s32 flags);
void AudioCommand_PlayFar(s32 cue);

s32 Shop_SelBuy(void)
{
    struct ShopRuntime *shop = gMenuWork;
    s32 price_window = 0;
    s32 stock_window;
    s32 message_window;
    s32 party_window;
    s32 selected = 0;
    s32 quantity;
    struct ItemDefinition *item;
    s32 ask_again;
    s32 item_window;
    s32 redraw;
    s32 cursor;
    s32 count;
    s32 item_id;
    struct ItemDefinition *stock_item;
    s32 result;
    s32 prev;
    s32 member;
    s32 slot;
    s32 i;
    s16 *ids;

    shop->item_window = 0;
    shop->money_window = UiWindow_CreateFar(30 - SHOP_MONEY_WIDTH, 7, SHOP_MONEY_WIDTH, 4, 2);
    Shop_DrawMoney();
    price_window = UiWindow_CreateFar(0, 8, SHOP_PRICE_WIDTH, 4, 2);

next_page:
    cursor = selected;
    stock_window = UiWindow_CreateFar(0, 12, 30, 4, 2);
    shop->cursor.anchor->active = 18;
    shop->mode = 12;
    message_window = UiWindow_CreateFar(0, 17, 30, 3, 2);
    redraw = 1;

    for (;;) {
        count = shop->stock_count;
        if (redraw != 0) {
            ids = shop->stock_item_ids;
            item_id = ids[cursor];
            stock_item = Item_GetDirectFar(item_id);
            redraw = 0;
            Shop_PlaceCursor((void *)stock_window, cursor % 7 * 32 - 8, 8);
            shop->mode = 4;
            Shop_DrawStock(stock_window, cursor);
            Shop_DrawMsg(message_window, item_id + (s32)MsgItemPlainName);
            RenderOutput_RedrawSavedRectFar(price_window);
            Shop_DrawItemPrice(price_window, item_id, stock_item->price, 0);
        }
        if ((gKeyState & KEY_A) != 0) {
            selected = cursor;
            AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
            result = 0;
            goto stock_done;
        }
        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            result = -1;
            goto stock_done;
        }
        if ((gKeysRepeat & KEY_LEFT) != 0) {
            prev = cursor;
            cursor--;
            cursor = (cursor + count) % count;
            if (prev != cursor) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            prev = cursor;
            cursor++;
            cursor = (cursor + count) % count;
            if (prev != cursor) {
                AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
            }
        }
        if ((gKeysRepeat & KEY_UP) != 0 && cursor - 7 >= 0) {
            cursor -= 7;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_DOWN) != 0) {
            if (cursor + 7 < (count + 6) / 7 * 7) {
                cursor += 7;
                redraw = 1;
            }
            if (cursor > count - 1)
                cursor = count - 1;
        }
        WaitFrames(1);
    }

stock_done:
    UiWork_FinalizeFar(message_window, 2);
    UiWork_FinalizeFar(stock_window, 2);
    WaitFrames(1);
    if (result != 0)
        goto close;

    shop->selected_item = shop->stock_item_ids[selected];
    UiMessage_ShowAndWait((s32)MsgWhoGetsThis);
    item = Item_GetDirectFar(shop->selected_item);
    quantity = 1;
    member = 0;
    ask_again = 0;
    cursor = 0;
    party_window = UiWindow_CreateFar(0, 14, 13, 3, 2);
    shop->cursor.anchor->active = 4;
    shop->mode = 12;
    PsynergyMenu_InitializeEntryObjectsFar(party_window, 2, 0, 8, 0);
    item_window = UiWindow_CreateFar(16, 11, 14, 9, 2);
    redraw = 1;

    for (;;) {
        if (ask_again != 0) {
            ask_again = 0;
            UiMessage_ShowAndWait((s32)MsgWhoGetsThis);
            redraw = 1;
        }
        if (redraw != 0) {
            redraw = 0;
            cursor = (cursor + shop->party_member_count) % shop->party_member_count;
            member = shop->party_member_ids[cursor];
            Shop_PlaceCursor((void *)party_window, cursor * 24 - 12, 0);
            shop->mode = 3;
            Shop_DrawParty(party_window, cursor, shop->selected_item);
            if (Item_GetEquipmentGroupFar(shop->selected_item) == 0)
                Shop_DrawUnitItem(item_window, member, shop->selected_item);
            else
                Shop_DrawEquipComparison(item_window, member, shop->selected_item);
        }
        if ((gKeyState & KEY_A) != 0) {
            slot = Inventory_AddItemFar(member, shop->selected_item);
            if (slot < 0) {
                AudioCommand_PlayFar(SOUND_MENU_CANCEL);
                UiWork_PushValueSlotFar(member, 1);
                UiWork_PushValueSlotFar(shop->selected_item, 2);
                if (Inventory_CountFar(member) == 15)
                    UiMessage_ShowAndWait((s32)MsgPackFull);
                else
                    UiMessage_ShowAndWait((s32)MsgCannotCarryItem);
                continue;
            }
            Inventory_RemoveFar(member, slot);
            if ((u32)item->price > gGameState.coins) {
                AudioCommand_PlayFar(SOUND_MENU_CANCEL);
                UiMessage_ShowAndRestoreState((s32)MsgBuyNotEnoughMoney);
                result = -1;
                goto party_done;
            }
            if (Item_IsCompatibleWithOwnerFar(member, shop->selected_item) == 0) {
                UiWork_PushValueSlotFar(member, 1);
                UiMessage_ShowAndWait((s32)MsgCannotEquipStillWant);
                if (UiMessage_ShowChoice(0) != 0) {
                    ask_again = 1;
                    continue;
                }
            }
            AudioCommand_PlayFar(SOUND_MENU_CONFIRM);
            WaitFrames(1);
            quantity = Shop_SelBuyNum(member, shop->selected_item);
            ask_again = 1;
            if (quantity == -1)
                continue;
            Shop_BuyDone(member, shop->selected_item, quantity);
            Shop_BuySpecialItem(party_window, item_window);
            result = 0;
            goto party_done;
        }
        if ((gKeyState & KEY_B) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CANCEL);
            result = -1;
            goto party_done;
        }
        if ((gKeysRepeat & KEY_LEFT) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            cursor--;
            redraw = 1;
        }
        if ((gKeysRepeat & KEY_RIGHT) != 0) {
            AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
            cursor++;
            redraw = 1;
        }
        WaitFrames(1);
    }

party_done:
    Menu_ReleaseEntryObjectsFar();
    UiWork_FinalizeFar(item_window, 2);
    UiWork_FinalizeFar(party_window, 2);
    WaitFrames(1);
    if (result == 0 && shop->party_action == 2) {
        for (i = 0; i < quantity; i++)
            Item_AdjustCounterFar(shop->selected_item, -1);
        if (AbilityMenu_BuildAvailableList() == 0)
            goto close;
        if (selected > shop->stock_count - 1)
            selected = shop->stock_count - 1;
    }
    UiMessage_ShowAndWait((s32)MsgBuyAnythingElse);
    goto next_page;

close:
    UiWork_FinalizeFar(price_window, 2);
    UiWork_FinalizeFar(shop->money_window, 2);
    WaitFrames(1);
    return 0;
}
