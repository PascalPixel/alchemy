/// Animates an actor over six frames.
void Actor_Animate(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(object, (frame >> 1) % 6);
}
