#include "islets.h"

void Func_02002d0c(s32 id) {
  /* FAKEMATCH(barrier): SPLIT_CONSTANT builds each constant as the game does, a
   * shifted byte plus an addend; as plain constants, 8 of 28 instructions
   * differ in tla-en. */
  struct Actor *object = Actor_GetById(id);

  Actor_SetSingleChildFlags(object, 0);
  object->unk5c[0] = 3;
  object->motionFlags = 0;
  object->priorityFlags |= 0x80;
  object->acceleration = SPLIT_CONSTANT(0x2600, 102);
  object->speedLimit = SPLIT_CONSTANT(0x4c00, 204);
}

void Func_02002d48(s32 id, s32 column, s32 row) {
  struct Actor *object = Actor_GetById(id);
  s32 x = (column * 32 + object->action) << 16;
  s32 z = (row * 32 + (s16)object->unk66) << 16;

  Func_02003e0c(object, x, object->y, z);
}
