// Draft, differ 18 of 45: overlay 692's copy of the function drafted in
// resource_691_c_02000668.c (overlay 691's Func_02000668, differ 18 there), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
s32 ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
s32 Func_020005dc(s32 terrain, s32 x, s32 z, s32 height);

void Func_02000880(u16 *list) {
  /* FAKEMATCH: the loop is written with goto so that it carries no loop notes:
   * the bytes rebuild the 0xffff terminator and the zero on every pass, and a
   * while loop hoists both (differ 57). */
  goto check;
loop:
  {
    struct ObjectRuntime *object = Object_GetById(*list);
    s32 height = Map_GetTerrainHeight(object->terrainId, object->x, object->z);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    list += 3;
    Func_020005dc(object->terrainId, object->x >> 20, object->z >> 20,
                  (height >> 19) + 4);
  }
check:
  if (*list != 0xffff)
    goto loop;
}
