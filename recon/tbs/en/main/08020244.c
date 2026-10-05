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
 * - The up/down searches are "do { step; if (mode == 0) break; } while
 *   (wanted)": the ROM's tail is "bne check; b top" (125).
 * - gKeyState and gKeysRepeat are volatile s32 (as MENU_LIST.H has them):
 *   the ROM reads gKeyState twice (117).
 * - "y = 0" after "summary = save->summary", and save declared after list
 *   (105, found by devsearch, plain).
 * - The ROM reads the second glow byte as [save + (slot*64 + 0x1074)] + 1,
 *   the shape extract_bit_field gives a byte bit-field (a probe of eleven
 *   array and pointer spellings never gave it). SaveSummary's two glow
 *   bytes are u16 bit-fields palette_glow and palette_glow_sub here, and
 *   SAVE2.C writes them by those names; all twelve editions still match
 *   (98). The render work is struct UiRenderWork, render->dirty.
 * Remaining (98): reload register choice. Reload hands out spill registers
 * round robin (reload1.c allocate_reload_reg), and from the dirty store on
 * the list loop (ROM: constant r1, render r3) and the main loop's tests
 * and constants are rotated by one against the ROM, so one reload more or
 * fewer happens at that store: a zero-instruction-count shift is needed
 * between the area constant and the dirty store. Measured with devsearch
 * (S2 devices, not adopted): a "+r"(status) before the list loop gives 50
 * (everything from the store on matches, the list loop's setup through the
 * area constant then rotates the other way) but costs an ldr/str pair;
 * three "+r" devices in the searches and the up branch give 54; seed 93's
 * best (45) pinned result to r2, where the ROM has r5. The list loop counts
 * i from 0 up (loop.c reverses it to the ROM's 2..0 count; same 98);
 * indexing save->summary[i] with y = i * 16 instead is 158. A probe with a
 * spilled render pointer gives constant r3, render r1 for both
 * render->dirty = 1 and *(u8 *)(0xea3 + (u32)render) = 1; the ROM's
 * constant-first order would need the add's operands reloaded swapped.
 * The Japanese extent is 0x5f8 bytes against 0x580 in the other five
 * editions.
 *
 * 2026-10-05 sun deep finisher 13: 98 to 0, plain. The area name's base
 * is the message symbol MsgDebugEntryName (2459 in English, 2516 in
 * Japanese, 3039 in the other four: the RENDER_RESOURCE_BASE values), the
 * way MENU11.C draws area names, not a numeric constant. A constant is
 * forced into a pseudo inside the loop and then folded back into the add
 * by loop.c's single-use replacement, leaving it to reload (one reload
 * more); a symbol stays in its own register (the ROM's r3, loaded before
 * the ldrh), so reload's round robin lines up with the ROM from there on.
 * Measured: a volatile asm keeping the constant in a pseudo gives 4 (only
 * the barrier's reordering), the symbol 0. The four placeholder messages
 * are written as u8 arrays like SAVE.C's, and the clear as
 * 0x85000000 | (0xa70 / 4) through REG_DMA3, all still 0.
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
#include "IO_REG.H"

extern volatile s32 gKeyState;
extern volatile s32 gKeysRepeat;
extern u8 MsgSaveNoData[];
extern u8 MsgSaveContinue[];
extern u8 MsgSaveNotCleared[];
extern u8 MsgSaveCorrupted[];
extern u8 MsgDebugEntryName[];

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
    struct UiWindow *list;
    struct SaveWorkspace *save;
    struct UiWindow *status;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct SaveSummary *summary;
    struct RenderOutput *cursor;
    s32 redraw;
    struct UiRenderWork *render;
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
    Dma_Set(&zero, work, 0x85000000 | (0xa70 / 4), REG_DMA3);
    Scheduler_ScheduleCallbackAAfterFrames();
    list = UiWindow_Create(1, 2, 28, 7, 2);
    summary = save->summary;
    y = 0;
    for (i = 0; i < 3; i++, y += 16, summary++) {
        if (summary->level == 0) {
            UiText_DrawResource((s32)MsgSaveNoData, list, 10, y);
        } else if (summary->frames < stamp) {
            UiText_DrawResource((s32)MsgSaveContinue, list, 10, y);
        } else if (summary->play_time != *(u32 *)summary->unknown_38) {
            UiText_DrawResource((s32)MsgSaveCorrupted, list, 10, y);
        } else if (mode == 5 && (s8)summary->send_flag == 0) {
            UiText_DrawResource((s32)MsgSaveNotCleared, list, 10, y);
        } else {
            UiText_DrawString(summary->name, list, 12, y);
            UiText_DrawResource(summary->area + (s32)MsgDebugEntryName, list, 62, y);
            render->dirty = 1;
        }
    }
    UiWindow_DrawDividerLine(list, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(list, 0, 4, 27, 4);
    cursor = RenderResource_CreatePair(mode, list, 72, -24);

    while (1) {
        if (redraw != 0) {
            redraw = 0;
            if (save->summary[slot].level != 0) {
                PaletteGlow_Update(save->summary[slot].palette_glow, save->summary[slot].palette_glow_sub);
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
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
                || (mode == 1 && (s8)save->summary[slot].send_flag != 0)
                || (mode == 4 && (s8)save->summary[slot].flag_count == 0)
                || (mode == 5 && (s8)save->summary[slot].send_flag == 0)));
        } else if (gKeysRepeat & 0x80) {
            AudioCommand_PlayFar(111);
            redraw = 1;
            do {
                slot = (slot + 4) % 3;
                if (mode == 0)
                    break;
            } while ((save->summary[slot].level == 0
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
