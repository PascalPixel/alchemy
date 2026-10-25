#include "types.h"
#include "iwram_call.h"

extern u32 gMenu_PatternRowMasks[][2];

/// Draws two sets of six pattern tiles into OBJ VRAM at 0x06006000: each tile
/// filled with colour 4, then rows 1 to 7 XORed with a pair of row masks
/// (gMenu_PatternRowMasks), the first set's masks clamped to the row so its
/// patterns grow row by row, the second set skipping row 1. The mask table is
/// declared without const: as const its second load is lifted above the first
/// store.
void Func_080ff850(void) {
  s32 set;
  s32 n;

  for (set = 0; set < 2; set++) {
    for (n = 0; n < 6; n++) {
      u32 *tile = (u32 *)0x06006000 + set * 0x60 + n * 0x10;
      s32 x;

      Iwram_FillWords(tile, 64, 0x44444444);
      for (x = 1; x <= 7; x++) {
        s32 mi = n;

        if (set == 1 && x <= 1)
          continue;
        if (set == 0 && n > x - 2) {
          mi = x - 2;
          if (mi < 0)
            mi = 0;
        }
        tile[x] ^= gMenu_PatternRowMasks[mi][0];
        tile[x + 8] ^= gMenu_PatternRowMasks[mi][1];
      }
    }
  }
}
