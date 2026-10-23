/// @file
/// Vault's interiors, before and after the thieves are caught: the Bone left
/// on the floor of the house where the dog's owner offers bones. They are
/// "leftover bones from dinner".
#include "types.h"
#include "object_event.h"
#include "script_layer.h"

/// Searching VAULT_INTERIORS_CELL_INN_BARREL beside the girl at the inn takes a
/// Bone after the thieves are jailed. The floor Bone remains, so the party can
/// take more. With no room for it, LOCALID_GIRL_AT_THE_INN (as both maps name
/// her) shakes their head: "Aren't you a greedy lad!"
static void Vault_FindBone(void) {
  FieldScript_Start();
  if (PartyInventory_HasSpace() == 0) {
    ObjectMotion_SetAnimationAndWait(LOCALID_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
    FieldScript_WaitFrames(20);
    FieldScript_SetMessageId(MsgVaultWowHaveManyThingsArent);
    FieldScript_ShowMessageAndWait(LOCALID_GIRL_AT_THE_INN, 0);
  } else {
    FieldEffect_StartEffectObject22(ITEM_BONE, 3);
    PartyInventory_GiveItem(ITEM_BONE, 0);
  }
  FieldScript_End();
}
