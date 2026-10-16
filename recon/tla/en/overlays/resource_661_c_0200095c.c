// Draft, differ 2 of 25: entered at its test (goto); the cell address as
// integer arithmetic with the base first and the index as a temporary, as
// alchemy permute found. One pair is left: the listing builds 0xffff for the
// loop test after loading the next entry; this draft before.
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "../../../../games/tla/src/scene/yampi_desert/scene.h"
#include "map.h"

void Func_0200095c(u16 *list) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 index;

  goto next;
  do {
    index = Func_020041e0(Object_GetById(*list));
    list += 3;
    *(u8 *)((u32)map->layers[2].cells + (index << 2) + 2) = 232;
  next:;
  } while (*list != 0xffff);
}
