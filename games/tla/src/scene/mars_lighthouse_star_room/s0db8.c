#include "mars_lighthouse_star_room.h"

void ObjectMotion_SetActionVariant(u32 id, s32 variant);
void Object_SetModeById(u32 id, s32 mode);
void Func_0200432c(s32 id, s32 x, s32 z);
void Func_0200423c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Func_0200432c_(s32 id, s32 x, s32 z) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as the scene layer of The Broken Seal does. */
  Func_0200432c(id, x, z);
}

static __inline__ void Func_0200423c_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as the scene layer of The Broken Seal does. */
  Func_0200423c(a, b, c, d, e, f);
}

void Func_02000db8(s32 id) {
  struct ObjectRuntime *object = Object_GetById(id);

  ObjectDispatch_SetSingleChildFlags(Object_GetById(id), 0);
  ObjectMotion_SetActionVariant(id, 3);
  object->motionFlags = 0;
  /* FAKEMATCH: keeps the flag store before the next address is formed (differ 2
   * without) */
  __asm__("");
  object->priorityFlags |= 2;
  object->collisionFlags = 0;
  if (id == 13) {
    Func_0200432c_(13, 175 << 18, 224 << 16);
    Object_SetModeById(13, 10);
    Func_0200423c_(39, 22, 2, 1, 43, id);
  } else {
    Func_0200432c_(14, 165 << 18, 216 << 16);
    Object_SetModeById(14, 9);
    Func_0200423c_(31, 21, 2, 2, 40, 12);
  }
}
