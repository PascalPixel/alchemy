// Draft, differ 288 of 268, skeleton gap 32 (tla-en, 2026-11-01)
// Overlay 6b0's Func_02002a74, the twin of overlay 698's Func_020011a0. Fills
// the cell table from the map's cells whose event byte is the given kind or
// the next one, restores the copied cells of those already flagged, finds the
// leader's entry and loads the sprite's tiles for Func_02002948.
// Closed: both map loops as label-and-goto (38 to 32). The 64 stays in the
// body, so the two ands share one register as the native's do; written as for
// loops, loop.c hoists it and each and rebuilds it.
// Left, allocation first; the rest of the gap mostly follows from it. The
// native has the table in r8, entry r9, kind + 1 r10, cell r11, cellX r7,
// cellZ r6, and i (slot 16) and j (slot 12) on the stack, with entries in slot
// 8: the declared order here gives those slots once i spills. Here the table
// is r9, entry r8, cell r10, i r11, kind + 1 r6, and cellZ r4 with a save
// around each call. Priorities from the lreg dump: last 1.93 (refs 9, live
// 14), cellX 1.55 and cellZ 1.32 (refs 15), entry .635, cell .541, table .536,
// i .533. So the native needs cellX and cellZ above last, the table above
// entry and cell, and i below them all. last must also refuse r4 and r0 to r3:
// by the caller-save rule that takes 4 * calls >= refs, or a conflict with the
// else arm's locals, yet its only uses are the two compares and it crosses one
// call. Six uses (refs 18) would lift cellX and cellZ past it, and the listing
// shows only five. Not found.
// Also left: gcse's PRE turns the second loop's table->count into a register,
// inserted after the store to current and in the preheader, where the native
// rereads it. No loop form changes that (while, if plus goto, bottom entry).
// Tried with no gain: last as an outer, bare or s32 variable; event s32; cellX
// and cellZ as outers or swapped; flag++ in the store; entries set from the
// table, before or after the clear (the entry pointer falls to .512 after it,
// but the code moves away); indexed table->entries[n] (gap 44); a separate
// pointer in the second loop (37/32); i and j swapped; a mask local for 64,
// which cprop folds; s16 mask locals merge the table->x loads the native keeps.
// Not adopted: the second loop's counter as i measures 19/26 (differ 265), but
// that reuses a local for a second value, and the native has it in r2 with i
// on the stack.
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
  j = 0;
  if (j < height) {
  outer:
    {
    struct CellMapCell *cell = rows + (j << 7);

    i = 0;
    if (i < width) {
    inner:
      {
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
      i++;
      cell++;
      if (i < width)
        goto inner;
    }
    }
    j++;
    if (j < height)
      goto outer;
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
