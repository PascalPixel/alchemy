#include "types.h"

s32 UiText_GetSpeakerPortrait(u32 entry);
void UiGlyph_LoadEntryWithPalette(u32 icon, s32 unused, s32 *slot, s32 *tile,
                                  s32 palette, s32 reuse);

s32 Func_08045528(u32 entry, s32 slot) {
  s32 tile;

  if (entry > 7)
    entry = 0;
  UiGlyph_LoadEntryWithPalette(UiText_GetSpeakerPortrait(entry), 0, &slot,
                               &tile, 14, 1);
  return tile;
}
