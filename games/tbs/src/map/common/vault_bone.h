/// @file
/// Vault's interiors, before and after the thieves are caught: the Bone left
/// on the floor of the house where the dog's owner offers bones. They are
/// "leftover bones from dinner".
#include "types.h"
#include "object_event.h"
#include "script_layer.h"

/// Searching VAULT_INTERIORS_CELL_INN_BARREL beside the girl at the inn takes a
/// Bone after the thieves are jailed. The floor Bone remains, so the party can
/// take more. With no room for it, ACTOR_GIRL_AT_THE_INN (as both scenes name
/// her) shakes their head: "Aren't you a greedy lad!"
static void Vault_FindBone(void) {
  Engine_EventStart();
  if (PartyInventory_HasSpace() == 0) {
    Engine_ActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN,
                                    ANIMATION_SHAKE_HEAD);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
    Engine_EventShowMessage(ACTOR_GIRL_AT_THE_INN, 0);
  } else {
    Engine_ItemShowFound(ITEM_BONE, 3);
    Engine_PartyGiveItem(ITEM_BONE, 0);
  }
  Engine_EventEnd();
}
