// Draft, differ 4 of 259 in tla-en under -mthumb-slow-loads (2026-10-26). A
// device search (each statement followed by an empty asm or a one-pass block,
// wrapped in one, or swapped with the next, kept when it helps) reaches 2 with
// an empty asm after `item = Item_Get(code & 0x1ff);`, and finds nothing
// closer.
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

extern u8 *gUiIcon_FramePointerTable[];
extern u8 *gUiIcon_ItemIconPointers[];
extern u8 *gUiIcon_MarkPointers[];
extern u8 *gUiIcon_DigitPointers[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);

#define ICON_FRAME 1
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
  work = Ram_HeapSlots->glyphWork;
  if (work == NULL)
    return -1;
  if (layers & ICON_FRAME) {
    work->encoded = gUiIcon_FramePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }
  work->encoded = gUiIcon_ItemIconPointers[item->icon];
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, overlay);
  if ((layers & ICON_EQUIPPED) && (code & 0x400)) {
    work->encoded = gUiIcon_MarkPointers[1];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
  }
  if ((layers & ICON_BROKEN) && (code & 0x200)) {
    work->encoded = gUiIcon_MarkPointers[0];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
  }
  if ((layers & ICON_ARTIFACT) && (code & 0x200) && (item->flags & 1) &&
      (item->flags & 2)) {
    work->encoded = gUiIcon_MarkPointers[2];
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

    work->encoded = gUiIcon_DigitPointers[ones];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 1);
    if (count / 10 != 0) {
      work->encoded = gUiIcon_DigitPointers[count / 10 + 9];
      work->width = 2;
      work->height = 2;
      UiGlyph_DecodeWithHeapRoutines(work, 1);
    }
  }
  return 256;
}
