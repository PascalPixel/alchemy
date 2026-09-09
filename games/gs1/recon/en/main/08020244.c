#include "types.h"
#include "gs1_edition.h"

/*
 * Menu_RunOwnerSelectionLoop (main:08020244, 1408 bytes).
 *
 * Modal "pick one of the three owner slots" loop shared by the menus that
 * need a target owner.  The caller supplies the slot to start on and a mode
 * code that selects which slots may be chosen:
 *
 *   mode 0  every slot, no filtering and no wrap search at all
 *   mode 1  slot must be present and its field_31 flag must be clear
 *   mode 4  slot must be present and its field_32 flag must be set
 *   mode 5  slot must be present and its field_31 flag must be set
 *   other   slot must simply be present
 *
 * On entry the requested slot is clamped to zero and then advanced forward
 * (at most three steps, so it wraps once) until it satisfies the mode's
 * predicate; if no slot does, the loop returns -2 without releasing the
 * block it just took from pool 55.  That leak is in the reference and is
 * preserved.
 *
 * The body draws a three-row roster into one wide window, then, every time
 * `redraw` is set, refreshes the three detail windows for the current slot:
 * the status summary (StatusMenu_DrawCharacterSummary), the object row
 * (ObjectPlacement_CreateGroup) and, only when the slot has a non-zero
 * element count total, the four-number row.  Up/Down step the selection
 * backwards/forwards through the acceptable slots, B cancels with -1 and A
 * accepts with the slot index.
 *
 * Evidence and open points, all from games/gs1/asm/08020244.s and the
 * project's already-recovered callees:
 *   - The 0x40-byte entry stride and the field offsets below are fixed by
 *     the callees that receive the slot record: UiText_DrawFourNumbersInRow
 *     reads four signed bytes at +0x28, ObjectPlacement_CreateGroup reads
 *     kinds[4] at +0x2c and a variant at +0x33, and
 *     StatusMenu_DrawCharacterSummary reads +0x10 (name), +0x1c, +0x1d,
 *     +0x20 and +0x24.  The remaining names are roles, not history.
 *   - The four literal-pool words 0, 1, 3 and 2 that select the "slot is
 *     not selectable" message cannot be plain integers: GCC would have
 *     emitted `movs`.  They are relocated message ids, so they use the
 *     project's Value_<hex> spelling.
 *   - 0x03001F1C and 0x03001E8C are spelled as plain addresses rather than
 *     through global_cells.h because the reference derives the second from
 *     the first with `subs r3, #144`, and only an integer constant folds
 *     that way; two relocated symbols cannot.  This draft does not actually
 *     reproduce the fold either - it emits two separate pool words - so the
 *     spelling is a hypothesis about the original source, not a byte win.
 *     Measured both ways: the global_cells.h ADDR_ form gives 1392 bytes and
 *     684 differing halfwords against the 1396 / 682 kept here, so the
 *     integer form is retained only by that two-halfword margin and either
 *     spelling leaves these two cells EN-specific.
 *   - Data_02000240 is the party-state block; bytes 0x205/0x206 sit past
 *     the extent party_state.h currently guards, so the block is taken as a
 *     byte array here rather than through that header.
 *   - The DMA3 zero-fill of the freshly taken block is the known
 *     "lone stmia" residual already documented for Func_0800bc48 and its
 *     siblings: no ordinary C on this route emits the unpaired grouped
 *     store, so three field assignments are the closest honest rendering.
 *
 * Measured state of this draft: candidate 1396 bytes against a 1408-byte
 * reference, 682 differing halfwords, 650 wrong instructions.  Every
 * reference branch, loop, switch-on-mode, call, argument and store is
 * represented.  Inside the roster loop the per-entry probes agree exactly:
 * the biased slot pointer (ldrb [r5, #0]), ldrh [r5, #26], ldr [r5, #4]
 * against ldr [r5, #28], and the mode-5 `movs r3, #21 / ldrsb r3, [r5, r3]`
 * guard.  The loop as a whole does not agree: the reference carries the row
 * offset in fp and the unbiased entry pointer in a stack slot, while this
 * draft carries two entry pointers in low registers and the row offset on
 * the stack, and the shared message draw is laid out after the four tests
 * instead of before them.
 *
 * The residual is one global allocation disagreement, not missing code.
 * The reference keeps the workspace pointer in r7, a low register, so every
 * per-slot probe is a single "ldrb rD, [r7, rIndex]"; this draft's workspace
 * pointer lands in r9, so the same probes must first materialise
 * "base + slot * 0x40" in a low register, which then becomes a common
 * subexpression, turns the three-step wrap searches into pointer induction
 * variables and pushes the row counter into r4 with a spill around every
 * call. Five spellings were tried against that: a live table pointer, the
 * byte-indexed form used here, struct-pointer and pointer-plus-index forms
 * for the +0x30 flag bytes, and array versus dereference forms for the
 * signed count bytes. They move the aggregate between 682 and 689 differing
 * halfwords without changing which register holds the workspace pointer, so
 * the axis is closed here rather than searched further.
 *
 * Two smaller residuals are independent of that and are also not missing
 * code: the DMA3 zero-fill emits three plain stores where the reference
 * emits the unpaired `stmia r3!, {r0, r1, r2}` / `subs r3, #12` pair (the
 * known lone-stmia residual), and the B-cancel block that plays cue 113 and
 * sets -1 is laid out at the end of this draft where the reference places it
 * ahead of the body, just after the -2 early return.
 */

#define Menu_RunOwnerSelectionLoop Func_08020244

#define Ui_ApplyTableOffsetToPair Func_080216b4
#define StatusMenu_DrawCharacterSummary Func_08020198
#define Menu_ClearFirstObjectRowAndScheduleUpdate Func_0801ff14
#define Menu_ClearSecondObjectRowAndScheduleUpdate Func_08020088
#define Menu_SpawnFourObjectsAtOrigin Func_0801ffd8
#define UiText_DrawFourNumbersInRow Func_08020150
#define Scheduler_ScheduleCallbackAAfterFrames Func_0801fd84
#define Scheduler_ScheduleCallbackA Func_0801fd98
#define Runtime_ReleaseHeapBlock Func_08002dd8

typedef struct DmaTransfer {
    const void *source;
    void *destination;
    u32 control;
} DmaTransfer;

/* One selectable owner slot; three of them live at workspace + 0x1040. */
struct MenuOwnerEntry {
    u8 unk_00[0x10];
    u8 name[12];        /* 0x10 */
    u8 present;         /* 0x1c */
    u8 unk_1d;          /* 0x1d */
    u16 label;          /* 0x1e */
    s32 field_20;       /* 0x20 */
    s32 field_24;       /* 0x24 */
    s8 counts[4];       /* 0x28 */
    s8 kinds[4];        /* 0x2c */
    s8 flags[4];        /* 0x30: [3] is the variant ObjectPlacement_CreateGroup reads */
    u8 icon;            /* 0x34 */
    u8 icon_sub;        /* 0x35 */
    u16 cost;           /* 0x36 */
    s32 field_38;       /* 0x38 */
    u8 unk_3c[4];       /* 0x3c */
};

/*
 * The three owner slots live at workspace + 0x1040 with a 0x40 stride. The
 * reference never keeps a table pointer live: every indexed read is formed
 * from the workspace byte pointer and a folded slot offset, so the scalar
 * probes below are spelled that way and only the record actually handed to
 * a callee is taken as a whole MenuOwnerEntry.
 */
#define SLOT(i)         ((struct MenuOwnerEntry *)(base + 0x1040 + (i) * 0x40))
#define SLOT_PRESENT(i) (base[(i) * 0x40 + 0x105C])
#define SLOT_FLAG_A(i)  (SLOT(i)->flags[1])
#define SLOT_FLAG_B(i)  (SLOT(i)->flags[2])
#define SLOT_ICON(i)    (base[(i) * 0x40 + 0x1074])
#define SLOT_ICON2(i)   (base[(i) * 0x40 + 0x1075])
#define SLOT_COUNT(i,k) (((s8 *)base)[(i) * 0x40 + 0x1068 + (k)])

extern u8 Data_02000240[];
extern u8 Value_00000000;
extern u8 Value_00000001;
extern u8 Value_00000002;
extern u8 Value_00000003;

void *Runtime_AllocateHeapBlock(s32 pool, s32 size);
void Runtime_ReleaseHeapBlock(s32 pool);
u32 Func_08077300(void);
s32 Modulo(s32 value, s32 divisor);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 cue);
void Func_0801ccc0(s32 icon, s32 icon_sub);
void *UiWindow_Create(s32 x, s32 y, s32 width, s32 height, s32 kind);
void UiWindow_FillTilemapRect(void *win, s32 x, s32 y, s32 width, s32 height);
void Func_0801e41c(void *win, s32 a, s32 b, s32 c, s32 d);
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
void *RenderResource_CreatePair(s32 mode, void *win, s32 x, s32 y);
void Ui_ApplyTableOffsetToPair(void *pair);
void UiWork_Finalize(void *work, s32 kind);
void RenderOutput_RedrawSavedRect(void *win);

s32 Menu_RunOwnerSelectionLoop(s32 idx, s32 mode)
{
    struct MenuOwnerEntry *e;
    DmaTransfer *dma;
    u8 *base;
    u8 *render;
    void *work;
    void *win;
    void *pair;
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
    base = *(u8 **)0x03001F1C;
    win1 = 0;
    win2 = 0;
    win3 = 0;
    redraw = 1;
    render = *(u8 **)0x03001E8C;
    limit = Func_08077300();
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
    dma = (DmaTransfer *)0x040000D4;
    dma->source = &zero;
    dma->destination = work;
    dma->control = 0x8500029C;
    Scheduler_ScheduleCallbackAAfterFrames();

    win = UiWindow_Create(1, 2, 28, 7, 2);
    row = 0;
    cnt = 2;
    e = SLOT(0);
    do {
        if (e->present == 0) {
            msg = (s32)&Value_00000000;
        } else if (e->cost < limit) {
            msg = (s32)&Value_00000001;
        } else if (e->field_20 != e->field_38) {
            msg = (s32)&Value_00000003;
        } else if (mode == 5 && e->flags[1] == 0) {
            msg = (s32)&Value_00000002;
        } else {
            UiText_DrawString(e->name, win, 12, row);
            UiText_DrawResource(e->label + RENDER_RESOURCE_BASE, win, 62, row);
            render[RENDER_DIRTY_OFS] = 1;
            goto next;
        }
        UiText_DrawResource(msg, win, 10, row);
    next:
        cnt--;
        row += 16;
        e++;
    } while (cnt >= 0);

    Func_0801e41c(win, 0, 2, 27, 2);
    Func_0801e41c(win, 0, 4, 27, 4);
    pair = RenderResource_CreatePair(mode, win, 72, -24);

loop:
    if (redraw != 0) {
        redraw = 0;
        if (SLOT_PRESENT(idx) != 0) {
            e = SLOT(idx);
            Func_0801ccc0(SLOT_ICON(idx), SLOT_ICON2(idx));
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
            Func_0801ccc0(Data_02000240[0x205], Data_02000240[0x206]);
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
        Func_0801e41c(win, 0, 2, 27, 2);
        Func_0801e41c(win, 0, 4, 27, 4);
        UiWindow_FillTilemapRect(win, 0, idx * 2, 26, 1);
    }
    Ui_ApplyTableOffsetToPair(pair);
    WaitFrames(1);

    if ((*(volatile u32 *)0x03001B04 & 0x40) != 0) {
        Audio_PlayCue(111);
        redraw = 1;
        do {
            idx = Modulo(idx + 2, 3);
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
            idx = Modulo(idx + 4, 3);
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
    Func_0801ccc0(Data_02000240[0x205], Data_02000240[0x206]);
    WaitFrames(1);

done:
    return result;
}
