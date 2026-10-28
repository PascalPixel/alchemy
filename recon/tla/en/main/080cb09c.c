// Near miss, differ 10 of 75 (16 before the cell was written as byte
// offsets from u8 tables, as Actor_GetTriggerCellAheadOfCurrent's draft
// 080cc54c is): the native starts the 0x100000 radius (movs r0, #128)
// before it copies y, and outside the world map builds the cell table's
// offset (movs #156) in r0 where agscc takes r1; reading x before the cell
// table changes nothing.
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

extern u8 gMapBlocks[];

struct Actor *Func_080cb8e8(void);
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
    cell = gMapBlocks +
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
