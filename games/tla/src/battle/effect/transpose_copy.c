/// @file
/// Tiles: copy a byte image rotated 90 degrees clockwise.

#include "types.h"

/// Rotate w-by-h src clockwise into h-by-w dst.
/// The top row of src becomes the rightmost column of dst.
void Graphics_TransposeCopy(const u8 *src, u8 *dst, s32 w, s32 h) {
  s32 y;
  s32 x;
  s32 col;
  s32 offset;
  u8 *d;
  const u8 *s;

  y = 0;
  if (h != 0) {
    offset = 0;
    col = h;
    do {
      x = 0;
      if (w != 0) {
        d = (u8 *)(col + (u32)dst - 1);
        s = (const u8 *)(offset + (u32)src);
        do {
          x++;
          *d = *s;
          s++;
          d += h;
        } while (x != w);
      }
      y++;
      offset += w;
      col--;
    } while (y != h);
  }
}
