/// @file
/// Vault and its interiors: a villager's thought read with Mind Read.
#include "types.h"
#include "actor.h"
#include "script_layer.h"

/// Shows the message already set as the actor's thought: the
/// actor stops and stands while the thought is shown.
static void Vault_ShowThought(s32 actorId) {
  u8 *paused = &Actor_GetById(actorId)->paused;
  s32 zero = 0;

  *paused = 1;
  Event_Start();
  Import_ActorSetAnimation(actorId, ANIM_STAND);
  Event_WaitFrames(2);
  Event_ShowMessageAndWait(actorId, 0);
  Event_End();
  *paused = zero;
}
