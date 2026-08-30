#include "shop.h"

s32 Modulo(s32 value, s32 divisor);
void WaitFrames(s32 frames);
void Func_08015120(s32 a, s32 b);
void Func_080b0574(s32 message);
s32 Func_08077028(s32 unit_id, s32 item_id);
s32 Func_08077248(s32 unit_id);
s32 Func_08077058(s32 unit_id);
void Func_080f9010(s32 message);
void Func_080b04dc(s32 message);
void Func_08077298(s32 amount);
void Func_080772a0(s32 flag);
void Func_080030f8(s32 frames);

#define INPUT_NEW_KEYS (*(volatile u32 *)ADDR_03001C94)
#define INPUT_REPEAT_KEYS (*(volatile u32 *)ADDR_03001B04)

#define Shop_ConfirmItemPurchase Func_080b24e4

/* Confirm-purchase loop for one already-chosen item at a fixed price. */
void Shop_ConfirmItemPurchase(s32 list_window, s32 item_window)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    s32 price;
    s32 saved_field = *(u16 *)((u8 *)shop + 0x39e);
    s32 redraw = 1;
    u8 *party_state = (u8 *)0x02000240;
    s32 party_index = *(s8 *)(party_state + 0x11c);
    s32 money = *(s32 *)(party_state + 0x118);
    s32 unit_id = 0;
    s32 selection = 0;

    price = *(s16 *)(0x080b4146 + party_index * 2);

    if (price > money)
        return;

    *(u16 *)((u8 *)shop + 0x39e) = 0xe4;
    Func_08015120(0xe4, 2);
    Func_080b0574(0xcc3);
    Func_08015120(*(u16 *)((u8 *)shop + 0x39e), 2);
    Func_080b0574(0xcc4);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selection = Modulo(
                selection + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor((void *)list_window, selection * 24 - 12, 0);
            shop->mode = 3;
            Shop_UpdatePartyMemberList(
                list_window, selection, *(u16 *)((u8 *)shop + 0xca6));
            Func_080b1470(
                item_window, unit_id, *(u16 *)((u8 *)shop + 0xca6));
        }

        if ((INPUT_NEW_KEYS & 1) != 0) {
            if (Func_08077028(unit_id, *(u16 *)((u8 *)shop + 0xc9e)) < 0) {
                Func_080f9010(113);
                Func_08015120(unit_id, 1);
                Func_08015120(*(u16 *)((u8 *)shop + 0xc9e), 2);
                if (Func_08077248(unit_id) == 15) {
                    Func_080b04dc(0xc9e);
                } else {
                    Func_080b04dc(0xca6);
                }
                continue;
            }
            Func_08077058(unit_id);
            Func_080f9010(101);
            Func_080b0574(0xca1);
            Func_08077298(-price);
            Func_080772a0(1);
            break;
        }

        if ((INPUT_NEW_KEYS & 2) != 0) {
            Func_080b0574(0xcc5);
            Func_080f9010(113);
            break;
        }

        if ((INPUT_REPEAT_KEYS & 0x20) != 0) {
            Func_080f9010(111);
            selection--;
            redraw = 1;
        }
        if ((INPUT_REPEAT_KEYS & 0x10) != 0) {
            Func_080f9010(111);
            selection++;
            redraw = 1;
        }
        WaitFrames(1);
    }

    *(u16 *)((u8 *)shop + 0x39e) = saved_field;
}
