/// Sets an actor's update by mode.
void Actor_SetFlightMode(struct Actor *object, s32 mode) {
  if (mode == 0) {
    object->update = 0;
    Actor_SetSpriteColorMode(object, 0);
    Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  } else if (mode == 1) {
    object->update = (void (*)(void))Func_02003b38;
  } else {
    object->update = (void (*)(void))Actor_CycleChildValues;
    Event_PlaySound(SE_291);
  }
}
