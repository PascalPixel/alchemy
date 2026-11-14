/// Flickers an actor's child between 7 and 0.
void Actor_FlickerChild(struct Actor *object) {
  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(object, 7);
  else
    Actor_SetSpriteColorMode(object, 0);
}
