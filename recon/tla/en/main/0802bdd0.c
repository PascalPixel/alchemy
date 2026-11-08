// Matches (tla-en, 2026-11-08, -mtune=gba; tla-ja at 0802bd4c, -de, -es, -fr and -it also differ 0).
// 2026-11-08: written as Map_SetWindowCellTile (field/map/perspective.c) without its view test, keeping its reads of the followed point, whose values go unused: their test is the second basic block that leaves px and py global (20 to 0). The temporary and the identical-arm test below it are gone.
// Editions (2026-11-07): the 124 bytes, literal pool included, are the same in all six (tla-ja at 0802bd4c, the others at 0802bdd0), so this one form serves every edition and needs no patch level; each matches with it.
// 2026-11-06: a temporary that holds the tile id before the shifts and then the divisor 2 (found by permuter) settles the allocation, 49 to 20; either half alone 51, and without the dead if 57.
// 2026-11-06: the listing's untied py (and px in r7) need px and py to be global pseudos, which takes a second basic block. An if/else with identical arms does that and leaves no trace: jump2's cross-jumping merges the arms and deletes the test (checked in the pass dumps), so it is kept below as a dead device, 57 to 49. Arm placement measured: the shifts in the arms with the tile id after 49, the tile id first 51, the tile id in the arms 55 to 57; the store or the calls in the arms do not cross-jump (differ 84 to 108). Plain reshapes that leave px and py global without a test, (px >> 3) / 2 kept in px, differ 69; cell locals 73, inner-block locals 57 to 75, a label alone 57, a loop over the two layers 97.
// 2026-11-05: the listing keeps py from tying to its cell (the sum goes to the sign's register), which local-alloc would refuse only for a pseudo live across blocks; px in r7 says the same, and the twin's view test is such a block, but the listing keeps no trace of a test. One-pass cuts change nothing.
// 2026-11-05: still 57 of 59 by hand and by three minutes of permuter (57). With cell locals, a local 1 and tile += 0x400 (the listing's shapes) 73.
// 2026-10-18: The Broken Seal's
// Map_SetWindowCellTile without the view test: puts world map tile
// (x / 16, y / 16) at cell (px / 16, py / 16) of the tile grid and draws it
// in both layers.
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
  s32 *position;
  s32 originX;
  s32 originY;
  s32 tile;

  window = (struct MapTileWindow *)Ram_HeapSlots->mapWork;
  /* FAKEMATCH(dead): the form kept; as no reads of the followed point, 57 of 59 instructions differ in tla-en. */
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
  Map_WriteLayerCellTile(0, px / 2, py / 2, tile, 1);
  Map_WriteLayerCellTile(1, px / 2, py / 2, tile + 0x400, 1);
}
