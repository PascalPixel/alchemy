/// @file
/// Blocks the three cells through an object's cell on layers 0 and 2 (collision code
/// 255): the column above and below it in mode 0, the row beside it otherwise.
#include "trial_road.h"
#include "map.h"

void Func_02000cf8(s32 id, s32 mode) {
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s32 index;
  struct MapCell *low;
  struct MapCell *high;

  if (object == 0)
    return;
  index = (object->x >> 20) + ((object->z >> 20) << 7);
  low = (struct MapCell *)map->layers[0].cells + index;
  high = (struct MapCell *)map->layers[2].cells + index;
  if (mode == 0) {
    low[-128].collisionCode = 255;
    low->collisionCode = 255;
    low[128].collisionCode = 255;
    high[-128].collisionCode = 255;
    high->collisionCode = 255;
    high[128].collisionCode = 255;
  } else {
    low[-1].collisionCode = 255;
    low->collisionCode = 255;
    low[1].collisionCode = 255;
    high[-1].collisionCode = 255;
    high->collisionCode = 255;
    high[1].collisionCode = 255;
  }
}
