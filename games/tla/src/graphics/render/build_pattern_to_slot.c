#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "glyph.h"
#include "item.h"
#include "ram_buffer.h"

s32 Icon_BuildMiscIconTiles(s32 no, s32 unused, s32 slot) {
  GlyphTransfer *work;

  work = Runtime_AllocateHeapBlock(0x44, sizeof(GlyphTransfer));
  Icon_DrawMisc(no);
  VramCache_Load(slot, 0x80, work->tiles);
  Runtime_ReleaseHeapBlock(0x44);
  return 1;
}

extern u8 *gItemIcons[];

void UiGlyph_DecodeWithHeapRoutines(GlyphTransfer *work, s32 layer);

/// Decodes item code's icon (the first icon for code 0) into the glyph work.
void Icon_DrawItem(s32 code) {
  GlyphTransfer *state = Ram_HeapSlots->glyphWork;
  struct ItemDefinition *item = Item_Get(0x1ff & code);

  if (code != 0)
    state->encoded = gItemIcons[item->icon];
  else
    state->encoded = gItemIcons[0];
  state->width = 2;
  state->height = 2;
  UiGlyph_DecodeWithHeapRoutines(state, 0);
}
