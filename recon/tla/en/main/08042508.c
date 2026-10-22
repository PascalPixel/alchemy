#include "types.h"
#include "ram_buffer.h"

struct CharacterGlyphWork {
  u8 unk0000[0x137c];
  u16 loaded[2];
  u16 glyphSlot[2];
};

s32 UiText_GetSpeakerPortrait(s32 key);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);

void Ui_LoadCharacterEntryForSlot(u32 slot, s32 key, s32 unused) {
  struct CharacterGlyphWork *work =
      (struct CharacterGlyphWork *)Ram_HeapSlots->windowTiles;
  s32 id = UiText_GetSpeakerPortrait(key);
  s32 tile;
  s32 glyph;

  if (id == -1)
    return;
  if (slot > 1) {
    if (work->loaded[1] == id)
      slot = 1;
    else if (work->loaded[0] == id)
      slot = 0;
    else
      return;
  }
  glyph = work->glyphSlot[slot];
  UiGlyph_LoadEntryWithPalette(id, unused, &glyph, &tile, slot + 14, 1);
}
