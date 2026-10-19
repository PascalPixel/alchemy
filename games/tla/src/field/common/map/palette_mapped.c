#include "types.h"
#include "ram_buffer.h"

extern u8 gMapBlocks[];

/// Copies one world map cell's two tile entries (top and bottom) to a BG map.
static __inline__ void CopyWorldMapTileColumn(u8 *cellBytes, u8 *screenTile) {
  u32 metatileIndex = *(u16 *)cellBytes;
  u16 *metatileTiles = (u16 *)Ram_MapCellBuffer;

  metatileTiles += metatileIndex * 2;
  *(u16 *)screenTile = *metatileTiles++;
  *(u16 *)(screenTile + 64) = *metatileTiles;
}

/// Draws world map tile row tileRow (two screen rows) into the BG map at
/// 0x06004000, from the 32 by 32 cell grid in gMapBlocks.
void Map_RenderPaletteMappedRow(u32 tileRow) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 column;

  cellBytes = gMapBlocks + ((((s32)tileRow / 2) & 31) << 7);
  {
    /* FAKEMATCH(pin): the BG map address held in r5, loaded right after the
     * cell address is formed; as the plain constant added to the row offset,
     * 10 of 57 instructions differ in tla-en. */
    register u32 screenBase asm("r5") = 0x06004000;

    /* FAKEMATCH(barrier): the pin's empty asm, which keeps the load where it
     * is; as the plain constant, 10 of 57 instructions differ in tla-en. */
    __asm__ volatile("" : "+l"(screenBase));
    screenTile = (u8 *)(((tileRow & 62) << 6) + screenBase);
  }
  column = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    column++;
    screenTile += 2;
    cellBytes += 4;
  } while (column <= 31);

  {
    /* FAKEMATCH(pin): the step to the next half's screen row held in r2; as
     * screenTile += 0xfc0 beside the pin above, 10 of 57 instructions differ
     * in tla-en (reload builds the step in r5). */
    register u32 screenStep asm("r2") = 0xfc0;

    /* FAKEMATCH(barrier): the pin's empty asm; as screenTile += 0xfc0, 10 of
     * 57 instructions differ in tla-en. */
    __asm__("" : "+l"(screenStep));
    screenTile += screenStep;
  }
  cellBytes += 0xf80;
  column = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    column++;
    screenTile += 2;
    cellBytes += 4;
  } while (column <= 31);
}

/// Draws world map tile column tileColumn (two screen columns) into the BG map
/// at 0x06004000.
void Map_RenderPaletteMappedColumn(u32 tileColumn) {
  u8 *cellBytes;
  u8 *screenTile;
  u32 row;
  /* FAKEMATCH(pin): the BG map address held in r5, loaded between the cell
   * offset's shift and add; as the plain constant added to the column offset,
   * 4 of 32 instructions differ in tla-en. */
  register u32 screenBase asm("r5") = 0x06004000;

  cellBytes = gMapBlocks + ((((s32)tileColumn / 2) & 31) << 2);
  /* FAKEMATCH(barrier): the pin's empty asm after the cell address; as the
   * plain constant, 4 of 32 instructions differ in tla-en. */
  __asm__ volatile("" : "+l"(screenBase));
  screenTile = (u8 *)((tileColumn & 62) + screenBase);
  row = 0;
  do {
    CopyWorldMapTileColumn(cellBytes, screenTile);
    row++;
    screenTile += 128;
    cellBytes += 128;
  } while (row <= 63);
}
