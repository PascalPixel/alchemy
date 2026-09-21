#include "TYPES.H"
#include "TBS_EDITION.H"
#include "DMA.H"

/* Runs the three-member menu used to choose a party member. */

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
    Dma_Set(&zero, work, 0x8500029C, (volatile u32 *)0x040000D4);
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
