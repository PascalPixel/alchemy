// Matches: differ 0 (tla-en, 2026-11-08); differ 0 in tla-ja, tla-de, tla-es,
// tla-fr and tla-it too, with no device. Func_020032d4's form (the store
// before cells += origin, loops from labels), with the slot's address loaded
// between entry and origin: its pseudo then lives across origin's computation
// and takes r1, and the reloads of a, origin, the 0 and height rotate through
// r2, r1, r2, r3 as the native does. Declared anywhere else it gives 4
// (before left), 59 (after top, markers, width or height) or 36 (after
// origin); cells += origin before the store gave at best 10.
// Scans the visible part of layer 1's cell grid (view bounds minX to maxZ) for
// cells whose collisionCode lies in [base, base + 4) and records each as a
// CellMarker of gData_02002aa0 (its centre in 16.16 pixels, heights a and b,
// its offset from base), marking the cell with 120 in layer 0's terrainIds.
// Both loops are labels and gotos: any real loop lets loop.c hoist the
// constants and the map loads, which the native keeps in the body.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"
#include "map.h"

struct CellMarker {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct CellMarkers {
  u16 block;
  u16 count;
  struct CellMarker markers[128];
};

extern struct CellMarkers gData_02002aa0;
extern u8 Data_0202c001[];

void Func_020017f8(u32 base, s32 a, s32 b) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *second = &map->layers[1];
  struct MapLayer *layer = &map->layers[0];
  struct MapCell *cells = second->cells;
  s32 left = *(s32 *)&layer->unk08[0] + map->minX;
  s32 top = *(s32 *)&layer->unk08[4] + map->minZ;
  struct CellMarkers *markers = &gData_02002aa0;
  s32 width = (map->maxX - map->minX) >> 20;
  s32 height = (map->maxZ - map->minZ) >> 20;
  struct CellMarker *entry = &markers->markers[markers->count];
  s32 *slot = (s32 *)(Data_0202c001 + 0x1df);
  s32 origin = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  struct MapCell *cell;
  s16 x, z;
  u32 value;

  *slot = a << 8;
  cells += origin;
  z = 0;
  if (z < height) {
  outer:
    cell = cells + ((u16)z << 7);
    x = 0;
    if (x < width) {
    inner:
      value = cell->collisionCode;
      if (value != 0 && value >= base && value < base + 4) {
        entry->x = ((u16)x << 20) + left + 0x80000;
        entry->z = ((u16)z << 20) + top + 0x80000;
        entry->rest = b << 19;
        entry->y = a << 19;
        entry->kind = value - base;
        entry->state = 1;
        entry++;
        markers->count++;
        {
          u8 *terrain = map->layers[0].terrainIds + origin;

          terrain[(u16)x + ((u16)z << 7)] = 120;
        }
      }
      {
        s32 next = (x << 16) + 0x10000;
        x = next >> 16;
        cell++;
        if ((s32)((u32)next >> 16) < width)
          goto inner;
      }
    }
    {
      s32 next = (z << 16) + 0x10000;
      z = next >> 16;
      if ((s32)((u32)next >> 16) < height)
        goto outer;
    }
  }
}
