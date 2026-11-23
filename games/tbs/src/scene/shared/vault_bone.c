/// @file
/// Vault's inn, before and after the thieves are caught: the Bone on the barrel
/// beside the girl at the inn, who offers the party "leftover bones from
/// dinner" for the dog in town.

/// Searching the barrel (CELL_BEHAVIOR_LOG_ROOMS_A_INN_BARREL) takes the Bone,
/// unless the party's bags are full.
static void Vault_FindBone(void) {
  Event_Start();
  if (PartyInventory_HasSpace() == 0) {
    Actor_SetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
    Event_Wait(20);
    Event_SetMessageId(MsgVaultWowHaveManyThingsArent);
    // "Wow, you have so many things. Aren't you a greedy lad!"
    Event_ShowMessageAndWait(ACTOR_GIRL_AT_THE_INN, 0);
  } else {
    ItemDisplay_ShowItem(ITEM_BONE, 3);
    PartyInventory_GiveItem(ITEM_BONE, 0);
  }
  Event_End();
}
