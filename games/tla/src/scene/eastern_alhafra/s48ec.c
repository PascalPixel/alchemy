#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "event_inline.h"

s32 Func_02004c34(struct FieldActor *object);
void Func_02004a0c(struct FieldActor *object, s32 mode);
#include "map.h"

void Func_020048ec(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02004c34(object);
  if (Event_GetFlag(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02004a0c(object, 2);
      break;
    case 1:
      Func_02004a0c(object, 4);
      break;
    case 2:
      Func_02004a0c(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02004a0c(object, 1);
      break;
    case 1:
      Func_02004a0c(object, 3);
      break;
    case 2:
      Func_02004a0c(object, 5);
      break;
    }
  }
}
