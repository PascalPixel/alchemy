#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "INN.H"
#include "DMA.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "SHOP.H"
#include "INN_RUNTIME.H"
#include "UI.H"
#include "TBS_EDITION.H"
#include "SYSTEM.H"

extern struct InnState *Data_03001f2c;

/* ui/ability_menu/build_available_list.c */
s32 Ability_GetMaximum(s32, s32);

s32 ShopCursor_Advance(s32);
void Shop_StepCursor(void);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void Battle_ResetEffectCounterFar(void);
u8 Party_ListActiveOwnersFar(void *);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);
s32 Resource_ResetEntry(u16);
s32 UiWork_FinalizePendingCoreFar();
extern u8 MsgWeaponShopWelcome[];
extern u8 MsgWhatWouldYouLike[];
extern u8 MsgWhatToSell[];
extern u8 MsgConnoisseur[];
extern u8 MsgOutOfStock[];
extern u8 MsgFixDamaged[];
extern u8 MsgAnythingElse[];
extern u8 MsgShopFarewell[];

struct ShopKeeperSprite {
    u8 unknown_00[40];
    u16 *resource;
};

struct ShopKeeper {
    u8 unknown_00[80];
    struct ShopKeeperSprite *sprite;
};

s32 EventTable_GetRowLimit(void);
void EventTable_ApplyRowAbilities(s32 row);
s32 EventTable_GetRowType(s32 row);
s32 EventTable_CopyRowHeader(s32 row, s16 *items);
void Shop_InitializeCursorWork(void);
struct ShopKeeper *Object_GetByIdFar(s32 id);
s32 UiWindow_CreateWithSideObjectFar(s32 resource, s32 a, s32 b, s32 c);
struct ShopCursorAnchor *RenderOutput_CreateFar(u32 resource, u32 flags, s32 window, s32 x, s32 y);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 target_x, s32 target_y);
void UiMessage_ShowAndWait(s32 message);
s32 Func_08015380(s32 choice);
s32 AbilityMenu_BuildAvailableList(void);
s32 Shop_SelBuy(void);
s32 Shop_PickUnit(void);
void UiWork_FinalizeFar(s32 window, s32 style);
void Inn_Cleanup(void);
void WaitFrames(s32 frames);

void Inventory_EquipFar(s32, s32);
s32 Inventory_AddItemFar(s32, s32);
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/*
 * This owner's view of gGameState, which games/THE BROKEN SEAL/INCLUDE/BATTLE_EFFECT_RUNTIME.H
 * declares as `struct BattleWork`. Two fields are evidence here; the paddings
 * are arithmetic to reach them.
 */
struct Work_080b0444 {
    u8 padding0[0x10];
    s32 value10;
    u8 padding14[0x108];
    s8 value11c;
};

extern struct Work_080b0444 gGameState;
void *Owner_GetStateFar(s32);

s32 Audio_Check(void);

s32 Shop_Run(s32 row, s32 keeper_id);

s32 AbilityMenu_BuildAvailableList(void)
{
    u8 *state;
    s16 *output;
    s32 index;
    s32 count;
    s32 offset;
    s8 mode;

    state = (u8 *)Data_03001f2c;
    count = 0;
    index = 0;
    output = (s16 *)(state + 0x26c);
    do {
        mode = *(s8 *)(state + 0x3a9);
        if (mode == Item_GetEquipmentGroupFar(index)&&
            Ability_GetMaximum(index, 0) != 0) {
            *output = index;
            count++;
            output++;
        }
        index++;
    } while (index <= 0x1ff);
    offset = count << 1;
    offset += 0x26c;
    *(u16 *)(state + offset) = 0;
    *(u8 *)(state + 0x3a6) = count;
    return count;
}

void Shop_StepCursor(void)
{
    ShopCursor_Advance(*(s32 *)((u32)&Data_03001f2c) + 0x380);
}

/* Shop work block (heap kind 55): clear it, set up the cursor state at
   +0x380 and cache the six cursor sprite frames before the cursor task
   Shop_StepCursor starts. */
void Shop_InitializeCursorWork(void)
{
    u8 *work;
    volatile s32 zero;
    s32 slot;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    Battle_ResetEffectCounterFar();
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500029c, (volatile u32 *)0x040000d4);
    work[0x3a8] = 12;
    work[0x3a7] = Party_ListActiveOwnersFar(work + 0x36e);
    slot = Resource_FindFreeEntry();
    *(u16 *)(work + 0x390) = slot;
    VramBlock_LoadCached(slot, 128, Shop_HandTiles);
    slot = Resource_FindFreeEntry();
    *(u16 *)(work + 0x392) = slot;
    VramBlock_LoadCached(slot, 128, Shop_UpArrowTiles);
    slot = Resource_FindFreeEntry();
    *(u16 *)(work + 0x394) = slot;
    VramBlock_LoadCached(slot, 128, Shop_DownArrowTiles);
    slot = Resource_FindFreeEntry();
    *(u16 *)(work + 0x396) = slot;
    VramBlock_LoadCached(slot, 128, Shop_GemTiles);
    slot = Resource_FindFreeEntry();
    *(u16 *)(work + 0x39a) = slot;
    VramBlock_LoadCached(slot, 128, Shop_SmallDownArrowTiles);
    slot = Resource_FindFreeEntry();
    *(u16 *)(work + 0x398) = slot;
    VramBlock_LoadCached(slot, 128, Shop_SmallUpArrowTiles);
    Scheduler_AddOrUpdateCallback((s32)Shop_StepCursor, 0xc80);
}

void Inn_Cleanup(void)
{
    struct InnRuntimeState *state;

    state = gMenuWork;
    Scheduler_RemoveCallback((s32)Shop_StepCursor);
    UiWork_FinalizePendingCoreFar();
    Resource_ResetEntry(state->resource_entries[0]);
    Resource_ResetEntry(state->resource_entries[1]);
    Resource_ResetEntry(state->resource_entries[2]);
    Resource_ResetEntry(state->resource_entries[3]);
    Resource_ResetEntry(state->resource_entries[4]);
    Resource_ResetEntry(state->resource_entries[5]);
    Runtime_ReleaseHeapBlock(0x37);
}

/* Runs a shop visit: set up the shop from its event-table row, show the
   keeper's window, then loop over the buy, sell, artifact and repair
   choices until the player leaves. */
s32 Shop_Run(s32 row, s32 keeper_id)
{
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *anchor;
    s32 window;
    s32 choice = 0;

    if (row >= EventTable_GetRowLimit() || row < 0)
        row = 0;
    EventTable_ApplyRowAbilities(row);
    Shop_InitializeCursorWork();
    shop = ((struct ShopRuntime *)gMenuWork);
    shop->shop_type = EventTable_GetRowType(row);
    if (row == 16)
        ((u8 *)shop)[0x3ac] = 1;
    if (row == 17)
        ((u8 *)shop)[0x3ac] = 1;
    if (row == 18)
        ((u8 *)shop)[0x3ac] = 1;
    shop->keeper_resource = *Object_GetByIdFar(keeper_id)->sprite->resource;
    window = UiWindow_CreateWithSideObjectFar(shop->keeper_resource, 0, 0, 0);
    if (window == 0)
        window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    anchor = RenderOutput_CreateFar(shop->cursor_icon, 0x40000000, window, 0, 0);
    anchor->kind = 1;
    anchor->unknown_00[4] = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = anchor;
    UiMessage_ShowAndWait((s32)MsgWeaponShopWelcome);
loop:
    {
        choice = Func_08015380(choice);
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

#if defined(TBS_EDITION_JA)
#define FINAL_ARG 2
#else
#define FINAL_ARG 1
#endif

/* 固定値を設定し、3つの項目フラグを1にする。 */
s32 Battle_ApplyPresetItemsAndFlags(void)
{
    gGameState.value10 = 0x30d40;
    gGameState.value11c = 0x1c;
    Inventory_EquipFar(1, Inventory_AddItemFar(1, 0x48d));
    Inventory_EquipFar(0, Inventory_AddItemFar(0, 0x40b));
    Inventory_AddItemFar(2, 0xe7);
    FIELD((void *)Owner_GetStateFar(3), s8 *, 0x131) = 1;
    FIELD((void *)Owner_GetStateFar(5), s8 *, 0x131) = 1;
    FIELD((void *)Owner_GetStateFar(2), s8 *, 0x140) = 1;
    Shop_Run(FINAL_ARG, 0x1e);
    return 0;
}

void AudioCommand_WaitForStateByteClear(void)
{
    while (Audio_Check() != 0) {
        WaitFrames(1);
    }
}
