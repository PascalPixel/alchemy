#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "item.h"
#include "edition.h"
#include "types.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "system.h"
#include "tbs_edition.h"
#include "inventory_menu.h"
#include "window.h"
#include "m7_interfaces.h"
#include "btlsys.h"
#include "equipment_menu.h"
#include "text/msg_ids.h"

#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

#define UiWindow_ClearInteriorTilesFar ((void (*)(s32,s32,s32,s32,s32))UiWindow_ClearInteriorTilesFar)


void UiWindow_DrawDividerLineFar(s32 window, s32 unused, s32 x, s32 y, s32 width);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
void GameFlag_ClearBitFar(s32 flag);
void AudioCommand_PlayFar(s32 cue);

s32 ItemMenu_SelectTarget(s32 mode)
{

    struct InventoryMenuState *menu;
    s32 window;
    s32 count;
    s32 selection;
    s32 pending;
    u8 result;
    s32 shown;
    s32 quantity;
    struct RenderOutput *marker;

    menu = gMenuWork;
    window = (s32)menu->item_window;
    selection = menu->pane_index[1];
    count = menu->party_count;
    pending = 1;
    result = 0;
    shown = 0;
    UiWindow_SetBounds((struct RenderInput *)window, 13, 5, 17, 12);
    RenderOutput_RedrawSavedRectFar((s32)menu->item_window);
    UnitGetFar(menu->owner_ids[menu->pane_index[0]]);
    Scheduler_AddOrUpdateCallback(EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            window = (s32)menu->item_window;
            UnitGetFar(menu->owner_ids[selection]);
            marker = menu->pane_icons[1];
            marker->oam.x = marker->x = ((menu->main_window->x + selection * 3) << 3) - 2;
            if (mode == 1) {
                ItemMenu_RefreshOwner(menu->owner_ids[selection], 1);
                UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
                UiWindow_ClearInteriorTilesFar(window, 0, 72, 120, 80);
                if (selection != menu->pane_index[0]) {
                    quantity = InventoryMenu_GetItemQuantity(menu->owner_ids[selection], menu->selected_items[0] & ITEM_ID_MASK);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
                        UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 0, 72);
                    else if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgInStock, window, STOCK_LABEL_X, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)MsgNoneInStock, window, 16, 72);
                    }
#else
                    if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)MsgInStock, window, STOCK_LABEL_X, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)MsgNoneInStock, window, 16, 72);
                    }
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
#if EDITION_INTERNATIONAL
                        UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 0, 72);
#else
                        UiText_DrawCharacterAtOffsetFar((s32)MsgTradeForWhat, window, 16, 72);
#endif
#endif
                }
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->owner_ids[selection]);
            }
            if (mode == 0) {
                if (ItemMenu_IsSpecial(menu->selected_items[0] & ITEM_ID_MASK))
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection], menu->selected_slots[0], 8);
                else
                    Menu_DrawOwnerStatusPanel((s32)menu->status_window, menu->owner_ids[selection], menu->selected_slots[0], 0);
                if (!GameFlag_TestFar(0x151) && !shown) {
#if EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#else
                    RenderOutput_ClearListFar((s32)menu->info_window);
#endif
#if EDITION_INTERNATIONAL
                    UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName, (s32)menu->info_window, 0, 0);
#else
                    UiText_DrawResourceFar((menu->selected_items[0] & ITEM_ID_MASK) + (s32)MsgItemPlainName, (s32)menu->info_window, 0, 0);
#endif
                    shown = 1;
                } else {
                    GameFlag_ClearBitFar(0x151);
                }
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            if (mode == 1 && selection == menu->pane_index[0]) {
                AudioCommand_PlayFar(114);
                continue;
            }
            AudioCommand_PlayFar(112);
            result = menu->owner_ids[selection];
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = 255;
            break;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            AudioCommand_PlayFar(111);
            pending = 1;
            selection++;
        }
    }
    marker = menu->pane_icons[1];
    menu->pane_index[1] = selection;
    UiIcon_PrepareObject(marker);
    marker->active = 13;
    EquipmentMenu_StartCompatibilityIndicators();
    WaitFrames(1);
    menu->pane_index[1] = selection;
    menu->selected_owner = menu->owner_ids[selection];
    menu->pane_owner[1] = menu->owner_ids[selection];
    return (s8)result;
}
