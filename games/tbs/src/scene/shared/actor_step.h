#ifndef ALCHEMY_ACTOR_STEP_H
#define ALCHEMY_ACTOR_STEP_H

#include "types.h"
#include "actor.h"
#include "fixed_math.h"
#include "macros.h"

// The pushed-statue scenes (Bilibin, its cave, Kalay, Sol Sanctum's entrance,
// the Vault's cave, Venus Lighthouse) and those of staged_actor.h.

/// Sets `position` to one step from `actor`: `step` packs the step in x in its
/// high half and the step in z in its low half, as the push tables do.
static inline void Actor_StepFrom(struct FieldPosition *position,
                                  struct Actor *actor, s32 step) {
  SET_POSITION(position, actor->x.fixed + (step & -0x10000), actor->y.fixed,
               actor->z.fixed + px(step));
}

#endif
