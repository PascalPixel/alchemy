/// @file
/// Vale's storm and Vale's homes: going indoors through a door.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"
#include "flags.h"

/// Takes exit no in the backdrop fade over 16 frames, first stopping the
/// storm's lightning while the storm rages (FLAG_VALE_STORM).
static void Vale_RequestBackdropExit(s32 no) {
  struct EventWork *p;
  if (Engine_GameFlagIsSet(FLAG_VALE_STORM) != 0) {
    FieldEffect_StopLightning();
  }
  p = gEventWork;
  p->startTransition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  p->transitionFrames = 16;
  Engine_EventRequestExit(no);
}
