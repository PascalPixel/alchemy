#include "types.h"
#include "field_particle.h"
#include "field_sprite.h"
#include "resource.h"
#include "dma.h"
#include "fixed_math.h"

void FieldParticle_SetPosition(struct FieldParticle *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

void Animation_ApplyChildArgumentFar(void *, s32);

void FieldParticle_SetActorMode(struct FieldParticle *effect, s32 mode) {
  Animation_ApplyChildArgumentFar(effect->object, mode);
}

void FieldParticle_SetCallback(struct FieldParticle *effect,
                            EffectCallback callback) {
  effect->callback = callback;
  effect->callbackDelay = 0;
  /* FAKEMATCH: a one-pass block loads the state's zero before the halfword zero
   * is built; stored straight the two rows trade places (differ 2). */
  do {
    effect->age = 0;
    effect->state = 0;
  } while (0);
}

/// Starts slot effect as a particle of resource object kind at (x, z): clears
/// it, creates its sprite in front, and sets it moving at up to 2 pixels a
/// frame toward its target at full size, turning a limited step at a time and
/// stopping on arrival, with a random value and z lifting the sprite.
void FieldParticle_Initialize(struct FieldParticle *effect, s32 kind, s32 x, s32 z) {
  s32 unit;
  struct FieldSprite *object;
  volatile u32 zero;

  Dma_Fill32Cell(zero, 0, effect, sizeof *effect);
  object = (struct FieldSprite *)ResourceObject_CreateFar(kind);
  effect->object = object;
  if (object != NULL)
    object->priority = 0;
  FieldParticle_SetPosition(effect, x, z);
  unit = 0x10000;
  effect->maxSpeed = 0x20000;
  effect->acceleration = effect->scaleY = effect->scaleX = unit;
  effect->originX = x;
  effect->originZ = z;
  // The sprite flags (nextFrame), through a byte pointer: as a field of
  // struct FieldSprite the origin stores move after it.
  ((s8 *)effect->object)[26] = 0;
  effect->stopAtTarget = 1;
  effect->limitTurnStep = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  FieldParticle_SetActorMode(effect, 1);
}
