/* 2026-10-05 sun w5 finisher 5: rewritten plainly, 491 (the old macro and
 * absolute-address draft scored 704). The workspace and the window work are
 * both read as heap slots (51 and HEAP_SLOT_WINDOW), which is what makes the
 * ROM form the window cell as "subs r3, #144" from the workspace slot's
 * address. The flags the ROM tests sign-extended (lsl/asr 24) are read as s8;
 * the djinn counts are summed as s8. The four placeholder messages are pool
 * words in the ROM (message ids 0, 1, 3 and 2, "(No saved data)",
 * "(Continue from a Sanctum)", "(The data is corrupted)", "(Not cleared
 * yet)"), so they are catalogue names here (MsgSaveNoData, MsgSaveContinue,
 * MsgSaveCorrupted, MsgSaveNotCleared) that the PO files do not give yet.
 * Measured then: while loops 519, retry labels 491, "goto check" 489.
 *
 * 2026-10-05 sun deep finisher 9: 491 to 151, in measured steps.
 * - The main loop names every field through save->summary[slot] (no
 *   summary pointer there): the ROM keeps slot*64 in r5 across the calls
 *   and the workspace in r7, slot in r8 (319).
 * - Each search is "for (i = 0; i < 3 && (wanted); i++)" with the slot
 *   step in the body: stmt.c rolls the test to the bottom, jump threading
 *   enters past i < 3, and the default search (no "||") is the one loop.c
 *   strength-reduces (adds #64), as in the ROM (297). A while loop ending
 *   in "if (++i > 2) break" ends with a condjump instead (397).
 * - Declarations in the order status, djinn, counts, summary, cursor,
 *   redraw, render, stamp give the ROM's frame (sp+32 down to sp+4) (239).
 * - "result = -2" goes to the common return (the ROM returns through r5),
 *   and the i == 3 test sits in each branch: jump2 cross-jumps the four
 *   copies into the ROM's one, entered straight from the searches; a single
 *   test after the chain keeps a "mode != 0" test the ROM does not have
 *   (235 with one test, 214 with "i = 0" first, 155 per branch).
 * - zero is volatile u32 (as in EFFECT32.C): the ROM stores it through r0
 *   after forming &zero (151).
 * Remaining (151): register choice r1/r2 around the list loop's setup and
 * step, the order of the pool loads at the area and flag stores, the main
 * loop's first block (ROM "movs r3, #0; mov r1, r8; ldr r2"), glow[1] read
 * as [save + (slot*64 + 0x1074)] + 1 in the ROM (here 0x105c + 25 from the
 * level constant; F1-F11 spellings of the pair in a probe all differ), the
 * up/down searches' tail polarity (ROM "bne check; b top", here "beq top"
 * as a long branch then "b check"), and gKeyState read through r2.
 * The Japanese extent is 0x5f8 bytes against 0x580 in the other five
 * editions.
 */
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "DMA.H"
#include "SAVE_STATE.H"
#include "GAME_STATE.H"
#include "WINDOW.H"
#include "HEAP_STATE.H"

extern u32 gKeyState;
extern u32 gKeysRepeat;
extern char MsgSaveNoData;
extern char MsgSaveContinue;
extern char MsgSaveNotCleared;
extern char MsgSaveCorrupted;

u32 Runtime_GetBuildStampTimeFar(void);
void WaitFrames(s32 frames);
void AudioCommand_PlayFar(s32 cue);
void PaletteGlow_Update(s32 icon, s32 icon_sub);
void UiWindow_FillTilemapRect(void *win, s32 x, s32 y, s32 width, s32 height);
void UiWindow_DrawDividerLine(void *win, s32 a, s32 b, s32 c, s32 d);
void UiText_DrawResource(s32 message, void *win, s32 x, s32 y);
void UiText_DrawString(void *text, void *win, s32 x, s32 y);
void UiText_DrawFourNumbersInRow(void *win, void *entry);
void StatusMenu_DrawCharacterSummary(void *win, void *entry);
void ObjectPlacement_CreateGroup(void *win, s32 x, s32 y, void *entry);
void Menu_ClearFirstObjectRowAndScheduleUpdate(void);
void Menu_ClearSecondObjectRowAndScheduleUpdate(void);
void Menu_SpawnFourObjectsAtOrigin(void *win, s32 x, s32 y);
void Scheduler_ScheduleCallbackAAfterFrames(void);
void Scheduler_ScheduleCallbackA(void);
struct RenderOutput *RenderResource_CreatePair(s32 mode, struct UiWindow *win, s32 x, s32 y);
void Ui_ApplyTableOffsetToPair(void *pair);
void RenderOutput_RedrawSavedRect(void *win);

s32 SaveMenu_SelectSlot(s32 slot, s32 mode)
{
    struct SaveWorkspace *save;
    struct UiWindow *list;
    struct UiWindow *status;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct SaveSummary *summary;
    struct RenderOutput *cursor;
    s32 redraw;
    u8 *render;
    u32 stamp;
    void *work;
    volatile u32 zero;
    s32 result;
    s32 i;
    s32 y;

    work = Runtime_AllocateHeapBlock(55, 0xa70);
    save = *(gWorkSlot + 51);
    status = NULL;
    djinn = NULL;
    counts = NULL;
    redraw = 1;
    render = *(gWorkSlot + HEAP_SLOT_WINDOW);
    stamp = Runtime_GetBuildStampTimeFar();
    if (slot < 0)
        slot = 0;
    if (mode == 1) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag != 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode == 4) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].flag_count == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode == 5) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    } else if (mode != 0) {
        for (i = 0; i < 3 && (save->summary[slot].level == 0); i++) {
            slot++;
            if (slot == 3)
                slot = 0;
        }
        if (i == 3) {
            result = -2;
            goto done;
        }
    }

    zero = 0;
    Dma_Set(&zero, work, 0x8500029c, (volatile u32 *)0x040000d4);
    Scheduler_ScheduleCallbackAAfterFrames();
    list = UiWindow_Create(1, 2, 28, 7, 2);
    y = 0;
    summary = save->summary;
    for (i = 2; i >= 0; i--, y += 16, summary++) {
        if (summary->level == 0) {
            UiText_DrawResource((s32)&MsgSaveNoData, list, 10, y);
        } else if (summary->frames < stamp) {
            UiText_DrawResource((s32)&MsgSaveContinue, list, 10, y);
        } else if (summary->play_time != *(u32 *)summary->unknown_38) {
            UiText_DrawResource((s32)&MsgSaveCorrupted, list, 10, y);
        } else if (mode == 5 && (s8)summary->send_flag == 0) {
            UiText_DrawResource((s32)&MsgSaveNotCleared, list, 10, y);
        } else {
            UiText_DrawString(summary->name, list, 12, y);
            UiText_DrawResource(summary->area + RENDER_RESOURCE_BASE, list, 62, y);
            render[RENDER_DIRTY_OFS] = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1) {
        if (redraw != 0) {
            redraw = 0;
            if (save->summary[slot].level != 0) {
                PaletteGlow_Update(save->summary[slot].palette_glow[0], save->summary[slot].palette_glow[1]);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, &save->summary[slot]);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, &save->summary[slot]);
                WaitFrames(1);
                if ((s8)save->summary[slot].djinn[0] + (s8)save->summary[slot].djinn[1] + (s8)save->summary[slot].djinn[2]
                        + (s8)save->summary[slot].djinn[3] != 0) {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, &save->summary[slot]);
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    Menu_SpawnFourObjectsAtOrigin(counts, 0, 0);
                } else {
                    Menu_ClearSecondObjectRowAndScheduleUpdate();
                    UiWork_Finalize(counts, 2);
                    counts = NULL;
                }
            } else {
                PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                UiWork_Finalize(counts, 2);
                UiWork_Finalize(djinn, 2);
                UiWork_Finalize(status, 2);
                counts = NULL;
                djinn = NULL;
                status = NULL;
            }
            RenderOutput_RedrawSavedRect(list);
            UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
            UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
            UiWindow_FillTilemapRect(list, 0, slot * 2, 26, 1);
        }
        Ui_ApplyTableOffsetToPair(cursor);
        WaitFrames(1);
        if (gKeysRepeat & 0x40) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 2) % 3;
            } while (mode != 0 && (save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeysRepeat & 0x80) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 4) % 3;
            } while (mode != 0 && (save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeyState & 2) {
            AudioCommand_PlayFar(113);
            result = -1;
            break;
        } else if (gKeyState & 1) {
            AudioCommand_PlayFar(112);
            result = slot;
            break;
        }
    }
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(counts, 2);
    UiWork_Finalize(djinn, 2);
    UiWork_Finalize(status, 2);
    UiWork_Finalize(list, 2);
    Scheduler_ScheduleCallbackA();
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    WaitFrames(1);
done:
    return result;
}
