#include "types.h"
#include "edition.h"
#include "dma.h"
#include "io_reg.h"
#include "resource.h"
#include "ram_buffer.h"

/// Set while the world map shows its alternative picture.
extern s16 Data_0202a640;
extern char ResourceId_Data023, ResourceId_Data024;

#define BG_VRAM 0x06000000

/// Draws one row of the world map picture on BG1, from a column to the right
/// edge of the map (at most 31 tiles; Japanese always draws 32), for
/// scrolling: decodes the picture's row into the cell buffer, which holds 24
/// rows and wraps, and copies its cells into the 32-column screen from tile
/// 256, wrapping at column 32.
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
            DMA_COPY_WORDS(16), REG_DMA3);
    cell++;
    tile++;
    if (i + column == 31)
      tile -= 32;
  }
}

static inline s32 WorldMap_AddColumn(s32 column, s32 cell) {
  /* FAKEMATCH(wrapper): the column added through an inline, so the sum
   * keeps the column as its first operand; as column + cell, 2 of 51
   * instructions differ in tla-en. */
  return column + cell;
}

/// Draws one column of the world map picture on BG1, 21 rows down from a
/// row, for scrolling: copies the column's cells from the decoded rows in the
/// cell buffer, which wraps after 24 rows, into the 32-column screen from
/// tile 256; the column twin of Func_080ecf74.
void Func_080ed030(s32 column, s32 row) {
  u32 i;

  for (i = 0; i <= 20; i++) {
    s32 line = row + i;
    s32 cell = line;
    s32 tile;

    if (line > 23)
      cell = line - 24;
    tile = ((column & 31) + (line << 5) + 128) % 768;
    cell <<= 6;
    /* FAKEMATCH(barrier): an empty asm on the counter and the row's offset
     * loads the cell buffer's address before the column is added; without
     * it, 2 of 51 instructions differ in tla-en. */
    asm("" : "+l"(i) : "l"(cell));
    cell = WorldMap_AddColumn(column, cell);
    cell <<= 6;
    tile <<= 6;
    /* FAKEMATCH(temporary): the screen's address in the line's register; as
     * (u8 *)BG_VRAM + (256 << 6) + tile, 6 of 51 instructions differ in
     * tla-en. */
    line = BG_VRAM + (256 << 6);
    Dma_Set(Ram_MapCellBuffer + cell, (u8 *)line + tile, DMA_COPY_WORDS(16),
            REG_DMA3);
  }
}
