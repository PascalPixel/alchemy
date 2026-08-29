#include "shop.h"

s32 Func_0808a080(s32 unit_id);
s32 Func_080150f8(s32 resource, s32 x, s32 y, s32 flags);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void Func_080150d8(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
struct ShopCursorAnchor *Func_080150c8(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);
void Func_080b0a20(struct ShopCursor *cursor, s32 target_x, s32 target_y);
void Func_080b28d4(s32 message);
void Func_080b010c(void);
void Func_080b0204(void);
s32 Func_080b280c(void);
void Func_080b2b10(void);
s32 Func_08015388(s32 prev);
void UiWindow_Close(s32 window, s32 style);

#define Shop_DrawMoney Func_080b10cc

#define Shop_ConfirmPartyAction Func_080b29a8

/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmPartyAction(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;

    Func_080b010c();
    shop = SHOP_RUNTIME;
    shop->party_action = party_action;

    {
        u16 *dst = (u16 *)((u8 *)shop + 0x3a4);
        *dst = *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Func_0808a080(unit_id) + 80) + 40));
    }

    list_window = Func_080150f8(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        Func_080150d8(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = Func_080150c8(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        0);
    cursor_anchor->kind = 1;
    cursor_anchor->unknown_00[4] = 0;
    Func_080b0a20(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    Func_080b28d4(0xd21);

    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
    Shop_DrawMoney();

    for (;;) {
        party_action = Func_08015388(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        Func_080b28d4(0xd24);
        if (Func_080b280c() == 0) {
            Func_080b28d4(0xd25);
        } else {
            Func_080b2b10();
        }
        shop->party_action = 0;
        Func_080b0a20(&shop->cursor, -32, 112);
        Func_080b28d4(0xd22);
    }

    Func_080b28d4(0xd23);
    UiWindow_Close(list_window, 2);
    UiWindow_Close(shop->money_window, 2);
    Func_080b0204();
    return 0;
}
