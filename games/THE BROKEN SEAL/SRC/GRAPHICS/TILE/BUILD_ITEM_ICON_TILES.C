#include "TYPES.H"
#include "GLYPH.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "ITEM.H"

extern void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 slot);

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);

s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

extern u8 *UiIcon_FramePointerTable[];
extern u8 *UiIcon_ItemIconPointers[];

void UiIcon_BuildItemIconTiles(u32 glyph, s32 with_base, s32 *src,
                   s32 *dst, s32 reuse)
{
    GlyphTransfer *work;
    s32 slot;

    slot = 0;
    work = Runtime_AllocateHeapBlock(17, sizeof(GlyphTransfer));

    if (glyph >= Ui_CountIconTableEntries())
        glyph = 0;

    if (with_base != 0) {
        work->encoded = UiIcon_FramePointerTable[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->encoded = UiIcon_ItemIconPointers[glyph];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(0x11);
}

extern GlyphTransfer *gGlyphWork;
/* The marks drawn over an item's icon: broken, equipped, artifact. */
extern u8 *UiIcon_MarkPointers[];
/* The count digits: the ones at 0 to 9, then the tens from 1. */
extern u8 *UiIcon_DigitPointers[];

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
    GlyphTransfer *work;
    s32 count;

    overlay = 0;
    count = 0;
    item = Item_Get(code & 0x1ff);
    work = gGlyphWork;
    if (work == NULL)
        return -1;
    if (layers & ICON_FRAME) {
        work->encoded = UiIcon_FramePointerTable[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        overlay = 1;
    }
    work->encoded = UiIcon_ItemIconPointers[item->icon];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, overlay);
    if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
        work->encoded = UiIcon_MarkPointers[1];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if ((layers & ICON_BROKEN) && (code & 0x200)) {
        work->encoded = UiIcon_MarkPointers[0];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
    if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) && (item->flags & 2)) {
        work->encoded = UiIcon_MarkPointers[2];
        work->width = 2;
        work->height = 2;
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

        work->encoded = UiIcon_DigitPointers[ones];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 1);
        if (count / 10 != 0) {
            work->encoded = UiIcon_DigitPointers[count / 10 + 9];
            work->width = 2;
            work->height = 2;
            UiGlyph_DecodeWithHeapRoutines(work, 1);
        }
    }
    return 256;
}
