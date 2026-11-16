/// Bobs an actor up and down, rocking its sprite.
s32 Actor_Bob(struct Actor *actor) {
  s32 step = actor->riseCounter & 63;
  struct FieldSprite *sprite = actor->sprite;

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  if (step == 0)
    actor->velocityY = toFIXED(3);
  if (step == 16)
    actor->velocityY = toFIXED(2);
  if (step == 24)
    actor->velocityY = toFIXED(3);
  sprite->rotation = Trig_Cos(step << 12) / 64;
  actor->riseCounter++;
  return -1;
}
