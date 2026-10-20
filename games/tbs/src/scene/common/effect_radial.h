#include "types.h"
#include "field_obj.h"
#include "field_effect.h"
#include "vector3.h"

// The Tunnel Ruins and Venus Lighthouse.

/// Burst nine particles out from the actor in a ring (the ninth on the first),
/// flung a third further east and west, slowing as they go
/// (Effect_AdvanceMotion).
static void SceneEffect_SpawnNineRadialEffects(s32 actor) {
  struct FieldActor *object;
  struct Vector3 vec;
  struct EffectOptions params;
  u32 i;
  s32 v;
  s32 x;
  s32 z;

  object = Object_GetById(actor);
  params.priority = 1;
  params.palette = 7;
  params.update = Effect_AdvanceMotion;
  for (i = 0; i <= 16; i += 2) {
    v = i << 12;
    vec.x = Engine_MathCos(v);
    vec.y = 0;
    z = Engine_MathSin(v);
    x = vec.x;
    vec.z = z;
    x += Math_Divide(x, 3);
    vec.x = x;
    Effect_Spawn(object->x.fixed, object->y.fixed, object->z.fixed, x, vec.y, z,
                 EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY | EFFECT_USE_PALETTE |
                     1,
                 &params);
  }
}
