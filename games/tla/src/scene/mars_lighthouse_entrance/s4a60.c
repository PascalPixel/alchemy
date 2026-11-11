#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "map.h"
#include "event_inline.h"

void Func_02004bc0(struct FieldActor *object, s32 value);
s32 Func_02004db8(struct FieldActor *object);

void Func_02004a60(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = (struct FieldActor *)Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02004db8(object);
  if (Event_GetFlag(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02004bc0(object, 2);
      break;
    case 1:
      Func_02004bc0(object, 4);
      break;
    case 2:
      Func_02004bc0(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02004bc0(object, 1);
      break;
    case 1:
      Func_02004bc0(object, 3);
      break;
    case 2:
      Func_02004bc0(object, 5);
      break;
    }
  }
}
