#include "actor_view.h"

void Animation_SetStateFlags(struct ActorView *object, u32 value) {
  if (object != NULL && object->kind == 1)
    object->target.child->stateFlags = value;
}
