// Under -mthumb-slow-loads (2026-10-26): differ 6 of 69 in tla-en, instruction
// order and register choice. No empty asm before any one statement lowers it.
// Near miss, differ 6: the native starts the mode offset (movs r2, #197) before
// it loads z, and loads the map block table into r1 before the cell index is
// summed; here z is loaded first and the table comes after the sum, into r2.
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

extern struct MapCell gMapBlocks[];

struct Actor *Func_080cb8e8(void);

s32 Func_080cb144(void) {
  struct DisplayBlock *block = Ram_HeapSlots->eventWork;
  struct Actor *object = Func_080cb8e8();
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 position[3];
  struct MapCell *cell;
  s32 x;
  s32 z;

  if (object == NULL)
    return 0;
  position[0] = x = object->x;
  position[1] = object->y;
  position[2] = z = object->z;
  if (block->mode == 3) {
    cell = &gMapBlocks[((x / 0x200000) & 31) + ((z / 0x200000) & 31) * 32];
  } else {
    cell = map->layers[0].cells;
    cell += x / 0x100000 + z / 0x100000 * 128;
  }
  return cell->collisionCode;
}
