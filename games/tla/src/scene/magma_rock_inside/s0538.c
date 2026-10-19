#include "types.h"
#include "ram_buffer.h"

struct MapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct MapLayers {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

extern u8 gMapCellBuffer[];

/// Fills the third byte of each cell in a layer rectangle with value.
/// Layers 3 and above use the shared buffer.
s32 Func_02000538(u32 layer, u32 x, u32 y, u32 width, u32 height, s32 value) {
  struct MapLayers *map = (struct MapLayers *)Ram_HeapSlots->mapWork;
  u8 *cells;

  if (map == NULL)
    return 0;
  /* FAKEMATCH: the barrier's reference gives the map r4 ahead of the width;
   * without it the allocator ranks the width first (6 references over 30 insns
   * against the map's 3 over 8) and the two registers swap (differ 12). The
   * coordinates serve as the loop counters, as the native keeps them in r1 and
   * r2. */
  asm volatile("" : : "l"(map));
  if (layer <= 2)
    cells = map->layers[layer].cells;
  else
    cells = gMapCellBuffer;
  cells += (x + (y << 7)) << 2;
  for (x = 0; x < height; x++) {
    u8 *cell = cells + (x << 9);

    for (y = 0; y < width; y++, cell += 4)
      cell[2] = value;
  }
  return 0;
}
