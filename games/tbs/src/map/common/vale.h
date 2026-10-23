/// @file
/// Vale's storm and Vale's homes: going indoors through a door.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"
#include "flags.h"

/// Takes warp no in the backdrop fade over 16 frames, first stopping the
/// storm's lightning while the storm rages (FLAG_VALE_STORM).
static void Vale_RequestBackdropWarp(s32 no) {
  struct EventWork *p;
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    FieldEffect_StopLightning();
  }
  p = gFieldWork;
  p->startTransition = MAP_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  p->transitionFrames = 16;
  Engine_ScriptRequestWarp(no);
}
