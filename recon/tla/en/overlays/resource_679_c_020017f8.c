// Draft, differ 208 of 159: scans the visible part of the map's cell grid (from
// the map work's layer at 0x108 and view bounds at 0xec) for cells whose
// byte 2 lies in [base, base + 4) and records each as a 32-byte entry of
// gData_02002aa0 (its centre in 16.16 pixels, heights a and b, its offset
// from base), marking the cell with 120 in the map's shape grid. Left: the native keeps the view origin, sizes and both heights on the stack and the loop counters as halfword pairs; the alignment is poor.
#include "types.h"
#include "ram_buffer.h"

extern u8 gData_02002aa0[];
extern u8 Data_0202c001[];

void Func_020017f8(u32 base, s32 a, s32 b) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  u8 *layer = map + 0x108;
  u8 *cells = *(u8 **)(map + 0x170);
  s32 left = *(s32 *)(layer + 8) + *(s32 *)(map + 0xec);
  s32 top = *(s32 *)(layer + 12) + *(s32 *)(map + 0xf0);
  s32 width = (*(s32 *)(map + 0xf4) - *(s32 *)(map + 0xec)) >> 20;
  s32 height = (*(s32 *)(map + 0xf8) - *(s32 *)(map + 0xf0)) >> 20;
  s32 origin = ((*(s32 *)(map + 0xf0) >> 20) << 7) + (*(s32 *)(map + 0xec) >> 20);
  u8 *entry = gData_02002aa0 + *(u16 *)(gData_02002aa0 + 2) * 32 + 4;
  u8 *cell;
  s16 x, z;
  u32 value;

  *(s32 *)(Data_0202c001 + 0x1df) = a << 8;
  cells += origin * 4;
  for (z = 0; (u16)z < height; z++) {
    cell = cells + ((u16)z << 9);
    for (x = 0; (u16)x < width; x++, cell += 4) {
      value = cell[2];
      if (value == 0 || value < base || value >= base + 4)
        continue;
      *(s32 *)entry = ((u16)x << 20) + left + 0x80000;
      *(s32 *)(entry + 8) = ((u16)z << 20) + top + 0x80000;
      *(s32 *)(entry + 12) = b << 19;
      *(s32 *)(entry + 4) = a << 19;
      *(u16 *)(entry + 16) = value - base;
      *(u16 *)(entry + 18) = 1;
      entry += 32;
      (*(u16 *)(gData_02002aa0 + 2))++;
      (*(u8 **)(map + 0x13c) + origin)[(u16)x + ((u16)z << 7)] = 120;
    }
  }
}
