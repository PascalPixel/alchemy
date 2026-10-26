// 26 October 2026, under -mthumb-slow-loads: differ 4 of 40: the native issues
// a load one slot later after a store, where alchemy-gcc issues it at once;
// alchemy permute (5 to 6 minutes) and an empty asm or one-pass block after any
// one statement do not move it.
// Near miss, differ 4 of 40, order only: the native builds the priority
// mask (movs r3, #3) before it loads the sprite and masks before it loads
// the attribute byte; the draft loads the sprite first. A direct
// object->animation cast, a one-pass block and a masked temporary were
// tried (differ 4, 4 and 15). 2026-10-19: the mask pinned to r3 (7), the
// masked priority behind an empty asm (3: the store masks again), the
// priority or the sprite pointer behind one (4 and 10).
#include "../../../../games/tla/src/scene/kandorean_temple/local.h"

void Func_02000930(struct ObjectRuntime *object, s32 priority) {
  struct FieldSprite *sprite;

  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->priorityFlags = 2;
  sprite = object->animation;
  sprite->priority = priority;
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_02005130(object, 2);
  Func_02005138(object, gData_02005494);
  Object_SetPartAttribute(object, 9);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
