#include "../../../../../../games/tla/src/scene/airs_rock_inside/airs_rock_inside.h"
#include "macros.h"

void Func_020017f4(struct FieldActor *object) {
  struct FieldActor *target = (struct FieldActor *)WORD_AT(object, 104);

  object->priorityFlags ^= 2 << (gFrameCount & 1);
  SET_POSITION(object, target->x, target->y + (128 << 12), target->z);
}
