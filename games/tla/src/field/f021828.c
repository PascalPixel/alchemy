/// @file
/// The grid outliner, compiled as ARM code and copied to RAM to run, entered
/// in Thumb through Data_08021824 (f021828_entries.s).

#include "types.h"

void Func_08021828(u32 width, u32 height, u8 *grid, u32 edge, u32 fill) {
  u32 size = width * height;
  u8 *next = grid + width + 1;
  s32 above = -width - 1;
  s32 below = width - 1;
  u8 *cell;
  u8 *end;
  u32 x;
  u32 y;
  u32 value;

  for (y = 1; y < height - 1; y++, next += 2) {
    for (x = 1; x < width - 1; x++) {
      value = *++next;
      if (value == 0 || value == 255)
        continue;
      value = next[-2];
      if (value == 0 || value == 255)
        continue;
      value = next[above];
      if (value == 0 || value == 255)
        continue;
      value = next[below];
      if (value == 0 || value == 255)
        continue;
      if (next[-1] != 0)
        next[-1] = 254;
      else
        next[-1] = 255;
    }
  }
  cell = grid;
  end = grid + size;
  while (cell != end) {
    value = *cell++;
    if (value == 0)
      continue;
    if (value > 253)
      cell[-1] = fill;
    else
      cell[-1] = edge;
  }
}
