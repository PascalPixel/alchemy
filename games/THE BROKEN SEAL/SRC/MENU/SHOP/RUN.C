#include "SHOP.H"
#include "UI.H"

/* Runs a shop visit: set up the shop from its event-table row, show the
   keeper's window, then loop over the buy, sell, artifact and repair
   choices until the player leaves. */

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
s32 Func_080b2720(s32 row, s16 *items);
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
    shop = SHOP_RUNTIME;
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
    UiMessage_ShowAndWait(0xc9b);
loop:
    {
        choice = Func_08015380(choice);
        shop->party_action = choice;
        if (choice == 0) {
            shop->stock_count = Func_080b2720(row, shop->stock_item_ids);
            UiMessage_ShowAndWait(0xca7);
            Shop_SelBuy();
        } else if (choice == 1) {
            UiMessage_ShowAndWait(0xca9);
            Shop_PickUnit();
        } else if (choice == 2) {
            if (AbilityMenu_BuildAvailableList() != 0) {
                UiMessage_ShowAndWait(0xcb8);
                Shop_SelBuy();
            } else {
                UiMessage_ShowAndWait(0xcb7);
                WaitFrames(1);
            }
        } else if (choice == 3) {
            UiMessage_ShowAndWait(0xcb9);
            Shop_RunPartyMemberSelection();
        } else {
            goto done;
        }
        ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
        UiMessage_ShowAndWait(0xca4);
        goto loop;
    }
done:
    UiMessage_ShowAndWait(0xca5);
    UiWork_FinalizeFar(window, 2);
    Inn_Cleanup();
    return 0;
}
