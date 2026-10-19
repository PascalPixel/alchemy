// Near miss, differ 57 of 59 (2026-10-18): The Broken Seal's
// Map_SetWindowCellTile without the view test: puts world map tile
// (x / 16, y / 16) at cell (px / 16, py / 16) of the tile grid and draws it
// in both layers. Left: allocation. The native keeps px in r7 and the
// constant 1 in r8 and spills the tile id to sp+4 across the first call (an
// 8-byte frame); this keeps the tile in a register and the parameters in r4
// and r6. Statement order (the tile first or last, px or py first) gives the
// same code here.
#include "types.h"
#include "ram_buffer.h"

struct MapTileWindow {
  s32 *position;
  u8 unk004[0x144];
  u16 tiles[32][32];
};

s32 Map_WriteLayerCellTile(s32 layer, s32 x, s32 y, s32 tile, s32 update);

void Func_0802bdd0(s32 x, s32 y, s32 px, s32 py) {
  struct MapTileWindow *window;
  s32 tile;

  window = (struct MapTileWindow *)Ram_HeapSlots->mapWork;
  tile = ((y >> 4) << 5) + (x >> 4);
  px >>= 3;
  py >>= 3;
  window->tiles[(py / 2) & 31][(px / 2) & 31] = tile;
  Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
  Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
}
