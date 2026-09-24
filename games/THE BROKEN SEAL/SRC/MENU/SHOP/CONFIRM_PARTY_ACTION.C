#include "SHOP.H"

s32 Object_GetByIdFar(s32 unit_id);
s32 UiWindow_CreateWithSideObjectFar(s32 resource, s32 x, s32 y, s32 flags);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void SideObject_CreateFar(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
struct ShopCursorAnchor *RenderOutput_CreateFar(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);
void ShopCursor_SetPositionImmediate(struct ShopCursor *cursor, s32 target_x, s32 target_y);
void UiMessage_ShowResolvedAndWait(s32 message);
void Shop_InitializeCursorWork(void);
void Inn_Cleanup(void);
s32 Shop_CountUnits(void);
void Sanctum_RunPartyService(void);
s32 Menu_SelectEntry19To1cFar(s32 prev);
void UiWindow_Close(s32 window, s32 style);



/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;

    Shop_InitializeCursorWork();
    shop = SHOP_RUNTIME;
    shop->party_action = list_window;

    {
        s32 shown =
            *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Object_GetByIdFar(unit_id) + 80) + 40));
        *(u16 *)((u8 *)shop + 0x3a4) = shown;
    }

    list_window = UiWindow_CreateWithSideObjectFar(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        SideObject_CreateFar(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = RenderOutput_CreateFar(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        0);
    cursor_anchor->kind = 1;
    cursor_anchor->unknown_00[4] = 0;
    ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    UiMessage_ShowResolvedAndWait(0xd21);

    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
    Shop_DrawMoney();

    for (;;) {
        party_action = Menu_SelectEntry19To1cFar(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        {
            s32 base = 0xd24;
            s32 message = base;

            base = 0;
            UiMessage_ShowResolvedAndWait(message);
            if (Shop_CountUnits() == 0) {
                UiMessage_ShowResolvedAndWait(message + 1);
            } else {
                Sanctum_RunPartyService();
            }
        }
        shop->party_action = 0;
        ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
        UiMessage_ShowResolvedAndWait(0xd22);
    }

    UiMessage_ShowResolvedAndWait(0xd23);
    UiWindow_Close(shop->money_window, 2);
    UiWindow_Close(list_window, 2);
    Inn_Cleanup();
    return 0;
}
