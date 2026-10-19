#include "types.h"

/// Fills the 32x30 tilemap at screen block 31: the upper 15 rows from tile
/// 0x210 on, the lower 15 from tile 48 on, each row repeating 16 tiles.
void BattleFx_BuildRepeatingTilemap(void) {
  u16 *dst = (u16 *)0x0600f800;
  s32 y;
  s32 x;

  for (y = 0; y != 30; y++) {
    if (y > 14) {
      for (x = 0; x != 32; x++)
        *dst++ = y * 32 + (x & 15) + 48;
    } else {
      for (x = 0; x != 32; x++)
        *dst++ = y * 32 + (x & 15) + 0x210;
    }
  }
}
