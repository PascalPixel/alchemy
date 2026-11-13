
// Babi Lighthouse, Crossbone Isle, Tret's rooms, the Tunnel Ruins and Venus
// Lighthouse, and the main image's Carry (whirlwind.c, with
// EFFECT_SPAWN_RESIDENT), whose landing ring of particles it moves. It is
// static in each overlay and global in the main image.
#ifdef EFFECT_SPAWN_RESIDENT
#define EFFECT_ADVANCE_LINKAGE
#else
#define EFFECT_ADVANCE_LINKAGE static
#endif

/// An effect's update routine: move by its velocity, which loses an eighteenth
/// of its x and a sixteenth of its z each frame, and grow and spin at its
/// rates.
EFFECT_ADVANCE_LINKAGE void FieldEffect_AdvanceMotion(struct Actor *actor) {
  struct FieldEffect *effect = ((struct FieldEffect *)actor);

  effect->x += effect->velocity.x;
  effect->y += effect->velocity.y;
  effect->z += effect->velocity.z;
  effect->velocity.x -= effect->velocity.x / 18;
  effect->velocity.z -= effect->velocity.z / 16;
  effect->scaleX += effect->scaleRateX;
  effect->scaleY += effect->scaleRateY;
  effect->sprite->rotation += effect->spin;
}

#undef EFFECT_ADVANCE_LINKAGE
