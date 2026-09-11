#include "scene.h"
#include "shop.h"

s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);

struct ShopCursorAnchor *Sys_Run(
    u32 resource,
    u32 flags,
    s32 window,
    s32 x,
    s32 y);

void UiWindow_Close(s32 window, s32 style);

/* Run the shop's yes/no party-action confirmation prompt for one unit. */
s32 Shop_ConfirmAct(s32 unit_id)
{
    s32 party_action = 0;
    s32 list_window = 0;
    struct ShopRuntime *shop;
    struct ShopCursorAnchor *cursor_anchor;

    Sys_unk2_4();
    shop = SHOP_RUNTIME;
    shop->party_action = list_window;

    {
        s32 shown =
            *(u16 *)(*(u32 *)(*(u32 *)((u8 *)Sys_Check(unit_id) + 80) + 40));
        *(u16 *)((u8 *)shop + 0x3a4) = shown;
    }

    list_window = Sys_SetMode(*(u16 *)((u8 *)shop + 0x3a4), 0, 0, 0);
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
    }
    if (list_window == 0) {
        list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
        Sys_SetRect(2, 0, 0, list_window, -4, -4);
    }

    cursor_anchor = Sys_Run(
        *(u16 *)((u8 *)shop + 0x390),
        0x40000000,
        list_window,
        0,
        0);
    cursor_anchor->kind = 1;
    cursor_anchor->unknown_00[4] = 0;
    Sys_Place(&shop->cursor, -32, 112);
    shop->cursor.anchor = cursor_anchor;
    Sys_Do(0xd21);

    shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
    Shop_DrawMoney();

    for (;;) {
        party_action = Sys_unk2(party_action);
        shop->party_action = party_action;
        if (party_action == -1)
            break;

        {
            s32 base = 0xd24;
            s32 message = base;

            base = 0;
            Sys_Do(message);
            if (Sys_unk3() == 0) {
                Sys_Do(message + 1);
            } else {
                Sys_unk3_4();
            }
        }
        shop->party_action = 0;
        Sys_Place(&shop->cursor, -32, 112);
        Sys_Do(0xd22);
    }

    Sys_Do(0xd23);
    UiWindow_Close(shop->money_window, 2);
    UiWindow_Close(list_window, 2);
    Sys_unk4();
    return 0;
}
