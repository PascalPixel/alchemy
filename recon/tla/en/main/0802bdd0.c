#include "types.h"
#include "ram_buffer.h"

struct MapCellWork {
  u8 unk000[0x148];
  u16 cells[32 * 32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Func_0802bdd0(s32 px, s32 py, s32 cx, s32 cy) {
  struct MapCellWork *map = (struct MapCellWork *)Ram_HeapSlots->mapWork;
  s32 tile;

  cx = (cx >> 3) / 2;
  cy = (cy >> 3) / 2;
  tile = ((py >> 4) << 5) + (px >> 4);
  map->cells[((cy & 31) << 5) + (cx & 31)] = tile;
  Map_WriteLayerCellTile(0, cx, cy, tile, 1);
  Map_WriteLayerCellTile(1, cx, cy, tile + 0x400, 1);
}
