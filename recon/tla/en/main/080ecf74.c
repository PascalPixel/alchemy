// First draft of Func_080ecf74: draws one row of the world map picture on BG1
// from a column to the right edge (at most 31 tiles), as each row of
// Func_080ece20 (recon/tla/en/main/080ece20.c), for scrolling. No ☀️ twin: its
// world map is one picture. Differ 72 of 83 (draft 79): register choice and
// order. The native computes the cell index (row << 6) + column first and
// keeps it in r9 across both calls, the graphics in r7 and the unwrapped row
// in r10; this draft computes the index after the decode, so graphics takes
// r10 and the tile r7. The loop is the same.
#include "types.h"
#include "dma.h"
#include "resource.h"
#include "ram_buffer.h"

extern s16 Data_0202a640;
extern char ResourceId_035, ResourceId_036; // resource ids, linker symbols

void Func_080ecf74(u32 column, s32 row) {
  s32 resource = (s32)&ResourceId_036;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u8 *cells;
  u32 i;
  u32 x;

  if (Data_0202a640 != 0)
    resource = (s32)&ResourceId_035;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row > 23)
    row -= 24;
  cell = (row << 6) + column;
  tile = ((column & 31) + (row << 5) + 128) % 768 + 256;
  Resource_DecodeType01((u8 *)graphics + graphics[unwrapped + 1],
                        Ram_MapCellBuffer + (row << 12));
  cells = Ram_MapCellBuffer + (cell << 6);
  for (i = 0, x = column; i <= 30 && x <= 63; i++, x++) {
    Dma_Set(cells, (void *)(0x06000000 + (tile << 6)), 0x84000010,
            (volatile u32 *)0x040000d4);
    cells += 64;
    tile++;
    if (x == 31)
      tile -= 32;
  }
}
