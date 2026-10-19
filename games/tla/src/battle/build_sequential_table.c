/// @file
/// Tiles: build a map of consecutive byte values.

#include "types.h"
#include "iwram_call.h"
#include "scene.h"

/// Writes consecutive bytes 0-255 followed by 0-223, then fills the remaining
/// 544 bytes with 0xff (1024 bytes in all).
void Graphics_BuildSequentialTileTable(s32 *dst) {
  s32 v;
  s32 n;
  s32 (*fill)(void *, s32, u32);

  // Advance by four entries because each store writes four bytes.
  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 63);

  v = 0x03020100;
  n = 0;
  do {
    n += 1;
    *dst++ = v;
    v += 0x04040404;
  } while (n >= 0 && n <= 55);

  /* FAKEMATCH: called through a local pointer to its fixed entry; as a plain
   * call through the header, all six ⚓️ editions differ. */
  fill = Iwram_FillWordsEntry;
  fill(dst, 544, -1);
}
