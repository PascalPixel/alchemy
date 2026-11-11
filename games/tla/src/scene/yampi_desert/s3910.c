#include "scene.h"
#include "map.h"

void Func_02003910(s32 id, u32 kind, s32 flag, s32 layer) {
  struct FieldActor *object = Actor_GetById(id);
  struct MapCell *cell = Ram_HeapSlots->mapWork->layers[layer].cells;

  cell += Func_020041e0(object);
  if (GameFlag_Test(flag)) {
    cell->collisionCode = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_02003fa8(object, 2);
      break;
    case 1:
      Func_02003fa8(object, 4);
      break;
    case 2:
      Func_02003fa8(object, 6);
      break;
    }
  } else {
    cell->collisionCode = 255;
    switch (kind) {
    case 0:
      Func_02003fa8(object, 1);
      break;
    case 1:
      Func_02003fa8(object, 3);
      break;
    case 2:
      Func_02003fa8(object, 5);
      break;
    }
  }
}
