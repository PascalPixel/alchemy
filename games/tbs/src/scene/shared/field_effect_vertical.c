/// @file
/// Vale and its home, storm and Babi scenes, Sol Sanctum's eruption, the World
/// Map and Venus Lighthouse's aerie: the golden ring that rises around someone
/// using Psynergy, as the main image's event_commands_arcs.c draws it. The
/// ring is two arcs of SPRITE_GOLDEN_RINGS, its back half under the caster and
/// its front half, the same arc upside down, in front.

/// How long a ring lives, in frames, swelling and shrinking over half a sine
/// wave.
#define VERTICAL_EFFECT_FRAMES 32

/// Each half of the ring is a 32x16 sprite: OBJ Shape 1, horizontal, at OBJ
/// Size 2 (GBATEK "OBJ Attribute 0" and "OBJ Attribute 1").
#define RING_HALF_SHAPE 1
#define RING_HALF_SIZE 2

/// The ring's back half: it rises a pixel a frame on the watched actor, up to
/// 13 pixels south of it while small.
static void FieldEffect_RiseRingBackHalf(struct Actor *actor) {
  struct Actor *anchor;
  s32 frame;
  FIXED amplitude;

  anchor = actor->watchedActor;
  actor->timer = actor->timer + 1;
  frame = actor->timer;

  if (frame >= VERTICAL_EFFECT_FRAMES) {
    Actor_Release(actor);
    return;
  }

  amplitude = Trig_Sin(frame * (DEGtoANG(180) / VERTICAL_EFFECT_FRAMES));
  actor->scaleX = amplitude;
  actor->scaleY = amplitude;
  actor->x.fixed = anchor->x.fixed;
  actor->y.fixed += toFIXED(1);
  actor->z.fixed = anchor->z.fixed + (FIXED_ONE - amplitude) * 5 + toFIXED(8);
}

/// The ring's front half, the back half's arc upside down, rising with it.
static void FieldEffect_RiseRingFrontHalf(struct Actor *actor) {
  struct Actor *anchor;
  s32 frame;
  FIXED amplitude;

  anchor = actor->watchedActor;
  actor->timer = actor->timer + 1;
  frame = actor->timer;

  if (frame >= VERTICAL_EFFECT_FRAMES) {
    Actor_Release(actor);
    return;
  }

  amplitude = Trig_Sin(frame * (DEGtoANG(180) / VERTICAL_EFFECT_FRAMES));
  actor->scaleX = amplitude;
  actor->scaleY = -amplitude;
  actor->x.fixed = anchor->x.fixed;
  actor->y.fixed += toFIXED(1);
  actor->z.fixed = anchor->z.fixed - (FIXED_ONE - amplitude) * 5 + toFIXED(16);
}
