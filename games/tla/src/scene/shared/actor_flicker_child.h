/// An actor's update that flashes it white, two frames in every four: the
/// Water Stone or the Aqua Stone as Douse hits it.
void Actor_FlickerChild(struct Actor *actor) {
  if (gFrameCount & 2)
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_WHITE);
  else
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
}
