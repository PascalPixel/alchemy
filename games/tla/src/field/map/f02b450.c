#include "types.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gMapCellBuffer[];

struct TilePos {
  s32 x, y;
};

void Func_0802b450(s32 srcX, s32 srcY, s32 width, s32 height, s32 dstX,
                   s32 dstY) {
  /* FAKEMATCH(temporary): cellPictureId set to 0xfff before its and, and the
   * screen and picture addresses built in parts through address locals; as
   * `cellPictureId = cell & 0xfff` and one expression for each address, 162
   * of 148 instructions differ in tla-en. */
  s32 y;
  u32 *src = (u32 *)gMapCellBuffer + (srcY * 128 + srcX);
  u32 *dst = &((u32 *)gMapCellBuffer)[dstY * 128 + dstX];
  struct MapLayer *layer = Ram_HeapSlots->mapWork->layers;
  struct TilePos views[3];
  s32 i;
  struct TilePos *view = views;
  s32 x;

  for (i = 2; i >= 0; i--) {
    view->x = layer->x >> 20;
    view->y = layer->y >> 20;
    view++;
    layer++;
  }
  for (y = dstY; y < dstY + height; y++) {
    {
      for (x = dstX; x < dstX + width; x++) {
        u32 cell = *src++;
        u32 cellPictureId;
        u32 offset;
        u32 row;
        u32 n;
        u32 column;
        u32 scaled;
        u32 mask;

        cellPictureId = 0xfff;
        mask = 0x7ff;
        /* FAKEMATCH(barrier): an empty asm on mask once it holds 0x7ff, which
         * gives mask's quantity the references that rank it above the
         * destination word in local allocation, so mask keeps r3, the word r2
         * and 0xfffff800 r1; as no asm, 49 of 148 instructions differ in
         * tla-en. */
        __asm__("" : "+r"(mask));
        cellPictureId = cell & cellPictureId;
        mask &= cell;
        mask |= *dst & 0xfffff800;
        *dst = mask;
        dst++;
        view = views;
        column = x & 15;
        i = 0;
        row = y & 15;
        n = row << 5;
        n += column;
        scaled = n * 4;
        offset = scaled;
        for (; i < 3; i += 1) {
          if (view->x <= x && view->x + 16 > x && view->y <= y &&
              view->y + 12 > y) {
            u32 screenAddress, pictureAddress;
            u8 *screenTile, *pictureRows;

            screenAddress = 0x06002800;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            scaled = cellPictureId * 8;
            pictureAddress = (u32)Ram_MapBlocks;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            *(u32 *)screenTile = *(u32 *)pictureRows;

            pictureAddress = (u32)Ram_MapBlocks + 4;
            pictureRows = (u8 *)pictureAddress;
            pictureRows += scaled;
            screenAddress = 0x06002840;
            screenTile = (u8 *)screenAddress;
            screenTile += offset;
            *(u32 *)screenTile = *(u32 *)pictureRows;
            break;
          }
          n = 2048;
          offset += n;
          view++;
        }
      }
      src += 128 - width;
      dst += 128 - width;
    }
  }
}
