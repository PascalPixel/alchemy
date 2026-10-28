#include "types.h"
#include "effect_slot.h"

/// Reports an unfinished target only for effects that stop on arrival.
/// A plain != adds two bytes to the English function.
u32 FieldParticle_HasPendingTarget(struct FieldParticle *effect) {
  u32 value;

  if (effect->stopAtTarget == 0) {
    return 0;
  }
  value = (u32)effect->targetX ^ (u32)EFFECT_NO_TARGET;
  return ((0u - value) | value) >> 31;
}
