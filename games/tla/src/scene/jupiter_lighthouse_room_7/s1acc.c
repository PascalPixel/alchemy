#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "map.h"
#include "event_inline.h"

void Func_02002ad4(struct FieldActor *object, s32 value);
s32 Func_02002c1c(struct FieldActor *object);

void Func_02001acc(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = ((struct FieldActor *)Actor_Get(id));
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02002c1c(object);
  if (Event_GetFlag(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02002ad4(object, 2);
      break;
    case 1:
      Func_02002ad4(object, 4);
      break;
    case 2:
      Func_02002ad4(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02002ad4(object, 1);
      break;
    case 1:
      Func_02002ad4(object, 3);
      break;
    case 2:
      Func_02002ad4(object, 5);
      break;
    }
  }
}
