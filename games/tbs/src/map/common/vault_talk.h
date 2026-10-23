/// @file
/// Vault and its interiors: a villager's thought read with Mind Read.
#include "types.h"
#include "object_event.h"
#include "script_layer.h"

/// Shows the message already set as the actor's thought: the actor stops and
/// stands while the thought is shown.
static void Vault_ShowThought(s32 actorId) {
  u8 *paused = &Object_GetById(actorId)->paused;
  s32 zero = 0;

  *paused = 1;
  Engine_EventStart();
  Engine_ActorSetAnimation(actorId, ANIMATION_STAND);
  Engine_EventWait(2);
  Engine_EventShowMessage(actorId, 0);
  Engine_EventEnd();
  *paused = zero;
}
