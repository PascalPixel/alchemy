/// Animates an actor over six frames.
void Actor_Animate(struct Actor *actor) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(actor, (frame >> 1) % 6);
}
