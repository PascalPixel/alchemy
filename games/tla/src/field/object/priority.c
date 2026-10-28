#include "actor_view.h"
#include "field_sprite.h"

void Animation_ApplyChildValues(struct ActorView *object, u32 value) {
  if (object != NULL && object->kind == 1)
    Animation_ApplyChildValuesToRecord(object->target.child, value);
}

void Actor_SetSpritePriorityDirect(struct ActorView *object,
                                      s32 priority) {
  struct FieldSprite *sprite;

  if (object == NULL || object->kind != 1) {
    return;
  }
  sprite = object->target.sprite;
  sprite->priority = priority;
  sprite->partPriority = priority;
  object->priorityFlags &= ~1;
}
