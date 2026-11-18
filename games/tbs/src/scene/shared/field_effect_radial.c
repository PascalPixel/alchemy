/// @file
/// The Tunnel Ruins and Venus Lighthouse: the ring of dust a carved stone or a
/// pillar raises as it lands on a stack.

/// Bursts nine white puffs of dust out from the actor in a ring, one each
/// eighth of a turn with the ninth on the first, flung a third further east
/// and west, slowing as they go (FieldEffect_AdvanceMotion).
static void FieldEffect_SpawnNineRadialEffects(s32 actorId) {
  struct Actor *actor;
  struct Vector3 velocity;
  struct EffectOptions options;
  u32 sixteenth;
  s32 angle;
  s32 x;
  FIXED z;

  actor = Actor_GetById(actorId);
  options.priority = 1;
  options.palette = SPRITE_COLOR_WHITE;
  options.update = FieldEffect_AdvanceMotion;
  for (sixteenth = 0; sixteenth <= TRIG_TURN / FACING_STEP; sixteenth += 2) {
    angle = sixteenth * FACING_STEP;
    velocity.x = Trig_Cos(angle);
    velocity.y = 0;
    z = Trig_Sin(angle);
    x = velocity.x;
    velocity.z = z;
    x += Math_Divide(x, 3);
    velocity.x = x;
    FieldEffect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, x,
                      velocity.y, z,
                      EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE | EFFECT_SCRIPT_MEDIUM,
                      &options);
  }
}
