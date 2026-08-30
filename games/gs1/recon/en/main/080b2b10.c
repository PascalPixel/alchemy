#include "types.h"
#include "shop.h"
#include "global_cells.h"
#include "sound_ids.h"

s32 Func_080b27b0(s32 unit_id, s32 kind);
s32 Func_080b2778(s32 unit_id, s32 kind);
void Func_080b2da8(s32 unit_id, s32 mode);
void Func_080b2e30(s32 enabled, s32 selected);
void Func_080b2ed8(s32 target, s32 selection);
void Func_080b28d4(s32 message);
void Func_080b2928(s32 message);
s32 Func_080b0664(s32 arg0);
s32 Func_080b280c(void);
void Func_080b3050(s32 member);
s32 Func_08077230(s32 amount);
void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
extern char Value_00000d27;

/*
 * Reached from Shop_ConfirmPartyAction (main:080b29a8) once the caller's
 * yes/no confirmation succeeds. Lets the player pick a party member from
 * shop->party_member_ids eligible for shop->party_action (checked through
 * Func_080b27b0's per-kind status test), previews the treatment's price via
 * Func_080b2778, and on confirmation charges the party's money and applies
 * the action through Func_080b2da8 before looping for another member.
 */
s32 Func_080b2b10(void)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    s32 price_window;
    s32 redraw;
    s32 kind;
    s32 list_window;
    s32 selection;
    s32 unit_id;
    s32 retry;
    s32 price;

    price_window = 0;
    redraw = 1;
    kind = shop->party_action;
    Func_080b28d4(0xd26);

    list_window = UiWindow_CreateFar(1, 12, 13, 3, 2);
    shop->cursor.anchor->kind = 4;
    shop->mode = redraw;
    Func_080a1028(list_window, 2, 0, 8, price_window);
    price_window = UiWindow_CreateFar(1, 16, 23, 3, 2);

    /* selection/unit_id/retry are zero-initialized here as three separate
     * statements (not folded into the loop's init clause): the reference
     * materializes 0 once in a low register for `selection` and copies it
     * into both high-register locals via cheap `mov`s (Thumb cannot load
     * an immediate directly into r8+), which only happens if unit_id and
     * retry both have real explicit initializers at this exact point. */
    selection = 0;
    unit_id = 0;
    retry = 0;
    while (selection < shop->party_member_count) {
        unit_id = shop->party_member_ids[selection];
        if (Func_080b27b0(unit_id, kind) != 0)
            break;
        selection++;
    }

    redraw = 1;
    for (;;) {
        if (retry != 0) {
            retry = 0;
            redraw = 1;
            Func_080b28d4(0xd26);
            selection = 0;
            while (selection < shop->party_member_count) {
                unit_id = shop->party_member_ids[selection];
                if (Func_080b27b0(unit_id, kind) != 0)
                    break;
                selection++;
            }
        }

        if (redraw != 0) {
            redraw = 0;
            selection = Modulo(
                selection + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor((void *)list_window, selection * 24 - 12, 0);
            shop->mode = 3;
            Func_080b2e30(list_window, selection);
            Func_080b2ed8(price_window, unit_id);
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            WaitFrames(1);
            price = Func_080b2778(unit_id, kind);
            if (Func_080b27b0(unit_id, kind) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                continue;
            }
            UiText_DrawQuantity(unit_id, 1);
            UiText_DrawQuantity(price, 5);
            /* retry briefly carries the 0xd27 message-id base for this
             * block's status prompts (retry+1/+2 on the two rejection
             * paths, +3/+4 once the purchase actually proceeds) before
             * being reused for its usual 0/1 retry-loop meaning below;
             * the reference keeps both roles in the same register since
             * their live ranges never overlap. */
            retry = (s32)&Value_00000d27;
            Func_080b28d4(retry);
            if (Func_080b0664(0) != 0) {
                Func_080b2928(retry + 2);
                retry = 1;
                continue;
            }
            if ((u32)price > (u32)SHOP_PARTY_STATE.money) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                Func_080b2928(retry + 1);
                retry = 1;
                continue;
            }
            UiText_DrawQuantity(unit_id, 1);
            Func_080b28d4(retry + 3);
            UiWork_FinalizePending();
            Func_080b2da8(unit_id, kind);
            Func_080b3050(selection);
            Func_08077230(-price);
            Shop_DrawMoney();
            UiText_DrawQuantity(unit_id, 1);
            Func_080b28d4(retry + 4);
            if (Func_080b280c() != 0) {
                retry = 1;
                continue;
            }
            break;
        } else if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            break;
        } else {
            if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
                selection -= 1;
            }
            if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
                Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
                redraw = 1;
                selection += 1;
            }
            WaitFrames(1);
            continue;
        }
    }

    Func_080a1030();
    UiWindow_Close(price_window, 2);
    UiWindow_Close(list_window, 2);
    WaitFrames(1);
    return 0;
}
