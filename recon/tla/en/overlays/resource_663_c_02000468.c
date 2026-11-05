// Draft, differ 6 of 40 (tla-en, 5 November). Left: order ties among the reads.
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
      struct MapCell *cell = *cells;
      s32 x = object->x >> 20;
      s32 row = (object->z - object->y) >> 20;
      s32 zero;

      { s32 at = x + ((row + 1) << 7); cell = *cells; cell += at; }
      zero = 0;
      *(u8 *)&cell->collisionCode = zero;
      {
      s32 at = x + ((row - 1) << 7);
      cell = *cells;
      cell += at;
      }
      *(u8 *)&cell->collisionCode = zero;
      object->marked = zero;
    }
  }
}
