#include "types.h"
#include "field_obj.h"
#include "field_effect.h"

// Mercury Lighthouse and its entrance: the trail of sparks the leader leaves
// while passing between floors.

// The leader's update hook while passing: each frame a small spark rises from a
// point that sweeps across the leader's chest, in palette 7 or 5 by turns.
//
// UB: update hooks return void. In tbs-en mercury_lighthouse_entrance, plain
// void differs in 21 bytes (extent 136 to 132 bytes). Keep the
// unused native result in matching builds.
#ifdef UBFIX
static void MercuryLighthouse_TrailSparks(union FieldObject *fieldObject) {
#else
static s32 MercuryLighthouse_TrailSparks(union FieldObject *fieldObject) {
#endif
  struct FieldActor *actor = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 velocityY;

  options.palette = 7;
  if ((gFrameCount & 1) == 0) {
    options.palette = 5;
  }
  options.startScaleX = 0xcccc;
  options.startScaleY = 0xcccc;
  options.priority = 0;
  velocityY = -((((u32)Engine_RandomNext() * 8) >> 16) * 0x3333);
  FieldFx_Spawn(actor->x.fixed + ((8 - (gFrameCount & 15)) << 16),
                actor->y.fixed + 0x1a0000, actor->z.fixed, 0, velocityY, 0,
                EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                    EFFECT_USE_PALETTE,
                &options);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}
