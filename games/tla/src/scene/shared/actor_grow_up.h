/// Rises beside the watched actor, expanding and shrinking over half a
/// sine wave.
void Actor_GrowUp(struct Actor *actor) {
  struct Actor *anchor = actor->watchedActor;
  s32 frame = ++actor->timer;
  FIXED amplitude;

  if (frame > 31) {
    Actor_Release(actor);
    return;
  }
  amplitude = Math_Sin(frame * (DEGtoANG(180) / 32));
  actor->scaleX = amplitude;
  actor->scaleY = amplitude;

  actor->x.fixed = anchor->x.fixed;
  actor->y.fixed += toFIXED(1);
  actor->z.fixed = anchor->z.fixed + (FIXED_ONE - amplitude) * 5 + toFIXED(8);
}
