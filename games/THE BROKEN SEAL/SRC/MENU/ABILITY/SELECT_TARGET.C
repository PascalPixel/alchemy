/* Select the party member targeted by the current Psynergy. The marker shares
 * the item-target selector's halfword position and nine-bit OAM x field. */
#include "TYPES.H"
#include "SYSTEM.H"

struct PsynergyTargetAttributes {
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

struct PsynergyTargetMarker {
    u8 reserved_00[5];
    u8 state;
    u16 x;
    u16 y;
    u8 reserved_0a[10];
    struct PsynergyTargetAttributes attributes;
};

struct PsynergyTargetWindow {
    u8 reserved_00[12];
    u16 x;
    u16 y;
};

struct PsynergyTargetMenu {
    u8 reserved_000[8];
    s32 selected_owner;
    u8 reserved_00c[4];
    struct PsynergyTargetWindow *window;
    u8 reserved_014[4];
    struct PsynergyTargetMarker *marker;
    s8 owner_selection;
    s8 selection;
    u8 reserved_01e[6];
    s32 status_window;
    u8 reserved_028[4];
    s32 info_window;
    u8 reserved_030[0x148];
    u16 selected_action;
    u8 reserved_17a[0x8e];
    u16 owner_ids[8];
    u8 action_count;
    u8 party_count;
    u8 action_owner;
    u8 target_owner;
};

extern struct PsynergyTargetMenu *gMenuWork;
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;
extern char MsgAbilityDescription;

void *Owner_GetStateFar(s32 owner);
s32 Math_Mod(s32 numerator, s32 denominator);
void UiMenu_SlideCursor(s32 x, s32 y);
void UiMenu_PositionCursor(s32 x, s32 y);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
void PsynergyMenu_CallIconRoutineWithValue(s32 menu, s32 owner);
s32 GameFlag_TestFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawMessageAt(s32 message, s32 window, s32 x, s32 y);
void UiIcon_PrepareObject(struct PsynergyTargetMarker *marker);
void Audio_PlayCue(s32 cue);

s32 PsynergyMenu_SelectTarget(s32 mode)
{
    struct PsynergyTargetMenu *menu;
    s32 selection;
    s32 count;
    s32 pending;
    s32 result;
    s32 shown;
    struct PsynergyTargetMarker *marker;

    menu = gMenuWork;
    selection = menu->selection;
    count = menu->party_count;
    pending = 1;
    result = 0;
    shown = 0;
    Owner_GetStateFar(menu->owner_ids[menu->owner_selection]);
    UiMenu_SlideCursor(selection * 24 - 10, 16);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = Math_Mod(selection + count, count);
            Owner_GetStateFar(menu->owner_ids[selection]);
            marker = menu->marker;
            marker->attributes.x = marker->x =
                ((menu->window->x + selection * 3) << 3) - 2;
            if (mode == 0) {
                Menu_DrawOwnerStatusPanel(menu->status_window,
                                          menu->owner_ids[selection], 0, 0);
                PsynergyMenu_CallIconRoutineWithValue(
                    (s32)menu, menu->owner_ids[selection]);
                if (!GameFlag_TestFar(0x151) && !shown) {
/* The Japanese edition clears the info window and draws the description
   as a message; the others redraw the saved window and draw it in place. */
#if defined(TBS_EDITION_JA)
                    RenderOutput_ClearListFar(menu->info_window);
                    UiText_DrawMessageAt(
                        (menu->selected_action & 0x3fff) + (s32)&MsgAbilityDescription,
                        menu->info_window, 0, 0);
#else
                    RenderOutput_RedrawSavedRectFar(menu->info_window);
                    UiText_DrawCharacterAtOffsetFar(
                        (menu->selected_action & 0x3fff) + (s32)&MsgAbilityDescription,
                        menu->info_window, 0, 0);
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
            Audio_PlayCue(112);
            result = menu->owner_ids[selection];
            break;
        }
        if (gKeyState & 2) {
            Audio_PlayCue(113);
            result = -1;
            break;
        }
        if (gKeysRepeat & 32) {
            Audio_PlayCue(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & 16) {
            Audio_PlayCue(111);
            selection++;
            pending = 1;
        }
    }
    marker = menu->marker;
    menu->selection = selection;
    UiIcon_PrepareObject(marker);
    marker->state = 13;
    WaitFrames(1);
    menu->selection = selection;
    menu->selected_owner = menu->owner_ids[selection];
    menu->target_owner = menu->owner_ids[selection];
    return result;
}
