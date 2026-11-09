#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/dma.h"

struct Data02002aa0Entry {
  s32 x;
  s32 y;
  s32 z;
  s32 rest;
  u16 kind;
  u16 state;
  u32 oam[3];
};

struct Data02002aa0 {
  u16 block;
  u16 count;
  struct Data02002aa0Entry records[128];
};

extern struct Data02002aa0 gData_02002aa0;
void Func_02001c3c(u32 *oam, s32 depth);

#define COPY_PICTURE(from, to)                                                 \
  Dma_Set((void *)(from), (void *)(offset + (to)), DMA_COPY_WORDS(16), REG_DMA3)

void Func_020015f8(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  // viewZ read through a pointer to viewX, its neighbour; as map->viewZ, 30
  // of 189 instructions differ in tla-en.
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  // DEAD: viewY is subtracted from both height and z and cancels in dy; the
  // source keeps its load and a store to the stack that nothing reads.
  s32 viewY = map->origin[1];
  struct Data02002aa0 *table = &gData_02002aa0;
  struct VramBlockCacheEntry *entry = &gResourceTableEntries[table->block];
  u32 tile = entry->offset >> 5;
  u32 offset = entry->offset;
  struct Data02002aa0Entry *record;
  s32 i;

  COPY_PICTURE(0x0600f000, 0x06010000);
  COPY_PICTURE(0x0600f400, 0x06010040);
  COPY_PICTURE(0x0600f040, 0x06010080);
  COPY_PICTURE(0x0600f440, 0x060100c0);
  COPY_PICTURE(0x0600f080, 0x06010100);
  COPY_PICTURE(0x0600f480, 0x06010140);
  COPY_PICTURE(0x0600f080, 0x06010180);
  COPY_PICTURE(0x0600f480, 0x060101c0);
  record = table->records;
  for (i = 127; i >= 0; i--, record++) {
    s32 dx, dy, height, z, palette, flip, kind;
    s32 x, y, left, top, picture;
    u32 clear;
    u32 *attr;

    if (record->state == 0)
      continue;
    dx = record->x - viewX;
    height = record->y - viewY;
    z = record->z - viewZ - viewY;
    dy = z - height;
    if (record->y == record->rest && record->state != 2)
      palette = 0xec00;
    else
      palette = 0xe800;
    y = dy >> 16;
    clear = 0;
    x = dx >> 16;
    left = x - 8;
    top = y - 8;
    if ((u32)(x + 7) > 334)
      continue;
    if ((u32)(y + 39) > 286)
      continue;
    left &= 0x1ff;
    /* FAKEMATCH(dead): a self-update of Ram_HeapSlots->mapWork after left's
     * mask; as nothing, 82 of 189 instructions differ in tla-en. */
    Ram_HeapSlots->mapWork += 0;
    top &= 255;
    attr = record->oam;
    kind = record->kind;
    picture = kind * 4;
    flip = 0;
    if (kind == 2)
      flip = 0x10000000;
    *attr++ = clear;
    *attr++ = top | (left << 16) | flip | 0x40000400;
    *attr = palette | (tile + picture);
    Func_02001c3c(record->oam, 0);
  }
}
