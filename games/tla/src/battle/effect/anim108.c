#include "types.h"
#include "object_runtime.h"

s32 Object_IsOutsideAnimation108(struct ObjectRuntime *object) {
  struct ObjectAnimationState *state;
  s32 value;

  if (object == 0 || object->animationKind != 1 ||
      (state = object->animation) == 0 || state->value28 == 0)
    return 1;
  value = *state->value28;
  value ^= 0x108;
  if (value != 0)
    return 1;
  return 0;
}
