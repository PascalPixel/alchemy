/// Blinks an actor's child between two values.
void Actor_Blink(struct Actor *actor) {
  switch (gFrameCount & 7) {
  case 0:
    Actor_SetSpriteColorMode(actor, 10);
    break;
  case 4:
    Actor_SetSpriteColorMode(actor, 0);
    break;
  }
}
