#include "types.h"
#include "actor.h"
#include "fixed_math.h"

/// Raises the object's scale in sixteenths to full size and commits its
/// position.
void BattleEffect_SnapScaleToFull(struct Actor *object) {
  s32 next;
  s32 scale;

  if (object != NULL) {
    scale = object->scaleX;
    if (scale <= 0xffff) {
      do {
        next = scale + Q16_ONE / 16;
        scale = next;
      } while (next <= 0xffff);
      object->scaleX = next;
      object->scaleY = next;
    }
    Actor_CommitPosition(object);
  }
}
