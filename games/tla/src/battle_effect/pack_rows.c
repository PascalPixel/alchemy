/// @file
/// Tiles: pack each horizontal pair of bytes into one byte.

#include "types.h"
#include "scene.h"

/// Copies 288 rows of 40 bytes, reducing each horizontal pair to one byte.
/// The second byte, from the odd column, survives.
/// stride: source bytes per row; destination uses half as many.
void Graphics_PackTileRows(u8 *src, u8 *dst, s32 stride) {
  s32 y = 0;
  s32 h = 288;
  s32 offset = 0;

  do {
    s32 x;

    for (x = 0; x != 40; x++) {
      dst[offset / 2 + x / 2] = src[offset + x];
    }
    offset += stride;
    y++;
  } while (y != h);
}
