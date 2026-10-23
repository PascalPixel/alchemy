#include "types.h"
#include "object_event.h"
#include "field_effect.h"
#include "vector3.h"

// The Tunnel Ruins and Venus Lighthouse.

/// Burst nine particles out from the object event in a ring (the ninth on the
/// first), flung a third further east and west, slowing as they go
/// (FieldEffect_AdvanceMotion).
static void FieldEffect_SpawnNineRadialEffects(s32 localId) {
  struct ObjectEvent *objectEvent;
  struct Vector3 vec;
  struct EffectOptions params;
  u32 i;
  s32 v;
  s32 x;
  s32 z;

  objectEvent = ObjectEvent_GetById(localId);
  params.priority = 1;
  params.palette = 7;
  params.update = FieldEffect_AdvanceMotion;
  for (i = 0; i <= 16; i += 2) {
    v = i << 12;
    vec.x = Engine_MathCos(v);
    vec.y = 0;
    z = Engine_MathSin(v);
    x = vec.x;
    vec.z = z;
    x += Math_Divide(x, 3);
    vec.x = x;
    FieldEffect_Spawn(objectEvent->x.fixed, objectEvent->y.fixed,
                      objectEvent->z.fixed, x, vec.y, z,
                      EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE | 1,
                      &params);
  }
}
