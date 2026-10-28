#include "local.h"

void Func_020013f8(void) {
  struct Actor *object = Actor_GetById(Func_02002a68());

  if (object != 0 && (object->animationKind & 15) == 1) {
    struct MainRoutineWork *party = &gMainRoutineWork;
    struct FieldSprite *sprite;
    s32 priority;

    asm volatile("" : "+r"(party));
    sprite = object->animation;
    priority = party->renderMode == 2 ? 2 : 1;
    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= 0xfe;
  }
}
