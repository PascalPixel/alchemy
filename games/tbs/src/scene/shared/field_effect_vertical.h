#include "types.h"
#include "actor.h"
#include "field_effect.h"
#include "scene_tables.h"
#include "fixed_math.h"

// Vale and its home, storm and Babi scenes, Sol Sanctum's eruption, the World
// Map and Venus Lighthouse's aerie: the pair of rising particles that
// WorldMap_CreateAttachedEffects attaches to a glowing actor, each an
// update routine that lives 32 frames.

/// Rises beside the watched actor, expanding and shrinking over half a
/// sine wave.
static void FieldEffect_AnimateVerticalPositive(struct Actor *actor) {
  struct Actor *anchor;
  s32 frame;
  s32 amplitude;

  anchor = actor->watchedActor;
  actor->timer = (actor->timer + 1);
  frame = actor->timer;

  if (frame > 31) {
    Actor_Release(actor);
    return;
  }

  amplitude = Trig_Sin(frame << 10);
  actor->scaleX = amplitude;
  actor->scaleY = amplitude;
  actor->x.fixed = anchor->x.fixed;
  actor->y.fixed += px(1);
  actor->z.fixed = anchor->z.fixed + (Q16_ONE - amplitude) * 5 + px(8);
}

/// Mirrors the rising particle vertically and passes behind the watched
/// actor.
static void FieldEffect_AnimateVerticalNegative(struct Actor *actor) {
  struct Actor *anchor;
  s32 frame;
  s32 amplitude;

  anchor = actor->watchedActor;
  actor->timer = (actor->timer + 1);
  frame = actor->timer;

  if (frame > 31) {
    Actor_Release(actor);
    return;
  }

  amplitude = Trig_Sin(frame << 10);
  actor->scaleX = amplitude;
  actor->scaleY = -amplitude;
  actor->x.fixed = anchor->x.fixed;
  actor->y.fixed += px(1);
  actor->z.fixed = anchor->z.fixed - (Q16_ONE - amplitude) * 5 + px(16);
}
