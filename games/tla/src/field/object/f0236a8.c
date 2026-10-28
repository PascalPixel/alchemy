#include "actor_view.h"

void Animation_SetDisplayFlag(struct ActorView *object, u32 value) {
  if (object != NULL && object->kind == 1)
    object->target.child->displayFlag = value;
}
