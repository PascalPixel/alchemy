#include "TYPES.H"
#include "RESOURCE.H"
#include "RESOURCE_IDS.H"
#include "DMA.H"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))
extern u8 *gSelectionWork;
void ShopCursor_AdvanceFar(void *);
void ShopCursor_MoveTowardTargetFar(void *);
s32 VramBlock_LoadCached(s32, s32, s32);
void Ui_ApplyTableScaleToObject(void *);

u8 *Runtime_AllocateBlock(s32 slot, u32 size);
void Scheduler_AddOrUpdateCallback(void *callback, s32 priority);
void GraphicsPalette_LoadSelectionResourcesAndAdvance(void);

/* The saved option bytes the option menu edits. */
struct Options {
    u8 unk_000[0x205];
    u8 a;       /* 0x205 */
    u8 b;       /* 0x206 */
    u8 unk_207[3];
    u8 d;       /* 0x20a */
    u8 unk_20b;
    u8 c;       /* 0x20c */
    u8 unk_20d[0x1d];
    u8 e;       /* 0x22a */
};

struct OptionWork {
    u8 unk_000[0x594];
    u8 value[5];
    u8 choices[5];
};

extern struct Options Data_02000240;

s32 Runtime_ReleaseHeapBlock(s32);
s32 Scheduler_RemoveCallback(s32);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    void *base;

    base = gSelectionWork;
    sel = FIELD(base, u16, 0x574);
    ShopCursor_AdvanceFar(base + 0x5A4);
    ShopCursor_MoveTowardTargetFar(base + 0x5B4);
    ShopCursor_MoveTowardTargetFar(base + 0x5C4);

    if (sel == 0) {
        src0 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src0 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        FIELD(FIELD(base, void *, 0x5B4), u8, 14),
        0x100,
        (s32)Resource_GetTableEntry(src0));

    if (sel == 1) {
        src1 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)&ResourceId_ShopCursorFrames;
    } else {
        src1 = (s32)&ResourceId_ShopCursorFrames;
    }
    VramBlock_LoadCached(
        FIELD(FIELD(base, void *, 0x5C4), u8, 14),
        0x100,
        (s32)Resource_GetTableEntry(src1));

    if (sel > 1) {
        s32 idx = sel * 3;
        s32 adj = 0x594 + sel;
        s32 off;

        adj = ((s8 *)base)[adj];
        idx += adj;
        off = 0x5D4 + idx * 4;
        Ui_ApplyTableScaleToObject(*(void **)((u8 *)base + off));
    }
    FIELD(base, u16, 0x57C)++;
}

/* Allocates and clears the option menu work block, copies the five option
   settings into it, each beside its number of choices, and schedules the
   menu. */
void OptionMenu_InitializeWork(void)
{
    struct OptionWork *work;
    volatile u32 zero;

    work = (struct OptionWork *)Runtime_AllocateBlock(20, 0x628);
    zero = 0;
    Dma_Set((const void *)&zero, work, 0x8500018a, (volatile u32 *)0x040000d4);
    work->value[0] = Data_02000240.a;
    work->choices[0] = 24;
    work->value[1] = Data_02000240.b;
    work->choices[1] = 15;
    work->value[2] = Data_02000240.c;
    work->choices[2] = 3;
    work->value[3] = Data_02000240.d;
    work->choices[3] = 2;
    work->value[4] = Data_02000240.e;
    work->choices[4] = 2;
    Scheduler_AddOrUpdateCallback(GraphicsPalette_LoadSelectionResourcesAndAdvance, 0xc80);
}

void Runtime_ScheduleCallbackAndReleaseBlock20B(void)
{
    Scheduler_RemoveCallback((s32)GraphicsPalette_LoadSelectionResourcesAndAdvance);
    Runtime_ReleaseHeapBlock(0x14);
}
