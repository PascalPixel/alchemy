#include "garoh.h"
#include "event_inline.h"
#include "map.h"

void Func_02000280(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *to = &map->layers[1];
  struct MapLayer *from = &map->layers[0];

  to->unk10 = from->unk10;
  to->unk14 = from->unk14;
  to->unk28 = from->unk28;
  to->unk2a = from->unk2a;
}
