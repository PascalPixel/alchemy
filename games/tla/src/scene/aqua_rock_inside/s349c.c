#include "../../../../games/tla/src/scene/aqua_rock_inside/local.h"

void Func_0200349c(u32 index, s32 height) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct PillarWork *work = &gData_0200536c;

  if (index <= 31) {
    struct PillarEntry *entry = &work->entries[index];

    if (entry->x != 0) {
      *(s32 *)(Ram_MapCollision + 0x1e4) = ((height / 0x80000) & 0xff) << 8;
      { u8 *ids = map->layers[0].terrainIds; s32 at = (entry->x >> 20) + (entry->z >> 20) * 128; s32 v = 121; ids[at] = v; }
      entry->height = height;
    }
  }
}
