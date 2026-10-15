/* Draft, differ 4 of 78: after the allocation the native shifts no1's index
 * before it builds the field offset 0x604 (ldr r3, =table; lsls r5, #2; movs
 * r2, #192; ldr r3, [r3, r5]; lsls r2, #3), this builds the offset first:
 * sched2 ranks the offset's movs (priority 55) above the shift (54).
 * Measured: the field through pointer locals (differ 4 to 38), a temporary
 * for the table entry with a barrier after it (4, the copy of the allocation
 * moving up) or a one-pass block (8), the store in a one-pass block (12), the
 * index as a byte offset (4); three minutes of permuter each from the plain
 * form and the barrier form found nothing better. */
#include "types.h"
#include "glyph.h"
#include "mem.h"

extern u8 *gUiIcon_BaseGlyphPointers[];
extern u8 *gUiIcon_OverlayPointerTable[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *data);

void Ui_BuildPairedPatternsToSlot(s32 no0, s32 no1, s32 *slot, s32 *ret, s32 flag)
{
    GlyphTransfer *work;

    work = Runtime_AllocateHeapBlock(68, sizeof(GlyphTransfer));
    work->encoded = gUiIcon_BaseGlyphPointers[no1];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    work->encoded = gUiIcon_OverlayPointerTable[no0];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (flag == 0)
        *slot = Resource_FindFreeEntry();
    *ret = VramBlock_LoadCached(*slot, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(68);
}
