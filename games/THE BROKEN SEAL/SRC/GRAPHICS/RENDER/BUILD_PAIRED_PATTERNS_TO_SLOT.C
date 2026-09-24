#include "TYPES.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad1[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} T;

extern s32 Runtime_AllocateHeapBlock(s32 no0, s32 no1);
extern s32 UiGlyph_DecodeWithHeapRoutines(T *, s32);
extern s32 Resource_FindFreeEntry(void);
extern s32 Resource_CopyData(s32, s32, u8 *);
extern s32 Runtime_ReleaseHeapBlock(s32);
extern s32 UiIcon_FramePointerTable[];
extern s32 UiIcon_OverlayPointerTable[];

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
    T *work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    work->f604 = UiIcon_FramePointerTable[no1];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    work->f604 = UiIcon_OverlayPointerTable[no0];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (flag == 0) {
        *slot = Resource_FindFreeEntry();
    }
    *ret = Resource_CopyData(*slot, 0x80, &work->f400);
    Runtime_ReleaseHeapBlock(0x11);
}
