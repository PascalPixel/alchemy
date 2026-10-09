#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "djinn_menu.h"
#include "battle_unit.h"
#include "btlsys.h"
#include "mem.h"
#include "iwram_call.h"
#include "render_input.h"
#include "callback_scheduler.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "window.h"
#include "game_state.h"
#include "resource.h"
#include "inventory_menu.h"

extern u8 MsgCurrentStatus[];
void UiWindow_CloseIfOpen(struct UiWindow **window, s32 release);
void UiText_DrawCharacterAtOffsetFar(s32 message,
    struct UiWindow *window, s32 x, s32 y);
void UiWindow_SetTilemapEntryFar(struct UiWindow *window,
    s32 tile, s32 x, s32 y, s32 palette);

extern u8 MsgDjinnName[];
extern u8 MsgDjinnPsyHelp[];
extern u8 MsgDjinnStatusHelp[];
extern u8 MsgDjinnCompareSet[];
extern u8 MsgDjinnCompareStandby[];
extern u8 MsgDjinnCompareGive[];
extern u8 MsgDjinnCompareTrade[];
extern u8 MsgDjinnCompareConfirm[];
extern u8 MsgDjinnTutorialClassPsynergy[];
extern u8 MsgDjinnTutorialSetChanges[];
s32 UiWork_FinalizeFar(s32, s32);
void UiWork_PushValueSlotFar(s32, s32);
s32 UiWork_SetParamNibbleFar(s32 color);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);
void Menu_RunPairedEntryAction(s32, s32);
// FourObjectMotion_SetSlotPhase は宣言なしで呼ぶ (C89 の暗黙の int): その呼び出しは r1 を r0 より先に設定する
s32 Trig_Sin(s32);
s32 Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);

s32 OwnerAction_RunCompareLoop(u32 mode)
{
    struct DjinnMenuWork *state;
    struct DjinnMenuBackdrop *scene;
    struct UiRenderWork *render;
    struct UiWindow *list;
    s32 count[2];
    s32 cursor[2];
    s32 anim;
    u32 dirty;
    s32 frame;
    s32 page;
    s32 entry;
    s32 swap;
    s32 step;
    s32 row;
    s32 result;
    /* FAKEMATCH: regmove copies a live pseudo operand of each help-line and into its result (adds r3, r1, #0); native copies the mask (mov r3, r8; movs r3, #224; movs r3, #31), which regmove leaves to reload only when the other operand is a hard register. attr in r2 as native.
     * An ordinary u32 attr, including separate help-block scopes, differs
     * by 229 aligned instructions (1276 native, 1283 built, 1165 common).
     * Removing attr and reusing the existing s32 entry for each of its
     * assignments/uses improves this to 181 (1276/1289/1192); entry is
     * assigned afresh before its later menu-action uses. The help masks
     * still remain in ordinary registers instead of native r8 and gain
     * copies of the entry operand. In the ordinary-u32 candidate, the
     * native movs r0,#128 at 0x080adab0 becomes negs r3,r2, and its mask
     * move through r8 becomes adds r3,r2,#0; ands r3,r0. The entry-reuse
     * candidate loads the packed entry in r7 instead of native r2 and
     * introduces an earlier literal-pool island. */
    register u32 attr asm("r2");
    s32 zero;
    s32 i;
    s32 j;

    state = gMenuWork;
    anim = 0;
    result = 0;
    dirty = 1;
    frame = 0;
    cursor[0] = 0;
    page = 2;
    state->cursor->active = 13;
    cursor[1] = 0;

    for (j = 3; j >= 0; j--) {
        state->owner_y[j] = 0xc8;
    }
    state->owner_x[state->selected_column[1]] = 0x78;
    state->owner_y[state->selected_column[1]] = 0x20;
    state->owner_x[state->selected_column[0]] = 0x10;
    state->owner_y[state->selected_column[0]] = 0x20;

    RenderOutput_ClearListFar((struct UiWindow *)state->list_window);
    WaitFrames(1);
    UiWindow_CloseIfOpen((struct UiWindow **)&state->help_window, 1);
    UiWindow_UpdateOrCreate(&state->help_window, 0, 0, 30, 5, 2);

    count[0] = 1;
    count[1] = 1;

    if (mode <= 1) {
        u16 *diff_work;
        struct BtlUnit *unit_copy;
        struct BtlUnit *unit;
        s32 id;
        s32 first_element;
        s32 first_index;
        s32 second_element;
        s32 second_index;
        s32 first_flag;
        s32 second_flag;
        s32 out_a;
        s32 out_b;
        s32 size;

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        size = sizeof(*unit_copy);
        unit_copy = Runtime_BumpAllocateAlternatePool(size);
        second_element = 0;
        second_index = 0;
        id = state->shown_owner[0];
        first_element = state->pair_element[0];
        first_index = state->pair_index[0] & 31;
        first_flag = (u16)(state->pair_entries[0] & 0x8000);

        unit = UnitGetFar(id);
        Iwram_CopyWords(unit_copy, unit, size);
        Djinn_DeactivateFar(id, first_element, first_index);
        if (mode == 0) {
            second_element = state->pair_element[1];
            second_index = state->pair_index[1] & 31;
            second_flag = (u16)(state->pair_entries[1] & 0x8000);
            Djinn_AddToOwnerFar(id, second_element, second_index);
            if (second_flag != 0) {
                Djinn_ActivateFar(id, second_element, second_index);
            }
        }
        UnitCalcFar(id);
        count[0] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, size);

        id = state->shown_owner[1];
        unit = UnitGetFar(id);
        Iwram_CopyWords(unit_copy, unit, size);
        if (mode == 0) {
            Djinn_DeactivateFar(id, second_element, second_index);
        }
        Djinn_AddToOwnerFar(id, first_element, first_index);
        if (first_flag != 0) {
            Djinn_ActivateFar(id, first_element, first_index);
        }
        UnitCalcFar(id);
        count[1] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, size);
        MemFree(unit_copy);
        MemFree(diff_work);
    } else if (mode - 2 <= 1) {
        u16 *diff_work;
        struct BtlUnit *unit_copy;
        struct BtlUnit *unit;
        s32 first_id;
        s32 first_element;
        s32 first_index;
        s32 apply_second;
        s32 out_a;
        s32 out_b;

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        unit_copy = Runtime_BumpAllocateAlternatePool(sizeof(*unit_copy));
        first_id = state->shown_owner[0];
        first_element = state->pair_element[0];
        first_index = state->pair_index[0] & 31;
        apply_second = (mode == 3);

        unit = UnitGetFar(first_id);
        Iwram_CopyWords(unit_copy, unit, sizeof(*unit_copy));
        Djinn_DeactivateFar(first_id, first_element, first_index);
        if (apply_second != 0) {
            Djinn_ActivateFar(first_id, first_element, first_index);
        }
        UnitCalcFar(first_id);
        count[0] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, sizeof(*unit_copy));
        count[1] = count[0];
        page = 0;
        MemFree(unit_copy);
        MemFree(diff_work);
    }

    count[0] = ((count[0] - 1) / 5) + 1;
    if (count[0] == 0) {
        count[0] = 1;
    }
    count[1] = ((count[1] - 1) / 5) + 1;
    if (count[1] == 0) {
        count[1] = 1;
    }

    UiWindow_UpdateOrCreate(&state->window, 0, 5, 15, 15, 2);
    UiWindow_UpdateOrCreate(&state->second_window, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->help_window);

    if (mode == 2) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareStandby, (struct UiWindow *)state->help_window, 96, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 96, 16);
    } else if (mode == 3) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareSet, (struct UiWindow *)state->help_window, 96, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 96, 16);
    } else if (mode == 0) {
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareTrade, (struct UiWindow *)state->help_window, 128, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 128, 8);
    } else {
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareGive, (struct UiWindow *)state->help_window, 128, 0);
        UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnCompareConfirm, (struct UiWindow *)state->help_window, 128, 8);
    }

    row = (mode == 1);
    attr = state->pair_entries[0];
    if ((attr & 0x8000) == 0) {
        UiWork_SetParamNibbleFar(2);
        attr = state->pair_entries[0];
    }
    UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + (s32)MsgDjinnName,
        (struct UiWindow *)state->help_window, 48, row * 8);
    UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window, ((state->pair_entries[0] & 0xe0) >> 5) + 0x5001, 5, row, zero = 0);
    UiWork_SetParamNibbleFar(15);

    if (mode == 0) {
        attr = state->pair_entries[1];
        if ((attr & 0x8000) == 0) {
            UiWork_SetParamNibbleFar(2);
            attr = state->pair_entries[1];
        }
        UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + (s32)MsgDjinnName,
            (struct UiWindow *)state->help_window, 48, 16);
        UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window, ((state->pair_entries[1] & 0xe0) >> 5) + 0x5001, 5, 2, mode);
        UiWork_SetParamNibbleFar(15);
    } else if (mode - 2 <= 1) {
        attr = state->pair_entries[0];
        if ((attr & 0x8000) != 0) {
            UiWork_SetParamNibbleFar(2);
            attr = state->pair_entries[0];
        }
        UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + (s32)MsgDjinnName,
            (struct UiWindow *)state->help_window, 48, 16);
        UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window, ((state->pair_entries[0] & 0xe0) >> 5) + 0x5001, 5, 2, zero);
        UiWork_SetParamNibbleFar(15);
        UiWindow_SetTilemapEntryFar((struct UiWindow *)state->help_window, 0xf296, 7, 1, zero);
    }

    for (;;) {
        s32 input_edge;
        s32 input_held;

        render = (struct UiRenderWork *)gWindowWork[0];
        scene = state->backdrop;
        input_edge = gKeyState;
        input_held = gKeysRepeat;

        if (scene->tutorial_step != 0) {
            s16 *saved;
            s32 size;

            size = 0x200;
            saved = Runtime_BumpAllocateAlternatePool(size);
            input_edge = 0;
            input_held = 0;
            scene->tutorial_timer += 1;

            switch (scene->tutorial_step) {
            case 5:
            case 12:
            case 23:
                if (scene->tutorial_timer == 100) {
                    input_held = 1;
                    input_edge = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step += 1;
                }
                break;
            case 10:
                if (scene->tutorial_timer == 60) {
                    s32 work;

                    Iwram_CopyWords(saved, render, size);
                    work = UiText_OpenMessageWindowFar((s32)MsgDjinnTutorialSetChanges, 8, 0, 1);
                    /* FAKEMATCH: needed to match: a one-pass block keeps the window copy before the cursor store */
                    do {
                        state->cursor->active = 1;
                    } while (0);
                    UiMenu_SlideCursor(2, 96);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    Iwram_CopyWords(render, saved, size);
                    UiWindow_MarkVisibleTileAttributesFar();
                    render->dirty = 1;
                    dirty = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step = 11;
                    /* FAKEMATCH: needed to match: a one-pass block keeps the step store before the cursor store */
                    do {
                        state->cursor->active = 13;
                    } while (0);
                }
                break;
            case 11:
                if (scene->tutorial_timer == 60) {
                    s32 work;

                    Iwram_CopyWords(saved, render, size);
                    work = UiText_OpenMessageWindowFar((s32)MsgDjinnTutorialClassPsynergy, 8, 0, 1);
                    /* FAKEMATCH: needed to match: a one-pass block keeps the window copy before the cursor store */
                    do {
                        state->cursor->active = 1;
                    } while (0);
                    UiMenu_SlideCursor(106, 56);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    Iwram_CopyWords(render, saved, size);
                    UiWindow_MarkVisibleTileAttributesFar();
                    render->dirty = 1;
                    WaitFrames(1);
                    dirty = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step = 12;
                    /* FAKEMATCH: needed to match: a one-pass block keeps the step store before the cursor store */
                    do {
                        state->cursor->active = 13;
                    } while (0);
                }
                break;
            default:
                break;
            }
            MemFree(saved);
        }

        if (dirty != 0) {
            swap = 1;
            if (mode <= 1) {
                UiWindow_ClearInteriorTilesFar((struct RenderInput *)state->help_window, 128, 16, 224, 24);
                if (page == 1) {
                    UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnStatusHelp, (struct UiWindow *)state->help_window, 128, 16);
                } else {
                    UiWork_PushValueSlotFar(
                        state->shown_owner[page != 2], 1);
                    UiText_DrawCharacterAtOffsetFar((s32)MsgDjinnPsyHelp, (struct UiWindow *)state->help_window, 128, 16);
                }
            }
            render->menu_busy = 1;
            RenderOutput_PrepareForRedrawFar(state->window);
            RenderOutput_PrepareForRedrawFar(state->second_window);
            if ((dirty >> 1) != 0) {
                WaitFrames(1);
            }

            if (mode == 3) {
                entry = 0;
                swap = 0;
            } else if (mode == 2) {
                entry = 1;
                swap = 0;
            } else {
                entry = 2;
            }

            switch (page) {
            default:
                Menu_RunPairedEntryAction(mode, 0);
                break;
            case 1:
                if (mode == 1) {
                    entry = 4;
                }
                DjinnMenu_DrawStatPreview(state->window, 0, 0,
                    state->shown_owner[1], page, 0, entry, 0, 0);
                DjinnMenu_DrawStatPreview(state->second_window, 0, 0,
                    state->shown_owner[1], page, 0, entry,
                    cursor[1] + 1, 0);
                break;
            case 0:
                if (mode == 1) {
                    entry = 1;
                    swap = 0;
                }
                DjinnMenu_DrawStatPreview(state->window, 0, 0,
                    state->pair_owner[0], page, swap, entry, page, 1);
                DjinnMenu_DrawStatPreview(state->second_window, 0, 0,
                    state->pair_owner[0], page, swap, entry,
                    cursor[0] + 1, 1);
                break;
            }
            render->menu_busy = 0;
        }

        if (page <= 1) {
            list = (struct UiWindow *)state->second_window;
            if (count[page] > 1) {
                s32 top;

                for (i = 0; i < count[page]; i++) {
                    s32 tile;

                    tile = 0xf031 + i;
                    if (i > 9) {
                        tile = 0xf030;
                    }
                    if (i == cursor[page]) {
                        tile -= 0x1000;
                    }
                    UiWindow_SetTilemapEntryFar(list, tile,
                        list->width - count[page] + i - 2,
                        -1, 0);
                }
                top = -1;
                UiWindow_SetTilemapEntryFar(list, 0xf128,
                    list->width - count[page] - 3, top, 0);
                UiWindow_SetTilemapEntryFar(list, 0xf129,
                    list->width - 2, top, 0);
                /* FAKEMATCH: a one-pass block. Its loop notes are a sched2 barrier carried by the block's first insn (the 0xea3 pool load): every set before it gains a dependent, so the bracket calls' arguments take native order (the dirty update's movs r3, #2 otherwise gave the second call's r3 copy one more dependent than its r0 copy), and the pool load is scheduled before ldrh as native. An asm barrier here fixed the arguments but put ldrh first (a true dependence on the asm's r7 makes it class 3). */
                do {
                    render->dirty |= 2 << (list->y >> 2);
                } while (0);
            }
        }

        frame = frame + 1;
        step = (frame % 60) - 5;
        if (step < 0) {
            step = 0;
        }
        if (step > 29) {
            step = 29;
        }
        FourObjectMotion_SetSlotPhase(0, (s32)0xffff4000);
        FourObjectMotion_SetSlotPhase(1, (s32)0xffff4000);

        if (mode <= 1) {
            s32 rise;
            s32 slide;

            rise = -Iwram_MulQ16(16, Trig_Sin((step % 30) * 0x444));
            if (rise < -6) {
                rise = -6;
            }
            if (rise > 12) {
                rise = 12;
            }
            slide = (step % 35) * 2;
            FourObjectMotion_SetSlotPosition(0, slide + 34, rise + 20, 0);
            if (mode == 0) {
                FourObjectMotion_SetSlotPosition(1, 99 - slide, 36 - rise, 0);
            }
        } else {
            FourObjectMotion_SetSlotPosition(0, 32, 30, 0);
        }

        if (dirty != 0) {
            dirty = 0;
            anim = Menu_GetModuloOfSum(anim, 2);
        }
        UiMenu_PositionCursor(anim * 56 + 80, 16);

        if ((frame & 3) == 0) {
            void *tile;
            u32 fill;

            tile = (void *)0x060052c0;
            fill = 0x44444444;
            if ((frame & 4) != 0) {
                Iwram_CopyWords(tile, Menu_BackdropFrameTile, 32);
            } else {
                Iwram_FillWords(tile, 32, fill);
            }
        }

        if ((input_edge & 1) != 0) {
            if (anim == 0) {
                result = 1;
                break;
            }
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if ((input_edge & 8) != 0) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        } else if ((input_edge & 2) != 0) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if ((input_edge & 0x100) != 0) {
            if (mode <= 1) {
                page = ((page + 1) % 3);
                dirty = 2;
                AudioCommand_PlayFar(111);
            }
        } else if ((input_held & 32) != 0) {
            if (page <= 1) {
                cursor[page] = cursor[page] - 1;
                cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                dirty = 1;
            }
        } else if ((input_held & 16) != 0) {
            if (page <= 1) {
                AudioCommand_PlayFar(111);
                Runtime_SetMainState19();
                cursor[page] = cursor[page] + 1;
                dirty = 1;
                cursor[page] = Menu_GetModuloOfSum(cursor[page], count[page]);
            }
        }
        WaitFrames(1);
    }

    FourObjectMotion_SetSlotPhase(0, 0);
    FourObjectMotion_SetSlotPhase(1, 0);
    Scheduler_AddOrUpdateCallback(Menu_UpdateEntryObjectTransforms, 0xc80);

    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    UiWindow_CloseIfOpen((struct UiWindow **)&state->help_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&state->help_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen((struct UiWindow **)&state->window, 1);
    UiWindow_CloseIfOpen((struct UiWindow **)&state->second_window, 1);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->list_window);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->unknown_window_028);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)state->status_window);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    WaitFrames(1);
    return result;
}

s32 DjinnMenu_ShowOwnerStatus(void)
{
    s32 cursor[2];
    s32 pages[2];
    s32 gained;
    s32 lost;
    s32 phase;
    s32 pending;
    s32 frame;
    struct UiWindow **left;
    struct UiWindow **right;
    struct UiRenderWork *render;
    u32 keys;
    u32 repeat;
    struct DjinnMenuWork *menu;
    struct BtlUnit *owner;
    struct UiWindow *window;
    u16 *actions;
    void *saved;
    s32 result;
    s32 tile;
    s32 i;
    s32 side;
    s32 *page;
    s32 blink;

    menu = gMenuWork;
    phase = 0;
    /* Deleting only the empty operand asm below gives 439 native / 438
     * built / 379 common instructions, differ 119, within the same
     * 0x420-byte extent. First movs r1,#1 becomes movs r0,#0; phase and
     * frame share that zero, and the native str r2,[sp,#48] becomes
     * str r0,[sp,#48], changing subsequent allocation and scheduling. */
    /* FAKEMATCH: keep this consumed zero separate from frame; initialization reshapes differ 27-274. */
    __asm__("" : "+r" (phase));
    pending = 1;
    frame = 0;
    menu->cursor->active = 13;
    result = 0;
    cursor[1] = cursor[0] = result;
    for (side = 3; side >= 0; side--)
        menu->owner_y[side] = 200;
    RenderOutput_ClearListFar((struct UiWindow *)menu->list_window);
    WaitFrames(1);
    page = pages;
    page[1] = page[0] = 1;
    actions = Runtime_BumpAllocateAlternatePool(96);
    saved = Runtime_BumpAllocateAlternatePool(332);
    owner = UnitGetFar(menu->shown_owner[0]);
    page[1] = page[0] = OwnerAction_DiffSlots(owner->action_slots, owner->action_slots,
        actions, &gained, &lost);
    MemFree(saved);
    MemFree(actions);
    page[0] = (page[0] - 1) / 6 + 1;
    if (page[0] == 0)
        page[0] = 1;
    page[1] = (page[1] - 1) / 6 + 1;
    if (page[1] == 0)
        page[1] = 1;
    left = (struct UiWindow **)&menu->window;
    UiWindow_UpdateOrCreate(left, 0, 5, 15, 15, 2);
    right = (struct UiWindow **)&menu->second_window;
    UiWindow_UpdateOrCreate(right, 15, 5, 15, 15, 2);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->help_window);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->status_window);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus, (struct UiWindow *)menu->status_window, 0, 0);
    UiText_DrawCharacterAtOffsetFar((s32)MsgCurrentStatus + 2, (struct UiWindow *)menu->status_window, 0, 16);
    side = 0;
    while (1) {
        render = (struct UiRenderWork *)gWindowWork[0];
        keys = gKeyState;
        repeat = gKeysRepeat;
        if (pending) {
            render->menu_busy = 1;
            RenderOutput_PrepareForRedrawFar((struct UiWindow *)menu->window);
            RenderOutput_PrepareForRedrawFar((struct UiWindow *)menu->second_window);
            DjinnMenu_DrawStatPreview((struct UiWindow *)menu->window, 0, 0, menu->pair_owner[0], 0, 0, 3, 0, 1);
            DjinnMenu_DrawStatPreview((struct UiWindow *)menu->second_window, 0, 0, menu->pair_owner[0], 0, 0, 3, cursor[0] + 1, 1);
            render->menu_busy = 0;
        }
        if (page[side] > 1) {
            window = (struct UiWindow *)menu->second_window;
            for (i = 0; i < page[side]; i++) {
                tile = 0xf031 + i;
                if (i > 9)
                    tile = 0xf030;
                if (i == cursor[side])
                    tile -= 0x1000;
                UiWindow_SetTilemapEntryFar(window, tile, window->width - page[side] + i - 2, -1, 0);
            }
            UiWindow_SetTilemapEntryFar(window, 0xf128, window->width - page[side] - 3, -1, 0);
            UiWindow_SetTilemapEntryFar(window, 0xf129, window->width - 2, -1, 0);
            /* FAKEMATCH: needed to match: a one-pass block keeps the page-cap arguments before the dirty update */
            do {
                render->dirty |= 2 << (window->y >> 2);
            } while (0);
        }
        frame++;
        blink = frame % 60 - 5;
        if (blink < 0)
            blink = 0;
        if (blink > 29)
            blink = 29;
        FourObjectMotion_SetSlotPosition(0, 32, 200, 0);
        if (pending) {
            pending = 0;
            phase = Menu_GetModuloOfSum(phase, 2);
        }
        if ((frame & 3) == 0) {
            if (frame & 4)
                Iwram_CopyWords((void *)0x060052c0, Menu_BackdropFrameTile, 32);
            else
                Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
        }
        if (keys & KEY_START) {
            AudioCommand_PlayFar(113);
            result = -2;
            break;
        }
        if (keys & (KEY_A | KEY_B | KEY_R | KEY_L)) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        }
        if (repeat & KEY_LEFT) {
            cursor[side]--;
            cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
        } else if (repeat & KEY_RIGHT) {
            cursor[side]++;
            AudioCommand_PlayFar(111);
            Runtime_SetMainState19();
            pending = 1;
            cursor[side] = Menu_GetModuloOfSum(cursor[side], page[side]);
        }
        WaitFrames(1);
    }
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    UiWindow_CloseIfOpen((struct UiWindow **)&menu->help_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate((struct UiWindow **)&menu->help_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(left, 1);
    UiWindow_CloseIfOpen(right, 1);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->list_window);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->unknown_window_028);
    RenderOutput_RedrawSavedRectFar((struct UiWindow *)menu->status_window);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    WaitFrames(1);
    return result;
}

s32 Djinn_MarkBalancedEntries(s8 *tbl, s32 self)
{
    s32 sp0;
    s32 cnt;
    s32 i;
    s8 *p;
    struct DjinnMenuWork *state;

    state = gMenuWork;
    cnt = 0;
    i = 0;
    if (cnt < state->owner_count) {
        p = tbl;
        do {
            *p = 0;
            if (i != self) {
                sp0 = cnt;
                if (Djinn_CheckTurnBalance(self, i) == 0) {
                    *p = 1;
                    cnt += 1;
                }
            }
            i += 1;
            p += 1;
        } while (i < state->owner_count);
    }
    return cnt;
}

s32 Djinn_CheckTurnBalance(s32 from, s32 to)
{
    struct DjinnMenuWork *work;
    s8 counts[16];
    u8 i;
    u8 j;
    s32 difference;
    s32 balanced;

    work = gMenuWork;
    Djinn_CountTurns((u8 *)counts);
    counts[from] -= 1;
    counts[to] += 1;
    balanced = 1;
    for (i = 0; i < work->owner_count; i++) {
        j = i;
        if (i < work->owner_count) {
            while (1) {
                j++;
                if (j >= work->owner_count)
                    break;
                difference = counts[i] - counts[j];
                if (difference < -1 || difference > 1) {
                    balanced = 0;
                    break;
                }
            }
        }
    }
    return balanced;
}

void Djinn_CountTurns(u8 *counts)
{
    struct DjinnMenuWork *work;
    struct BtlUnit *unit;
    s32 owner_index;
    s32 element;
    s32 bit;
    s32 count;

    work = gMenuWork;
    for (owner_index = 0; owner_index < work->owner_count; owner_index++) {
        unit = UnitGetFar(work->owners[owner_index]);
        count = 0;
        for (element = 0; element <= 3; element++) {
            for (bit = 0; bit <= 19; bit++) {
                if (unit->djinn_active[element] & (1 << bit))
                    count++;
                else if (unit->djinn_available[element] & (1 << bit))
                    count++;
            }
        }
        counts[owner_index] = count;
    }
}

void Resource_LoadPairedBlocksIfAvailable(void)
{
    s32 first_slot;
    s32 second_slot;
    struct InventoryMenuState *active_state;

    active_state = gMenuWork;
    first_slot = Resource_FindFreeEntry();
    active_state->resource_slots[0] = first_slot;
    if (first_slot != -1) {
        VramBlock_LoadCached(first_slot, 0x80, Menu_PairSpriteATiles);
    }
    second_slot = Resource_FindFreeEntry();
    active_state->resource_slots[1] = second_slot;
    if (second_slot != -1) {
        VramBlock_LoadCached(second_slot, 0x80, Menu_PairSpriteBTiles);
    }
}

void Menu_ResetTwoResourceEntries(void)
{
    struct InventoryMenuState *state;

    state = gMenuWork;
    Resource_ResetEntry((u16)state->resource_slots[0]);
    Resource_ResetEntry((u16)state->resource_slots[1]);
}

s32 UiIcon_LoadResourceIntoSlot(s32 resource_id, s32 slot)
{
    s32 resource_info;
    s32 selected_slot;
    s32 buffer;
    s32 allocation;

    allocation = (s32)Runtime_AllocateBlock(0x11, 0x608);
    selected_slot = slot;
    Ability_LoadGlyphFar(resource_id, 0, &selected_slot, &resource_info, 1);
    buffer = Resource_GetBuffer(slot, allocation + 0x400);
    Runtime_ReleaseHeapBlock(0x11);
    return buffer;
}

s32 UiIcon_CreateWithLoadedResource(struct RenderInput *window, s32 x, s32 y, s32 resource_id)
{
    s32 slot;

    slot = Resource_FindFreeEntry();
    if (slot != 0x60) {
        UiIcon_LoadResourceIntoSlot(resource_id, slot);
        RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
    }
}

s32 UiIcon_CreateStatChangeArrow(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    register s32 v asm("r5") = variant; /* FAKEMATCH: keeps the variant in r5 */
    struct RenderInput *w = window;
    s32 xx = x;
    struct InventoryMenuState *data = gMenuWork;

    if (v == 0)
        resource = (u16)data->resource_slots[0];
    else
        resource = (u16)data->resource_slots[1];
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}

s32 UiIcon_DrawVariantWithTileOffset(struct RenderInput *window, s32 x, s32 y, s32 variant)
{
    struct RenderOutput *object;
    u32 resource;
    register s32 v asm("r5") = variant; /* FAKEMATCH: keeps the variant in r5 */
    struct RenderInput *w = window;
    s32 xx = x;
    struct InventoryMenuState *data = gMenuWork;

    if (v == 0) {
        resource = (u16)data->resource_slots[0];
        y -= 3;
    } else {
        resource = (u16)data->resource_slots[1];
        y -= 4;
    }
    object = RenderOutput_CreateFar(resource, 0x40000000, w, xx, y);
    if (object == 0)
        return -1;
    object->kind = 0;
    object->anim_step = 0;
    object->active = 1;
    return 1;
}
