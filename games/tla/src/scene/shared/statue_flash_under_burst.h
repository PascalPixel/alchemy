/// The update of a statue Burst is going off at: it blinks red on the frames
/// its mask picks.
void Statue_FlashUnderBurst(struct Actor *actor) {
  if (gFrameCount & actor->flashMask)
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_RED_TINT);
  else
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_NORMAL);
}
