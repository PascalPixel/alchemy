#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "ITEM.H"

/* ui/icon/build_item_icon_tiles.c */
typedef struct {
    u8 pad0[0x400];
    u8 f400;
    u8 pad401[0x600 - 0x401];
    s16 f600;
    s16 f602;
    s32 f604;
} FontTransfer;

extern void UiGlyph_DecodeWithHeapRoutines(FontTransfer *work, s32 slot);

extern FontTransfer *Runtime_AllocateHeapBlock(s32 arg0, s32 arg1);

extern s32 VramBlock_LoadCached(s32 index, s32 size, u8 *destination);

extern s32 UiIcon_FramePointerTable[];
extern s32 UiIcon_ItemIconPointers[];

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    FontTransfer *work;
    s32 slot;

    slot = 0;
    work = Runtime_AllocateHeapBlock(0x11, 0x608);

    if (glyph >= Ui_CountIconTableEntries())
        glyph = 0;

    if (with_base != 0) {
        work->f604 = UiIcon_FramePointerTable[2];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->f604 = UiIcon_ItemIconPointers[glyph];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, &work->f400);
    Runtime_ReleaseHeapBlock(0x11);
}

extern FontTransfer *gGlyphWork;
/* The marks drawn over an item's icon: broken, equipped, artifact. */
extern s32 UiIcon_MarkPointers[];
/* The count digits: the ones at 0 to 9, then the tens from 1. */
extern s32 UiIcon_DigitPointers[];

#define ICON_FRAME 1
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

/*
 * Compose an item slot's 16 by 16 icon in the glyph work: the frame, the
 * item's own picture, then the marks its slot bits ask for and the stack
 * count, each drawn only when its layer bit is set.
 */
s32 ItemIcon_Compose(u32 code, u32 layers)
{
    s32 overlay;
    struct ItemDefinition *item;
    FontTransfer *work;
    s32 count;

    overlay = 0;
    count = 0;
    item = Item_Get(code & 0x1ff);
    work = gGlyphWork;
    if (work == NULL)
        return -1;
    if (layers & ICON_FRAME) {
        work->f604 = UiIcon_FramePointerTable[2];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        overlay = 1;
    }
    work->f604 = UiIcon_ItemIconPointers[item->icon];
    work->f600 = 2;
    work->f602 = 2;
    UiGlyph_DecodeWithHeapRoutines(work, overlay);
    if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
        work->f604 = UiIcon_MarkPointers[1];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if ((layers & ICON_BROKEN) && (code & 0x200)) {
        work->f604 = UiIcon_MarkPointers[0];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) && (item->flags & 2)) {
        work->f604 = UiIcon_MarkPointers[2];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if (layers & ICON_COUNT_ABOVE_ONE) {
        count = (code & 0xf800) >> 11;
        count++;
        if (count <= 1)
            count = 0;
    }
    if (layers & ICON_COUNT) {
        count = (code & 0xf800) >> 11;
        count++;
    }
    if (count != 0 && count <= 30) {
        s32 ones = count % 10;

        work->f604 = UiIcon_DigitPointers[ones];
        work->f600 = 2;
        work->f602 = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
        if (count / 10 != 0) {
            work->f604 = UiIcon_DigitPointers[count / 10 + 9];
            work->f600 = 2;
            work->f602 = 2;
            UiGlyph_DecodeWithHeapRoutines(work, 1);
        }
    }
    return 256;
}
