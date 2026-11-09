#include "types.h"
#include "actor.h"
#include "event_inline.h"

s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_020005cc(s32 terrain, s32 x, s32 z, s32 height);

void Func_02000668(u16 *list) {
  for (; *list != 0xffff; list += 3) {
    struct Actor *object = Actor_Get(*list);
    s32 height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
  }
}
