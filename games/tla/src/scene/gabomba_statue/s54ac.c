#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "event_inline.h"

struct MapLayer {
  u8 *cells;
  u8 unk04[52];
};

struct MapLayers {
  u8 unk000[0x138];
  struct MapLayer layers[4];
};

s32 Func_02005844(struct Actor *object);
void Func_02005624(struct Actor *object, s32 mode);

void Func_020054ac(s32 id, u32 mode, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  u8 *cell = ((struct MapLayers *)Ram_HeapSlots->mapWork)->layers[layer].cells;

  cell += Func_02005844(object) * 4;
  if (Event_GetFlag(flag)) {
    /* FAKEMATCH(temporary): the flags read into a local before the cell store
     * and a local holding 2, so the flags are ORed into the register that holds
     * 2; as one priorityFlags |= 2, 8 of 75 instructions differ in tla-en. */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell[2] = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (mode) {
    case 0:
      Func_02005624(object, 2);
      break;
    case 1:
      Func_02005624(object, 4);
      break;
    case 2:
      Func_02005624(object, 6);
      break;
    }
  } else {
    cell[2] = 255;
    switch (mode) {
    case 0:
      Func_02005624(object, 1);
      break;
    case 1:
      Func_02005624(object, 3);
      break;
    case 2:
      Func_02005624(object, 5);
      break;
    }
  }
}
