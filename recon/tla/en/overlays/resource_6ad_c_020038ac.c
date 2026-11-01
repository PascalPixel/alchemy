// Matches: differ 0 (tla-en, 2026-11-01)
// Overlay 6ad's Func_020038ac, the twin of overlay 698's Func_020011a0. Fills
// the cell table from the map's cells whose event byte is the given kind or
// the next one, restores the copied cells of those already flagged, finds the
// leader's entry and loads the sprite's tiles for Func_02003780.
// Matched only through ten tagged devices, which all stand in for one thing.
// The native's allocation fits refs not weighted by loop depth: event r5, cz r6,
// cx r7, then table r8, last r10, entry r9, cell r11 and i on the stack. Flow
// weights refs by natural-loop depth, so a plain build ranks last and i above
// the table. That weight is skipped under optimize_size, or for loops flow
// cannot see as natural. Each always-false entry into a loop's tail (dead) makes
// that loop irreducible while flow runs; combine folds the test and the stub
// goes before allocation. A compiler question for Pascal (K1, K3): if the
// compiler ever explains these ranks, the four entries go and the six barriers
// should go with them, leaving the 698 family's plain form.
// The barriers exist because the devices leave sched2 one tie: the native has
// count's store before entry's constant, which fits a block-ending jump keeping
// the store early. The two table asms give the store and the add more forward
// dependents. They cost the table refs, so the four empty asms lengthen its life
// to keep it below cz.
// The native's plain forms: rows set then added to (reload inherits r5 and its
// first store dies), sourceX and sourceZ read before the masks (read later they
// merge with the masks' ldrh), cx masked before cz (cx then outlives cz, so cz
// ranks first), and entry++ before count (reload's turn gives r5, r0, r1).
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
    /* FAKEMATCH(dead): an always-false entry into the row loop's tail; as no entry, 73 of 268 instructions differ in tla-en. */
    if (!((height << 1) & 1))
      goto row;
    j = height;
    goto rowNext;
  row:
    cell = rows + (j << 7);
    i = 0;
    if (i < width) {
      /* FAKEMATCH(dead): an always-false entry into the column loop's tail; as no entry, 85 of 268 instructions differ in tla-en. */
      if (!((width << 1) & 1))
        goto column;
      i = width;
      goto columnNext;
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
            s32 sourceX = table->x;
            s32 sourceZ = table->z;

            cx |= (s16)(table->x & 64);
            cz |= (s16)(table->z & 64);
            Func_020054e0(sourceX, sourceZ + 1, 1, 1, cx, cz);
          }
          entry++;
          table->count++;
          /* FAKEMATCH(barrier): an empty asm on the stored count; as no asm, 2 of 268 instructions differ in tla-en. */
          asm("" : "=m"(table->count) : "m"(table->count) : "r3");
          /* FAKEMATCH(barrier): an empty asm on the word over the count; as no asm, 2 of 268 instructions differ in tla-en. */
          asm("" : "=m"(*(s32 *)&table->z));
        }
      }
    columnNext:
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no asm, 224 of 268 instructions differ in tla-en. */
      asm("");
      /* FAKEMATCH(barrier): an empty asm lengthening the table's life; as no asm, 224 of 268 instructions differ in tla-en. */
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
  if (GameFlag_Test(0x109)) {
    struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
    s32 leaderX = leader->x >> 20;
    s32 leaderZ = leader->z >> 20;
    s32 k;

    entry = table->entries;
    k = 0;
    if (k < table->count) {
      /* FAKEMATCH(dead): an always-false entry into the leader loop's tail; as no entry, 236 of 268 instructions differ in tla-en. */
      if (!((leaderX << 1) & 1))
        goto leaderRow;
      k = leaderX;
      goto leaderNext;
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

    /* FAKEMATCH(dead): an always-false entry into the fill loop's tail; as no entry, 10 of 268 instructions differ in tla-en. */
    if (!(((s32)buffer << 1) & 1))
      goto fill;
    count = (s32)buffer;
    goto fillNext;
  fill:
    *word++ = 0x11111111;
  fillNext:
    if (--count >= 0)
      goto fill;
  }
  table->tile = Resource_FindFreeEntry();
  VramCache_Load(table->tile, 0x100, buffer);
  Heap_Release(buffer);
  Task_Add(Func_02003780, 0x480);
  return table->count;
}
