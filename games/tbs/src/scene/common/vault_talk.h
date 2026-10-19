// Vault and its interiors: a villager's thought read with Mind Read.
#include "types.h"
#include "field_obj.h"
#include "field_event.h"

// Show the message already set as the actor's thought: the actor stops and
// stands while the thought is shown.
static void Vault_ShowThought(s32 actorId) {
  u8 *paused = &Object_GetById(actorId)->paused;
  s32 zero = 0;

  *paused = 1;
  Engine_EventBegin();
  Engine_ActorSetAnimation(actorId, ANIM_STAND);
  Engine_EventWait(2);
  Engine_EventShowMessage(actorId, 0);
  Engine_EventEnd();
  *paused = zero;
}
