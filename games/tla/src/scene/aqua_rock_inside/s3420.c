/// @file
/// Scene code of overlay 0x67c, in ROM order, from s3420's offset up to the next object the overlay links.

#include "local.h"

void Func_02003420(s32 id, s32 kind, s32 height) {
  struct Actor *object = Actor_Get(id);
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry = &work->entries[work->count];

  if (object != 0) {
    entry->x = 0;
    entry->z = 0;
    entry->base = height << 19;
    entry->height = height << 19;
    entry->state = 2;
    entry->kind = kind;
    entry->position = &object->x;
    work->count++;
  }
}

void Func_02003460(s32 height) {
  struct PillarWork *work = &gData_0200536c;
  struct PillarEntry *entry;
  s32 i;

  *(s32 *)(Ram_MapCollision + 0x1e0) = ((height / 0x80000) & 0xff) << 8;
  entry = work->entries;
  for (i = 31; i >= 0; i--) {
    if (entry->x != 0)
      entry->height = height;
    entry++;
  }
}

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
