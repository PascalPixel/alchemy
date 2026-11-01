// Draft, differ 220 of 268, skeleton gap 36 (tla-en, 2026-11-01)
// Overlay 6a3's copy of the function drafted in resource_698_c_020011a0.c. Fills
// the cell table from the map's cells whose event byte is the given kind or the
// next one, restores the copied cells of those already flagged, finds the
// leader's entry and loads the sprite's tiles for Func_02003d44.
// Portable to the copies: the form below uses no overlay address but Data_02005b88,
// Func_0200473c and Func_02003d44. What closed the gap from 288 and 76: a separate
// counter for the second loop (the native has it in r2); `cells` read before
// `rows` is made; the second loop's bottom test read through (s16 *), because
// hashing the alias set stops gcse's PRE from carrying count in a register
// across the store to current (gap 78 to 57); the source column and row read as
// u16 locals for the masks, while the call's own table->x and table->z stay
// separate ldrsh loads, which also puts the mask's arguments in the right shape.
// Left, all allocation: the first register allocation decides nearly everything.
// The native's high registers go in the order table r8, kind + 1 r10, entry r9,
// cell r11, with i and j on the stack, and r5 event, r6 cellZ, r7 cellX; global.c
// hands r8, r10, r9, r11 out in priority order, so the native has table above
// kind + 1 above entry above cell above i. Here entry (0.67) r8, cell (0.57) r10,
// i (0.56) r9, table (0.36) r11, j on the stack, kind + 1 (2.08) r6 and cellX in r4
// saved around calls. kind + 1 in r10 with no caller-save implies a live range
// across the whole inner loop (it should cross at least 3 calls), though every use
// follows its set; nothing plain reproduces that. Also left, once allocation
// moves: the shared 64 (`adds r3, r2, #0`) and the temporaries in r4 and r5.
// Tried, no gain: table as the bare global (differ 294, gap 98, base rebuilt
// from entries), a second table pointer after the first loop (copy propagated
// away), no entries variable or entries first (entry stays 32 refs), a local
// holding 64 (gap 62), event and last at function scope, bare kind + 1, nested
// ifs, a goto second loop, reordered i and j. No device: none would reach differ 0.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"

struct CellTargetEntry {
  s16 x;
  s16 z;
  s16 flag;
  s16 unk06;
};

struct CellTarget {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
  u16 unk12;
  u32 oam[3];
  struct CellTargetEntry entries[1];
};

struct CellMapCell {
  u8 unk00[2];
  u8 event;
  u8 unk03;
};

struct CellMapLayer {
  s32 unk00[2];
  s32 originX;
  s32 originZ;
  u8 unk10[0x20];
  struct CellMapCell *cells;
};

struct CellMapWork {
  u8 unk000[0xec];
  s32 minX;
  s32 minZ;
  s32 maxX;
  s32 maxZ;
  u8 unk0fc[0xc];
  struct CellMapLayer layers[4];
};

extern struct CellTarget Data_02005b88;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_0200473c(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02003d44(void);

s32 Func_02003e70(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &Data_02005b88;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *cells;
  struct CellMapCell *rows;
  s32 x0, z0, width, height, minX, minZ;
  s32 i, j;
  struct CellTargetEntry *entries = entry;
  struct CellMapCell *cell;
  u32 *buffer;

  Iwram_ClearWords(table, 4);
  table->kind = kind;
  table->firstFlag = flag;
  table->flagByte = flagByte;
  table->x = x;
  table->z = z;
  table->current = -1;
  table->count = 0;
  map = (struct CellMapWork *)Ram_HeapSlots->mapWork;
  layer = &map->layers[0];
  minX = map->minX;
  cells = layer->cells;
  x0 = (layer->originX + minX) >> 20;
  minZ = map->minZ;
  z0 = (layer->originZ + minZ) >> 20;
  width = (map->maxX - minX) >> 20;
  height = (map->maxZ - minZ) >> 20;
  rows = cells + (((minZ >> 20) << 7) + (minX >> 20));
  for (j = 0; j < height; j++) {
    cell = rows + (j << 7);
    for (i = 0; i < width; i++, cell++) {
      u32 event = cell->event;
      u32 last;
      s32 cellX, cellZ;

      if (event == 0 || event < kind)
        continue;
      last = kind + 1;
      if (event > last)
        continue;
      cellX = i + x0;
      entry->x = cellX;
      cellZ = j + z0;
      entry->z = cellZ;
      entry->flag = flag;
      flag++;
      if (!GameFlag_Test(0x109)) {
        if (event == last)
          GameFlag_SetBit(entry->flag);
      } else if (GameFlag_Test(entry->flag)) {
        u16 sourceX = table->x;
        u16 sourceZ = table->z;
        s16 xBit = sourceX & 64;
        s16 zBit = sourceZ & 64;

        cellX |= xBit;
        cellZ |= zBit;
        Func_0200473c(table->x, table->z + 1, 1, 1, cellX, cellZ);
      }
      table->count++;
      entry++;
    }
  }
  if (GameFlag_Test(0x109)) {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;

    s32 n;

    entry = entries;
    n = 0;
    if (n < table->count) {
      do {
        if (entry->x == leaderX && entry->z == leaderZ)
          table->current = n;
        entry++;
        n++;
        // The cast keeps this read apart from the first, so count is reread.
      } while (n < *(s16 *)&table->count);
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;
    s32 count = 63;

  fill:
    *word++ = 0x11111111;
    if (--count >= 0)
      goto fill;
  }
  table->tile = Resource_FindFreeEntry();
  VramCache_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02003d44, 0x480);
  return table->count;
}
