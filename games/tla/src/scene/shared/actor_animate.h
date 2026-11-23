/// Steps an actor's sprite through the six colour swaps, SPRITE_COLOR_NORMAL to
/// SPRITE_COLOR_BRG, one every two frames: Actor_AnimateAndEmit without its
/// sparks. Nothing sets it as an update.
void Actor_CycleColorSwaps(struct Actor *actor) {
  u32 frame = gFrameCount;

  if (frame & 1)
    Actor_SetSpriteColorMode(actor, (frame >> 1) % 6);
}
