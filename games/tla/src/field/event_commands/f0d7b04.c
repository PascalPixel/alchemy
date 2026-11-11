#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_particle.h"
#include "fixed_math.h"

struct EffectVector {
  s32 x;
  s32 y;
  s32 z;
};

void *Actor_GetById(s32 id);
void Camera_WorldToScreen(struct EffectVector *position);
void BattleEffect_ClearOwnedSlot(struct FieldParticle *effect);
void Vector_AddPolarOffset(s32 magnitude, s32 angle,
                           struct EffectVector *position);

/// Field effect sprite callback of a Jupiter Djinni's light particles: each
/// circles its centre while it widens for 60 frames, circles 40, drifts toward
/// a point 20 pixels over the leader for 40, closes in for 60 and is freed. It
/// turns 1/32 turn a frame throughout, and its radius grows, then shrinks, by 1
/// pixel a frame. The slot's origin is the centre, in screen coordinates
/// (Camera_WorldToScreen).
void FieldEffect_RunAngledApproachPhases(struct FieldParticle *particle) {
  struct Actor *owner;
  struct EffectVector position;

  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  // Each age test reads age signed (ldrsh); as particle->age == 60 and so on,
  // 54 of 122 instructions differ in tla-en.
  if (particle->state == 0) {
    particle->orbitRadius += 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 1) {
    particle->orbitAngle += 1;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 2) {
    particle->orbitAngle += 1;
    position.x = owner->x;
    position.y = owner->y + 20 * Q16_ONE;
    position.z = owner->z;
    Camera_WorldToScreen(&position);
    particle->originX += (position.x - particle->originX) / 8;
    particle->originZ += (position.z - particle->originZ) / 8;
    if ((s16)particle->age == 40) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 3) {
    particle->orbitRadius -= 1;
    particle->orbitAngle += 1;
    if ((s16)particle->age == 60) {
      particle->age = 0;
      particle->state += 1;
    }
  } else if (particle->state == 4) {
    BattleEffect_ClearOwnedSlot(particle);
  }
  position.x = particle->originX;
  position.z = particle->originZ;
  Vector_AddPolarOffset(particle->orbitRadius << 16, particle->orbitAngle << 11,
                        &position);
  particle->x = position.x;
  particle->z = position.z;
}
