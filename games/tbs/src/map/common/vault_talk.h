/// @file
/// Vault and its interiors: a villager's thought read with Mind Read.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"

/// Shows the message already set as the object event's thought: the object
/// event stops and stands while the thought is shown.
static void Vault_ShowThought(s32 objectEventId) {
  u8 *paused = &ObjectEvent_GetById(objectEventId)->paused;
  s32 zero = 0;

  *paused = 1;
  Engine_ScriptStart();
  Engine_ObjectEventSetAnimation(objectEventId, ANIMATION_STAND);
  Engine_ScriptWait(2);
  Engine_ScriptShowMessage(objectEventId, 0);
  Engine_ScriptEnd();
  *paused = zero;
}
