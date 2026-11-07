#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "map.h"

struct Unk_02000468 {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x63 - 0x14];
  u8 unk63;
};

void Func_02000468(struct Unk_02000468 *unk) {
  struct MapWork *map;

  if (unk->unk63 != 0) {
    map = Ram_HeapSlots->mapWork;
    if (map != NULL) {
      struct MapCell **cells = &map->layers[2].cells;
      /* FAKEMATCH(dead): cell's initial value, overwritten unread; as a plain
         declaration, 14 of 40 instructions differ in tla-en. */
      struct MapCell *cell = *cells;
      s32 x = unk->x >> 20;
      s32 row = (unk->z - unk->y) >> 20;

      cell = *cells;
      cell += x + ((row + 1) << 7);
      cell->collisionCode = 0;
      cell = *cells;
      cell += x + ((row - 1) << 7);
      cell->collisionCode = 0;
      unk->unk63 = 0;
    }
  }
}
