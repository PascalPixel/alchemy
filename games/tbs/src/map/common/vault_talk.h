/// @file
/// Vault and its interiors: a villager's thought read with Mind Read.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"

/// Shows the message already set as the object event's thought: the object
/// event stops and stands while the thought is shown.
static void Vault_ShowThought(s32 localId) {
  u8 *paused = &ObjectEvent_GetById(localId)->paused;
  s32 zero = 0;

  *paused = 1;
  FieldScript_Start();
  Engine_ObjectEventSetAnimation(localId, ANIMATION_STAND);
  FieldScript_WaitFrames(2);
  FieldScript_ShowMessageAndWait(localId, 0);
  FieldScript_End();
  *paused = zero;
}
