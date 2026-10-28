#define OWN_UiIcon_BuildAbilityIconTiles
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "glyph.h"
#include "mem.h"
#include "resource.h"

struct BattleActionGlyph {
  u8 unk00[4];
  u16 icon;
};

extern u8 *gUiIcon_PicturePointerTable[];
extern u8 *gUiIcon_PsynergyIconPointers[];

struct BattleActionGlyph *BattleAction_Get(s32 action);
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                  s32 *tileOut, s32 reuse);
void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 overlay);
s32 Ui_CountSecondTableEntries(void);

void Ability_LoadGlyph(s32 action, s32 with_base, s32 *slot, s32 *tile,
                       s32 reuse) {
  UiIcon_BuildAbilityIconTiles(BattleAction_Get(action)->icon, with_base, slot,
                               tile, reuse);
}

/// Builds Psynergy icon glyph, over the icon frame when withBase is set, and
/// loads it into *cacheSlot (a free slot unless reuse is set). Returns the tile
/// in *tileOut.
// As ☀️'s function of the same name.
void UiIcon_BuildAbilityIconTiles(u32 glyph, s32 withBase, s32 *cacheSlot,
                                  s32 *tileOut, s32 reuse) {
  GlyphTransfer *work;
  s32 overlay;

  work = Runtime_AllocateHeapBlock(68, sizeof(GlyphTransfer));
  overlay = 0;

  if (glyph >= Ui_CountSecondTableEntries())
    glyph = 0;

  if (withBase != 0) {
    work->encoded = gUiIcon_PicturePointerTable[2];
    work->width = 2;
    work->height = 2;
    UiGlyph_DecodeWithHeapRoutines(work, 0);
    overlay = 1;
  }

  // Plainly, work->encoded = gUiIcon_PsynergyIconPointers[glyph] differs in 2
  // of 83 instructions in tla-en, as in UiIcon_BuildItemIconTiles: ⚓️ loads
  // the table's address between the two instructions of encoded's split
  // offset 0x604, where agscc finishes the shift first.
  {
    u32 offset = 192;
    u8 **table = gUiIcon_PsynergyIconPointers;
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
  Runtime_ReleaseHeapBlock(68);
}
