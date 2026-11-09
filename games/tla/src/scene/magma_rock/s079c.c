/// @file
/// For each id of a list ended by 0xffff: settles the object on its ground, copies the
/// cell under it, and blocks the cells ahead of and behind it on a cell with attribute
/// bit 6.
#include "iwram_call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "map.h"
#include "text/msg_ids.h"
#include "types.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 MapCell_GetAttributes(u32 layer, s32 x, s32 z);
void Actor_SetSingleChildFlags();
void Func_020005cc(s32 layer, s32 x, s32 z, s32 height);

void Func_0200079c(u16 *ids) {
  while (*ids != 0xffff) {
    struct Actor *object = Actor_Get(*ids);
    u8 *terrainId = &object->terrainId;
    s32 height = Map_GetTerrainHeight(*terrainId, object->x, object->z);
    u8 *blocked;
    struct MapWork *map;
    struct MapCell **cells;
    struct MapCell *cell;
    s32 x, z;

    object->unk4c =
        255 & MapCell_GetAttributes(0, object->x >> 20, object->z >> 20);
    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_020005cc(*terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
    blocked = (u8 *)object + 99;
    *blocked = 0;
    if (MapCell_GetAttributes(0, object->x >> 20, object->z >> 20) & 64) {
      map = Ram_HeapSlots->mapWork;
      if (map == NULL)
        return;
      cells = &map->layers[2].cells;
      z = (object->z - object->y) >> 20;
      x = object->x >> 20;
      cell = *cells;
      cell += x + ((z + 1) << 7);
      cell->collisionCode = 255;
      cell = *cells;
      cell += x + ((z - 1) << 7);
      cell->collisionCode = 255;
      *blocked = 1;
    }
    ids++;
  }
}
