#include "WORKSPACE_OPTIONS.H"
#include "TYPES.H"
#include "CALLBACK_SCHEDULER.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "DMA.H"

void ShopCursor_AdvanceFar(void *);
void ShopCursor_MoveTowardTargetFar(void *);
s32 VramBlock_LoadCached(s32, s32, s32);
void Ui_ApplyTableScaleToObject(void *);

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

/* The saved option bytes the option menu edits. */
struct Options {
    u8 unk_000[0x205];
    u8 color;   /* 0x205 */
    u8 brightness; /* 0x206 */
    u8 unk_207[3];
    u8 speech;  /* 0x20a */
    u8 unk_20b;
    u8 message_speed; /* 0x20c */
    u8 unk_20d[0x1d];
    u8 auto_sleep; /* 0x22a */
};

extern struct Options Data_02000240;

s32 Runtime_ReleaseHeapBlock(s32);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    struct WorkspaceWork *work;

    work = gSelectionWork;
    sel = work->page;
    ShopCursor_AdvanceFar(&work->cursor);
    ShopCursor_MoveTowardTargetFar(&work->marker[0]);
    ShopCursor_MoveTowardTargetFar(&work->marker[1]);

    if (sel == 0) {
        src0 = (work->cursor_frame & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src0 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        (u8)work->marker[0].output->index,
        0x100,
        (s32)Resource_GetTableEntry(src0));

    if (sel == 1) {
        src1 = (work->cursor_frame & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src1 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        (u8)work->marker[1].output->index,
        0x100,
        (s32)Resource_GetTableEntry(src1));

    if (sel > 1) {
        /* FAKEMATCH: keep the existing scalar table address. The shared
           frame array preserves extent but changes r1/r2 in all six
           editions (2026-10-03 ordinary-owner attempt aadcc37dd). */
        s32 idx = sel * 3;
        s32 adj = 0x594 + sel;
        s32 off;

        adj = ((s8 *)work)[adj];
        idx += adj;
        off = 0x5d4 + idx * 4;
        Ui_ApplyTableScaleToObject(*(void **)((u8 *)work + off));
    }
    work->cursor_frame++;
}

/* Allocates and clears the option menu work block, copies the five option
   settings into it, each beside its number of choices, and schedules the
   menu. */
void OptionMenu_InitializeWork(void)
{
    struct WorkspaceWork *work;
    volatile u32 zero;

    work = (struct WorkspaceWork *)Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    work->option[0] = Data_02000240.color;
    work->option_count[0] = 24;
    work->option[1] = Data_02000240.brightness;
    work->option_count[1] = 15;
    work->option[2] = Data_02000240.message_speed;
    work->option_count[2] = 3;
    work->option[3] = Data_02000240.speech;
    work->option_count[3] = 2;
    work->option[4] = Data_02000240.auto_sleep;
    work->option_count[4] = 2;
    Scheduler_AddOrUpdateCallback((s32)(GraphicsPalette_LoadSelectionResourcesAndAdvance), 0xc80);
}

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    Scheduler_RemoveCallback((u32)((s32)GraphicsPalette_LoadSelectionResourcesAndAdvance));
    Runtime_ReleaseHeapBlock(0x14);
}
