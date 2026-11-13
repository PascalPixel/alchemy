#include "types.h"
#include "actor.h"
#include "field_effect.h"

// Mercury Lighthouse and its entrance: the trail of sparks the leader leaves
// while passing between floors.

// The leader's update routine while passing: each frame a small spark rises
// from a point that sweeps across the leader's chest, in palette 7 or 5 by
// turns.
static UNREAD_RESULT MercuryLighthouse_TrailSparks(struct Actor *actor) {
  struct EffectOptions options;
  s32 velocityY;

  options.palette = 7;
  if ((gFrameCount & 1) == 0) {
    options.palette = 5;
  }
  options.startScaleX = 4 * Q16_ONE / 5;
  options.startScaleY = 4 * Q16_ONE / 5;
  options.priority = 0;
  velocityY = -((((u32)Import_RandomNext() * 8) >> 16) * (px(1) / 5));
  FieldEffect_Spawn(actor->x.fixed + px(8 - (gFrameCount & 15)),
                    actor->y.fixed + px(26), actor->z.fixed, 0, velocityY, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                        EFFECT_USE_PALETTE,
                    &options);
  RETURN_UNREAD(0);
}
