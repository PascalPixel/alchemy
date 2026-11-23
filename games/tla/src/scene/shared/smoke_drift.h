/// Each frame, a puff of chimney smoke drifts sideways ever faster, rises and
/// spreads, until its frames run out and it is gone.
void Smoke_Drift(struct Actor *actor) {
  actor->x.fixed += actor->timer << 8;
  actor->y.fixed += toFIXED(1) / 2;
  actor->scaleX += FIXED_ONE / 50;
  actor->scaleY += FIXED_ONE / 50;
  actor->timer += 2;
  if (--*(s32 *)&actor->watchedActor == 0)
    Actor_Release(actor);
}
