// Near miss, differ 2 of 259 (tla-en, 2026-10-30), with one one-pass block
// around the work's read (tagged below; milder than the empty asm after the
// item's fetch that also gave 2). Without it, 4 differ: the item's spill
// (str r0, [sp, #0]) comes after 0x03000000's movs instead of before, the
// movs winning on its class against the call (output- rather than
// data-dependent) at equal priority. Left: the code's reload (ldr r2, [sp,
// #8]) comes after the movs that starts 0x1ff instead of right after the
// code's home store. -mthumb-slow-loads makes a load from the address just
// stored wait a cycle, so the reload is not ready when the movs is; a barrier
// before the mask lets the reload go first but carries the layers' copy (mov
// fp, r1) and the zeroes up with it (differ 4 or 8), and a copy of the code
// through a one-pass block or barrier does the same (8 to 10). The mask as a
// local or macro, as 0x1ff & code, code % 0x200 or (s32)code changes nothing;
// (u16)code gives 21; the zeroes after the fetch give 9 or more.
// Draft, differ 20 of 259 (2026-10-18): only scheduling remains; ten times
// the native loads (the code from its stack slot, the item, the work) between
// the first two instructions of a split constant, where this builds the
// constant first (the lane's load-earlier near misses).
// ☀️'s ItemIcon_Compose, the glyph work being the heap slot
// glyphWork.
#include "types.h"
#include "glyph.h"
#include "ram_buffer.h"
#include "item.h"

extern u8 *gIcon_BasePointerTable[];
extern u8 *gItemIcons[];
extern u8 *gIcon_MarkPointers[];
extern u8 *gIcon_DigitPointers[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);

#define ICON_BASE 1              ///< the base under the icon
#define ICON_COUNT_ABOVE_ONE 2
#define ICON_COUNT 4
#define ICON_EQUIPPED 8
#define ICON_BROKEN 16
#define ICON_ARTIFACT 32

s32 ItemIcon_Compose(u32 code, u32 layers) {
  s32 overlay;
  struct ItemDefinition *item;
  GlyphTransfer *work;
  s32 count;

  overlay = 0;
  count = 0;
  item = Item_Get(code & 0x1ff);
  /* FAKEMATCH(one-pass): the work's read in a once-only block, which keeps
   * the item's spill before the RAM base; as a plain statement, 4 of 259
   * instructions differ in tla-en. */
  do {
    work = Ram_HeapSlots->glyphWork;
  } while (0);
  if (work == NULL)
    return -1;
  if (layers & ICON_BASE) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }
  work->encoded = gItemIcons[item->icon];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, overlay);
  if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
    work->encoded = gIcon_MarkPointers[1];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
  }
  if ((layers & ICON_BROKEN) && (code & 0x200)) {
    work->encoded = gIcon_MarkPointers[0];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) &&
      (item->flags & 2)) {
    work->encoded = gIcon_MarkPointers[2];
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

    work->encoded = gIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (count / 10 != 0) {
      work->encoded = gIcon_DigitPointers[count / 10 + 9];
      work->width = 2;
      work->height = 2;
      UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
  }
  return 256;
}
