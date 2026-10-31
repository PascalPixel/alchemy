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

void Func_080d7b04(struct FieldParticle *slot) {
  struct Actor *owner;
  struct EffectVector position;

  owner = Actor_GetById(gMainRoutineWork.currentOwner);
  // Each age test reads age signed (ldrsh); as slot->age == 60 and so on, 54
  // of 122 instructions differ in tla-en.
  if (slot->state == 0) {
    slot->orbitRadius += 1;
    slot->orbitAngle += 1;
    if ((s16)slot->age == 60) {
      slot->age = 0;
      slot->state += 1;
    }
  } else if (slot->state == 1) {
    slot->orbitAngle += 1;
    if ((s16)slot->age == 40) {
      slot->age = 0;
      slot->state += 1;
    }
  } else if (slot->state == 2) {
    slot->orbitAngle += 1;
    position.x = owner->x;
    position.y = owner->y + 20 * Q16_ONE;
    position.z = owner->z;
    Camera_WorldToScreen(&position);
    slot->originX += (position.x - slot->originX) / 8;
    slot->originZ += (position.z - slot->originZ) / 8;
    if ((s16)slot->age == 40) {
      slot->age = 0;
      slot->state += 1;
    }
  } else if (slot->state == 3) {
    slot->orbitRadius -= 1;
    slot->orbitAngle += 1;
    if ((s16)slot->age == 60) {
      slot->age = 0;
      slot->state += 1;
    }
  } else if (slot->state == 4) {
    BattleEffect_ClearOwnedSlot(slot);
  }
  position.x = slot->originX;
  position.z = slot->originZ;
  Vector_AddPolarOffset(slot->orbitRadius << 16, slot->orbitAngle << 11,
                        &position);
  slot->x = position.x;
  slot->z = position.z;
}
