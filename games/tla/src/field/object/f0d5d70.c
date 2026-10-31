#include "types.h"
#include "actor.h"

struct ContextAnimation {
  u8 unk00[0x1a];
  u8 flags;
};

struct ContextEffect {
  u8 unk00[5];
  u8 param;
};

void *ResourceMetadata_RegisterFar(void *, s32);
void wait(s32 frames);

void Func_080d5d70(s32 id, s32 side) {
  u32 zero;
  u8 kind;
  struct Actor *object;
  struct ContextAnimation *context;
  struct ContextEffect *effect;
  s32 mode;

  object = ObjectTable_Get(id);
  context = object->animation;
  mode = (side != 1);
  mode = 26 - mode;
  effect = ResourceMetadata_RegisterFar(context, 267);
  zero = 0;
  // Through a byte pointer: as context->flags the kind is set first.
  *&context->flags = zero;
  kind = 15;
  effect->param = kind;
  object->x = (object->x & 0xFFF00000) + 0x80000;
  object->z = (object->z & 0xFFF00000) + 0x100000;
  object->velocityX = zero;
  object->velocityZ = zero;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  Actor_SetMode(object, mode);
  wait(18);
}
