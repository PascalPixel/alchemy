// Draft, differ 304 of 268, skeleton gap 72 (tla-en, 2026-11-01)
// Overlay 6ad's Func_020038ac, the twin of overlay 698's Func_020011a0. Fills
// the cell table from the map's cells whose event byte is the given kind or
// the next one, restores the copied cells of those already flagged, finds the
// leader's entry and loads the sprite's tiles for Func_02003780.
// Left: allocation drives most of the gap. The native keeps event, cellZ and
// cellX in r5 to r7, the table, kind + 1, the entries and the cell in r8 to
// r11, and i and j on the stack. Here kind + 1 takes r6 and cellZ takes r4 with
// a save around each call, so i keeps a register and the two ands with 64 each
// load their own constant. Global.c ranks kind + 1 (9 refs over 13 insns, 2.07)
// above cellX and cellZ (15 refs over 29 and 30, about 1.5). It would need a
// life of 20 or more insns, or one more ref each on cellX and cellZ; the
// listing shows neither. Loop depth comes from the CFG, so a goto loop does
// not change the weights. Pinning kind + 1 costs the table its register.
// Also left: gcse's PRE turns the leader loop's table->count into a register
// reloaded after the store to current, where the native rereads it.
// Tried with no gain: event as u8 or s32, kind signed, kind + 1 as s32 or bare,
// outer cellX and cellZ, the flag and test spellings, and a shared mask or
// separate dstX and dstZ.
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

extern struct CellTarget gData_020070a8;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_020054e0(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02003780(void);

s32 Func_020038ac(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_020070a8;
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
            u16 x = table->x;
            u16 z = table->z;
            s16 xBit = x & 64;
            s16 zBit = z & 64;

            Func_020054e0(table->x, table->z + 1, 1, 1, cellX | xBit,
                          cellZ | zBit);
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
  Task_Add(Func_02003780, 0x480);
  return table->count;
}
