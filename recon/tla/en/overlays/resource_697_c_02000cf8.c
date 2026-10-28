// Draft, differ 51 of 57: blocks the three cells through an object's cell on
// layers 0 and 2 (byte 2 set to 255): the column above and below it in mode
// 0, the row beside it otherwise.
#include "../../../../games/tla/src/scene/trial_road/trial_road.h"

void Func_02000cf8(s32 id, s32 mode) {
  struct TrialRoadMapWork *map = Ram_HeapSlots->mapWork;
  struct Actor *object = Actor_GetById(id);
  s32 index;
  u8 *low;
  u8 *high;

  if (object == 0)
    return;
  index = (object->x >> 20) + ((object->z >> 20) << 7);
  low = map->layers[0].cells + index * 4;
  high = map->layers[2].cells + index * 4;
  if (mode == 0) {
    low[-0x200 + 2] = 0xff;
    low[2] = 0xff;
    low[0x200 + 2] = 0xff;
    high[-0x200 + 2] = 0xff;
    high[2] = 0xff;
    high[0x200 + 2] = 0xff;
  } else {
    low[-4 + 2] = 0xff;
    low[2] = 0xff;
    low[4 + 2] = 0xff;
    high[-4 + 2] = 0xff;
    high[2] = 0xff;
    high[4 + 2] = 0xff;
  }
}
