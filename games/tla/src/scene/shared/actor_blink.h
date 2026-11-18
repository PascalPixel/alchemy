/// Blinks an actor red, four frames in every eight: the update of a leader a
/// flame or a fireball has hurt.
void Actor_Blink(struct Actor *actor) {
  switch (gFrameCount & 7) {
  case 0:
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_RED_TINT);
    break;
  case 4:
    Actor_SetSpriteColorMode(actor, SPRITE_COLOR_NORMAL);
    break;
  }
}
