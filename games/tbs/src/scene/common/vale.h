/// @file
/// Vale's storm and Vale's homes: going indoors through a door.
#include "types.h"
#include "actor.h"
#include "script_layer.h"
#include "flags.h"

/// Takes exit no in the backdrop fade over 16 frames, first stopping the
/// storm's lightning while the storm rages (FLAG_VALE_STORM).
static void Vale_RequestBackdropExit(s32 no) {
  struct FieldWork *p;
  if (GameFlag_Get(FLAG_VALE_STORM) != 0) {
    Weather_StopLightning();
  }
  p = gFieldWork;
  p->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  p->transitionFrames = 16;
  Import_EventSetExitRequest(no);
}
