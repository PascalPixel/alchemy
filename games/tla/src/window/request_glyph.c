#include "types.h"
#include "glyph.h"
#include "heap_release.h"
#include "graphics.h"
#include "resource.h"

extern u8 *gIcon_BasePointerTable[];

void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                               s32 *tileOut, s32 reuse);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);
s32 Icon_CountItemIcons(void);

u8 *BattleAction_Get(s32);

/// Builds the icon of a battle action's item.
// As ☀️'s Ability_RequestGlyph.
void Ability_RequestGlyph(s32 action, s32 withBase, s32 *slot, s32 *tile,
                          s32 reuse) {
  Icon_BuildItemIconTiles(*(u16 *)(BattleAction_Get(action) + 4), withBase, slot,
                            tile, reuse);
}

/// Builds item icon glyph, over the icon base when withBase is set, and loads
/// it into *cacheSlot (a free slot unless reuse is set). Returns the tile in
/// *tileOut.
// As ☀️'s function of the same name.
void Icon_BuildItemIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                               s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  overlay = 0;
  work = HeapSlot_Alloc(68, sizeof(GlyphTransfer));

  if (glyph >= Icon_CountItemIcons())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gIcon_BasePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }

  // Plainly, work->encoded = gItemIcons[glyph] differs in 2 of
  // 83 instructions in tla-en: ⚓️ loads the table's address between the two
  // instructions of encoded's split offset 0x604, where agscc finishes the
  // shift first.
  {
    u32 offset = 192;
    u8 **table = gItemIcons;
    /* FAKEMATCH(pin): the field's address held in r1, as the native keeps it;
     * as a plain local, 12 of 83 instructions differ in tla-en. */
    register u8 **field asm("r1");

    /* FAKEMATCH(barrier): an empty asm reading the table with the offset's
     * first part, which loads the table before the shift; as no barrier, 11 of
     * 83 instructions differ in tla-en. */
    asm volatile("" : "+l"(offset) : "l"(table));
    offset = (offset << 3) + 4;
    field = (u8 **)((u8 *)work + offset);
    /* FAKEMATCH(barrier): an empty asm on the field's address, which keeps the
     * + 4 out of the store; as no barrier, 9 of 83 instructions differ in
     * tla-en. */
    asm volatile("" : "+l"(field));
    // Indexed as an integer sum: table[glyph] reads [index, table] (differ 2).
    *field = *(u8 **)((u32)table + (glyph << 2));
  }
  work->width = 2;
  work->height = 2;
  UiGlyph_DecodeWithHeapRoutines(work, overlay);

  if (reuse == 0)
    *cacheSlot = Resource_FindFreeEntry();

  *tileOut = VramCache_Load(*cacheSlot, 0x80, work->tiles);
  HeapSlot_Release(68);
}
