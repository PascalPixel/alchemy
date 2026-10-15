/* Draft, differ 2 of 83: before the item icon's store the native loads the
 * table address between the first two instructions of the split offset 0x604
 * (movs r3, #192; ldr r2, =table; lsls r3, r3, #3), this finishes the shift
 * first. As in Resource_InitializeTable, the native behaves as if the split
 * constant's first instruction had a load's two-cycle latency under
 * -mtune=arm9tdmi; agscc gives it one, and sched2 ranks the shift (45) above
 * the load (44). Measured: block temporaries for the table, the entry or the
 * field (2 to 20), an operand barrier on the table (12). */
#include "types.h"
#include "glyph.h"
#include "mem.h"

extern u8 *gUiIcon_BaseGlyphPointers[];
extern u8 *gUiIcon_ItemIconPointers[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);
s32 Ui_CountIconTableEntries(void);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *data);

void UiIcon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *src, s32 *dst, s32 reuse)
{
    GlyphTransfer *work;
    s32 slot;

    slot = 0;
    work = Runtime_AllocateHeapBlock(68, sizeof(GlyphTransfer));

    if (glyph >= Ui_CountIconTableEntries())
        glyph = 0;

    if (withBase != 0) {
        work->encoded = gUiIcon_BaseGlyphPointers[2];
        work->width = 2;
        work->height = 2;
        UiGlyph_DecodeWithHeapRoutines(work, 0);
        slot = 1;
    }

    work->encoded = gUiIcon_ItemIconPointers[glyph];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, slot);

    if (reuse == 0)
        *src = Resource_FindFreeEntry();

    *dst = VramBlock_LoadCached(*src, 0x80, work->tiles);
    Runtime_ReleaseHeapBlock(68);
}
