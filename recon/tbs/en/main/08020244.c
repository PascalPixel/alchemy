/* 2026-10-04 canonical save-summary recovery; uncredited draft.
 * Removed four fabricated Value_* literal-address imports: the native pool
 * holds message ids 0, 1, 3 and 2. The two native __modsi3 calls are ordinary
 * signed C remainder operations, not a separately named Modulo function.
 * The actual workspace owns three SaveSummary records. Use SAVE_STATE.H,
 * its gSaveWorkspace, WINDOW.H and gWindowWork, and the real GameState
 * palette_glow fields; remove the private record and duplicate declarations.
 * The unknown word at summary+0x38 remains unnamed and read at word width.
 * EN diagnostic: prior17572 with five unresolved symbols; literal/remainder
 * repair18402; shared types/owners19112 with all imports resolved. Remaining
 * differences: 98 register,17 stack,90 operand,92 reorder,35 insert,75 delete.
 * Routing-only remeasure against this EN listing: JA19267 (three unresolved
 * old callback/window names), DE19282, ES/FR/IT19252. Those are diagnostics,
 * not proofs against each localized native extent. Native EN extent1408B.
 * No FAKEMATCH, compiler change, filler or adoption is introduced. Broad
 * control flow, allocation and localized callback contracts still need work.
 */
#include "RUNTIME_MEM.H"
#include "TYPES.H"
#include "TBS_EDITION.H"
#include "RENDER_INPUT.H"
#include "DMA.H"
#include "SAVE_STATE.H"
#include "GAME_STATE.H"
#include "WINDOW.H"

/* Selects one of the three saved-game summaries. */

/*
 * The three owner slots live at workspace + 0x1040 with a 0x40 stride. The
 * reference never keeps a table pointer live: every indexed read is formed
 * from the workspace byte pointer and a folded slot offset, so the scalar
 * probes below are spelled that way and only the record actually handed to
 * a callee is taken as the shared SaveSummary.
 */
#define SLOT(i)         ((struct SaveSummary *)(base + 0x1040 + (i) * 0x40))
#define SLOT_PRESENT(i) (base[(i) * 0x40 + 0x105C])
#define SLOT_FLAG_A(i)  (SLOT(i)->send_flag)
#define SLOT_FLAG_B(i)  (SLOT(i)->flag_count)
#define SLOT_ICON(i)    (base[(i) * 0x40 + 0x1074])
#define SLOT_ICON2(i)   (base[(i) * 0x40 + 0x1075])
#define SLOT_COUNT(i,k) (((s8 *)base)[(i) * 0x40 + 0x1068 + (k)])


u32 Runtime_GetBuildStampTimeFar(void);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
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
struct RenderOutput *RenderResource_CreatePair(s32 mode, struct RenderInput *win, s32 x, s32 y);
void Ui_ApplyTableOffsetToPair(void *pair);
void RenderOutput_RedrawSavedRect(void *win);

s32 SaveMenu_SelectSlot(s32 idx, s32 mode)
{
    struct SaveSummary *e;
    u8 *base;
    u8 *render;
    void *work;
    void *win;
    struct RenderOutput *pair;
    void *win1;
    void *win2;
    void *win3;
    u32 limit;
    u32 zero;
    s32 cnt;
    s32 row;
    s32 redraw;
    s32 msg;
    s32 result;

    work = Runtime_AllocateHeapBlock(55, 0xA70);
    base = (u8 *)gSaveWorkspace;
    win1 = 0;
    win2 = 0;
    win3 = 0;
    redraw = 1;
    render = gWindowWork[0];
    limit = Runtime_GetBuildStampTimeFar();
    if (idx < 0) {
        idx = 0;
    }

    if (mode == 1) {
        cnt = 0;
        while (SLOT_PRESENT(idx) == 0 || SLOT_FLAG_A(idx) != 0) {
            idx++;
            if (idx == 3) {
                idx = 0;
            }
            cnt++;
            if (cnt > 2) {
                break;
            }
        }
    } else if (mode == 4) {
        cnt = 0;
        while (SLOT_PRESENT(idx) == 0 || SLOT_FLAG_B(idx) == 0) {
            idx++;
            if (idx == 3) {
                idx = 0;
            }
            cnt++;
            if (cnt > 2) {
                break;
            }
        }
    } else if (mode == 5) {
        cnt = 0;
        while (SLOT_PRESENT(idx) == 0 || SLOT_FLAG_A(idx) == 0) {
            idx++;
            if (idx == 3) {
                idx = 0;
            }
            cnt++;
            if (cnt > 2) {
                break;
            }
        }
    } else if (mode == 0) {
        goto ready;
    } else {
        cnt = 0;
        while (SLOT_PRESENT(idx) == 0) {
            idx++;
            if (idx == 3) {
                idx = 0;
            }
            cnt++;
            if (cnt > 2) {
                break;
            }
        }
    }
    if (cnt == 3) {
        result = -2;
        goto done;
    }

ready:
    zero = 0;
    Dma_Set(&zero, work, 0x8500029C, (volatile u32 *)0x040000D4);
    Scheduler_ScheduleCallbackAAfterFrames();

    win = UiWindow_Create(1, 2, 28, 7, 2);
    row = 0;
    cnt = 2;
    e = SLOT(0);
    do {
        if (e->level == 0) {
            msg = 0;
        } else if (e->frames < limit) {
            msg = 1;
        } else if (e->play_time != *(u32 *)e->unknown_38) {
            msg = 3;
        } else if (mode == 5 && e->send_flag == 0) {
            msg = 2;
        } else {
            UiText_DrawString(e->name, win, 12, row);
            UiText_DrawResource(e->area + RENDER_RESOURCE_BASE, win, 62, row);
            render[RENDER_DIRTY_OFS] = 1;
            goto next;
        }
        UiText_DrawResource(msg, win, 10, row);
    next:
        cnt--;
        row += 16;
        e++;
    } while (cnt >= 0);

    UiWindow_DrawDividerLine(win, 0, 2, 27, 2);
    UiWindow_DrawDividerLine(win, 0, 4, 27, 4);
    pair = RenderResource_CreatePair(mode, win, 72, -24);

loop:
    if (redraw != 0) {
        redraw = 0;
        if (SLOT_PRESENT(idx) != 0) {
            e = SLOT(idx);
            PaletteGlow_Update(SLOT_ICON(idx), SLOT_ICON2(idx));
            if (win1 == 0) {
                win1 = UiWindow_Create(1, 10, 14, 9, 2);
            }
            StatusMenu_DrawCharacterSummary(win1, e);
            WaitFrames(1);
            if (win2 == 0) {
                win2 = UiWindow_Create(16, 10, 13, 3, 2);
            }
            Menu_ClearFirstObjectRowAndScheduleUpdate();
            ObjectPlacement_CreateGroup(win2, 0, 0, e);
            WaitFrames(1);
            if (SLOT_COUNT(idx, 0) + SLOT_COUNT(idx, 1) + SLOT_COUNT(idx, 2)
                    + SLOT_COUNT(idx, 3) != 0) {
                if (win3 == 0) {
                    win3 = UiWindow_Create(16, 14, 13, 5, 2);
                }
                UiText_DrawFourNumbersInRow(win3, e);
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                Menu_SpawnFourObjectsAtOrigin(win3, 0, 0);
            } else {
                Menu_ClearSecondObjectRowAndScheduleUpdate();
                UiWork_Finalize(win3, 2);
                win3 = 0;
            }
        } else {
            PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
            Menu_ClearSecondObjectRowAndScheduleUpdate();
            Menu_ClearFirstObjectRowAndScheduleUpdate();
            UiWork_Finalize(win3, 2);
            UiWork_Finalize(win2, 2);
            UiWork_Finalize(win1, 2);
            win3 = 0;
            win2 = 0;
            win1 = 0;
        }
        RenderOutput_RedrawSavedRect(win);
        UiWindow_DrawDividerLine(win, 0, 2, 27, 2);
        UiWindow_DrawDividerLine(win, 0, 4, 27, 4);
        UiWindow_FillTilemapRect(win, 0, idx * 2, 26, 1);
    }
    Ui_ApplyTableOffsetToPair(pair);
    WaitFrames(1);

    if ((*(volatile u32 *)0x03001B04 & 0x40) != 0) {
        Audio_PlayCue(111);
        redraw = 1;
        do {
            idx = (idx + 2) % 3;
        } while (mode != 0
                 && (SLOT_PRESENT(idx) == 0
                     || (mode == 1 && SLOT_FLAG_A(idx) != 0)
                     || (mode == 4 && SLOT_FLAG_B(idx) == 0)
                     || (mode == 5 && SLOT_FLAG_A(idx) == 0)));
        goto loop;
    }
    if ((*(volatile u32 *)0x03001B04 & 0x80) != 0) {
        Audio_PlayCue(111);
        redraw = 1;
        do {
            idx = (idx + 4) % 3;
        } while (mode != 0
                 && (SLOT_PRESENT(idx) == 0
                     || (mode == 1 && SLOT_FLAG_A(idx) != 0)
                     || (mode == 4 && SLOT_FLAG_B(idx) == 0)
                     || (mode == 5 && SLOT_FLAG_A(idx) == 0)));
        goto loop;
    }
    if ((*(volatile u32 *)0x03001C94 & 2) != 0) {
        Audio_PlayCue(113);
        result = -1;
        goto teardown;
    }
    if ((*(volatile u32 *)0x03001C94 & 1) == 0) {
        goto loop;
    }
    Audio_PlayCue(112);
    result = idx;

teardown:
    Menu_ClearSecondObjectRowAndScheduleUpdate();
    Menu_ClearFirstObjectRowAndScheduleUpdate();
    UiWork_Finalize(win3, 2);
    UiWork_Finalize(win2, 2);
    UiWork_Finalize(win1, 2);
    UiWork_Finalize(win, 2);
    Scheduler_ScheduleCallbackA();
    Runtime_ReleaseHeapBlock(55);
    PaletteGlow_Update(gGameState.palette_glow[0], gGameState.palette_glow[1]);
    WaitFrames(1);

done:
    return result;
}
