// Near miss, differ 321 of 268 (tla-en, 2026-10-31): the shape is the
// native's (same calls, branches, loads, stores and stack slots for rows, x0,
// z0, width and height); what is left is register allocation. Overlay 6ab's
// twin of overlay 698's Func_020011a0 (resource_698_c_020011a0.c).
// The native gives the second loop its own counter (r2) while the first
// loop's i lives at [sp, #16], and reads table->entries again for it, which
// gcse turns into a copy at [sp, #8]: both written so here (288 when the
// loops share i and an `entries` copy, 284 sharing i only). Left: in the
// first loop the native keeps the table in r8, the entry in r9, kind + 1 in
// r10, the cell in r11, event, z and x in r5, r6, r7 and i on the stack;
// here (priorities from the lreg dump: table 36 refs over 334, doubled by
// its constant REG_EQUIV; entry 32/250; cell 11/59; i 16/118; kind + 1
// 9/13; x 15/27; z 15/31) kind + 1 takes r6, z goes to r4 saved around each
// call at [sp, #8], and entry, i, cell and table take r8 to r11, so the
// entries copy moves to [sp, #12]. The second loop's count is PRE'd (ldrh
// kept, reloaded after the current store, extended by shifts) where the
// native reloads it with ldrsh each pass; the native also addresses minZ as
// minX's address + 4 and loads the cells early. Tried without effect: a
// `last` variable, storing z before x, s32 event, one combined test, the
// inner loop as goto (flow weights refs by CFG loop depth anyway).
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

extern struct CellTarget gData_02004cf4;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02004234(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02003804(void);

s32 Func_02003930(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_02004cf4;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 minX, minZ, x0, z0, width, height, i, j;
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
      if (event > kind + 1)
        continue;
      entry->x = i + x0;
      entry->z = j + z0;
      entry->flag = flag;
      flag++;
      if (!GameFlag_Test(0x109)) {
        if (event == kind + 1)
          GameFlag_SetBit(entry->flag);
      } else if (GameFlag_Test(entry->flag)) {
        Func_02004234(table->x, table->z + 1, 1, 1,
                      (i + x0) | (s16)(table->x & 64),
                      (j + z0) | (s16)(table->z & 64));
      }
      table->count++;
      entry++;
    }
  }
  if (GameFlag_Test(0x109)) {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;
    s32 k;

    entry = table->entries;
    for (k = 0; k < table->count; k++, entry++) {
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
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
  Task_Add(Func_02003804, 0x480);
  return table->count;
}
