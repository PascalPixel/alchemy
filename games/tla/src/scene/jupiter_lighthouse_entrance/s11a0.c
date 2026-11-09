#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"

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

extern struct CellTarget gData_02002308;

void *Heap_Alloc(s32 size);
void Heap_Release(void *buffer);
s32 Resource_FindFreeEntry(void);
void Func_02001074(void);

s32 Func_020011a0(u32 kind, s32 flag, s32 flagByte, s32 x, s32 z) {
  struct CellTarget *table = &gData_02002308;
  struct CellTargetEntry *entry = table->entries;
  struct CellMapWork *map;
  struct CellMapLayer *layer;
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
  rows = layer->cells;
  x0 = (layer->originX + map->minX) >> 20;
  z0 = (layer->originZ + map->minZ) >> 20;
  width = (map->maxX - map->minX) >> 20;
  height = (map->maxZ - map->minZ) >> 20;
  rows += ((map->minZ >> 20) << 7) + (map->minX >> 20);
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
          cx = i + x0;
          entry->x = cx;
          cz = j + z0;
          entry->z = cz;
          entry->flag = flag;
          flag++;
          if (!Event_GetFlag(0x109)) {
            if (event == last)
              Event_SetFlag(entry->flag);
          } else if (Event_GetFlag(entry->flag)) {
            s32 sourceX = table->x;
            s32 sourceZ = table->z;

            cx |= (s16)(table->x & 64);
            cz |= (s16)(table->z & 64);
            MapGrid_CopyCells(sourceX, sourceZ + 1, 1, 1, cx, cz);
          }
          entry++;
          table->count++;
          /* FAKEMATCH(barrier): an empty asm on the stored count; as no asm, 2
           * of 268 instructions differ in tla-en. */
          asm("" : "=m"(table->count) : "m"(table->count) : "r3");
          /* FAKEMATCH(barrier): an empty asm on the word over the count; as no
           * asm, 2 of 268 instructions differ in tla-en. */
          asm("" : "=m"(*(s32 *)&table->z));
        }
      }
    columnNext:
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no
       * asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      i++;
      cell++;
      if (i < width)
        goto column;
    }
  rowNext:
    j++;
    if (j < height)
      goto row;
  }
  if (Event_GetFlag(0x109)) {
    struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;
    s32 k;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
    leaderRow:
      do {
        if (entry->x == leaderX && entry->z == leaderZ)
          table->current = k;
      leaderNext:
        k++;
        entry++;
        // A cast read: gcse keeps it in the loop.
      } while (k < *(s16 *)&table->count);
    }
  }
  buffer = Heap_Alloc(0x100);
  {
    u32 *word = buffer;
    s32 count = 63;

  fill:
    *word++ = 0x11111111;
  fillNext:
    if (--count >= 0)
      goto fill;
  }
  table->tile = Resource_FindFreeEntry();
  Vram_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_AddFrame(Func_02001074, 0x480);
  return table->count;
}
