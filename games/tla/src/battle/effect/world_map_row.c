#include "types.h"
#include "edition.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"
#include "ram_buffer.h"

// Set while the world map shows its alternative picture.
extern s16 Data_0202a640;
extern char ResourceId_Data023, ResourceId_Data024;

#define BG_VRAM 0x06000000

// Draws one row of the world map picture on BG1, from a column to the right
// edge of the map (at most 31 tiles; Japanese always draws 32), for
// scrolling: decodes the picture's row into the cell buffer, which holds 24
// rows and wraps, and copies its cells into the 32-column screen from tile
// 256, wrapping at column 32.
void Func_080ecf74(u32 column, s32 row) {
  s32 resource = (s32)&ResourceId_Data024;
  u32 *graphics;
  s32 unwrapped;
  s32 cell;
  s32 tile;
  u32 i;

  if (Data_0202a640 != 0)
    resource = (s32)&ResourceId_Data023;
  graphics = Resource_GetTableEntry(resource);
  unwrapped = row;
  if (row > 23)
    row -= 24;
  cell = (row << 6) + column;
  tile = (column & 31) + (row << 5);
  tile = (tile + 128) % 768 + 256;
  Resource_DecodeType01((u8 *)graphics + graphics[unwrapped + 1],
                        Ram_MapCellBuffer + (row << 12));
#if LANGUAGE == LANGUAGE_JAPANESE
  for (i = 0; i < 32; i++) {
#else
  for (i = 0; i <= 30 && column + i <= 63; i++) {
#endif
    Dma_Set(Ram_MapCellBuffer + (cell << 6), (void *)(BG_VRAM + (tile << 6)),
            (DMA_ENABLE | DMA_32BIT) << 16 | 16, REG_DMA3);
    cell++;
    tile++;
    if (i + column == 31)
      tile -= 32;
  }
}
