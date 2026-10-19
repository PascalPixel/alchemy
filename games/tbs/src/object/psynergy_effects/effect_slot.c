/// @file
/// Effect slots: the particles Psynergy effects move and draw.
/// Part of psynergy_effects.c, compiled only through it.

#include "../psynergy_effects_internal.h"

/// Updates one effect slot: counts its age, runs its callback (after any
/// delay), moves it and draws it.
void EffectSlot_Update(struct EffectSlot *effect) {
  if (effect->active != 0) {
    effect->age++;
    if (effect->callbackDelay != 0)
      effect->callbackDelay--;
    else if (effect->callback != 0)
      effect->callback(effect);
    // A callback may release its own slot; do not move or draw it afterward.
    if (effect->active != 0) {
      if (effect->updateMotion != 0)
        EffectSlot_UpdateMotion(effect);
      if (effect->render != 0)
        BattleFx_DrawScaledObject(effect);
    }
  }
}

/// Draws an effect slot's sprite at its screen position and scale, when it is
/// near the screen. With flag 4 the slot's z also lifts it (0x1fc pixels
/// less z).
void BattleFx_DrawScaledObject(struct EffectSlot *object) {
  struct AnimationObject *sprite;
  s32 offset;
  struct SpriteScale scale;
  struct SpritePlacement position;

  sprite = object->object;
  offset = 0;
  if (object->flags & 4)
    offset = 0x1fc0000 - object->z;
  scale.x = Iwram_MulQ16(object->scaleX, sprite->scale);
  scale.y = Iwram_MulQ16(object->scaleY, sprite->scale);
  position.x = object->x;
  position.height = offset;
  position.y = object->z + offset;
  position.groundHeight = 0;
  if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 &&
      object->z < 0xe00000)
    Object_ApplyProjectedPlacementFar(sprite, &position, &scale, 0);
}

/// Moves an effect slot toward its target, turning at most maxTurnStep a
/// frame when limitTurnStep is set and speeding up by acceleration to maxSpeed.
/// A slot that stops at its target lands on it once within 8 pixels.
void EffectSlot_UpdateMotion(struct EffectSlot *effect) {
  s32 deltaX;
  s32 deltaZ;
  s32 distance;
  s32 pixelX;
  s32 pixelZ;
  s32 speed;
  s16 angle;
  s32 heading;
  s32 facing;
  s16 turn;

  if (effect->targetX == EFFECT_NO_TARGET)
    return;
  deltaX = effect->targetX - effect->x;
  deltaZ = effect->targetZ - effect->z;
  if (effect->stopAtTarget != 0) {
    pixelX = deltaX / 0x10000;
    pixelZ = deltaZ / 0x10000;
    distance = Iwram_Sqrt(pixelX * pixelX + pixelZ * pixelZ) << 16;
    if (distance < 0x800000)
      distance = FixedSqrt(Iwram_MulQ16(deltaX, deltaX) +
                           Iwram_MulQ16(deltaZ, deltaZ));
    if (distance <= 0x80000) {
      EffectSlot_SetPosition(effect, effect->targetX, effect->targetZ);
      return;
    }
  }
  angle = ArcTan2(deltaZ, deltaX);
  if (effect->limitTurnStep != 0) {
    heading = effect->heading;
    turn = angle - heading;
    if ((turn >= 0 ? turn : -turn) >= effect->maxTurnStep) {
      if (turn < 0) {
        if (-turn > effect->maxTurnStep)
          turn = -effect->maxTurnStep;
      } else if (turn > effect->maxTurnStep) {
        turn = effect->maxTurnStep;
      }
      angle = turn + heading;
    }
  }
  facing = (u16)angle;
  effect->heading = facing;
  speed = effect->speed + effect->acceleration;
  if (speed > effect->maxSpeed)
    speed = effect->maxSpeed;
  angle = facing;
  effect->speed = speed;
  effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
  effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

/// Returns nonzero while an effect slot that stops at its target still has one
/// (it has not arrived), 0 once it has arrived or when it never stops.
u32 EffectSlot_HasPendingTarget(struct EffectSlot *effect) {
  // A plain != changes the English instructions at the same extent.
  u32 targetX;

  if (effect->stopAtTarget == 0) {
    return 0;
  }
  targetX = effect->targetX ^ (u32)EFFECT_NO_TARGET;
  return ((0u - targetX) | targetX) >> 31;
}

/// Places an effect slot at (x, z), at rest with no target.
void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z) {
  effect->targetX = EFFECT_NO_TARGET;
  effect->targetZ = EFFECT_NO_TARGET;
  effect->x = x;
  effect->z = z;
  effect->speed = 0;
}

/// Sets the animation of an effect slot's sprite.
void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode) {
  AnimationObjects_SelectAnimationFar(effect->object, mode);
}

/// Gives an effect slot its per-frame callback, starting at state 0.
void EffectSlot_SetCallback(struct EffectSlot *effect,
                            EffectCallback callback) {
  effect->callback = callback;
  effect->callbackDelay = 0;
  effect->age = 0;
  effect->state = 0;
}

/// Fills an effect slot with a new sprite of kind at (x, z): stopping at its
/// target, top speed 2, acceleration and scale 1, drawn, animation 1.
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z) {
  s32 unit;
  struct AnimationObject *object;

  Dma_Fill32(0, effect, sizeof *effect);
  object = ResourceObject_CreateFar(kind);
  effect->object = object;
  if (object != NULL)
    object->image.priority = 0;
  EffectSlot_SetPosition(effect, x, z);
  /* FAKEMATCH(temporary): 1.0 in a local set before maxSpeed takes r2 beside
     2.0, which lets sched2 load the object ahead of the three stores; as the
     constant in the chain, 12 of 74 instructions differ in tbs-en. */
  unit = 0x10000;
  effect->maxSpeed = 0x20000;
  effect->acceleration = effect->scaleY = effect->scaleX = unit;
  effect->originX = x;
  effect->originZ = z;
  // BUG: ResourceObject_CreateFar can return NULL, but this and the animation
  // setup below still require a sprite. Callers also immediately use it, so
  // guarding this store alone would not establish allocation-failure recovery.
  {
    struct AnimationObject *sprite = effect->object;
    sprite->flags = 0;
  }
  effect->stopAtTarget = 1;
  effect->limitTurnStep = 1;
  effect->updateMotion = 1;
  effect->render = 1;
  effect->active = 1;
  effect->randomValue = Random16();
  effect->flags = 4;
  EffectSlot_SetObjectMode(effect, 1);
}

/// Frees an effect slot and its sprite.
void BattleFx_ClearOwnedSlot(struct EffectSlot *slot) {
  if (slot->object)
    ResourceObject_ReleaseFar(slot->object);
  Dma_Fill32(0, slot, sizeof *slot);
}
