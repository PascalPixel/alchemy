#include "actor_view.h"

void Actor_WaitForCommandEnd(struct ActorView *object) {
  s32 cnt = 0;

  if (object->value00 != 0 &&
      ((const s32 *)object->value00)[object->value04] != 17) {
    do {
      wait(1);
      cnt++;
      if (cnt > 599)
        break;
    } while (object->value00 != 0 &&
             ((const s32 *)object->value00)[object->value04] != 17);
  }
}

void Actor_SetChildByteValue(struct ActorView *object,
                                      u32 value) {
  if (object != NULL && (object->kind & 0xf) == 1)
    object->target.child->value1a = value;
}
