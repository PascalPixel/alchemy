#include "types.h"
#include "scene.h"

/* ui/icon/build_item_icon_tiles.c */
typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern FontTransfer *Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern s32 Resource_FindFreeSlot(void);
extern s32 Resource_CopyData(s32 index, s32 size, u8 *destination);

extern s32 gRom[];
extern s32 gRom2[];

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    slot = 0;
    work = Runtime_AllocateHeapBlock(0x11, 0x608);

    if (glyph >= Ui_Check())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = gRom[2];
        work->f600 = 2;
        work->f602 = 2;
        Ui_Apply(work, 0);
        slot = 1;
    }

    work->f604 = gRom2[glyph];
    work->f600 = 2;
    work->f602 = 2;
    Ui_Apply(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeSlot();

    *dst = Resource_CopyData(*src, 0x80, &work->f400);
    Ui_Do(0x11);
}

/* ui/icon/copy_resource_to_slot.c */
s32 Resource_CopyData(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

s32 UiIcon_CopyResourceToSlot(s32 arg0, s32 arg1, s32 arg2)
{
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Ui_Apply(arg0, arg1);
    Resource_CopyData(arg2, 0x80, work + 0x400);
    Ui_Check(0x11);
    return 1;
}

/* ui/render/build_pattern_to_slot.c */
s32 Resource_CopyData(s32, s32, s32);
s32 Runtime_AllocateHeapBlock(s32 no, s32 arg1);

s32 Ui_BuildPatternToSlot(s32 no, s32 arg1, s32 slot)
{
    s32 work;

    work = Runtime_AllocateHeapBlock(0x11, 0x608);
    Ui_Do(no);
    Resource_CopyData(slot, 0x80, work + 0x400);
    Ui_Check(0x11);
    return 1;
}
