#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "owner_state.h"
#include "global_cells.h"
#include "tbs_edition.h"
#include "system.h"
#include "iwram_call.h"
#include "battle_unit.h"
#include "mem.h"
#include "ui.h"
#include "djinn_menu.h"
#include "text/msg_ids.h"
#include "io_reg.h"
#include "keystate.h"
#include "window.h"
#include "game_state.h"
#include "btlsys.h"
void UiText_DrawNumberInWindowFar(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
struct BtlAct * BtlActGet(s32 action_id);

#define gWindowWork (*(struct UiWork * *)&gWindowWork)

#define UiIcon_DrawVariantWithTileOffset ((void (*)(s32,s32,s32,s32))UiIcon_DrawVariantWithTileOffset)
#define UiIcon_CreateWithLoadedResource ((void (*)(s32,s32,s32,s32))UiIcon_CreateWithLoadedResource)
#define SideObject_CreateFar ((void (*)(s32,s32,s32,s32,s32,s32))SideObject_CreateFar)
#define Djinn_DeactivateFar ((void (*)(s32,s32,s32))Djinn_DeactivateFar)
#define OwnerAction_DiffSlots ((s8 (*)(void*,void*,u16*,s32*,s32*))OwnerAction_DiffSlots)

extern struct DjinnMenuWork *gMenuWork;

#define DJINN_ENTRY_SET (-0x8000)



void UiText_DrawNumberAtOffsetFar(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const void *text, s32 window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 palette);
void UiWork_SetParamNibbleFar(s32 value);
void UiWindow_DrawDividerLineFar(s32 window, s32 x0, s32 y0, s32 x1, s32 y1);
void Djinn_ActivateFar(s32 owner, s32 element, s32 index);

#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)
#if EDITION_INTERNATIONAL
#define CURSOR_WIDTH 6
#else
#define CURSOR_WIDTH 5
#endif
#undef Djinn_DeactivateFar
#define Djinn_ActivateFar ((s32 (*)(s32, s32, s32))Djinn_ActivateFar)
s32 Trade_RemoveOfferFar(s32 owner, s32 element, s32 number);
void UiWork_PushValueSlotFar(s32 value, s32 kind);
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, u32 flags);
void UiWork_FinalizeFar(struct UiWindow *work, s32 mode);
void UiWindow_ApplyRectAtObjectOrigin(s32 window, s32 x, s32 y, s32 width, s32 height, s32 kind);
s32 Djinn_MarkBalancedEntries(s8 *balanced, s32 self);
void FourObjectMotion_SetSlotPhase(s32 slot, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 slot, s32 element, s32 kind);

u32 __umodsi3(u32, u32);

s32 DjinnMenu_SelectDjinn(s32 mode)
{
    s8 buf[8];
    u8 balanced[16];
    struct DjinnMenuWork *state = gMenuWork;
    struct DjinnMenuBackdrop *lists = state->backdrop;
    struct BtlUnit *owner;
    s32 window;
    s32 redraw = 1;
    u32 refresh;
    s32 i;
    s32 j;
    s32 sel;
    s32 x = (u16)(state->selection[mode] % 10);
    s32 y = (u16)(state->selection[mode] / 10);
    s32 savedY = 0;
    u32 djinn = 0;
    s32 groupMode = 0;
    s32 setAll = 0;
    s32 result;
    s32 message;
    u32 pressed;
    s32 count;
    u32 repeat;
    struct UiWindow *work;
    s32 step;
    s8 *status;

    sel = -1;
    status = buf;
    for (i = 7; i >= 0; i--)
        status[i] = 0;
    if (mode == 0) {
        DjinnMenu_DrawElementList(&lists->list);
        for (i = 0; i < state->owner_count; i++) {
            if (lists->list.counts[i] == 0)
                status[i] = 4;
        }
        savedY = y;
        for (i = 0; i < state->owner_count; i++) {
            if (status[x] == 4) {
                x++;
                x = Menu_GetModuloOfSum(x, state->owner_count);
            }
        }
    } else {
        Djinn_MarkBalancedEntries((s8 *)balanced, state->selected_column[0]);
        for (i = 0; i < state->owner_count; i++) {
            if (i == state->selected_column[0]) {
                buf[i] = 7;
            } else if (balanced[i] != 0) {
                buf[i] = 0;
            } else {
                buf[i] = 3;
                if (lists->list.counts[i] == 0)
                    status[i] = buf[i] | 4;
            }
        }
    }
    if (mode == 1) {
        s32 fromX;
        s32 fromY;
        u16 source_cursor;
        s32 n;
        source_cursor = state->selection[0];
        fromX = (u16)__umodsi3(source_cursor, 10);
        fromY = (u16)__udivsi3(source_cursor, 10);
        Menu_DrawAtWindowOffset(state->list_window, fromX * 7 + 1, fromY + 2, CURSOR_WIDTH, 1, 14);
        UiWindow_ApplyRectAtObjectOrigin(state->list_window, fromX * 7 + 1, 2, CURSOR_WIDTH, 7, 6);
        for (n = 0; n < state->owner_count; n++) {
            if (n == state->selected_column[0]) {
                if (state->pair_entries[0] & 0x8000)
                    message = (s32)&MsgDjinnStandbyAction;
                else
                    message = (s32)&MsgDjinnSetAction;
            } else if (buf[n] & 2) {
                message = (s32)&MsgDjinnGiveAction;
            } else {
                message = (s32)&MsgDjinnTradeAction;
            }
            UiText_DrawCharacterAtOffsetFar(message, state->list_window, n * 56 + 8, 8);
        }
    }
    RenderOutput_RedrawSavedRectFar(state->help_window);
    state->cursor->active = 1;
    for (;;) {
        if (redraw != 0) {
            refresh = redraw;
            redraw = 0;
            sel = -1;
            if (!(buf[x] & 1))
                sel = y;
            window = state->status_window;
            owner = UnitGetFar(state->owners[x]);
            RenderOutput_RedrawSavedRectFar(window);
            UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(owner->class_index + (s32)MsgClassName, window, 0, 8);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, 48, 0);
            UiText_DrawNumberInWindowFar(owner->level, 2, window, 72, 0);
            if (mode == 0)
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnCharacterStatus, window, 0, 16);
            if (sel != -1)
                djinn = lists->list.ids[x][sel];
            RenderOutput_RedrawSavedRectFar(state->help_window);
            if (mode == 1) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
                UiWindow_SetTilemapEntryFar(state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 0, 0, 0);
                UiWork_PushValueSlotFar(state->shown_owner[0], 1);
                UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20 + DJINN_NUMBER(state->pair_entries[0]) + 300, 4);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 8, 0);
#else
                UiWork_PushValueSlotFar(state->shown_owner[0], 1);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 0, 0);
                UiWork_PushValueSlotFar(DJINN_ELEMENT(state->pair_entries[0]) * 20 + DJINN_NUMBER(state->pair_entries[0]) + 300, 4);
                UiWindow_SetTilemapEntryFar(state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 6, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel + 1, state->help_window, 56, 0);
#endif
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel + 2, state->help_window, 0, 8);
            }
            if (sel == -1) {
                FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
            } else if (mode == 0) {
                if (groupMode) {
                    if (setAll == 0) {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnSetAll, state->help_window, 0, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnAllStandby, state->help_window, 0, 0);
                    }
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        else
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnRecovering, state->help_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                } else {
                    UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose, state->help_window, 0, 0);
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                            UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose + 3, state->help_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        } else {
                            UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose + 2, state->help_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        }
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnChoose + 4, state->help_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                }
            } else {
                if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                    else
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                    FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 0);
                } else {
                    FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                    FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 54, 1);
                }
                if (refresh & 2)
                    FourObjectMotion_SetSlotPhase(mode, 0);
            }
            RenderOutput_ClearListFar(state->list_window);
            if (sel != -1) {
                UiText_DrawCharacterAtOffsetFar((s32)&MsgUnleashEffect, state->list_window, 0, 80);
#if EDITION_INTERNATIONAL
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 96, 224, 104);
                UiText_DrawCharacterAtOffsetFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + (s32)MsgDjinnEffect, state->list_window, 0, 96);
#else
                UiText_DrawResourceFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + (s32)MsgDjinnEffect, state->list_window, 0, 96);
#endif
            }
            if (!(buf[x] & 1))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 14);
            gWindowWork->dirty = 1;
        }
        if (buf[x] & 1)
            UiMenu_PositionCursor(x * 56 - 8, 52);
        else
            UiMenu_PositionCursor(x * 56 - 8, y * 8 + 60);
        WaitFrames(1);
        if (!(gKeysHeld & KEY_R) || (gKeysPressedLatch & 0x100)) {
            if (groupMode)
                redraw = 1;
            groupMode = 0;
            setAll = 0;
        }
        repeat = gKeysRepeat;
        pressed = gKeyState;
        step = lists->tutorial_step;
        if (step != 0) {
            pressed = 0;
            lists->tutorial_timer++;
            repeat = 0;
            switch (step) {
            case 28:
                while (!(gKeyState & KEY_A)) {
                    UiMenu_PositionCursor(150, 26);
                    WaitFrames(1);
                }
                pressed = 2;
                repeat = 2;
                break;
            case 27:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialRecovery, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialSetAgain, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    lists->tutorial_timer = 0;
                    BattlePlacement_UpdateTimedEntriesFar();
                    BattlePlacement_UpdateTimedEntriesFar();
                    BattlePlacement_UpdateTimedEntriesFar();
                    Djinn_DeactivateFar(0, 0, 0);
                    Trade_AddOfferFar(0, 0, 0);
                    UnitCalcFar(0);
                    repeat = 2;
                    pressed = 2;
                }
                break;
            case 1:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialGive, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 2;
                }
                break;
            case 2:
            case 4:
            case 8:
            case 9:
            case 22:
                if (lists->tutorial_timer == 90) {
                    repeat = 1;
                    pressed = 1;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                }
                break;
            case 3:
                if (lists->tutorial_timer == 90) {
                    repeat = 16;
                    pressed = 16;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 4;
                }
                break;
            case 6:
            case 7:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialSetAfterGive, 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 8;
                }
                break;
            case 13:
                if (lists->tutorial_timer == 40) {
                    lists->tutorial_timer = 0;
                    repeat = 2;
                    pressed = 2;
                    lists->tutorial_step++;
                }
                break;
            case 14:
            case 16:
            case 17:
                if (lists->tutorial_timer == 40) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step++;
                }
                break;
            case 15:
                if (lists->tutorial_timer == 60) {
                    gGameState.message_speed = 1;
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialPowers, 9, 9, 1);
                    UiMenu_SlideCursor(2, 146);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialUnleash, 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar((s32)&MsgDjinnTutorialDemonstrate, 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 16;
                }
                break;
            case 18:
            case 20:
                if (lists->tutorial_timer == 90) {
                    repeat = 1;
                    pressed = 1;
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 21;
                }
                break;
            case 21:
                if (lists->tutorial_timer == 90) {
                    lists->tutorial_timer = 0;
                    repeat = 32;
                    pressed = 32;
                    lists->tutorial_step = 22;
                }
                break;
            case 24:
                if (lists->tutorial_timer == 60) {
                    lists->tutorial_timer = 0;
                    lists->tutorial_step = 25;
                    repeat = 2;
                    pressed = 2;
                }
                break;
            }
        }
        if (mode == 0 && (pressed & 0x100)) {
            if (sel == -1) {
                AudioCommand_PlayFar(114);
                continue;
            }
            i = 0;
            if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                i = 1;
            groupMode = 1;
            gKeysPressedLatch = 0;
            if (i == 0) {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar(state->list_window);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
                UiText_DrawResourceFar((s32)&MsgDjinnNowRecovering, state->list_window, 0, 96);
                redraw = 1;
                continue;
            }
            if (djinn & 0x8000) {
                AudioCommand_PlayFar(175);
                Djinn_DeactivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                Trade_AddOfferFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            } else {
                AudioCommand_PlayFar(139);
                Djinn_ActivateFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
                Trade_RemoveOfferFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn));
            }
            UnitCalcFar(DJINN_OWNER(djinn));
            DjinnMenu_DrawElementList(&lists->list);
            redraw = 1;
        }
        if (mode == 0 && (pressed & 0x200)) {
            result = 7;
            AudioCommand_PlayFar(112);
            break;
        }
        if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
            i = 1;
            if (!(buf[x] & 1)) {
                if (sel == -1) {
                    AudioCommand_PlayFar(114);
                    continue;
                }
                i = 0;
                if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    i = 1;
            }
            if (i == 0) {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar(state->list_window);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
                UiText_DrawResourceFar((s32)&MsgDjinnNowRecovering, state->list_window, 0, 96);
                continue;
            }
            if (mode == 1) {
                if (buf[x] & 1) {
                    if (x == state->selected_column[0]) {
                        if (state->pair_entries[0] & 0x8000)
                            result = 2;
                        else
                            result = 1;
                    } else {
                        result = 3;
                    }
                } else {
                    result = 4;
                }
            } else {
                result = 1;
            }
            AudioCommand_PlayFar(112);
            break;
        }
        if (pressed & 8) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        }
        if (pressed & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (mode == 0 && (pressed & 4)) {
            if (groupMode) {
                setAll ^= 1;
                if (setAll)
                    AudioCommand_PlayFar(139);
                else
                    AudioCommand_PlayFar(175);
                for (i = 0; i < state->owner_count; i++) {
                    for (j = 0; j < lists->list.counts[i]; j++) {
                        u32 entry = lists->list.ids[i][j];
                        s32 usable;
                        usable = 0;
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)) || Djinn_IsActiveFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
                            usable = 1;
                        if (usable) {
                            if (setAll) {
                                if (!(entry & 0x8000)) {
                                    Djinn_ActivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    Trade_RemoveOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    UnitCalcFar(DJINN_OWNER(entry));
                                }
                            } else if (entry & 0x8000) {
                                Djinn_DeactivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                Trade_AddOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                UnitCalcFar(DJINN_OWNER(entry));
                            }
                        }
                    }
                }
                DjinnMenu_DrawElementList(&lists->list);
                redraw = 1;
            } else {
                result = 10;
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
                AudioCommand_PlayFar(112);
                break;
            }
        }
        if (repeat & 0x40) {
            AudioCommand_PlayFar(111);
            if (buf[x] & 4)
                continue;
            Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            if (buf[x] & 4)
                continue;
            if (buf[x] & 1) {
                buf[x] &= ~1;
                y = 0;
            } else if (y == 0 && (buf[x] & 2)) {
                buf[x] |= 1;
                redraw = 2;
                continue;
            }
            y--;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            savedY = y;
            redraw = 2;
            continue;
        }
        if (repeat & 0x80) {
            AudioCommand_PlayFar(111);
            if (buf[x] & 4)
                continue;
            Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            y++;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            if ((buf[x] & 1) && !(buf[x] & 4)) {
                buf[x] &= ~1;
                y = 0;
            } else if (y == 0 && (buf[x] & 2)) {
                buf[x] |= 1;
            }
            savedY = y;
            redraw = 2;
            continue;
        }
        if (repeat & 0x20) {
            AudioCommand_PlayFar(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            x--;
            x = Menu_GetModuloOfSum(x, state->owner_count);
            if (mode == 0) {
                for (i = 0; i < state->owner_count; i++) {
                    if (buf[x] == 4) {
                        x--;
                        x = Menu_GetModuloOfSum(x, state->owner_count);
                    }
                }
            }
            y = savedY;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            redraw = 2;
            continue;
        }
        if (repeat & 0x10) {
            AudioCommand_PlayFar(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, CURSOR_WIDTH, 1, 15);
            x++;
            x = Menu_GetModuloOfSum(x, state->owner_count);
            if (mode == 0) {
                for (i = 0; i < state->owner_count; i++) {
                    if (buf[x] == 4) {
                        x++;
                        x = Menu_GetModuloOfSum(x, state->owner_count);
                    }
                }
            }
            y = savedY;
            count = lists->list.counts[x];
            if (count == 0)
                count = 1;
            y = Menu_GetModuloOfSum(y, count);
            redraw = 2;
        }
    }
    state->selected_column[mode] = x;
    if (sel != -1) {
        /* FAKEMATCH: entry fixed in r2 keeps regmove from tying the owner mask's and to entry where it dies; the native ties it to the 0xf00 register r3 (differ 10 -> 0) */
        register u32 entry asm("r2");
        state->pair_entries[mode] = entry = lists->list.ids[x][sel];
        state->pair_index[mode] = entry % 32;
        state->pair_element[mode] = DJINN_ELEMENT(entry);
        state->pair_owner[mode] = DJINN_OWNER(entry);
    }
    state->selection[mode] = x + y * 10;
    return result;
}
#undef Djinn_ActivateFar
#define Djinn_DeactivateFar ((void (*)(s32,s32,s32))Djinn_DeactivateFar)

s32 Djinn_ListOwnerEntries(u16 *out, s32 owner, s32 element)
{
    struct BtlUnit *state = UnitGetFar(owner);
    s32 count = 0;
    s32 row;
    s32 bit;
    s32 entry;

    if (element == -1) {
        for (row = 0; row < 4; row++) {
            for (bit = 0; bit < 20; bit++) {
                if (state->djinn_active[row] & (1 << bit)) {
                    entry = (row << 5) | bit | DJINN_ENTRY_SET;
                    entry |= owner << 8;
                    out[count++] = entry;
                } else if (state->djinn_available[row] & (1 << bit)) {
                    out[count++] = (row << 5) | bit | (owner << 8);
                }
            }
        }
    } else {
        for (bit = 0; bit < 20; bit++) {
            if (state->djinn_active[element] & (1 << bit))
                out[count++] = (element << 5) | bit | DJINN_ENTRY_SET;
            else if (state->djinn_available[element] & (1 << bit))
                out[count++] = (element << 5) | bit;
        }
    }
    return count;
}

#if EDITION_INTERNATIONAL
#define ALT_PARAM 2
#else
#define ALT_PARAM 4
#endif

s32 Menu_RunPairedEntryAction(s32 mode, s32 param)
{
    s32 sp14;
    struct DjinnMenuWork *menu;

    menu = gMenuWork;
    if (mode == 0) {
        sp14 = mode;
        DjinnMenu_DrawStatPreview(menu->second_window, 0, 0, menu->pair_owner[1], 1, mode, 2, param, 1);
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->pair_owner[0], mode, 1, 2, param, mode);
    } else {
        DjinnMenu_DrawStatPreview(menu->second_window, 0, 0, menu->shown_owner[1], 1, 0, ALT_PARAM, param, 1);
        DjinnMenu_DrawStatPreview(menu->window, 0, 0, menu->shown_owner[0], 0, 0, 1, param, 0);
    }
    return 1;
}

#if EDITION_INTERNATIONAL
#define PREVIEW_DIGITS 3
#define PREVIEW_CURRENT_X 48
#define PREVIEW_MAX_X 80
#define PREVIEW_SLASH_X 72
#define PREVIEW_LUCK_X 56
#define PREVIEW_ARROW_X icon_x
#define PREVIEW_PP_X 88
#else
#define PREVIEW_DIGITS 4
#define PREVIEW_CURRENT_X 32
#define PREVIEW_MAX_X 72
#define PREVIEW_SLASH_X 64
#define PREVIEW_LUCK_X 48
#define PREVIEW_ARROW_X (x * 8 + 64)
#define PREVIEW_PP_X 56
#endif
static __inline__ void Unit_Copy(struct BtlUnit *dst, struct BtlUnit *src)
{
    Iwram_CopyWords(dst, src, sizeof(struct BtlUnit));
}

s32 DjinnMenu_DrawStatPreview(s32 window, s32 x, s32 y, s32 owner,
    s32 give, s32 take, s32 mode, s32 page, s32 side)
{
    struct BtlUnit *state;
    struct BtlUnit *saved;
    s32 give_element;
    s32 give_index;
    s32 give_set;
    s32 take_element;
    s32 take_index;
    s32 take_set;
    s32 lost;
    s32 gained;
    u16 actions[48];
    struct DjinnMenuWork *menu;

    state = UnitGetFar(owner);
    menu = gMenuWork;
    give_element = menu->pair_element[give];
    give_index = menu->pair_index[give];
    give_set = menu->pair_entries[give] & 0x8000;
    take_element = menu->pair_element[take];
    take_index = menu->pair_index[take];
    take_set = menu->pair_entries[take] & 0x8000;
    saved = (struct BtlUnit *)MemAlloc(sizeof(struct BtlUnit));
    Unit_Copy(saved, state);

    if (page == 0) {
        if (mode == 3) {
            UiText_DrawNumberInWindowFar(state->max_hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 56);
            UiText_DrawNumberInWindowFar(state->max_ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_MAX_X, y * 8 + 64);
            UiText_DrawNumberInWindowFar(state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
            UiText_DrawNumberInWindowFar(state->ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 56);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextSlash, window, x * 8 + PREVIEW_SLASH_X, y * 8 + 64);
        } else {
            UiText_DrawNumberInWindowFar(state->hp, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 56);
            UiText_DrawNumberInWindowFar(state->ep, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 64);
        }
        UiText_DrawNumberInWindowFar(state->atk, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 72);
        UiText_DrawNumberInWindowFar(state->def, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 80);
        UiText_DrawNumberInWindowFar(state->agi, PREVIEW_DIGITS, window, x * 8 + PREVIEW_CURRENT_X, y * 8 + 88);
        UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + PREVIEW_LUCK_X, y * 8 + 96);
    }

    switch (mode) {
    case 0:
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    case 1:
        give_index &= 31;
        Djinn_DeactivateFar(owner, give_element, give_index);
        break;
    case 2:
        if (give_set) {
            give_index &= 31;
            Djinn_DeactivateFar(owner, give_element, give_index);
        }
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        if (take_set)
            Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    case 4:
        Djinn_AddToOwnerFar(owner, take_element, take_index & 31);
        if (take_set)
            Djinn_ActivateFar(owner, take_element, take_index & 31);
        break;
    }

    UnitCalcFar(owner);
    state = UnitGetFar(owner);

    if (page == 0) {
        s32 label;

        UiText_DrawStringAtOffsetFar(state->name, window, x * 8 + 40, y * 8);
        UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, x * 8 + 40, y * 8 + 16);
        UiText_DrawNumberInWindowFar(state->level, 2, window, x * 8 + 88, y * 8 + 16);
        label = (s32)MsgPreviewStatLabel;
        UiText_DrawCharacterAtOffsetFar(label, window, x * 8, y * 8 + 56);
        UiText_DrawCharacterAtOffsetFar(label + 1, window, x * 8, y * 8 + 64);
        UiText_DrawCharacterAtOffsetFar(label + 2, window, x * 8, y * 8 + 72);
        UiText_DrawCharacterAtOffsetFar(label + 3, window, x * 8, y * 8 + 80);
        UiText_DrawCharacterAtOffsetFar(label + 4, window, x * 8, y * 8 + 88);
        UiText_DrawCharacterAtOffsetFar(label + 5, window, x * 8, y * 8 + 96);
        UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + saved->class_index, window, x * 8, y * 8 + 32);
    }

    {
        s32 i = 0;

    if (page == 0) {
        s32 column;
#if EDITION_INTERNATIONAL
        s32 icon_x;
#endif
        s32 px;

        if (saved->class_index != state->class_index) {
            px = x * 8;
            UiText_DrawCharacterAtOffsetFar((s32)MsgClassName + state->class_index, window, px, y * 8 + 48);
            UiWindow_SetTilemapEntryFar(window, 0xf296, x + 2, 5, 0);
        }
        column = x;
        if (saved->class_index != state->class_index)
            column += 5;
        for (i = 0; i < 4; i++) {
            UiWindow_SetTilemapEntryFar(window, 0x5001 + i, column + i * 2, y + 5, 0);
            UiWindow_SetTilemapEntryFar(window,
                state->djinn_active_counts[i] + 0xf030,
                column + i * 2 + 1, y + 5, 0);
        }
#if EDITION_INTERNATIONAL
        icon_x = x * 8 + 70;
#endif
        if (state->hp != saved->hp) {
            UiText_DrawNumberInWindowFar(state->hp, 4, window, x * 8 + 72, y * 8 + 56);
            if (state->hp > saved->hp)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 56, 1);
        }
        if (state->ep != saved->ep) {
            UiText_DrawNumberInWindowFar(state->ep, 4, window, x * 8 + 72, y * 8 + 64);
            if (state->ep > saved->ep)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 64, 1);
        }
        if (state->atk != saved->atk) {
            UiText_DrawNumberInWindowFar(state->atk, 4, window, x * 8 + 72, y * 8 + 72);
            if (state->atk > saved->atk)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 72, 1);
        }
        if (state->def != saved->def) {
            UiText_DrawNumberInWindowFar(state->def, 4, window, x * 8 + 72, y * 8 + 80);
            if (state->def > saved->def)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 80, 1);
        }
        if (state->agi != saved->agi) {
            UiText_DrawNumberInWindowFar(state->agi, 4, window, x * 8 + 72, y * 8 + 88);
            if (state->agi > saved->agi)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 88, 1);
        }
        if (state->luck != saved->luck) {
            UiText_DrawNumberInWindowFar(state->luck, 2, window, x * 8 + 88, y * 8 + 96);
            if (state->luck > saved->luck)
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 0);
            else
                UiIcon_DrawVariantWithTileOffset(window, PREVIEW_ARROW_X, y * 8 + 96, 1);
        }
    }
    }

    if (page > 0) {
        s32 rows;
        s32 first;
        s8 count;
        s8 row;
        s8 line;

        rows = 6 - (mode != 3);
        first = rows * (page - 1);
        count = OwnerAction_DiffSlots(saved->action_slots,
            state->action_slots, actions, &gained, &lost);
        line = 0;
        for (row = 0; first < count && row < rows; row++, first++) {
            UiIcon_CreateWithLoadedResource(window, x * 8, (y + line * 2) * 8 + 4,
                actions[first] & 0x3fff);
            if (actions[first] & 0x8000)
                UiWork_SetParamNibbleFar(4);
            else if (actions[first] & 0x4000)
                UiWork_SetParamNibbleFar(2);
            else
                UiWork_SetParamNibbleFar(15);
            UiText_DrawCharacterAtOffsetFar((s32)MsgAbilityName + (actions[first] & 0x3fff), window,
                x * 8 + 16, (y + line * 2) * 8 + 8);
            UiText_DrawNumberAtOffsetFar(BtlActGet(actions[first])->pp_cost, 2, window,
                x * 8 + 88, (y + line * 2) * 8 + 8);
            line++;
        }
        UiWork_SetParamNibbleFar(15);
        UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyPp, window, x * 8 + PREVIEW_PP_X, y * 8);
        if (mode != 3) {
            s32 lines;

            lines = 0;
            if (gained) {
                UiWork_SetParamNibbleFar(4);
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyGained, window, x * 8, y * 8 + 88);
                lines = 1;
            }
            if (lost) {
                UiWork_SetParamNibbleFar(2);
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyLost, window, x * 8, (y + lines) * 8 + 88);
                lines++;
            }
            if (lines == 0)
                UiText_DrawCharacterAtOffsetFar((s32)MsgPsynergyUnchanged, window, x * 8, y * 8 + 88);
            UiWork_SetParamNibbleFar(15);
            UiWindow_DrawDividerLineFar(window, 0, 11, 13, 11);
        }
        gWindowWork->dirty = 1;
    }

    if (page == 0)
        SideObject_CreateFar(owner, 0, side, window, 0, 0);
    Unit_Copy(state, saved);
    MemFree(saved);
    return 1;
}
