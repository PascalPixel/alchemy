/// A whirlwind blows out of the stone face at `source`.
struct Actor *Whirlwind_Spawn(struct Actor *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct Actor *object;

  Event_PlaySound(SE_WHIRLWIND);
  position.x =
      source->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(source->facing));
  position.z =
      source->z.fixed + Math_MulFX(toFIXED(8), Math_Sin(source->facing));
  object = FieldActor_Create(SPRITE_WHIRLWIND, position.x, source->y.fixed,
                             position.z);
  Actor_SelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  object->facing = source->facing;
  object->timer = 0;
  object->riseCounter = 0;
  object->riseEnabled = 0;
  object->update = Whirlwind_Update;
  object->speedLimit = toFIXED(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y.fixed, position.z - toFIXED(6), 0, 0,
                    0, EFFECT_USE_PALETTE | EFFECT_USE_PRIORITY, &options);
  return object;
}
