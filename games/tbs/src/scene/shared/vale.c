/// @file
/// Vale's storm and Vale's homes: going indoors through a door.

/// Takes the exit in the backdrop fade over 16 frames, first stopping the
/// storm's lightning while the storm rages (FLAG_VALE_STORM).
static void Vale_RequestBackdropExit(s32 exitNumber) {
  struct FieldState *state;
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Weather_StopLightning();
  }
  state = gFieldState;
  state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  state->transitionFrames = MAP_TRANSITION_FRAMES;
  Import_EventSetExitRequest(exitNumber);
}
