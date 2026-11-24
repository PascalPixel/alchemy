/// A whirlwind blows out of the stone face at `source`. Mars Lighthouse keeps
/// a copy nothing calls, its whirlwinds starting from a gust.
struct Actor *Whirlwind_Spawn(struct Actor *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct Actor *actor;

  Event_PlaySound(SE_WHIRLWIND);
  position.x =
      source->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(source->facing));
  position.z =
      source->z.fixed + Math_MulFX(toFIXED(8), Math_Sin(source->facing));
  actor = FieldActor_Create(SPRITE_WHIRLWIND, position.x, source->y.fixed,
                            position.z);
  Actor_SelectAnimation(actor, 2);
  Actor_SetSpriteFlags(actor, 0);
  actor->motionFlags = 0;
  actor->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  actor->facing = source->facing;
  actor->timer = 0;
  actor->sceneCounter = 0;
  actor->sceneByte = 0;
  actor->update = Whirlwind_Update;
  actor->speedLimit = toFIXED(3) / 2;
  options.priority = 1;
  options.palette = SPRITE_COLOR_WHITE;
  FieldEffect_Spawn(position.x, source->y.fixed, position.z - toFIXED(6), 0, 0,
                    0, EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY, &options);
  return actor;
}
