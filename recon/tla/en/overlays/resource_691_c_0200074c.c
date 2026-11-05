// Draft, differ 34 of 40 (tla-en, 2026-11-05). Clears markers on the tiles ahead of
// and behind the object, stored in the third byte of each layer-two cell.
// One pointer through the map and its cells, and the first cell's base a local.
// Left: the native holds that pointer in r0 and the object in r5 from entry, though
// it calls nothing; here the object ranks first (5 refs over 13 against 6 over 23)
// and keeps r0, the base takes r0 after it and the zero r4.
#include "types.h"
#include "call.h"
#include "ram_buffer.h"

struct MapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct MapLayers {
  u8 unk000[0x108];
  struct MapLayer layers[3];
};

struct Object0200074c {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x4f];
  u8 marked;
};

void Func_0200074c(struct Object0200074c *object) {
  void *cells;
  u8 *base;
  s32 x;
  s32 z;

  if (object->marked == 0)
    return;
  cells = Ram_HeapSlots->mapWork;
  if (cells == NULL)
    return;
  cells = &((struct MapLayers *)cells)->layers[2].cells;
  z = (object->z - object->y) >> 20;
  x = object->x >> 20;
  base = *(u8 **)cells;
  base[((x + ((z + 1) << 7)) << 2) + 2] = 0;
  (*(u8 **)cells)[((x + ((z - 1) << 7)) << 2) + 2] = 0;
  object->marked = 0;
}
