#include "types.h"
#include "object_runtime.h"
#include "field_sprite.h"

void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);

/// Moves the object: left of x 320 it speeds up rightward and grows, past it
/// it speeds up leftward and shrinks; its priority follows its direction and
/// its sprite spins.
void Func_02000f9c(struct ObjectRuntime *object) {
  s32 scale;

  object->y += object->gravity;
  object->x += object->bounce;
  object->z += object->unk4c;
  if ((object->x >> 19) <= 39) {
    object->bounce += 0x18000;
    /* FAKEMATCH: the barrier ends the scheduling block after the speed store,
     * so the step constant is started before scaleX is loaded; plainly it is
     * started before the store (differ 4), and a one-pass block here loads
     * scaleX first (differ 2). */
    __asm__("");
    scale = object->scaleX + 0xccc;
  } else {
    object->bounce -= 0x18000;
    scale = object->scaleX - 0x51e;
  }
  object->scaleX = scale;
  object->scaleY = scale;
  if (object->bounce > 0)
    Object_SetSpritePriority(object, 1);
  else
    Object_SetSpritePriority(object, 2);
  ((struct FieldSprite *)object->animation)->rotation += 0x1800;
}
