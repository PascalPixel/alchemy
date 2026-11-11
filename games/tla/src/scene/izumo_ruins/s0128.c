#include "izumo_ruins.h"
#include "map.h"

void Func_02000128(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_02001508(object);
  if (Event_GetFlag(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02001428(object, 2);
      break;
    case 1:
      Func_02001428(object, 4);
      break;
    case 2:
      Func_02001428(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02001428(object, 1);
      break;
    case 1:
      Func_02001428(object, 3);
      break;
    case 2:
      Func_02001428(object, 5);
      break;
    }
  }
}
