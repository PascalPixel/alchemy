// Draft, differ 113 of 161, skeleton gap 39 (tla-en, 2026-11-01)
// Adds a water spout on every map cell of the given layer whose spout kind lies
// in [first, first + 8), at the given heights, and marks those cells as spout
// cells (120).
// The loops are built from labels because the listing shows no loop work: both
// 0x80000 loads stay in the body and neither counter is strength-reduced; any
// for or while form reduces both and hoists the constants (gap 75). The counters
// are s16 read as u16. What is left is mainly each counter's tail: the listing
// computes (counter << 16) + 0x10000, then asr and lsr 16, where this emits
// adds #1 first. Spelling that sum out, in u16 temporaries or directly, gives the
// lsl first but drops the asr and folds the counter into its shifted value
// (gap 43). The rest follows from it: cells stays spilled and the head's
// row << 16 lands in r8, not fp, so first takes ip, not sl.
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

void Func_020032d4(u32 first, s32 level, s32 baseLevel) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *layer = map->layers;
  struct MapCell *cells = layer->cells;
  s32 originX = LAYER_S32(layer, 8) + map->minX;
  s32 originZ = LAYER_S32(layer, 12) + map->minZ;
  struct WaterSpouts *work = &gData_0200536c;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 depth = (map->maxZ - map->minZ) >> 20;
  s32 corner = (map->minX >> 20) + ((map->minZ >> 20) << 7);
  struct WaterSpout *spout = &work->spouts[work->count];
  s16 row;

  *(s32 *)(Data_0202c001 + 0x1df) = level << 8;
  cells += corner;
  row = 0;
  if ((u16)row < depth) {
    struct MapCell *cell;
    s16 column;
  rowTop:
    cell = cells + ((u16)row << 7);
    column = 0;
    if ((u16)column < width) {
    columnTop:
      {
        u32 kind = cell->collisionCode;

        if (kind != 0 && kind >= first && kind < first + 8) {
          u16 x = column;
          u16 y = row;

          spout->x = (x << 20) + originX + 0x80000;
          spout->z = (y << 20) + originZ + 0x80000;
          spout->baseY = baseLevel << 19;
          spout->y = level << 19;
          spout->frame = kind - first;
          spout->anchor = 0;
          spout->active = 1;
          spout++;
          work->count++;
          (map->layers[0].terrainIds + corner)[x + (y << 7)] = 120;
        }
      }
      cell++;
      {
        u16 next = column + 1;
        column = next;
        if (next < width)
          goto columnTop;
      }
    }
    {
      u16 next = row + 1;
      row = next;
      if (next < depth)
        goto rowTop;
    }
  }
}
