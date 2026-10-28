/// @file
/// Vault's interiors, before and after the thieves are caught: the Bone left
/// on the floor of the house where the dog's owner offers bones. They are
/// "leftover bones from dinner".
#include "types.h"
#include "actor.h"
#include "event.h"

/// Searching CELL_BEHAVIOR_LOG_ROOMS_A_INN_BARREL beside the girl at the inn
/// takes a Bone after the thieves are jailed. The floor Bone remains, so the
/// party can take more. With no room for it, ACTOR_GIRL_AT_THE_INN (as both
/// scenes name her) shakes their head: "Aren't you a greedy lad!"
static void Vault_FindBone(void) {
  Event_Start();
  if (PartyInventory_HasSpace() == 0) {
    Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_SetMessageId(MsgVaultWowHaveManyThingsArent);
    Event_ShowMessageAndWait(ACTOR_GIRL_AT_THE_INN, 0);
  } else {
    ItemDisplay_ShowItem(ITEM_BONE, 3);
    PartyInventory_GiveItem(ITEM_BONE, 0);
  }
  Event_End();
}
