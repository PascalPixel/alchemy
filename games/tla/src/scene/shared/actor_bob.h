/// Bobs an actor up and down, rocking its sprite.
s32 Actor_Bob(struct Actor *object) {
  s32 step = object->riseCounter & 63;
  struct FieldSprite *sprite = object->sprite;

  object->motionFlags = 3;
  if (step == 0)
    object->velocityY = PIXELS(3);
  if (step == 16)
    object->velocityY = PIXELS(2);
  if (step == 24)
    object->velocityY = PIXELS(3);
  sprite->rotation = Trig_Cos(step << 12) / 64;
  object->riseCounter++;
  return -1;
}
