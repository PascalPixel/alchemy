#include "TYPES.H"
#include "SHOP.H"
#include "GLOBAL_CELLS.H"
#include "SOUND_IDS.H"

void UiText_DrawQuantity(s32 value, s32 slot);
void UiWindow_Close(s32 window, s32 style);
s32 Func_080b27b0(s32 unit_id, s32 kind);
#define Shop_CanServe Func_080b27b0
s32 Func_080b2778(s32 unit_id, s32 kind);
#define Shop_ServicePrice Func_080b2778
void Func_080b2da8(s32 unit_id, s32 mode);
#define BattleUnit_ResetStateByMode Func_080b2da8
void Func_080b2e30(s32 enabled, s32 selected);
#define Shop_HiliteUnit Func_080b2e30
void Func_080b2ed8(s32 target, s32 selection);
#define Shop_DrawSelMsg Func_080b2ed8
void Func_080b28d4(s32 message);
#define UiMessage_ShowResolvedAndWait Func_080b28d4
void Func_080b2928(s32 message);
#define UiMessage_ShowResolvedAndRestoreState Func_080b2928
s32 Func_080b0664(s32 arg0);
#define UiMessage_ShowChoiceVariant Func_080b0664
s32 Func_080b280c(void);
#define Shop_CountUnits Func_080b280c
void Func_080b3050(s32 member);
s32 Func_08077230(s32 amount);
void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
extern char Value_00000d27;

#define Sanctum_RunPartyService Func_080b2b10

/* Lets the player choose which party member receives the chosen sanctum
 * service, starting on the first member who needs it; left and right move the
 * cursor and B leaves. A on a member who needs the service quotes the
 * donation. Declining it, or being unable to pay, says so and starts the
 * choice again; paying treats the member and, while others still need the
 * service, starts the choice again. */
s32 Sanctum_RunPartyService(void)
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
    s32 message;

    price_window = 0;
    redraw = 1;
    kind = shop->party_action;
    UiMessage_ShowResolvedAndWait(0xd26);

    list_window = UiWindow_CreateFar(1, 12, 13, 3, 2);
    shop->cursor.anchor->kind = 4;
    shop->mode = redraw;
    Func_080a1028(list_window, 2, 0, 8, price_window);
    price_window = UiWindow_CreateFar(1, 16, 23, 3, 2);

    selection = 0;
    unit_id = 0;
    retry = 0;
    while (selection < shop->party_member_count) {
        unit_id = shop->party_member_ids[selection];
        if (Shop_CanServe(unit_id, kind) != 0)
            break;
        selection++;
    }

    redraw = 1;
    for (;;) {
        if (retry != 0) {
            retry = 0;
            UiMessage_ShowResolvedAndWait(0xd26);
            redraw = 1;
            selection = 0;
            while (selection < shop->party_member_count) {
                unit_id = shop->party_member_ids[selection];
                if (Shop_CanServe(unit_id, kind) != 0)
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
            Shop_HiliteUnit(list_window, selection);
            Shop_DrawSelMsg(price_window, unit_id);
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            WaitFrames(1);
            price = Shop_ServicePrice(unit_id, kind);
            if (Shop_CanServe(unit_id, kind) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                continue;
            }
            UiText_DrawQuantity(unit_id, 1);
            UiText_DrawQuantity(price, 5);
            message = (s32)&Value_00000d27;
            UiMessage_ShowResolvedAndWait(message);
            if (UiMessage_ShowChoiceVariant(0) != 0) {
                UiMessage_ShowResolvedAndRestoreState(message + 2);
                retry = 1;
                continue;
            }
            if ((u32)price > (u32)SHOP_PARTY_STATE.money) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
                UiMessage_ShowResolvedAndRestoreState(message + 1);
                retry = 1;
                continue;
            }
            UiText_DrawQuantity(unit_id, 1);
            UiMessage_ShowResolvedAndWait(message + 3);
            UiWork_FinalizePending();
            BattleUnit_ResetStateByMode(unit_id, kind);
            Func_080b3050(selection);
            Func_08077230(-price);
            Shop_DrawMoney();
            UiText_DrawQuantity(unit_id, 1);
            UiMessage_ShowResolvedAndWait(message + 4);
            if (Shop_CountUnits() != 0) {
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
