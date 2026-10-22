#include "types.h"
#include "field_obj.h"
#include "field_effect.h"
#include "field_scene.h"
#include "fixed_math.h"

// Vale and its home, storm and Babi scenes, Sol Sanctum's eruption, the world
// map and Venus Lighthouse's aerie: the pair of rising particles that
// WorldMap_CreateLinkedEffects attaches to a glowing actor, each an update
// hook that lives 32 frames.

/// Rises beside the linked actor, expanding and shrinking over half a sine
/// wave.
static void
FieldEffect_AnimateVerticalPositive(union FieldObject *fieldObject) {
  struct FieldActor *self = &fieldObject->actor;
  struct FieldActor *anchor;
  s32 frame;
  s32 amplitude;

  anchor = self->linkedObject;
  self->timer = (self->timer + 1);
  frame = self->timer;

  if (frame > 31) {
    Engine_ObjectDispatchRelease(self);
    return;
  }

  amplitude = Engine_MathSin(frame << 10);
  self->scaleX = amplitude;
  self->scaleY = amplitude;
  self->x.fixed = anchor->x.fixed;
  self->y.fixed += PIXELS(1);
  self->z.fixed = anchor->z.fixed + (Q16_ONE - amplitude) * 5 + PIXELS(8);
}

/// Mirrors the rising particle vertically and passes behind the linked actor.
static void
FieldEffect_AnimateVerticalNegative(union FieldObject *fieldObject) {
  struct FieldActor *self = &fieldObject->actor;
  struct FieldActor *anchor;
  s32 frame;
  s32 amplitude;

  anchor = self->linkedObject;
  self->timer = (self->timer + 1);
  frame = self->timer;

  if (frame > 31) {
    Engine_ObjectDispatchRelease(self);
    return;
  }

  amplitude = Engine_MathSin(frame << 10);
  self->scaleX = amplitude;
  self->scaleY = -amplitude;
  self->x.fixed = anchor->x.fixed;
  self->y.fixed += PIXELS(1);
  self->z.fixed = anchor->z.fixed - (Q16_ONE - amplitude) * 5 + PIXELS(16);
}
