// Draft, differ 18 of 45, written with goto as the bytes rebuild 0xffff on
// every pass: the native keeps the terrain byte address in r3 before it copies
// it to r8, finishes the height (asrs r5) after the two position loads, and
// loads the list entry between the halves of 0xffff; this form schedules each
// one step off.
#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
s32 Func_020005cc(s32 terrain, s32 x, s32 z, s32 height);

void Func_02000668(u16 *list) {
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the 0xffff terminator and the zero on every pass, and a
   * while loop hoists both (differ 57). */
  goto check;
loop:
  {
    struct Actor *object = Actor_GetById(*list);
    s32 height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSingleChildFlags(object, 0);
    list += 3;
    Func_020005cc(object->terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
  }
check:
  if (*list != 0xffff)
    goto loop;
}
