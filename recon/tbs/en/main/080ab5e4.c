/* 080ab5e4 DjinnMenu_SelectDjinn: listing 2105, draft 2106, common 2072, differ 67.
   Message symbols are whole, except where the native adds to one pool base:
   the pair help text is MsgDjinnOwnerLabel + 1 and + 2, and the single-djinn
   help is MsgDjinnChoose + 2, + 3 and + 4 (1589 -> 1504). The balance flags
   are unsigned bytes: the native tests them with ldrb, indexed by i (1495).
   The set-all help tests setAll == 0 first, as the native's pool order and
   fall-through show (1493).
   Native frame 108, draft 104. Declaring the status array before the balance
   array puts them in native relative order, with the same score. Unsigned
   redraw and block-local refresh are unchanged; initializing redraw after
   the cursor gives 1595; direct status-array access gives 1657. No devices.
   Register map, native: r9 lists, r10 refresh, r11 repeat, r8 the help
   window address (CSE), r4 the message base (caller-saved at sp+8), redraw
   spilled to sp+72 (the frame gap). Draft: sl lists, r9 redraw, r8 refresh,
   help address caller-saved in r4. The balance loop's native keeps a second
   status induction pointer for the |= 4 store and indexes balanced by i.
   Remaining: control flow, register allocation and the four-byte frame gap.
   Permuter (2026-10-06, best 539): nearly all of its gain is one rewrite,
   a temporary for the selected column inside the balance loop. As a
   block-local `s8 *column = state->selected_column` it gives 596 (tagged
   FAKEMATCH); the inverted if (i != column) with it gives 1503, the
   permuter's inverted while (1) form with *(balanced + i) 577. Hoisting
   the pointer before the loop: 1503-1512. Its 24 declaration reorders and
   register keywords change nothing; sel = -1 after the cursor goes active
   gives 1422 alone but 610 with the column pointer; redraw = 0 after the
   status redraw 1443 alone. Statement order the native shows, from the
   same run: tutorial step 28 sets pressed before repeat, step 13 advances
   the step after pressed (529); the B result is stored before its sound
   (594 alone); the mode 1 result tests the column flag first (508).
   Measured and not kept: dropping the opening sel = -1 gives 479 but loses
   the native's -1 store at sp+60; the first count loop through an s8
   temporary 481 alone, 500 with that; redraw = 0 after the status redraw
   583; the permuter's help-text nesting 514; the move loops as do-while 521.
   The set-all loop indexes ids[i][j]: the native's induction is i * 10
   halfwords and a strength-reduced pointer, as GCC makes from the index,
   not an explicit u16 *p (461). Tutorial step 27 sets pressed after repeat:
   the native has no store or caller-save of pressed there, its tail is
   shared (2105 instructions, 504). The final entry is a u32 assigned
   through state->pair_entries[mode] = entry = ids[x][sel]: the native loads
   it with ldrh and never re-extends (350). A second permuter run (8 min,
   2 jobs) confirmed step 27 and found step 3 setting repeat before pressed
   (261). Not kept: its sel = -1 after the status clear and savedY = y
   before the first list draw give 238, but the native stores -1 before
   the clear and savedY after the count loop; its count assignment inside
   the test changes nothing.
   Up to 350 register allocation was a tie: lists (520 refs, 1589 insns)
   and the set-all pointer giv (43 refs, 73 insns) both scored 29452 in
   global.c's allocno_compare, so lists, the lower allocno, took sl; at 261
   lists is in r9 and the giv and refresh in sl, as the native has them.
   Remaining (261, one instruction over): reload choices in the opening
   loops (the native reloads mode into r7 and reuses it as the 0 for i),
   and scattered lo-register swaps.
   The native keeps ok in i's stack slot (sp+68) in both select handlers,
   as when reload spills two pseudos from one hard register; the draft
   gives ok r8. Reusing i as the A handler's flag gives 249, in both
   handlers 1264 (the frame slots shift); a separate flag per handler 437.
   Without the column pointer the draft fell back to 1308. The constants
   4 and 1 in the cursor-move blocks take r6 and r5 where the native has
   r5 and r6. A third permuter run from 261 (7 min, 2 jobs) found only the
   same sel = -1 and savedY moves again (238), and ids[i] + j for ids[i][j].
   The status pointer is assigned after sel = -1, not in its declaration
   (261 -> 194, 2105 instructions): the native's reloads show the order.
   Its -1 takes r1 and the status pointer r2 before the hoisted zero for
   the clear loop reuses r2, so the loop-end copy reloads status into r5;
   then mode is reloaded into r7 across the list draw and the count
   loop's i++ is reloaded once. Assigned before sel = -1: 261.
   The column pointer device is gone (194 -> 152, no devices): the native
   balance loop keeps two status induction pointers, one for the 7, 0 and
   3 stores and one for the |= 4 store, and reloads state for the column
   test. The 7, 0 and 3 stores go through buf and the flag store through
   status, reading buf: status[i] = buf[i] | 4, which CSE folds to 7 while
   the loop keeps its own pointer. Device removed with all stores through
   status: 1325; buf for 7 and 0 only, status[i] |= 4: 168; buf for 3 too
   with status[i] |= 4 or = 7: 1318-1360; buf[i] = 7 alone: 215.
   The far calls return what the headers declare (152 -> 140): Djinn_
   DeactivateFar u32, Trade_AddOfferFar u32 *, Djinn_ActivateFar and
   Trade_RemoveOfferFar s32, BattlePlacement_UpdateTimedEntriesFar s32.
   A returned r0 delays the next call's first argument, as in tutorial
   step 27, where the native sets r1 and r2 before r0. UiMenu_SlideCursor
   is void as declared (no change); UiWork_FinalizeFar as s32 gives 144,
   FourObjectMotion_ReplaceSlot as void 146: not kept.
   The A handler's flag is i (140 -> 128): the native keeps it in i's
   slot, sp+68, and with it the whole handler matches but for the owner,
   element and number registers. The R handler keeps its own ok, still in
   r8 where the native has sp+68: i there gives 1267, both 1255; j 635,
   count 617, step 177, result 329, message 194, refresh 161, window 629;
   its type (u32, u8, s8, u16) changes nothing.
   Remaining at 128 (2107 instructions, two over): the redraw block's
   unleash text loads its 0x666 into r1 before the sum where the native
   loads it into r3 after, and the cursor-box draw that follows reloads x
   (+1); the R handler's ok in r8; owner, element and number permuted
   among r5-r7 in both handlers; the constants 4 and 1 swapped between r5
   and r6 in the cursor moves; the final entry's & 31 copied entry first
   (+1, the other extra instruction).
   The final pair index is entry % 32 (128 -> 117, 2106 instructions):
   the native builds 31 in the result register and ands entry into it, as
   GCC expands an unsigned modulo; the & 31 form ties entry instead. The
   owner store still ties entry where the native ties 0xf00. Not kept:
   s32 entry 130; the three fields read back from pair_entries 179. A
   fourth permuter run from 140 (5 min, 2 jobs, seed 11) found nothing.
   The Japanese copy needs #if branches for message symbols its edition
   lacks (MsgDjinnNowRecovering is undeclared there) before it can be
   scored.
   Loop depth weights the allocation (2026-10-06): flow.c makes one
   natural loop per back edge, so a block's register refs count 1 + the
   continues reachable from it, and global.c ranks the cursor moves'
   constants 4 and 1 by those refs. The up handler tests buf[x] & 4 with
   its own continue before an unconditional draw, as the native's direct
   jump to the loop top shows (117 -> 73): the 4 then outranks the 1 in
   both cursor moves, and the A handler's owner, element and number match.
   The R handler's flag is i as well (73 -> 67): the native keeps it in
   i's slot, sp+68.
   Remaining at 67: the R handler's owner and number swapped between r6
   and r7 (global priority: owner 36 refs over 13 insns beats number 60
   over 24; a weight of 13 instead of 12, one more continue reachable from
   it but not from the up handler, would flip it); the redraw block's
   0x666 reload register (r1 for r3, reload's round-robin) and the reload
   of x after it (+1 instruction); fp copied through r2 for r1 in one
   test; the final owner store.
 * Stubborn (2026-10-06): re-scored differ 1589; not first-try. Score+parked from sun100 queue-3. */
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

#define DJINN_OWNER(v) (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v) ((v) & 0x1f)

extern struct DjinnMenuWork *gMenuWork;
extern const char DjinnMenu_TextLevel[];

void Owner_RecalculateStatsFar(s32 owner);
s32 Trade_CanOfferDjinnFar(s32 owner, s32 element, s32 number);
u32 Djinn_IsActiveFar(s32 owner, s32 element, s32 number);
s32 Djinn_ActivateFar(s32 owner, s32 element, s32 number);
u32 Djinn_DeactivateFar(s32 owner, s32 element, s32 number);
s32 Trade_RemoveOfferFar(s32 owner, s32 element, s32 number);
u32 *Trade_AddOfferFar(s32 owner, s32 element, s32 number);
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
void UiMenu_SlideCursor(s32 x, s32 y);
s32 Menu_GetModuloOfSum(s32 value, s32 count);
s32 Menu_DrawAtWindowOffset(s32 window, s32 x, s32 y, s32 width, s32 kind, s32 tile);
void UiWindow_ApplyRectAtObjectOrigin(s32 window, s32 x, s32 y, s32 width, s32 height, s32 kind);
void DjinnMenu_DrawElementList(struct DjinnListTable *lists);
s32 Djinn_MarkBalancedEntries(s8 *balanced, s32 self);
s32 FourObjectMotion_SetSlotPosition(s32 slot, s32 x, s32 y, s32 hidden);
void FourObjectMotion_SetSlotPhase(s32 slot, s32 phase);
s32 FourObjectMotion_ReplaceSlot(s32 slot, s32 element, s32 kind);
s32 BattlePlacement_UpdateTimedEntriesFar(void);

u32 __umodsi3(u32, u32);
u32 __udivsi3(u32, u32);

s32 DjinnMenu_SelectDjinn(s32 mode)
{
    s8 buf[8];
    u8 balanced[16];
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
        Menu_DrawAtWindowOffset(state->list_window, fromX * 7 + 1, fromY + 2, 6, 1, 14);
        UiWindow_ApplyRectAtObjectOrigin(state->list_window, fromX * 7 + 1, 2, 6, 7, 6);
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
            owner = Owner_GetStateFar(state->owners[x]);
            RenderOutput_RedrawSavedRectFar(window);
            UiText_DrawStringAtOffsetFar(owner->name, window, 0, 0);
            UiText_DrawCharacterAtOffsetFar(owner->class_index + (s32)MsgClassName, window, 0, 8);
            UiText_DrawStringAtOffsetFar(DjinnMenu_TextLevel, window, 48, 0);
            UiNumber_DrawAt(owner->level, 2, window, 72, 0);
            if (mode == 0)
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnCharacterStatus, window, 0, 16);
            if (sel != -1)
                djinn = lists->list.ids[x][sel];
            RenderOutput_RedrawSavedRectFar(state->help_window);
            if (mode == 1) {
                UiText_DrawQuantity(state->shown_owner[0], 1);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel, state->help_window, 0, 0);
                UiText_DrawQuantity(DJINN_ELEMENT(state->pair_entries[0]) * 20 + DJINN_NUMBER(state->pair_entries[0]) + 300, 4);
                UiWindow_SetTilemapEntryFar(state->help_window, DJINN_ELEMENT(state->pair_entries[0]) + 0x5001, 6, 0, 0);
                UiText_DrawCharacterAtOffsetFar((s32)&MsgDjinnOwnerLabel + 1, state->help_window, 56, 0);
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
                    Owner_RecalculateStatsFar(0);
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
            Owner_RecalculateStatsFar(DJINN_OWNER(djinn));
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
            if (buf[x] & 4)
                continue;
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
        u32 entry;
        state->pair_entries[mode] = entry = lists->list.ids[x][sel];
        state->pair_index[mode] = entry % 32;
        state->pair_element[mode] = DJINN_ELEMENT(entry);
        state->pair_owner[mode] = DJINN_OWNER(entry);
    }
    state->selection[mode] = x + y * 10;
    return result;
}
