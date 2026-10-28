#include "types.h"
#include "glyph.h"
#include "heap_release.h"

extern u8 *gIcon_BasePointerTable[];
extern u8 *gIcon_OverlayPointerTable[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);

void Icon_BuildCommandIconTiles(s32 no0, s32 no1, s32 *slot, s32 *ret,
                                  s32 flag) {
  GlyphTransfer *work;

  work = HeapSlot_Alloc(68, sizeof(GlyphTransfer));
  work->encoded = gIcon_BasePointerTable[no1];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, 0);
  work->encoded = gIcon_OverlayPointerTable[no0];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, 1);
  if (flag == 0)
    *slot = Resource_FindFreeEntry();
  *ret = VramCache_Load(*slot, 0x80, work->tiles);
  HeapSlot_Release(68);
}
