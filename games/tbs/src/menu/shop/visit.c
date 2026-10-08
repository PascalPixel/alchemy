#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "party_state.h"
#include "btlsys.h"
#include "object_runtime.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "shop.h"
#include "ui.h"
#include "tbs_edition.h"
#include "system.h"
#include "io_reg.h"
#include "text/msg_ids.h"
s32 AudioCommand_GetStateByteFar(void);

#define Inventory_EquipFar ((void (*)(s32,s32))Inventory_EquipFar)

extern struct ShopRuntime *gMenuWork;

void Battle_ResetEffectCounterFar(void);
s32 UiWork_FinalizePendingCoreFar();

struct ObjectRuntime *Object_GetByIdFar(s32 id);
void UiWork_FinalizeFar(s32 window, s32 style);

s32 AbilityMenu_BuildAvailableList(void)
{
    struct ShopRuntime *state;
    s16 *output;
    s32 index;
    s32 count;
    s8 mode;

    state = gMenuWork;
    count = 0;
    index = 0;
    output = state->stock_item_ids;
    do {
        mode = state->shop_type;
        if (mode == Item_GetEquipmentGroupFar(index) &&
            Item_AdjustCounterFar(index, 0) != 0) {
            *output = index;
            count++;
            output++;
        }
        index++;
    } while (index <= 0x1ff);
    state->stock_item_ids[count] = 0;
    state->stock_count = count;
    return count;
}

void Shop_StepCursor(void)
{
    ShopCursor_Advance(&gMenuWork->cursor);
}

void Shop_InitializeCursorWork(void)
{
    struct ShopRuntime *work;
    volatile s32 zero;
    s32 slot;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    Battle_ResetEffectCounterFar();
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500029c, REG_DMA3);
    work->mode = 12;
    work->party_member_count = Party_ListActiveOwnersFar(work->party_member_ids);
    slot = Resource_FindFreeEntry();
    work->cursor_icon = slot;
    VramBlock_LoadCached(slot, 128, Shop_HandTiles);
    slot = Resource_FindFreeEntry();
    work->previous_page_icon = slot;
    VramBlock_LoadCached(slot, 128, Shop_UpArrowTiles);
    slot = Resource_FindFreeEntry();
    work->next_page_icon = slot;
    VramBlock_LoadCached(slot, 128, Shop_DownArrowTiles);
    slot = Resource_FindFreeEntry();
    work->gem_icon = slot;
    VramBlock_LoadCached(slot, 128, Shop_GemTiles);
    slot = Resource_FindFreeEntry();
    work->stat_down_icon = slot;
    VramBlock_LoadCached(slot, 128, Shop_SmallDownArrowTiles);
    slot = Resource_FindFreeEntry();
    work->stat_up_icon = slot;
    VramBlock_LoadCached(slot, 128, Shop_SmallUpArrowTiles);
    Scheduler_AddOrUpdateCallback(Shop_StepCursor, 0xc80);
}

void Inn_Cleanup(void)
{
    struct ShopRuntime *state;

    state = gMenuWork;
    Scheduler_RemoveCallback(Shop_StepCursor);
    UiWork_FinalizePendingCoreFar();
    Resource_ResetEntry(state->cursor_icon);
    Resource_ResetEntry(state->previous_page_icon);
    Resource_ResetEntry(state->next_page_icon);
    Resource_ResetEntry(state->gem_icon);
    Resource_ResetEntry(state->stat_up_icon);
    Resource_ResetEntry(state->stat_down_icon);
    Runtime_ReleaseHeapBlock(0x37);
}

s32 Shop_Run(s32 row, s32 keeper_id)
{
    struct ShopRuntime *shop;
    struct RenderOutput *anchor;
    s32 window;
    s32 choice = 0;

    if (row >= EventTable_GetRowLimit() || row < 0)
        row = 0;
    EventTable_ApplyRowAbilities(row);
    Shop_InitializeCursorWork();
    shop = ((struct ShopRuntime *)gMenuWork);
    shop->shop_type = EventTable_GetRowType(row);
    if (row == 16)
        shop->warrior_shop = 1;
    if (row == 17)
        shop->warrior_shop = 1;
    if (row == 18)
        shop->warrior_shop = 1;
    shop->keeper_resource = ((struct AnimationObject *)Object_GetByIdFar(keeper_id)->animation)->entries[0]->anim_id;
    window = UiWindow_CreateWithSideObjectFar(shop->keeper_resource, 0, 0, 0);
    if (window == 0)
        window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    anchor = RenderOutput_CreateFar(shop->cursor_icon, 0x40000000, (struct RenderInput *)window, 0, 0);
    anchor->active = 1;
    anchor->kind = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = anchor;
    UiMessage_ShowAndWait((s32)MsgWeaponShopWelcome);
loop:
    {
        choice = Menu_SelectEntry11To14Far(choice);
        shop->party_action = choice;
        if (choice == 0) {
            shop->stock_count = EventTable_CopyRowHeader(row, shop->stock_item_ids);
            UiMessage_ShowAndWait((s32)MsgWhatWouldYouLike);
            Shop_SelBuy();
        } else if (choice == 1) {
            UiMessage_ShowAndWait((s32)MsgWhatToSell);
            Shop_PickUnit();
        } else if (choice == 2) {
            if (AbilityMenu_BuildAvailableList() != 0) {
                UiMessage_ShowAndWait((s32)MsgConnoisseur);
                Shop_SelBuy();
            } else {
                UiMessage_ShowAndWait((s32)MsgOutOfStock);
                WaitFrames(1);
            }
        } else if (choice == 3) {
            UiMessage_ShowAndWait((s32)MsgFixDamaged);
            Shop_SelUnit();
        } else {
            goto done;
        }
        ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
        UiMessage_ShowAndWait((s32)MsgAnythingElse);
        goto loop;
    }
done:
    UiMessage_ShowAndWait((s32)MsgShopFarewell);
    UiWork_FinalizeFar(window, 2);
    Inn_Cleanup();
    return 0;
}

#if EDITION_INTERNATIONAL
#define FINAL_ARG 1
#else
#define FINAL_ARG 2
#endif

s32 Battle_ApplyPresetItemsAndFlags(void)
{
    gGameState.coins = 0x30d40;
    gGameState.shop_gifts = 0x1c;
    Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
    Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
    Inventory_AddItemFar(2, 0xe7);
    UnitGetFar(3)->doku = 1;
    UnitGetFar(5)->doku = 1;
    UnitGetFar(2)->akuryo = 1;
    Shop_Run(FINAL_ARG, 0x1e);
    return 0;
}

void AudioCommand_WaitForStateByteClear(void)
{
    while (AudioCommand_GetStateByteFar() != 0) {
        WaitFrames(1);
    }
}
