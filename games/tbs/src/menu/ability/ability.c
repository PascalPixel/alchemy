#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "far_runtime.h"
#include "owner_state.h"
#include "psynergy_menu.h"
#include "game_state.h"
#include "types.h"
#include "io_reg.h"
#include "system.h"
#include "btlsys.h"
#include "text/msg_ids.h"

#define gKeyState (*(volatile u32 *)&gKeyState)
#define gKeysRepeat (*(volatile u32 *)&gKeysRepeat)
#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)
#define RenderOutput_RedrawSavedRectFar ((void (*)(s32))RenderOutput_RedrawSavedRectFar)

#define UiText_GetResourceDimensionsFar ((s32 (*)(s32,s32*,s32*,s32*,s32*))UiText_GetResourceDimensionsFar)
#define UiText_DrawStringInWindowFar ((void (*)(const u8*,s32*,s32,s32))UiText_DrawStringInWindowFar)

void Menu_SpawnIconEntries(struct PsynergyMenuState *menu, s32 window);
void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 mode);
void AudioCommand_PlayFar(s32 cue);

s32 PsynergyMenu_SetupActionIcons(u16 *owner_ids, u16 *unused)
{
    /* FAKEMATCH: the low-byte CollectActions result keeps adds r5,r0,#0; explicit narrowing of s32 selects mov r5,r0. */
    struct PsynergyMenuState *menu;
    s32 selection;
    s32 count;
    s32 pending;
    s32 result;
    struct BtlUnit *owner;
    s32 window;
    s32 shown;
    s32 i;
    u16 *actions;
    s32 action_count;
    u8 found;

    menu = (struct PsynergyMenuState *)gMenuWork;
    selection = menu->tab_index[0];
    count = (s8)menu->tab_counts[0];
    pending = 1;
    result = 0;
    shown = 0;
    menu->mode = result;
    owner = UnitGetFar(owner_ids[selection]);
    if (UiWindow_UpdateOrCreate(&menu->psynergy_window, 13, 3, 17, 10, 2))
        Menu_SpawnIconEntries(menu, menu->psynergy_window);
    if (UiWindow_UpdateOrCreate(&menu->shortcut_window, 13, 13, 17, 4, 2)) {
        menu->cursor_icon = RenderOutput_CreateFromResourceFar(2, 0, (struct RenderInput *)menu->shortcut_window, 0, result);
        menu->cursor_icon->active = 13;
    }
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (count + selection) % count;
            window = menu->status_window;
            owner = UnitGetFar(owner_ids[selection]);
            PsynergyMenu_RefreshOwnerPsynergy(owner_ids[selection]);
            Menu_DrawOwnerStatusPanel(window, owner_ids[selection], 0, 0);
            PsynergyMenu_DrawShortcuts(menu->shortcut_window, owner_ids[selection]);
            PsynergyMenu_CallIconRoutineWithValue((s32)menu, owner_ids[selection]);
            for (i = 3; i >= 0; i--)
                menu->row_positions[i] = 0x1e;
            menu->row_positions[selection] = 0x1a;
            if (!GameFlag_TestFar(0x151) && !shown) {
                RenderOutput_ClearListFar((s32)menu->info_window);
#if EDITION_INTERNATIONAL
                RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
#endif
                UiText_DrawWorkValueWithLabel((s32)menu->info_window);
                shown = 1;
            } else {
                GameFlag_ClearBitFar(0x151);
            }
        }
        UiMenu_PositionCursor(selection * 24 - 10, 16);
        WaitFrames(1);
        if (gKeyState & KEY_A) {
            if (menu->psynergy_count) {
                AudioCommand_PlayFar(112);
                result = owner_ids[selection];
                break;
            }
            AudioCommand_PlayFar(114);
        }
        if ((gKeyState & KEY_L) || (gKeyState & KEY_R)) {
            result = owner_ids[selection];
            if (gKeyState & KEY_L)
                menu->mode = 1;
            else
                menu->mode = 2;
            actions = MemAlloc(64);
            found = PsynergyMenu_CollectActions((struct BtlUnit *)owner, actions, 1);
            MemFree(actions);
            action_count = (s8)found;
            if (action_count == 0) {
                menu->mode = action_count;
                AudioCommand_PlayFar(114);
            } else {
                AudioCommand_PlayFar(112);
                break;
            }
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            AudioCommand_PlayFar(111);
            selection++;
            pending = 1;
        }
    }
    menu->tab_index[0] = selection;
    menu->selected_owner = owner_ids[selection];
    menu->owner_ids[0] = owner_ids[selection];
    return result;
}

void UiText_DrawCharacterAtOffsetFar(s32 message, s32 *, s32 x, s32 y);

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

void Menu_DrawOwnerStatusPanel(s32 window, s32 owner, s32 slot, s32 style);
void AudioCommand_PlayFar(s32 cue);

void PsynergyMenu_RefreshOwnerPsynergy(s32 owner_id)
{
    u16 *psynergies;
    struct PsynergyMenuState *menu;
    struct BtlUnit *owner;

    menu = gMenuWork;
    owner = (struct BtlUnit *)UnitGetFar(owner_id);
    psynergies = menu->psynergies;
    menu->psynergy_count =
        PsynergyMenu_CollectActions(owner, psynergies, 2);
    RenderOutput_RedrawSavedRectFar(menu->psynergy_window);
    PsynergyMenu_RefreshOwnerEntries(0x6c, 0x20, 8);
    PsynergyMenu_DrawPsynergyIcons(psynergies);
    if (menu->psynergy_count == 0) {
        UiText_DrawCharacterAtOffsetFar(
            (s32)MsgPsynergyMenuEmpty,
            (s32 *)menu->psynergy_window,
            0,
            0x18);
    }
}

s32 PsynergyMenu_ReturnTrue(void)
{
    return 1;
}

void PsynergyMenu_NoOp(void)
{
}

s32 PsynergyMenu_SelectTarget(s32 mode)
{
    /* FAKEMATCH: the unsigned marker/OAM view keeps halfword narrowing before the nine-bit assignment; direct casts remove it. */
    struct PsynergyMenuState *menu;
    s32 selection;
    s32 count;
    s32 pending;
    s32 result;
    s32 shown;
    struct PsynergyTargetMarker *marker;

    menu = ((struct PsynergyMenuState *)gMenuWork);
    selection = menu->tab_index[1];
    count = menu->owner_count;
    pending = 1;
    result = 0;
    shown = 0;
    UnitGetFar(menu->owner_table[menu->tab_index[0]]);
    UiMenu_SlideCursor(selection * 24 - 10, 16);
    while (!GameFlag_TestFar(0x150)) {
        if (pending) {
            pending = 0;
            selection = (selection + count) % count;
            UnitGetFar(menu->owner_table[selection]);
            marker = (struct PsynergyTargetMarker *)menu->pane_icon[1];
            marker->attributes.x = marker->x =
                ((((struct RenderInput *)menu->auxiliary_window)->x + selection * 3) << 3) - 2;
            if (mode == 0) {
                Menu_DrawOwnerStatusPanel(menu->status_window,
                                          menu->owner_table[selection], 0, 0);
                PsynergyMenu_CallIconRoutineWithValue(
                    (s32)menu, menu->owner_table[selection]);
                if (!GameFlag_TestFar(0x151) && !shown) {
#if EDITION_INTERNATIONAL
                    RenderOutput_RedrawSavedRectFar((s32)menu->info_window);
                    UiText_DrawCharacterAtOffsetFar(
                        (menu->pane_action[0] & 0x3fff) + (s32)MsgAbilityDescription,
                        (s32)menu->info_window, 0, 0);
#else
                    RenderOutput_ClearListFar((s32)menu->info_window);
                    UiText_DrawResourceFar(
                        (menu->pane_action[0] & 0x3fff) + (s32)MsgAbilityDescription,
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
        if (gKeyState & KEY_A) {
            AudioCommand_PlayFar(112);
            result = menu->owner_table[selection];
            break;
        }
        if (gKeyState & KEY_B) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (gKeysRepeat & KEY_LEFT) {
            AudioCommand_PlayFar(111);
            selection--;
            pending = 1;
        }
        if (gKeysRepeat & KEY_RIGHT) {
            AudioCommand_PlayFar(111);
            selection++;
            pending = 1;
        }
    }
    marker = (struct PsynergyTargetMarker *)menu->pane_icon[1];
    menu->tab_index[1] = selection;
    UiIcon_PrepareObject((struct RenderOutput *)marker);
    marker->state = 13;
    WaitFrames(1);
    menu->tab_index[1] = selection;
    menu->selected_owner = menu->owner_table[selection];
    menu->owner_ids[1] = menu->owner_table[selection];
    return result;
}

s32 PsynergyMenu_SetShortcut(s32 owner, s32 psynergy, s32 shortcut)
{
    s32 id = psynergy & 0x3fff;
    s32 code =
        (owner << 10) | id;

    if (shortcut == 0) {
        gGameState.first_shortcut = code;
    } else {
        gGameState.second_shortcut = code;
    }
    return 1;
}

#if EDITION_INTERNATIONAL

void UiWork_PushValueSlotFar(s32 value, s32 slot);

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
    s32 left;
    s32 top;
    s32 width;
    s32 height;
    s32 wide;

    if (gGameState.first_shortcut != 0 &&
        gGameState.second_shortcut != 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutChangeHelp, (s32 *)window, 0, -8);
    else
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutLabel, (s32 *)window, 0, -8);

    UiText_GetResourceDimensionsFar(
        (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 10)
        wide = 1;
    else
        wide = 0;
    if (gGameState.first_shortcut != 0) {
        UiWork_PushValueSlotFar(gGameState.first_shortcut & 0x3ff, 4);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutNameL, (s32 *)window, 0, 0);
        if (wide == 0)
            UiText_DrawStringAtOffsetFar(
                UnitGetFar(gGameState.first_shortcut >> 10),
                (s32 *)window, 80, 0);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyL, (s32 *)window, 0, 0);
    }

    UiText_GetResourceDimensionsFar(
        (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 10)
        wide = 1;
    else
        wide = 0;
    if (gGameState.second_shortcut != 0) {
        UiWork_PushValueSlotFar(gGameState.second_shortcut & 0x3ff, 4);
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutNameR, (s32 *)window, 0, 8);
        if (wide == 0)
            UiText_DrawStringAtOffsetFar(
                UnitGetFar(gGameState.second_shortcut >> 10),
                (s32 *)window, 80, 8);
        UiWork_SetParamNibbleFar(15);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyR, (s32 *)window, 0, 8);
    }
    return 1;
}
#else

s32 PsynergyMenu_DrawShortcuts(s32 window, s32 owner)
{
    s32 left;
    s32 top;
    s32 width;
    s32 height;
    u16 wide;

    if (gGameState.first_shortcut != 0 &&
        gGameState.second_shortcut != 0)
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutChangeHelp, (s32 *)window, 0, -8);
    else
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutLabel, (s32 *)window, 0, -8);

    UiText_GetResourceDimensionsFar(
        (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 11)
        wide = 1;
    else
        wide = 0;
    if (wide)
        UiText_DrawStringInWindowFar(Menu_ShortcutLString, (s32 *)window, 0, 0);
    else
        UiText_DrawStringInWindowFar(Menu_ShortcutLColonString, (s32 *)window, 0, 0);
    if (gGameState.first_shortcut != 0) {
        if (wide)
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 8, 0);
        else
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.first_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 16, 0);
        UiText_DrawStringAtOffsetFar(
            UnitGetFar(gGameState.first_shortcut >> 10),
            (s32 *)window, 80, 0);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyL, (s32 *)window, 24, 0);
    }

    UiText_GetResourceDimensionsFar(
        (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
        &left, &top, &width, &height);
    if (width < 0 || width > 11)
        wide = 1;
    else
        wide = 0;
    if (wide)
        UiText_DrawStringInWindowFar(Menu_ShortcutRString, (s32 *)window, 0, 8);
    else
        UiText_DrawStringInWindowFar(Menu_ShortcutRColonString, (s32 *)window, 0, 8);
    if (gGameState.second_shortcut != 0) {
        if (wide)
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 8, 8);
        else
            UiText_DrawCharacterAtOffsetFar(
                (gGameState.second_shortcut & 0x3ff) + (s32)MsgAbilityName,
                (s32 *)window, 16, 8);
        UiText_DrawStringAtOffsetFar(
            UnitGetFar(gGameState.second_shortcut >> 10),
            (s32 *)window, 80, 8);
        UiWork_SetParamNibbleFar(15);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgShortcutEmptyR, (s32 *)window, 24, 8);
    }
    return 1;
}
#endif
