// Draft, differ 306 of 268, skeleton gap 38 (tla-en, 2026-11-01)
// Overlay 6b0's Func_02002a74, the twin of overlay 698's Func_020011a0. Fills
// the cell table from the map's cells whose event byte is the given kind or
// the next one, restores the copied cells of those already flagged, finds the
// leader's entry and loads the sprite's tiles for Func_02002948.
// Left: allocation first. The native keeps the table in r8, the entries in r9,
// kind + 1 in r10, the cell in r11, cellX and cellZ in r7 and r6, and i and j
// on the stack; here r11 is the table, r8 the entries, r9 i, r10 the cell, r6
// kind + 1, and cellZ takes r4 with a save around each call. Priorities say
// kind + 1 (2.07) should lose to cellX and cellZ (1.67, 1.4) and to the table
// (0.54), so its live range or refs differ in a way not yet found; plain forms
// for it (a user variable, an outer one, the bare expression) change nothing,
// and an extra copy of cellX or cellZ is merged by CSE.
// Also left: gcse's PRE turns the second loop's table->count into a register
// reloaded after the store to current, where the native rereads it; volatile
// stops that but is a device and widens the gap (44/50). The two ands with 64
// share one register in the native, which here is rematerialized.
// Tried: table as the bare global (gap 49/45), a separate pointer in the
// second loop (42/37), bit-field count (no change).
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

extern struct CellTarget Data_02004458;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02003350(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02002948(void);

s32 Func_02002a74(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &Data_02004458;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  s32 x0, z0, width, height;
  s32 i, j;
  struct CellTargetEntry *entries = entry;
  struct CellMapCell *cells;
  s32 minX, minZ;
  s32 *bounds;
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
  bounds = &map->minX;
  minX = *bounds;
  cells = layer->cells;
  x0 = (layer->originX + minX) >> 20;
  bounds++;
  minZ = *bounds;
  z0 = (layer->originZ + minZ) >> 20;
  width = (map->maxX - minX) >> 20;
  height = (map->maxZ - minZ) >> 20;
  rows = cells + ((minZ >> 20) << 7) + (minX >> 20);
  for (j = 0; j < height; j++) {
    struct CellMapCell *cell = rows + (j << 7);

    for (i = 0; i < width; i++, cell++) {
      u32 event = cell->event;

      if (event != 0 && event >= kind) {
        u32 last = kind + 1;

        if (event <= last) {
          s32 cellX = i + x0;
          s32 cellZ = j + z0;

          entry->x = cellX;
          entry->z = cellZ;
          entry->flag = flag;
          flag++;
          if (!GameFlag_Test(0x109)) {
            if (event == last)
              GameFlag_SetBit(entry->flag);
          } else if (GameFlag_Test(entry->flag)) {
            Func_02003350(table->x, table->z + 1, 1, 1,
                          cellX | (s16)(table->x & 64),
                          cellZ | (s16)(table->z & 64));
          }
          table->count++;
          entry++;
        }
      }
    }
  }
  if (GameFlag_Test(0x109)) {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;
    s32 n;

    entry = entries;
    for (n = 0; n < table->count; n++, entry++) {
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = n;
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
  Task_Add(Func_02002948, 0x480);
  return table->count;
}
