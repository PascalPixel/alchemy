#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"

struct FieldActor *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
s32 Func_02005f64(struct FieldActor *object);
void Func_02005d5c(struct FieldActor *object, s32 mode);
#include "map.h"

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = Object_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02005f64(object);
  if (GameFlag_Test(flag)) {
    /* FAKEMATCH: the flags are read before the cell store and ORed into the
     * register that holds 2, as the native does; as one |= the 0 for the cell
     * is built before the read */
    u8 flags = object->priorityFlags;
    s32 bits;

    cell->collisionCode = 0;
    bits = 2;
    bits |= flags;
    object->priorityFlags = bits;
    switch (kind) {
    case 0:
      Func_02005d5c(object, 2);
      break;
    case 1:
      Func_02005d5c(object, 4);
      break;
    case 2:
      Func_02005d5c(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02005d5c(object, 1);
      break;
    case 1:
      Func_02005d5c(object, 3);
      break;
    case 2:
      Func_02005d5c(object, 5);
      break;
    }
  }
}
