#include "types.h"
#include "io_reg.h"
#include "ram_buffer.h"
#include "map.h"

struct TilePos {
  s32 x, y;
};

void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
                                    u32 width, u32 height) {
  u32 *src = (u32 *)Ram_MapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = (u32 *)Ram_MapCellBuffer + (dstY * 128 + dstX);
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos tile[3];
  s32 i;
  struct TilePos *pos = tile;
  u32 y;
  u32 x;

  // The three layers' visible areas: their scroll positions in cells.
  for (i = 2; i >= 0; i--) {
    pos->x = layer->x >> 20;
    pos->y = layer->y >> 20;
    pos++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    for (x = dstX; x < dstX + width; x++) {
      u32 val = *src++;
      u32 cell = val & 0xfff;
      u32 column, row, mapCell, offset, bytes;

      *dst = (*dst & 0xfffff000) | cell;
      column = x & 15;
      dst++;
      i = 0;
      row = y & 15;
      pos = tile;
      // offset is the cell's place in a 32x32 BG map: 128 bytes a cell row,
      // 4 a cell.
      mapCell = row << 5;
      mapCell += column;
      offset = mapCell * 4;
      // The first layer whose visible area (16x12 cells) holds the cell has
      // the cell picture written to its BG map too, and only that one (a map
      // lays its layers out in different parts of the grid); the layers' BG
      // maps follow each other, 2,048 bytes apart, from screen block 5.
      for (; i < 3; i++) {
        if (pos->x <= (s32)x && pos->x + 16 > (s32)x && pos->y <= (s32)y &&
            pos->y + 12 > (s32)y) {
          u32 screenAddress, cellPictureAddress;
          u8 *screenTile, *cellPictureRows;

          // The cell picture's (8 bytes) top two tiles, and its bottom two
          // 64 bytes further on.
          screenAddress = (u32)BG_SCREEN_BLOCK(5);
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          bytes = cell * 8;
          cellPictureAddress = (u32)Ram_MapBlocks;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += bytes;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;

          cellPictureAddress = (u32)Ram_MapBlocks + 4;
          cellPictureRows = (u8 *)cellPictureAddress;
          cellPictureRows += bytes;
          screenAddress = (u32)BG_SCREEN_BLOCK(5) + 64;
          screenTile = (u8 *)screenAddress;
          screenTile += offset;
          *(u32 *)screenTile = *(u32 *)cellPictureRows;
          break;
        }
        // bytes, reused for the step to the next layer's BG map.
        bytes = 2048;
        offset += bytes;
        pos++;
      }
    }
    src += 128 - width;
    dst += 128 - width;
  }
}
