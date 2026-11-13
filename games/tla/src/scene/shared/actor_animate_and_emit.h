/// Animates an actor, running Func_02004634 every sixteenth frame.
void Actor_AnimateAndEmit(struct Actor *object) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(object, (frame >> 1) % 6);
  if ((gFrameCount & 15) == 0)
    Func_02004634(object);
}
