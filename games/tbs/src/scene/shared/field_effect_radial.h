#include "types.h"
#include "actor.h"
#include "field_effect.h"
#include "vector3.h"

// The Tunnel Ruins and Venus Lighthouse.

/// Burst nine particles out from the actor in a ring (the ninth on the
/// first), flung a third further east and west, slowing as they go
/// (FieldEffect_AdvanceMotion).
static void FieldEffect_SpawnNineRadialEffects(s32 actorId) {
  struct Actor *actor;
  struct Vector3 vec;
  struct EffectOptions params;
  u32 i;
  s32 v;
  s32 x;
  s32 z;

  actor = Actor_GetById(actorId);
  params.priority = 1;
  params.palette = 7;
  params.update = FieldEffect_AdvanceMotion;
  for (i = 0; i <= 16; i += 2) {
    v = i << 12;
    vec.x = Trig_Cos(v);
    vec.y = 0;
    z = Trig_Sin(v);
    x = vec.x;
    vec.z = z;
    x += Math_Divide(x, 3);
    vec.x = x;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, x, vec.y,
                      z,
                      EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &params);
  }
}
