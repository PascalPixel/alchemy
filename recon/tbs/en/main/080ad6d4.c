/* 080ad6d4 OwnerAction_RunCompareLoop: structural draft. differ 293 of 1276 (1281 draft instructions); was 709, and 1136 before shared work structures and real IWRAM calls. Frame, spill slots, mode in fp, render in sl and the swap preview now match. Remaining: in the help line the constant 224 wins a hi register from 0x8000 and 0 (see Open), which moves the literal pools; native loads 0xba0 from the pool where this builds it as 186<<4. Hoisting the two output pairs leaves the count unchanged. No function counts yet.
 * Stubborn (2026-10-06): re-scored differ 927; not first-try. Score+parked from sun100 queue-3.
 * Trial (2026-10-06): the page dispatch as a switch with the default (paired entry action) first, then case 1, then case 0, matches the native block order (default, page 1, page 0): 927 -> 892.
 * Trial (2026-10-06): dirty as u32 gives the native lsrs for dirty >> 1; differ stays 892.
 * Trial (2026-10-06): swap = 1 as the first statement of the dirty block, where native sets r8 before the mode test: 892 -> 814.
 * Trial (2026-10-06): attr as u32 loads pair_entries with ldrh where u16 gave ldrsh and a zero-extend: 814 -> 709 (s32 gives 721).
 * Trial (2026-10-06): input_edge and input_held as u32 leave 709 unchanged.
 * Trial (2026-10-06): mode fixed in fp (FAKEMATCH) gives the native 136-byte frame and array offsets: 709 -> 716; with anim, dirty, frame, page declared in that order (spill slots 100, 96, 92, 88 as native): 662.
 * Trial (2026-10-06): input_edge and input_held declared in the loop block take the lowest spill slots (36, 32) as native: 662 -> 608.
 * Trial (2026-10-06): first_flag declared after second_index (native slots 80, 76, 72, 68): 608 -> 590.
 * Trial (2026-10-06): a work local in each tutorial case instead of one shared: scene takes r6 as native: 590 -> 588.
 * Trial (2026-10-06): row for the help line, apart from the loop's step: row takes r5 as native: 588 -> 568.
 * Trial (2026-10-06): the two stores to menu_busy after the loop through gWindowWork[0] directly, leaving 0 in sl as native: 568 -> 549.
 * Trial (2026-10-06): y = row * 8 computed before the help-line draw (permuter hint, as a temporary): the loop allocates as native: 549 -> 458.
 * Trial (2026-10-06): j for the owner_y loop apart from the tile loop's i: the counter takes r2 as native: 458 -> 446. Cursor zeroed before anim, dirty, page and frame shares one zero everywhere (444), unlike native; page, frame, dirty, anim order gives 453.
 * Trial (2026-10-06): one id for both owners in the swap preview, where first_id and second_id were two: id keeps r6 and unit r7 as native: 446 -> 408. Reloading attr once after the first 0x8000 test instead of inside it gives 411; row and y declared last leave 408.
 * Trial (2026-10-06, session 2): the swap preview's last three Iwram_CopyWords through a local function pointer: gcse propagates the constant back and nothing changes (408); the same pointer fixed in r5 (FAKEMATCH, the 332/address tie below) matches the whole swap preview: 408 -> 293.
 * Open (2026-10-06): 224 (the element mask, 7 refs over 172 insns, priority 814) outranks 0x8000 and the shared 0 (4 refs over 100 insns, 800 each) and takes r8; it would need 176 insns or 6 refs to fall behind them; native keeps 0x8000 in r8 and 0 in sl and rematerializes 224. A permuter run over the help-line section (4 minutes) found nothing below 458 before the id and j changes. */
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "DJINN_MENU.H"
#include "BATTLE_UNIT.H"
#include "GAME_STATE.H"
#include "WINDOW.H"
#include "CALLBACK_SCHEDULER.H"
#include "IWRAM_CALL.H"
extern struct DjinnMenuWork *gMenuWork;

extern u8 Menu_BackdropFrameTile[];
extern volatile u32 gKeyState;
extern volatile u32 gKeysRepeat;

struct BattleUnit *Owner_GetStateFar(s32);
void WaitFrames(s32);
void AudioCommand_PlayFar(s32);
s32 Menu_GetModuloOfSum(s32, s32);
void Owner_RecalculateStatsFar(s32);

void RenderOutput_RedrawSavedRectFar(s32);
void RenderOutput_PrepareForRedrawFar(s32);
s32 UiWork_FinalizeFar(s32, s32);
void UiWindow_CloseIfOpen(s32 *, s32);
s32 UiWindow_UpdateOrCreate(s32 *, s32, s32, s32, s32, s32);
void UiWindow_SetTilemapEntryFar(s32, s32, s32, s32, s32);
void UiText_DrawCharacterAtOffsetFar(s32, s32, s32, s32);
void UiText_DrawQuantity(s32, s32);
s32 UiWork_SetParamNibbleFar(s32);
s32 UiText_OpenMessageWindowFar(s32, s32, s32, s32);
s32 UiWork_IsCompleteFar(void);

void Runtime_SetMainState19(void);
void Menu_RunPairedEntryAction(s32, s32);
void UiMenu_PositionCursor(s32, s32);
s32 OwnerAction_DiffSlots(struct OwnerActionSlot *, struct OwnerActionSlot *, u16 *, s32 *, s32 *);
void FourObjectMotion_SetSlotPhase(s32, s32);
s32 FourObjectMotion_SetSlotPosition(s32, s32, s32, s32);

void Runtime_BumpFree(void *);
s32 Trig_Sin(s32);
void UiWindow_ClearInteriorTilesFar(s32, s32, s32, s32, s32);
void RenderOutput_ClearListFar(s32);
void UiWindow_MarkVisibleTileAttributesFar(void);
void Djinn_AddToOwnerFar(s32, s32, s32);
void Djinn_ActivateFar(s32, s32, s32);
void Djinn_DeactivateFar(s32, s32, s32);
void Menu_UpdateEntryObjectTransforms(void);
void UiMenu_SlideCursor(s32, s32);
s32 DjinnMenu_DrawStatPreview(s32, s32, s32, s32, s32, s32, s32, s32, s32);

s32 OwnerAction_RunCompareLoop(u32 mode_arg)
{
    /* FAKEMATCH: as a pseudo mode ranks below the pointers in r5-r10 and is spilled; native keeps it in fp. */
    register u32 mode asm("r11") = mode_arg;
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
    s32 y;
    s32 result;
    u32 attr;
    s32 i;
    s32 j;

    state = gMenuWork;
    anim = 0;
    dirty = 1;
    page = 2;
    frame = 0;

    state->cursor->active = 13;

    cursor[0] = 0;
    cursor[1] = 0;

    for (j = 3; j >= 0; j--) {
        state->owner_y[j] = 0xc8;
    }
    state->owner_x[state->selected_column[1]] = 0x78;
    state->owner_y[state->selected_column[1]] = 0x20;
    state->owner_x[state->selected_column[0]] = 0x10;
    state->owner_y[state->selected_column[0]] = 0x20;

    RenderOutput_ClearListFar(state->list_window);
    WaitFrames(1);
    UiWindow_CloseIfOpen(&state->help_window, 1);
    UiWindow_UpdateOrCreate(&state->help_window, 0, 0, 30, 5, 2);

    count[0] = 1;
    count[1] = 1;

    if (mode <= 1) {
        u16 *diff_work;
        struct BattleUnit *unit_copy;
        struct BattleUnit *unit;
        s32 id;
        s32 first_element;
        s32 first_index;
        s32 second_element;
        s32 second_index;
        s32 first_flag;
        s32 second_flag;
        s32 out_a;
        s32 out_b;
        /* FAKEMATCH: the routine's address and the size 332 tie for r5 (4 refs over 90 insns each) and the size, made first as the argument, wins by pseudo number; native keeps the address in r5 and builds 332 at each call. A plain local is propagated away by gcse. */
        register s32 (*copy)(void *, const void *, s32) asm("r5");

        diff_work = Runtime_BumpAllocateAlternatePool(96);
        unit_copy = Runtime_BumpAllocateAlternatePool(sizeof(*unit_copy));
        second_element = 0;
        second_index = 0;
        id = state->shown_owner[0];
        first_element = state->pair_element[0];
        first_index = state->pair_index[0] & 31;
        first_flag = (u16)(state->pair_entries[0] & 0x8000);

        unit = Owner_GetStateFar(id);
        Iwram_CopyWords(unit_copy, unit, sizeof(*unit_copy));
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
        Owner_RecalculateStatsFar(id);
        count[0] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        copy = Iwram_CopyWords;
        copy(unit, unit_copy, sizeof(*unit_copy));

        id = state->shown_owner[1];
        unit = Owner_GetStateFar(id);
        copy(unit_copy, unit, sizeof(*unit_copy));
        if (mode == 0) {
            Djinn_DeactivateFar(id, second_element, second_index);
        }
        Djinn_AddToOwnerFar(id, first_element, first_index);
        if (first_flag != 0) {
            Djinn_ActivateFar(id, first_element, first_index);
        }
        Owner_RecalculateStatsFar(id);
        count[1] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        copy(unit, unit_copy, sizeof(*unit_copy));
        Runtime_BumpFree(unit_copy);
        Runtime_BumpFree(diff_work);
    } else if (mode - 2 <= 1) {
        u16 *diff_work;
        struct BattleUnit *unit_copy;
        struct BattleUnit *unit;
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

        unit = Owner_GetStateFar(first_id);
        Iwram_CopyWords(unit_copy, unit, sizeof(*unit_copy));
        Djinn_DeactivateFar(first_id, first_element, first_index);
        if (apply_second != 0) {
            Djinn_ActivateFar(first_id, first_element, first_index);
        }
        Owner_RecalculateStatsFar(first_id);
        count[0] = OwnerAction_DiffSlots(
            unit_copy->action_slots, unit->action_slots, diff_work, &out_b, &out_a);
        Iwram_CopyWords(unit, unit_copy, sizeof(*unit_copy));
        count[1] = count[0];
        page = 0;
        Runtime_BumpFree(unit_copy);
        Runtime_BumpFree(diff_work);
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
    RenderOutput_RedrawSavedRectFar(state->help_window);

    if (mode == 2) {
        UiText_DrawCharacterAtOffsetFar(0xba5, state->help_window, 96, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 96, 16);
    } else if (mode == 3) {
        UiText_DrawCharacterAtOffsetFar(0xba4, state->help_window, 96, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 96, 16);
    } else if (mode == 0) {
        UiText_DrawCharacterAtOffsetFar(0xba7, state->help_window, 128, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 128, 8);
    } else {
        UiText_DrawCharacterAtOffsetFar(0xba6, state->help_window, 128, 0);
        UiText_DrawCharacterAtOffsetFar(0xbc1, state->help_window, 128, 8);
    }

    row = (mode == 1);
    attr = state->pair_entries[0];
    if ((attr & 0x8000) == 0) {
        UiWork_SetParamNibbleFar(2);
        attr = state->pair_entries[0];
    }
    y = row * 8;
    UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
        state->help_window, 48, y);
    attr = state->pair_entries[0];
    UiWindow_SetTilemapEntryFar(
        state->help_window, ((attr & 0xe0) >> 5) + 0x5001, 5, row, 0);
    UiWork_SetParamNibbleFar(15);

    if (mode == 0) {
        attr = state->pair_entries[1];
        if ((attr & 0x8000) == 0) {
            UiWork_SetParamNibbleFar(2);
            attr = state->pair_entries[1];
        }
        UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
            state->help_window, 48, 16);
        attr = state->pair_entries[1];
        UiWindow_SetTilemapEntryFar(
            state->help_window, ((attr & 0xe0) >> 5) + 0x5001, 5, 2, mode);
        UiWork_SetParamNibbleFar(15);
    } else if (mode - 2 <= 1) {
        attr = state->pair_entries[0];
        if ((attr & 0x8000) != 0) {
            UiWork_SetParamNibbleFar(2);
            attr = state->pair_entries[0];
        }
        UiText_DrawCharacterAtOffsetFar(((attr & 0xe0) >> 5) * 20 + (attr & 31) + 0x45f,
            state->help_window, 48, 16);
        attr = state->pair_entries[0];
        UiWindow_SetTilemapEntryFar(
            state->help_window, ((attr & 0xe0) >> 5) + 0x5001, 5, 2, 0);
        UiWork_SetParamNibbleFar(15);
        UiWindow_SetTilemapEntryFar(state->help_window, 0xf296, 7, 1, 0);
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

            saved = Runtime_BumpAllocateAlternatePool(0x200);
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

                    Iwram_CopyWords(saved, render, 0x200);
                    work = UiText_OpenMessageWindowFar(0xc43, 8, 0, 1);
                    state->cursor->active = 1;
                    UiMenu_SlideCursor(2, 96);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    Iwram_CopyWords(render, saved, 0x200);
                    UiWindow_MarkVisibleTileAttributesFar();
                    render->dirty = 1;
                    dirty = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step = 11;
                    state->cursor->active = 13;
                }
                break;
            case 11:
                if (scene->tutorial_timer == 60) {
                    s32 work;

                    Iwram_CopyWords(saved, render, 0x200);
                    work = UiText_OpenMessageWindowFar(0xc42, 8, 0, 1);
                    state->cursor->active = 1;
                    UiMenu_SlideCursor(106, 56);
                    gGameState.message_speed = 1;
                    while (UiWork_IsCompleteFar() == 0) {
                        WaitFrames(1);
                    }
                    UiWork_FinalizeFar(work, 1);
                    Iwram_CopyWords(render, saved, 0x200);
                    UiWindow_MarkVisibleTileAttributesFar();
                    render->dirty = 1;
                    WaitFrames(1);
                    dirty = 1;
                    scene->tutorial_timer = 0;
                    scene->tutorial_step = 12;
                    state->cursor->active = 13;
                }
                break;
            default:
                break;
            }
            Runtime_BumpFree(saved);
        }

        if (dirty != 0) {
            swap = 1;
            if (mode <= 1) {
                UiWindow_ClearInteriorTilesFar(state->help_window, 128, 16, 224, 24);
                if (page == 1) {
                    UiText_DrawCharacterAtOffsetFar(0xba1, state->help_window, 128, 16);
                } else {
                    UiText_DrawQuantity(
                        state->shown_owner[page != 2], 1);
                    UiText_DrawCharacterAtOffsetFar(0xba0, state->help_window, 128, 16);
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
                for (i = 0; i < count[page]; i++) {
                    s32 tile;

                    tile = 0xf031 + i;
                    if (i > 9) {
                        tile = 0xf030;
                    }
                    if (i == cursor[page]) {
                        tile -= 0x1000;
                    }
                    UiWindow_SetTilemapEntryFar((s32)list, tile,
                        list->width - count[page] + i - 2,
                        -1, 0);
                }
                UiWindow_SetTilemapEntryFar((s32)list, 0xf128,
                    list->width - count[page] - 3, -1, 0);
                UiWindow_SetTilemapEntryFar((s32)list, 0xf129,
                    list->width - 2, -1, 0);
                render->dirty |= 2 << (list->y >> 2);
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
            if ((frame & 4) != 0) {
                Iwram_CopyWords((void *)0x060052c0, Menu_BackdropFrameTile, 32);
            } else {
                Iwram_FillWords((void *)0x060052c0, 32, 0x44444444);
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
    Scheduler_AddOrUpdateCallback((s32)Menu_UpdateEntryObjectTransforms, 0xc80);

    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 1;
    UiWindow_CloseIfOpen(&state->help_window, 1);
    WaitFrames(1);
    UiWindow_UpdateOrCreate(&state->help_window, 13, 0, 17, 5, 2);
    UiWindow_CloseIfOpen(&state->window, 1);
    UiWindow_CloseIfOpen(&state->second_window, 1);
    RenderOutput_RedrawSavedRectFar(state->list_window);
    RenderOutput_RedrawSavedRectFar(state->unknown_window_028);
    RenderOutput_RedrawSavedRectFar(state->status_window);
    ((struct UiRenderWork *)gWindowWork[0])->menu_busy = 0;
    WaitFrames(1);
    return result;
}
