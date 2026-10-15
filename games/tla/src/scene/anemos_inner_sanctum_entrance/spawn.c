#include "anemos_inner_sanctum_entrance.h"

struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);

static __inline__ struct ObjectRuntime *Object_Create(s32 type, s32 x, s32 y,
                                                      s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps the argument copies, as ☀️'s
   * scene layer does. */
  return Engine_ObjectCreate(type, x, y, z);
}

struct ObjectRuntime *Func_020000d0(s32 x, s32 y, s32 z, s32 type) {
  struct ObjectRuntime *object = Object_Create(type, x, y, z);

  if (object != NULL) {
    Object_SetSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    ObjectDispatch_SetSingleChildFlags(object, 0);
    Object_SetPartAttribute(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}
