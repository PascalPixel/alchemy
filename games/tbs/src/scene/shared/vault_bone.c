/* Vault's inn, before and after the thieves are caught: the Bone on the barrel
   beside the girl at the inn, who offers the party "leftover bones from dinner"
   for the dog in town. */

/* Searching the barrel (METATILE_BEHAVIOR_LOG_ROOMS_A_INN_BARREL) takes the
   Bone, unless the party's bags are full. */
static void scVaultFindBone(void)
{
	fdScriptStart();
	if(gmPartyInventoryHasSpace() == 0) {
		fdActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgVaultWowHaveManyThingsArent);
		/* "Wow, you have so many things. Aren't you a greedy lad!" */
		fdScriptShowMessageAndWait(ACTOR_GIRL_AT_THE_INN, 0);
	} else {
		fdItemDisplayShowItem(
			ITEM_BONE, ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		fdPartyInventoryGiveItem(ITEM_BONE, 0);
	}
	fdScriptEnd();
}
