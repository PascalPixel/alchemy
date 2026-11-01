// Draft, differ 203 of 268, skeleton gap 18 (tla-en, 2026-11-01)
// Overlay 6ab's twin of overlay 698's Func_020011a0, in the goto form of
// resource_6a5_c_02004ec8.c (321 as for loops: loop.c hoists the 64).
// Left: register ranks only. The native keeps event r5, cz r6, cx r7, the table
// r8, last r10, the entry r9, the cell r11 and i at sp+16. Here last ranks second
// (9 refs over 13 insns, weighted by CFG loop depth) and takes r6, so cz is
// caller-saved in r4 at sp+8, entries moves to sp+12 and i takes r11. The native
// order fits unweighted refs (last 3, too dear to caller-save): a compiler
// question, out of scope here.
// The second loop now matches: a read of count through another type stops
// gcse's PRE (268 and gap 32 as table->count; goto, while, continue, a union
// on current and volatile current do not help).
// Tried with no gain: either order of cx and cz, of i and j, of event and last,
// block locals, u8 event, a mask local (286), cx and cz ored first (299).
// Devices tried and taken out, none reaches 0: `asm("" : : "r"(last))` before
// table->count++ gives 134, gap 6, with cx set first 130: i spills and cx, cz, the
// slots and the spill all come right, leaving r8 to r11 permuted (last, cell,
// entry, table). The native needs the table above last and the entry, but it is
// doubled by its constant REG_EQUIV (0.26). An empty asm writing table lifts the
// doubling but costs the constant (222 late, 261 early) and still ranks below
// the entry (0.56 against 0.64).
// Draft, differ 268 of 268, skeleton gap 32 (tla-en, 2026-11-01)
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
            Func_02004234(table->x, table->z + 1, 1, 1, cx | (s16)(table->x & 64),
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
    k = 0;
    if (k < table->count) do {
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = k;
      k++;
      entry++;
    } while (k < *(s16 *)&table->count); // A cast read: gcse keeps it in the loop.
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
