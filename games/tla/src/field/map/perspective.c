#include "types.h"

#define ABS(v) ((v) < 0 ? -(v) : (v))

struct MapTileWindow_08010d48 {
  s32 *position;
  u8 unk004[0x144];
  u16 tiles[32][32];
};

#include "ram_buffer.h"

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Map_SetWindowCellTile(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow_08010d48 *window;
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow_08010d48 *)Ram_HeapSlots->mapWork;
  originX = 0;
  originY = 0;
  position = window->position;
  if (position != 0) {
    originX = *position++;
    originY = position[1];
  }

  originX >>= 24;
  originY >>= 24;
  x >>= 4;
  y >>= 4;
  px >>= 3;
  py >>= 3;
  tile = (y << 5) + x;
  window->tiles[(py / 2) & 31][(px / 2) & 31] = tile;

  if (ABS(originX - px) <= 1 && ABS(originY - py) <= 1) {
    Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
    Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
  }
}
