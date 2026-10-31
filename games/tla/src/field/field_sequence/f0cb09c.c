#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "actor.h"

struct DisplayBlock {
  u8 unk000[0x18a];
  s8 mode;
  s8 pending;
  u8 unk18c[0x40];
  u8 *object;
};

void *Func_080cb8e8(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

s32 Func_080cb09c(void) {
  struct DisplayBlock *block = Ram_HeapSlots->eventWork;
  struct Actor *object = Func_080cb8e8();
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 position[3];
  u8 *cell;

  if (object == NULL)
    return 0;
  position[0] = object->x;
  position[1] = object->y;
  position[2] = object->z;
  Vector_AddPolarOffset(0x100000, object->angle, position);
  if (block->mode == 3) {
    s32 x, z;

    x = position[0];
    z = position[2];
    cell = Ram_MapBlocks +
           ((((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)) << 2);
  } else {
    s32 x, z;

    x = position[0];
    cell = (u8 *)map->layers[0].cells;
    z = position[2];
    cell += (((x / 0x100000) + ((z / 0x100000) << 7)) << 2);
  }
  return cell[2];
}
