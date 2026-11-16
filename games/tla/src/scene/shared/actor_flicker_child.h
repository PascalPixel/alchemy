/// Flickers an actor's child between 7 and 0.
void Actor_FlickerChild(struct Actor *actor) {
  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(actor, 7);
  else
    Actor_SetSpriteColorMode(actor, 0);
}
