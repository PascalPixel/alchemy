// Draft, differ 225 of 268, skeleton gap 14/21 (tla-en, 2026-11-01)
// Overlay 69a's copy of the function drafted in resource_698_c_020011a0.c. Now
// the plain form of the adopted copy in jupiter_lighthouse_aerie_beacon/s4ec8.c
// with none of its devices: goto loops (real for loops: differ 277 to 287, gap
// 38 or more), cx and cz, entries last so its slot is sp+8.
// Left: allocation. The native has table r8, last r10, entry r9, cell r11, cx
// r7, cz r6, with i and j on the stack, so it ranks table > last > entry > cell
// > i. Here last (9 refs over 14 insns) outranks cx and cz and takes r6, cx
// falls to r4 with caller saves (the 4 extra sp saves and reloads around the
// calls), i takes r8 and table r11. The native's last must live past the arm
// that uses r4, or it would take r4 itself, yet nothing there reads it; table
// also needs about 7 more refs to pass entry's 32. No plain C found for either.
// The search loop shares i here: its own k is what the native has (k in r2),
// but differ 259, gap 33/36, as entry then outranks table from the start.
// Tried without gain: no last, event as s32 or u8, last set before the tests
// (gap 20/25), entries set first, sourceX and sourceZ locals, cx |= before the
// call, mask locals, stores of entry after cx and cz (differ 221, gap 17/24),
// swapped i and j, cz and cx declared either way. Arms on event == last that
// keep last alive are a dead device and made it worse (differ 295). The
// adopted copies reach differ 0 only through barrier and dead devices.
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

extern struct CellTarget gData_020023a0;

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void Func_02001adc(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_02001104(void);

s32 Func_02001230(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_020023a0;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
  struct CellMapCell *rows;
  struct CellMapCell *cell;
  s32 x0, z0, width, height, i, j;
  struct CellTargetEntry *entries = entry;
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
  rows = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  rows += ((map->minZ >> 20) << 7) + (map->minX >> 20);
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
          cx = i + x0;
          entry->x = cx;
          cz = j + z0;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!GameFlag_Test(0x109)) {
            if (event == last)
              GameFlag_SetBit(entry->flag);
          } else if (GameFlag_Test(entry->flag)) {
            Func_02001adc(table->x, table->z + 1, 1, 1,
                          cx | (s16)(table->x & 64), cz | (s16)(table->z & 64));
          }
          entry++;
          table->count++;
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

    entry = entries;
    i = 0;
    if (i < table->count) {
    search:
      if (entry->x == leaderX && entry->z == leaderZ)
        table->current = i;
      entry++;
      i++;
      if (i < table->count)
        goto search;
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
  Task_Add(Func_02001104, 0x480);
  return table->count;
}
