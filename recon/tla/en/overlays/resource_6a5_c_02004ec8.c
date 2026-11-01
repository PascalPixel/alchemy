// Draft, differ 268 of 268, skeleton gap 32 (tla-en, 2026-11-01)
// Left: register ranks, which cost the r4 spill around both GameFlag_Test calls
// and put i in r11 where the native spills it. The native keeps event r5, cz r6,
// cx r7, the table r8, last r10, the entry r9, the cell r11 and i at sp+16. Here
// last ranks second (9 refs over 14 insns) and takes r6, so cz is caller-saved
// in r4. Flow weights refs by natural-loop depth (3 in the inner loop), and that
// happens for goto and for loops alike. The native order fits unweighted refs:
// last has 3, ranks below the table and is too dear to caller-save. No C shape
// tried gives that: no last variable, block locals, either order of cx and cz,
// masks in locals. For loops also hoist the 64 (gap 38). One compiler question.
// GCSE also moves the leader search's count load into the arm that stores
// current (ldrh and shifts); the native reloads it with ldrsh at the loop's end.
// A goto loop or a continue does not change that.
// Overlay 6a5's copy of overlay 698's Func_020011a0, drafted in
// resource_698_c_020011a0.c with this overlay's symbols.
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

extern struct CellTarget Data_02006b74;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_0200579c(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02004d9c(void);

s32 Func_02004ec8(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &Data_02006b74;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *cells;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  u32 *buffer;
  u32 event, last;
  s32 cz, cx;

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
  cells = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  rows = cells + ((map->minZ >> 20) << 7) + (map->minX >> 20);
  // Goto loops: as for loops (either one) loop.c moves the 64 of the cell
  // copy's masks out of them and 321 of 268 instructions differ.
  j = 0;
  if (j < height) {
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
    column:
      event = cell->event;
      if (event != 0 && event >= kind) {
        last = kind + 1;
        if (event <= last) {
          cz = j + z0;
          cx = i + x0;
          entry->x = cx;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!GameFlag_Test(0x109)) {
            if (event == last)
              GameFlag_SetBit(entry->flag);
          } else if (GameFlag_Test(entry->flag)) {
            Func_0200579c(table->x, table->z + 1, 1, 1, cx | (s16)(table->x & 64),
                          cz | (s16)(table->z & 64));
          }
          table->count++;
          entry++;
        }
      }
      i++;
      cell++;
      if (i < width)
        goto column;
    }
    j++;
    if (j < height)
      goto row;
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
  Task_Add(Func_02004d9c, 0x480);
  return table->count;
}
