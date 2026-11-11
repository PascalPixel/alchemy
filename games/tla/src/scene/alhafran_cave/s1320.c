#include "local.h"

void Func_02001320(s32 id, u32 kind, s32 flag, s32 layer) {
  struct Actor *object = Actor_Get(id);
  struct AlhafranCaveMapCell *cell =
      ((struct AlhafranCaveMapWork *)Ram_HeapSlots->mapWork)
          ->layers[layer]
          .cells;

  cell += Func_02001588();
  if (Event_GetFlag(flag)) {
    cell->state = 0;
    object->priorityFlags |= 2;
    switch (kind) {
    case 0:
      Func_020013f0(object, 2);
      break;
    case 1:
      Func_020013f0(object, 4);
      break;
    case 2:
      Func_020013f0(object, 6);
      break;
    }
  } else {
    cell->state = 255;
    switch (kind) {
    case 0:
      Func_020013f0(object, 1);
      break;
    case 1:
      Func_020013f0(object, 3);
      break;
    case 2:
      Func_020013f0(object, 5);
      break;
    }
  }
}
