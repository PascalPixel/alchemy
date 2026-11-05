// Measured under -Os, 5 November 2026: differ 97 (tla-en); notes below may predate it.
// Draft, differ 113 of 161, skeleton gap 39 (tla-en, 2026-11-01)
// Adds a water spout on every map cell of the given layer whose spout kind lies
// in [first, first + 8), at the given heights, and marks those cells as spout
// cells (120).
// The loops are built from labels because the listing shows no loop work: both
// 0x80000 loads stay in the body and neither counter is strength-reduced; any
// for or while form reduces both and hoists the constants (gap 75). This pass
// dropped the x, y and next locals with no change in measure.
// Left, step: the listing computes (counter << 16) + 0x10000, then asr and
// lsr 16; this emits adds #1, lsls. u16 counters with ++column < width give
// that form (a u16 plus goes through zero-extension, which combine distributes;
// an s16 plus does not), but gcse's PRE then shares counter << 16 with the body
// and the row head, leaving gap 41 and differ 185.
// Left, allocation: the listing gives map ip, row lr, cells r8, first sl, work
// r9, row << 16 fp, and width and depth the stack. This gives width fp and
// cells the stack, with first in ip. The -O2 priorities (cells 5 refs, width 6,
// first 10, from the lreg dump) cannot give that order; the listing's order
// fits refs not weighted by loop depth, and its tail fits no PRE. Both are
// compiler questions, out of scope here.
// Tried without effect: a sweep of 72 pairs of s16 or u16 counters with next,
// pre and post steps, declaration scopes, cell++ after the counter, kind
// outside the loop. Worse: cells + corner at the row head (gap 43), terrainIds
// indexed through layer (gap 54) or with corner in the index (gap 41).
#include "types.h"
#include "call.h"
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
          (map->layers[0].terrainIds + corner)[(u16)column +
                                               ((u16)row << 7)] = 120;
        }
      }
      {
        s32 next = (column << 16) + 0x10000;
        column = next >> 16;
        cell++;
        if ((u32)next >> 16 < width)
          goto columnTop;
      }
    }
    {
      s32 next = (row << 16) + 0x10000;
      row = next >> 16;
      if ((u32)next >> 16 < depth)
        goto rowTop;
    }
  }
}
