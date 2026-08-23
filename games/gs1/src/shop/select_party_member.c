#include "shop.h"
#include "gs1_edition.h"
#include "sound_ids.h"

#if defined(GS1_EDITION_JA)
#define BASE_W 11
#else
#define BASE_W 12
#endif

s32 Modulo(s32, s32);
void WaitFrames(s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
void UiWindow_Close(s32, s32);
s32 Ability_GetAvailability(s32);
void Func_080a1028(s32, s32, s32, s32, s32);
void Func_080a1030(void);
void Func_080b1bd0(s32);
void Func_080b211c(s32);
void Audio_PlayCue(s32);

/*
 * Keep an actor-selection menu active while dispatching the chosen actor into
 * one of two action screens.  The menu itself closes only when cancelled.
 */
s32 Shop_SelectPartyMember(void)
{
    struct ShopRuntime *shop = SHOP_RUNTIME;
    s32 list_window;
    s32 selection = 0;
    s32 redraw = 1;
    s32 unit_id = 0;

    shop->money_window = Func_08015010(0, 9, BASE_W, 4, 2);
    Shop_DrawMoney();
    shop->item_window = Func_08015010(16, 12, 14, 8, 2);
    list_window = Func_08015010(0, 14, 13, 3, 2);
    shop->cursor.anchor->kind = 4;
    shop->mode = 12;
    Func_080a1028(list_window, 2, 0, 8, 0);

    for (;;) {
        if (redraw != 0) {
            redraw = 0;
            selection = Modulo(
                selection + shop->party_member_count,
                shop->party_member_count);
            unit_id = shop->party_member_ids[selection];
            Shop_PlaceCursor(
                (void *)list_window,
                selection * 24 - 12,
                0);
            shop->mode = 3;
            Shop_UpdatePartyMemberList(list_window, selection, 0);
            Shop_DrawPartyMemberItemGrid(shop->item_window, unit_id);
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 1) != 0) {
            WaitFrames(1);
            if (Ability_GetAvailability(unit_id) == 0) {
                Audio_PlayCue(SOUND_MENU_CANCEL);
            } else {
                Audio_PlayCue(SOUND_MENU_CONFIRM);
                if (shop->party_action == 1)
                    Func_080b1bd0(unit_id);
                else
                    Func_080b211c(unit_id);
                shop->cursor.anchor->kind = 4;
                shop->mode = 12;
                redraw = 1;
            }
            continue;
        }

        if ((*(volatile u32 *)ADDR_03001C94 & 2) != 0) {
            Audio_PlayCue(SOUND_MENU_CANCEL);
            Func_080a1030();
            UiWindow_Close(list_window, 2);
            UiWindow_Close(shop->item_window, 2);
            UiWindow_Close(shop->money_window, 2);
            WaitFrames(1);
            return 0;
        }

        if ((*(volatile u32 *)ADDR_03001B04 & 0x20) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            selection--;
            redraw = 1;
        }
        if ((*(volatile u32 *)ADDR_03001B04 & 0x10) != 0) {
            Audio_PlayCue(SOUND_MENU_CURSOR_MOVE);
            selection++;
            redraw = 1;
        }
        WaitFrames(1);
    }
}
