/// @file
/// Vault's interiors, before and after the thieves are caught: the Bone left
/// on the floor of the house where the dog's owner offers "leftover bones from
/// dinner".
#include "types.h"
#include "field_obj.h"
#include "field_event.h"

/// Search spot 41: take the Bone, or, with no room for it, actor 18 shakes
/// their head: "Wow, you have so many things. Aren't you a greedy lad!"
static void Vault_FindBone(void) {
  Engine_EventBegin();
  if (PartyInventory_HasSpace() == 0) {
    Engine_ActorSetAnimationAndWait(18, ANIM_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
    Engine_EventShowMessage(18, 0);
  } else {
    Engine_ItemShowFound(ITEM_BONE, 3);
    Engine_PartyGiveItem(ITEM_BONE, 0);
  }
  Engine_EventEnd();
}
