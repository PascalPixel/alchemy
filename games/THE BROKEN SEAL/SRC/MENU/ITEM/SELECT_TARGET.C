#include "TYPES.H"
#include "SYSTEM.H"

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

struct TargetWindow {
    u8 reserved_00[12];
    u16 x;
    u16 y;
};

/* The item menu work block at 0x03001F2C, as the target selector sees it. */
struct ItemTargetMenu {
    u8 reserved_000[8];
    s32 selected_owner;               /* 0x008 */
    u8 reserved_00c[4];
    struct TargetWindow *window;      /* 0x010 */
    u8 reserved_014[4];
    struct TargetMarker *marker;      /* 0x018 */
    s8 item_selection;                /* 0x01c */
    s8 selection;                     /* 0x01d */
    u8 reserved_01e[2];
    s32 item_window;                  /* 0x020 */
    s32 status_window;                /* 0x024 */
    u8 reserved_028[4];
    s32 info_window;                  /* 0x02c */
    u8 reserved_030[0x144];
    u16 selected_slot;                /* 0x174 */
    u8 reserved_176[2];
    u16 selected_item;                /* 0x178 */
    u8 reserved_17a[0x8e];
    u16 owner_ids[8];                 /* 0x208 */
    u8 item_count;                    /* 0x218 */
    u8 party_count;                   /* 0x219 */
    u8 item_owner;                    /* 0x21a */
    u8 target_owner;                  /* 0x21b */
};

extern struct ItemTargetMenu *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern char Value_00000075;
extern char Value_00000b2f;
extern char Value_00000b30;
extern char Value_00000b31;

void UiWindow_SetBounds(s32 window, s32 x, s32 y, s32 width, s32 height);
void RenderOutput_RedrawSavedRectFar(s32 window);
void *Owner_GetStateFar(s32 owner);
s32 Scheduler_AddOrUpdateCallback(s32 callback, s32 priority);
void EquipmentMenu_UpdateCompatibilityIndicators(void);
void EquipmentMenu_StartCompatibilityIndicators(void);
s32 Math_Mod(s32 numerator, s32 denominator);
void ItemMenu_RefreshOwner(s32 owner, s32 mode);
void UiWindow_DrawDividerLineFar(s32 window, s32 unused, s32 x, s32 y, s32 width);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 unused, s32 x, s32 y, s32 height);
s32 Func_080a3d9c(s32 owner, s32 item);
void UiText_DrawNumberInWindowFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
s32 ItemMenu_Count(s32 owner);
void ItemMenu_DrawEquipPreview(s32 owner, s32 slot, s32 mode, s32 target);
s32 ItemMenu_IsSpecial(s32 item);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void UiMenu_PositionCursor(s32 x, s32 y);
void UiIcon_PrepareObject(struct TargetMarker *icon);
void Audio_PlayCue(s32 cue);

/* Choose the party member an item is used on (mode 0) or given to (mode 1):
   left and right step through the party, showing the stock or the equipment
   preview for that member; A returns the member and B -1. */
s8 ItemMenu_SelectTarget(s32 mode)
{
    struct ItemTargetMenu *menu;
    s32 window;
    s32 count;
    s32 selection;
    s32 pending;
    u8 result;
    s32 shown;
    s32 quantity;
    struct TargetMarker *marker;

    menu = gMenuWork;
    window = menu->item_window;
    selection = menu->selection;
    count = menu->party_count;
    pending = 1;
    result = 0;
    shown = 0;
    UiWindow_SetBounds(window, 13, 5, 17, 12);
    RenderOutput_RedrawSavedRectFar(menu->item_window);
    Owner_GetStateFar(menu->owner_ids[menu->item_selection]);
    Scheduler_AddOrUpdateCallback((s32)EquipmentMenu_UpdateCompatibilityIndicators, 0xc80);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = Math_Mod(selection + count, count);
            window = menu->item_window;
            Owner_GetStateFar(menu->owner_ids[selection]);
            marker = menu->marker;
            marker->attributes.x = marker->x = ((menu->window->x + selection * 3) << 3) - 2;
            if (mode == 1) {
                ItemMenu_RefreshOwner(menu->owner_ids[selection], 1);
                UiWindow_DrawDividerLineFar(window, 0, 9, 16, 9);
                UiWindow_ClearInteriorTilesFar(window, 0, 72, 120, 80);
                if (selection != menu->item_selection) {
                    quantity = Func_080a3d9c(menu->owner_ids[selection], menu->selected_item & 0x1ff);
                    if (quantity != 0) {
                        UiText_DrawNumberInWindowFar(quantity, 2, window, 8, 72);
                        UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b2f, window, 24, 72);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b31, window, 16, 72);
                    }
                    if (ItemMenu_Count(menu->owner_ids[selection]) == 15 && quantity == 0)
                        UiText_DrawCharacterAtOffsetFar((s32)&Value_00000b30, window, 0, 72);
                }
                ItemMenu_DrawEquipPreview(menu->item_owner, menu->selected_slot, 0, menu->owner_ids[selection]);
            }
            if (mode == 0) {
                if (ItemMenu_IsSpecial(menu->selected_item & 0x1ff))
                    Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_ids[selection], menu->selected_slot, 8);
                else
                    Menu_DrawOwnerStatusPanel(menu->status_window, menu->owner_ids[selection], menu->selected_slot, 0);
                if (!GameFlag_TestFar(0x151) && !shown) {
                    RenderOutput_RedrawSavedRectFar(menu->info_window);
                    UiText_DrawCharacterAtOffsetFar((menu->selected_item & 0x1ff) + (s32)&Value_00000075, menu->info_window, 0, 0);
                    shown = 1;
                } else {
                    GameFlag_ClearBitFar(0x151);
                }
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & 1) {
            if (mode == 1 && selection == menu->item_selection) {
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
    marker = menu->marker;
    menu->selection = selection;
    UiIcon_PrepareObject(marker);
    marker->state = 13;
    EquipmentMenu_StartCompatibilityIndicators();
    WaitFrames(1);
    menu->selection = selection;
    menu->selected_owner = menu->owner_ids[selection];
    menu->target_owner = menu->owner_ids[selection];
    return result;
}
