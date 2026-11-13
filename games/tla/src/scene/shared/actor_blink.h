/// Blinks an actor's child between two values.
void Actor_Blink(struct Actor *object) {
  switch (gFrameCount & 7) {
  case 0:
    Actor_SetSpriteColorMode(object, 10);
    break;
  case 4:
    Actor_SetSpriteColorMode(object, 0);
    break;
  }
}
