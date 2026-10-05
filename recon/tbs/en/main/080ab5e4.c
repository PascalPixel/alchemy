#include "TYPES.H"
#include "IO_REG.H"
#include "TBS_EDITION.H"
#include "DJINN_MENU.H"
#include "KEYSTATE.H"
#include "WINDOW.H"
#include "GAME_STATE.H"
#include "BATTLE_RUNTIME.H"
#include "BATTLE_UNIT.H"
#include "text/MSG_IDS.H"

TEXT_MESSAGE_ENUM(MsgDjinnSetAll);
TEXT_MESSAGE_ENUM(MsgDjinnTutorialGive);
TEXT_MESSAGE_ENUM(MsgClassName);

#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)

enum DjinnMenuMessage {
    DJINN_MSG_SET_ALL = 0x00,
    DJINN_MSG_ALL_STANDBY = 0x01,
    DJINN_MSG_CHOOSE = 0x02,
    DJINN_MSG_STANDBY_HELP = 0x04,
    DJINN_MSG_SET_HELP = 0x05,
    DJINN_MSG_RECOVERING = 0x06,
    DJINN_MSG_CHARACTER_STATUS = 0x11,
    DJINN_MSG_UNLEASH_EFFECT = 0x15,
    DJINN_MSG_GIVE = 0x16,
    DJINN_MSG_SET = 0x17,
    DJINN_MSG_STANDBY = 0x18,
    DJINN_MSG_TRADE = 0x19,
    DJINN_MSG_OWNER = 0x1a,
    DJINN_MSG_ABILITY = 0x1b,
    DJINN_MSG_WHAT_WILL_YOU_DO = 0x1c,
    DJINN_MSG_NOW_RECOVERING = 0x26
};

enum DjinnTutorialMessage {
    TUTORIAL_MSG_GIVE = 0x0,
    TUTORIAL_MSG_SET_AFTER_GIVE = 0x1,
    TUTORIAL_MSG_POWERS = 0x4,
    TUTORIAL_MSG_UNLEASH = 0x5,
    TUTORIAL_MSG_DEMONSTRATE = 0x6,
    TUTORIAL_MSG_RECOVERY = 0xc,
    TUTORIAL_MSG_SET_AGAIN = 0xd
};

#define DJINN_MESSAGE(n) (MsgDjinnSetAll + (n))
#define TUTORIAL_MESSAGE(n) (MsgDjinnTutorialGive + (n))

extern struct DjinnMenuWork *gMenuWork;
extern const char DjinnMenu_TextLevel[];

void Owner_RecalculateStatsFar(s32 owner);
s32 Trade_CanOfferDjinnFar(s32 owner, s32 element, s32 number);
u32 Djinn_IsActiveFar(s32 owner, s32 element, s32 number);
void Djinn_ActivateFar(s32 owner, s32 element, s32 number);
void Djinn_DeactivateFar(s32 owner, s32 element, s32 number);
void Trade_RemoveOfferFar(s32 owner, s32 element, s32 number);
void Trade_AddOfferFar(s32 owner, s32 element, s32 number);
void RenderOutput_RedrawSavedRectFar(s32 window);
void RenderOutput_ClearListFar(s32 window);
void UiWindow_ClearInteriorTilesFar(s32 window, s32 x, s32 y, s32 width, s32 height);
void UiText_DrawCharacterAtOffsetFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawResourceFar(s32 message, s32 window, s32 x, s32 y);
void UiText_DrawStringAtOffsetFar(const char *text, s32 window, s32 x, s32 y);
void UiNumber_DrawAt(s32 value, s32 digits, s32 window, s32 x, s32 y);
void UiText_DrawQuantity(s32 value, s32 kind);
void UiWindow_SetTilemapEntryFar(s32 window, s32 tile, s32 x, s32 y, s32 flags);
struct UiWindow;
struct UiWindow *UiText_OpenMessageWindowFar(s32 message, s32 x, s32 y, u32 flags);
s32 UiWork_IsCompleteFar(void);
void UiWork_FinalizeFar(struct UiWindow *work, s32 mode);
void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);
void UiMenu_PositionCursor(s32 x, s32 y);
s32 UiMenu_SlideCursor(s32 x, s32 y);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
s32 Menu_DrawAtWindowOffset(s32 window, s32 x, s32 y, s32 width, s32 kind, s32 tile);
void UiWindow_ApplyRectAtObjectOrigin(s32 window, s32 x, s32 y, s32 width, s32 height, s32 kind);
void DjinnMenu_DrawElementList(struct DjinnListTable *lists);
s32 Djinn_MarkBalancedEntries(s8 *balanced, s32 self);
s32 FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 hidden);
void FourObjectMotion_SetSlotPhase(s32 slot, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 slot, s32 element, s32 kind);
void BattlePlacement_UpdateTimedEntriesFar(void);

u32 __umodsi3(u32, u32);
u32 __udivsi3(u32, u32);

s32 DjinnMenu_SelectDjinn(s32 mode)
{
    s8 balanced[16];
    s8 buf[8];
    struct DjinnMenuWork *state = gMenuWork;
    struct DjinnMenuBackdrop *lists = state->backdrop;
    struct BattleUnit *owner;
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
    s32 ok;
    s32 result;
    s32 message;
    u32 pressed;
    s32 count;
    u32 repeat;
    struct UiWindow *work;
    s32 step;
    s8 *status = buf;

    sel = -1;
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
        Djinn_MarkBalancedEntries(balanced, state->selected_column[0]);
        for (i = 0; i < state->owner_count; i++) {
            if (i == state->selected_column[0]) {
                status[i] = 7;
            } else if (balanced[i] != 0) {
                status[i] = 0;
            } else {
                status[i] = 3;
                if (lists->list.counts[i] == 0)
                    status[i] |= 4;
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
        Menu_DrawAtWindowOffset(state->list_window, fromX * 7 + 1, fromY + 2, 6, 1, 14);
        UiWindow_ApplyRectAtObjectOrigin(state->list_window, fromX * 7 + 1, 2, 6, 7, 6);
        for (n = 0; n < state->owner_count; n++) {
            if (n == state->selected_column[0]) {
                if (state->pair_entries[0] & 0x8000)
                    message = DJINN_MESSAGE(DJINN_MSG_STANDBY);
                else
                    message = DJINN_MESSAGE(DJINN_MSG_SET);
            } else if (buf[n] & 2) {
                message = DJINN_MESSAGE(DJINN_MSG_GIVE);
            } else {
                message = DJINN_MESSAGE(DJINN_MSG_TRADE);
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
            owner = Owner_GetStateFar(state->owners[x]);
            RenderOutput_RedrawSavedRectFar(window);
            UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(owner->class_index + MsgClassName, window, 0, 8);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, 48, 0);
            UiNumber_DrawAt(owner->level, 2, window, 72, 0);
            if (mode == 0)
                UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_CHARACTER_STATUS), window, 0, 16);
            if (sel != -1)
                djinn = lists->list.ids[x][sel];
            RenderOutput_RedrawSavedRectFar(state->help_window);
            if (mode == 1) {
                UiText_DrawQuantity(state->shown_owner[0], 1);
                UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_OWNER), state->help_window, 0, 0);
                UiText_DrawQuantity(DJINN_ELEMENT(state->pair_entries[0]) * 20 + DJINN_NUMBER(state->pair_entries[0]) + 300, 4);
                UiWindow_SetTilemapEntryFar(state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 6, 0, 0);
                UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_ABILITY), state->help_window, 56, 0);
                UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_WHAT_WILL_YOU_DO), state->help_window, 0, 8);
            }
            if (sel == -1) {
                FourObjectMotion_SetSlotPosition(mode, 0, 200, 0);
            } else if (mode == 0) {
                if (groupMode) {
                    if (setAll != 0) {
                        UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_ALL_STANDBY), state->help_window, 0, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_SET_ALL), state->help_window, 0, 0);
                    }
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        else
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_RECOVERING), state->help_window, 0, 16);
                        FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 1);
                    }
                    if (refresh & 2)
                        FourObjectMotion_SetSlotPhase(mode, 0);
                } else {
                    UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_CHOOSE), state->help_window, 0, 0);
                    if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn))) {
                            UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_SET_HELP), state->help_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 1);
                        } else {
                            UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_STANDBY_HELP), state->help_window, 0, 16);
                            FourObjectMotion_ReplaceSlot(mode, DJINN_ELEMENT(djinn), 2);
                        }
                        FourObjectMotion_SetSlotPosition(mode, x * 56 + 48, 62, 0);
                    } else {
                        UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_RECOVERING), state->help_window, 0, 16);
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
                UiText_DrawCharacterAtOffsetFar(DJINN_MESSAGE(DJINN_MSG_UNLEASH_EFFECT), state->list_window, 0, 80);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 96, 224, 104);
                UiText_DrawCharacterAtOffsetFar(DJINN_ELEMENT(djinn) * 20 + DJINN_NUMBER(djinn) + 0x666, state->list_window, 0, 96);
            }
            if (!(buf[x] & 1))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, 6, 1, 14);
            ((struct UiRenderWork *)gWindowWork[0])->dirty = 1;
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
                repeat = 2;
                pressed = 2;
                break;
            case 27:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_RECOVERY), 9, 9, 1);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_SET_AGAIN), 9, 9, 1);
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
                    pressed = 2;
                    Trade_AddOfferFar(0, 0, 0);
                    Owner_RecalculateStatsFar(0);
                    repeat = 2;
                }
                break;
            case 1:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_GIVE), 9, 9, 1);
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
                    pressed = 16;
                    lists->tutorial_timer = 0;
                    repeat = 16;
                    lists->tutorial_step = 4;
                }
                break;
            case 6:
            case 7:
                if (lists->tutorial_timer == 60) {
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_SET_AFTER_GIVE), 9, 9, 1);
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
                    lists->tutorial_step++;
                    repeat = 2;
                    pressed = 2;
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
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_POWERS), 9, 9, 1);
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
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_UNLEASH), 9, 9, 1);
                    while (UiWork_IsCompleteFar() == 0)
                        WaitFrames(1);
                    while (!(gKeyState & KEY_A)) {
                        UiMenu_PositionCursor(2, 146);
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    DjinnMenu_DrawElementList(&lists->list);
                    WaitFrames(1);
                    work = UiText_OpenMessageWindowFar(TUTORIAL_MESSAGE(TUTORIAL_MSG_DEMONSTRATE), 9, 9, 1);
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
            ok = 0;
            if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                ok = 1;
            groupMode = 1;
            gKeysPressedLatch = 0;
            if (ok == 0) {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar(state->list_window);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
                UiText_DrawResourceFar(DJINN_MESSAGE(DJINN_MSG_NOW_RECOVERING), state->list_window, 0, 96);
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
            Owner_RecalculateStatsFar(DJINN_OWNER(djinn));
            DjinnMenu_DrawElementList(&lists->list);
            redraw = 1;
        }
        if (mode == 0 && (pressed & 0x200)) {
            AudioCommand_PlayFar(112);
            result = 7;
            break;
        }
        if ((pressed & 1) || (mode == 1 && (pressed & 0x100))) {
            ok = 1;
            if (!(buf[x] & 1)) {
                if (sel == -1) {
                    AudioCommand_PlayFar(114);
                    continue;
                }
                ok = 0;
                if (Trade_CanOfferDjinnFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)) || Djinn_IsActiveFar(DJINN_OWNER(djinn), DJINN_ELEMENT(djinn), DJINN_NUMBER(djinn)))
                    ok = 1;
            }
            if (ok == 0) {
                AudioCommand_PlayFar(114);
                RenderOutput_ClearListFar(state->list_window);
                UiWindow_ClearInteriorTilesFar(state->list_window, 0, 80, 216, 104);
                UiText_DrawResourceFar(DJINN_MESSAGE(DJINN_MSG_NOW_RECOVERING), state->list_window, 0, 96);
                continue;
            }
            if (mode == 1) {
                if (!(buf[x] & 1))
                    result = 4;
                else if (x != state->selected_column[0])
                    result = 3;
                else if (state->pair_entries[0] & 0x8000)
                    result = 2;
                else
                    result = 1;
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
                    u16 *p = lists->list.ids[i];
                    for (j = 0; j < lists->list.counts[i]; j++) {
                        u32 entry = *p++;
                        s32 usable;
                        usable = 0;
                        if (Trade_CanOfferDjinnFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)) || Djinn_IsActiveFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry)))
                            usable = 1;
                        if (usable) {
                            if (setAll) {
                                if (!(entry & 0x8000)) {
                                    Djinn_ActivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    Trade_RemoveOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                    Owner_RecalculateStatsFar(DJINN_OWNER(entry));
                                }
                            } else if (entry & 0x8000) {
                                Djinn_DeactivateFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                Trade_AddOfferFar(DJINN_OWNER(entry), DJINN_ELEMENT(entry), DJINN_NUMBER(entry));
                                Owner_RecalculateStatsFar(DJINN_OWNER(entry));
                            }
                        }
                    }
                }
                DjinnMenu_DrawElementList(&lists->list);
                redraw = 1;
            } else {
                result = 10;
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, 6, 1, 15);
                AudioCommand_PlayFar(112);
                break;
            }
        }
        if (repeat & 0x40) {
            AudioCommand_PlayFar(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, 6, 1, 15);
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
            Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, 6, 1, 15);
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
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, 6, 1, 15);
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
        } else if (repeat & 0x10) {
            AudioCommand_PlayFar(111);
            if (!(buf[x] & 4))
                Menu_DrawAtWindowOffset(state->list_window, x * 7 + 1, y + 2, 6, 1, 15);
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
        } else {
            continue;
        }
        y = savedY;
        count = lists->list.counts[x];
        if (count == 0)
            count = 1;
        y = Menu_GetModuloOfSum(y, count);
        redraw = 2;
    }
    state->selected_column[mode] = x;
    if (sel != -1) {
        u16 entry = lists->list.ids[x][sel];
        state->pair_entries[mode] = entry;
        state->pair_index[mode] = DJINN_NUMBER(entry);
        state->pair_element[mode] = DJINN_ELEMENT(entry);
        state->pair_owner[mode] = DJINN_OWNER(entry);
    }
    state->selection[mode] = x + y * 10;
    return result;
}
