// Draft, differ 33 of 40 (tla-en, 2026-11-05).
// Draft of overlay 663's Func_02000468, rewritten from the listing's translation.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "map.h"

struct CellFlagObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x63 - 0x14];
  u8 marked;
};

void Func_02000468(struct CellFlagObject *object) {
  struct MapWork *map;

  if (object->marked != 0) {
    map = Ram_HeapSlots->mapWork;
    if (map != NULL) {
      struct MapCell **cells = &map->layers[2].cells;
      s32 row = (object->z - object->y) >> 20;
      s32 x = object->x >> 20;
      struct MapCell *cell = &(*cells)[x + ((row + 1) << 7)];

      cell->collisionCode = 0;
      cell = &(*cells)[x + ((row - 1) << 7)];
      cell->collisionCode = 0;
      object->marked = 0;
    }
  }
}
