#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"
#include "system/vector3.h"

struct DisplayBlock {
  u8 unk000[0x18a];
  s8 mode;
  s8 pending;
  u8 unk18c[0x40];
  u8 *object;
};

void *Func_080cb8e8(void);

s32 Func_080cb144(void) {
  struct DisplayBlock *block = Ram_HeapSlots->eventWork;
  struct Actor *object = Func_080cb8e8();
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct Vector3 position;
  struct MapCell *cell;

  if (object == NULL)
    return 0;
  position.x = object->x;
  position.y = object->y;
  position.z = object->z;
  if (block->mode == 3) {
    // The block table at its fixed address: as the linked gMapBlocks it loads
    // after the cell index is summed, into r2 (4 of 69 instructions differ).
    cell =
        (struct MapCell *)Ram_MapBlocks +
        (((position.x / 0x200000) & 31) + ((position.z / 0x200000) & 31) * 32);
  } else {
    cell = map->layers[0].cells;
    cell += position.x / 0x100000 + position.z / 0x100000 * 128;
  }
  return cell->collisionCode;
}
