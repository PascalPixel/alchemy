// Draft, differ 288 of 268: overlay 6a2's copy of the function drafted in
// resource_698_c_020011a0.c (overlay 698's Func_020011a0), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "adventure.h"
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

extern struct CellTarget Data_02002764;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001bb0(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_020011e0(void);

s32 Func_0200130c(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &Data_02002764;
  struct CellTargetEntry *entry = table->entries;
  struct CellTargetEntry *entries = entry;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 minX, minZ, x0, z0, width, height, i, j;
  u32 *buffer;
  u32 last;

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
  x0 = (layer->originX + minX) >> 20;
  minZ = map->minZ;
  z0 = (layer->originZ + minZ) >> 20;
  width = (map->maxX - minX) >> 20;
  height = (map->maxZ - minZ) >> 20;
  rows = layer->cells + ((minZ >> 20) << 7) + (minX >> 20);
  for (j = 0; j < height; j++) {
    cell = rows + (j << 7);
    for (i = 0; i < width; i++, cell++) {
      u32 event = cell->event;

      if (event == 0 || event < kind)
        continue;
      last = kind + 1;
      if (event > last)
        continue;
      entry->x = i + x0;
      entry->z = j + z0;
      entry->flag = flag;
      flag++;
      if (!GameFlag_Test(0x109)) {
        if (event == last)
          GameFlag_SetBit(entry->flag);
      } else if (GameFlag_Test(entry->flag)) {
        Func_02001bb0(table->x, table->z + 1, 1, 1,
                      (i + x0) | (s16)(table->x & 64),
                      (j + z0) | (s16)(table->z & 64));
      }
      table->count++;
      entry++;
    }
  }
  if (GameFlag_Test(0x109)) {
    struct Actor *leader = Actor_GetById(gAdventure.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;

    entry = entries;
    for (i = 0; i < table->count; i++, entry++) {
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = i;
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
  Task_Add(Func_020011e0, 0x480);
  return table->count;
}
