#include "types.h"
#include "ram_buffer.h"
#include "map.h"

struct WaterSpout {
  s32 x;
  s32 y;
  s32 z;
  s32 baseY;
  u16 frame;
  u16 active;
  s32 *anchor;
  u32 top[3];
  u32 column[3];
};

struct WaterSpouts {
  u16 resource;
  u16 count;
  struct WaterSpout spouts[32];
};

extern struct WaterSpouts gData_0200536c;
extern u8 Data_0202c001[];

#define LAYER_S32(layer, off) (*(s32 *)((layer)->unk08 + (off) - 8))

// Adds a water spout on every map cell of the given layer whose spout kind lies
// in [first, first + 8), at the given heights, and marks those cells as spout
// cells (120). The loops are built from labels: both 0x80000 loads stay in the
// body and neither counter is strength-reduced.
void Func_020032d4(u32 first, s32 level, s32 baseLevel) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *layer = map->layers;
  struct MapCell *cells = layer->cells;
  s32 originX = LAYER_S32(layer, 8) + map->minX;
  s32 originZ = LAYER_S32(layer, 12) + map->minZ;
  struct WaterSpouts *work = &gData_0200536c;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 depth = (map->maxZ - map->minZ) >> 20;
  struct WaterSpout *spout = &work->spouts[work->count];
  s32 corner = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  s16 row;

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner;
  row = 0;
  if (row < depth) {
    struct MapCell *cell;
    s16 column;
  rowTop:
    cell = cells + ((u16)row << 7);
    column = 0;
    if (column < width) {
    columnTop:
      {
        u32 kind = cell->collisionCode;

        if (kind != 0 && kind >= first && kind < first + 8) {
          spout->x = ((u16)column << 20) + originX + 0x80000;
          spout->z = ((u16)row << 20) + originZ + 0x80000;
          spout->baseY = baseLevel << 19;
          spout->y = level << 19;
          spout->frame = kind - first;
          spout->anchor = 0;
          spout->active = 1;
          spout++;
          work->count++;
          {
            u8 *terrain = map->layers[0].terrainIds + corner;

            terrain[(u16)column + ((u16)row << 7)] = 120;
          }
        }
      }
      {
        s32 next = (column << 16) + 0x10000;
        column = next >> 16;
        cell++;
        if ((s32)((u32)next >> 16) < width)
          goto columnTop;
      }
    }
    {
      s32 next = (row << 16) + 0x10000;
      row = next >> 16;
      if ((s32)((u32)next >> 16) < depth)
        goto rowTop;
    }
  }
}
