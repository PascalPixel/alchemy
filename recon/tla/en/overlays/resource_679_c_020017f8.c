// Differ 14 of 159 (tla-en, 2026-11-07).
// 7 November: the word at Data_0202c001 + 0x1df through a local pointer (permuter) gives
// 19 to 14. Left: allocation, as below.
// Before: differ 19 of 159 (tla-en, 5 November 2026), from 84: Func_020032d4's form.
// Left: the native stores a << 8 before adding origin, with the pool address in r1
// and a in r2; written in that order the rotation shifts by one (36).
// Measured under -Os, 5 November 2026: differ 84 (tla-en); notes below may predate it.
// Draft, differ 108 of 159, skeleton gap 34 (tla-en, 2026-11-01)
// Scans the visible part of layer 1's cell grid (view bounds minX to maxZ) for
// cells whose collisionCode lies in [base, base + 4) and records each as a
// CellMarker of gData_02002aa0 (its centre in 16.16 pixels, heights a and b,
// its offset from base), marking the cell with 120 in layer 0's terrainIds.
// Left: both loops are labels and gotos because any real loop lets loop.c hoist
// the constants and the map loads, which the native keeps in the body (gap 60
// or more as for, while or do). Each counter's step is one structural miss: the
// native builds (x << 16) + 0x10000 with the constant loaded by reload, then
// takes asrs and lsrs of it; the draft does adds #1, lsls. Spelling it
// t = (u16)x + 1; x = t; if ((u16)t < width) gets that form for x (gap 31) but
// is a forced temporary and gcse's PRE then shares x << 16 with the body, so it
// is not kept; the native's step has no x << 16 for gcse to see, and the
// three-insn combine that would split it was not found. The rest is allocation:
// the native keeps z in lr, cells in r8, base in sl and z << 16 in r11 with
// width on the stack; the draft has base in lr, z << 16 in r8, z in sl, width in
// r11 and cells on the stack, with base and z << 16 outranking z and cells in
// global's priorities. Declaration order and scopes do not move them.
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
  s32 origin = ((map->minZ >> 20) << 7) + (map->minX >> 20);
  struct MapCell *cell;
  s32 *slot = (s32 *)(Data_0202c001 + 0x1df);
  s16 x, z;
  u32 value;

  cells += origin;
  *slot = a << 8;
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
