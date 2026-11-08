#include "types.h"
#include "actor.h"

/// Adds the velocity to the position and decays it by one eighth each
/// update.
void Func_020011b8(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce >> 3;
  object->gravity -= object->gravity >> 3;
  object->unk4c -= object->unk4c >> 3;
}
