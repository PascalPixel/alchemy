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
 * Measured: the four slot searches written as while loops score 519; the
 * loop optimizer then strength-reduces the slot address (adds #64) and reads
 * the flag with ldrsb, which the ROM does not, so the ROM's searches are not
 * loops to loop.c: written with a retry label they score 491, and as a
 * "goto check" with the test at the bottom 489. Remaining: allocation (the
 * ROM keeps slot in r8, the workspace in r7, the count in r6 and work in r5;
 * here slot r6, workspace r8, count r4), the stack order of stamp, render
 * and redraw (ROM sp+4, sp+8, sp+12), and the flag address formed from
 * 0x105c + 20 in the ROM (here a fresh 0x1070 pool word). The Japanese
 * extent is 0x5f8 bytes against 0x580 in the other five editions.
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
    struct SaveSummary *summary;
    struct UiWindow *list;
    struct UiWindow *status;
    struct UiWindow *djinn;
    struct UiWindow *counts;
    struct RenderOutput *cursor;
    u8 *render;
    void *work;
    u32 stamp;
    u32 zero;
    s32 redraw;
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
        i = 0;
    retry1:
        if (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag != 0) {
            slot++;
            if (slot == 3)
                slot = 0;
            i++;
            if (i <= 2)
                goto retry1;
        }
    } else if (mode == 4) {
        i = 0;
    retry2:
        if (save->summary[slot].level == 0 || (s8)save->summary[slot].flag_count == 0) {
            slot++;
            if (slot == 3)
                slot = 0;
            i++;
            if (i <= 2)
                goto retry2;
        }
    } else if (mode == 5) {
        i = 0;
    retry3:
        if (save->summary[slot].level == 0 || (s8)save->summary[slot].send_flag == 0) {
            slot++;
            if (slot == 3)
                slot = 0;
            i++;
            if (i <= 2)
                goto retry3;
        }
    } else if (mode != 0) {
        i = 0;
    retry4:
        if (save->summary[slot].level == 0) {
            slot++;
            if (slot == 3)
                slot = 0;
            i++;
            if (i <= 2)
                goto retry4;
        }
    }
    if (mode != 0 && i == 3)
        return -2;

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
                summary = &save->summary[slot];
                PaletteGlow_Update(summary->palette_glow[0], summary->palette_glow[1]);
                if (status == NULL)
                    status = UiWindow_Create(1, 10, 14, 9, 2);
                StatusMenu_DrawCharacterSummary(status, summary);
                WaitFrames(1);
                if (djinn == NULL)
                    djinn = UiWindow_Create(16, 10, 13, 3, 2);
                Menu_ClearFirstObjectRowAndScheduleUpdate();
                ObjectPlacement_CreateGroup(djinn, 0, 0, summary);
                WaitFrames(1);
                if ((s8)summary->djinn[0] + (s8)summary->djinn[1] + (s8)summary->djinn[2]
                        + (s8)summary->djinn[3] != 0) {
                    if (counts == NULL)
                        counts = UiWindow_Create(16, 14, 13, 5, 2);
                    UiText_DrawFourNumbersInRow(counts, summary);
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
    return result;
}
