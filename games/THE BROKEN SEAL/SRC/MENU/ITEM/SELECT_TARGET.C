#include "EDITION.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "TBS_EDITION.H"
#include "INVENTORY_MENU.H"
#include "WINDOW.H"
#include "M7_INTERFACES.H"
#include "BATTLE_RUNTIME.H"
#include "EQUIPMENT_MENU.H"

struct TargetMarkerAttributes {
    u16 y : 8;
    u16 affine_mode : 2;
    u16 object_mode : 2;
    u16 mosaic : 1;
    u16 palette_256 : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

struct TargetMarker {
    u8 reserved_00[5];
    u8 state;
    u16 x;
    u16 y;
    u8 reserved_0a[10];
    struct TargetMarkerAttributes attributes;
};

extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern char MsgItemPlainName;
extern char MsgInStock;
extern char MsgTradeForWhat;
extern char MsgNoneInStock;

void UiWindow_SetBounds(struct WindowBounds *window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_DrawDividerLineFar(s32 window, s32 unused, s32 x, s32 y, s32 width);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 unused, s32 x, s32 y, s32 height);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void UiMenu_PositionCursor(s32 x, s32 y);
void UiIcon_PrepareObject(struct RenderOutput *icon);
void Audio_PlayCue(s32 cue);

/* Choose the party member an item is used on (mode 0) or given to (mode 1):
   left and right step through the party, showing the stock or the equipment
   preview for that member; A returns the member and B -1. */
s32 ItemMenu_SelectTarget(s32 mode)
{
    /* The existing unsigned coordinate/OAM prefix retains the native chained
       halfword stores: the canonical coordinate view measured 816 bytes
       against 824, losing the unsigned-halfword mask. */
    struct InventoryMenuState *menu;
    s32 window;
    s32 count;
    s32 selection;
    s32 pending;
    u8 result;
    s32 shown;
    s32 quantity;
    struct TargetMarker *marker;

    menu = gMenuWork;
    window = (s32)menu->item_window;
    selection = menu->pane_index[1];
    count = menu->party_count;
    pending = 1;
    result = 0;
    shown = 0;
    UiWindow_SetBounds((struct WindowBounds *)window, 13, 5, 17, 12);
    RenderOutput_RedrawSavedRectFar((s32)menu->item_window);
    Owner_GetStateFar(menu->owner_ids[menu->pane_index[0]]);
    Scheduler_AddOrUpdateCallback((s32)EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            window = (s32)menu->item_window;
            Owner_GetStateFar(menu->owner_ids[selection]);
            marker = (struct TargetMarker *)menu->pane_icons[1];
            marker->attributes.x = marker->x = ((menu->main_window->x + selection * 3) << 3) - 2;
            if (mode == 1) {
                ItemMenu_RefreshOwner(menu->owner_ids[selection], 1);
                UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
                UiWindow_ClearInteriorTilesFar(window, 0, 72, 120, 80);
                if (selection != menu->pane_index[0]) {
                    quantity = InventoryMenu_GetItemQuantity(menu->owner_ids[selection], menu->selected_items[0] & 0x1ff);
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || \
    defined(TBS_EDITION_IT)
                    /* Here a full bag asks what to trade instead of saying
                       none are in stock. */
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgTradeForWhat, window, 0, 72);
                    else if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgInStock, window, STOCK_LABEL_X, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgNoneInStock, window, 16, 72);
                    }
#else
                    if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgInStock, window, STOCK_LABEL_X, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgNoneInStock, window, 16, 72);
                    }
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
#if EDITION_INTERNATIONAL
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgTradeForWhat, window, 0, 72);
#else
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgTradeForWhat, window, 16, 72);
#endif
#endif
                }
                ItemMenu_DrawEquipPreview(menu->pane_owner[0], menu->selected_slots[0], 0, menu->owner_ids[selection]);
            }
            if (mode == 0) {
                if (ItemMenu_IsSpecial(menu->selected_items[0] & 0x1ff))
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
                    UiText_DrawCharacterAtOffsetFar((menu->selected_items[0] & 0x1ff) + (s32)&MsgItemPlainName, (s32)menu->info_window, 0, 0);
#else
                    UiText_DrawMessageAt((menu->selected_items[0] & 0x1ff) + (s32)&MsgItemPlainName, (s32)menu->info_window, 0, 0);
#endif
                    shown = 1;
                } else {
                    GameFlag_ClearBitFar(0x151);
                }
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & 1) {
            if (mode == 1 && selection == menu->pane_index[0]) {
                Audio_PlayCue(114);
                continue;
            }
            Audio_PlayCue(112);
            result = menu->owner_ids[selection];
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            result = 255;
            break;
        }
        if (gKeysRepeat & 32) {
            Audio_PlayCue(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & 16) {
            Audio_PlayCue(111);
            pending = 1;
            selection++;
        }
    }
    marker = (struct TargetMarker *)menu->pane_icons[1];
    menu->pane_index[1] = selection;
    UiIcon_PrepareObject((struct RenderOutput *)marker);
    marker->state = 13;
    EquipmentMenu_StartCompatibilityIndicators();
    WaitFrames(1);
    menu->pane_index[1] = selection;
    menu->selected_owner = menu->owner_ids[selection];
    menu->pane_owner[1] = menu->owner_ids[selection];
    return (s8)result;
}
