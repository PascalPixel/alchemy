#include "local.h"

void Func_020013f8(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());

  if (object != 0 && (object->animationKind & 15) == 1) {
    struct FieldSprite *sprite;
    s32 priority;

    sprite = object->animation;
    priority = gMainRoutineWork.renderMode == 2 ? 2 : 1;
    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= 0xfe;
  }
}
