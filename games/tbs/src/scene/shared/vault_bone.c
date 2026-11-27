/* @file */
/* Vault's inn, before and after the thieves are caught: the Bone on the barrel
 */
/* beside the girl at the inn, who offers the party "leftover bones from */
/* dinner" for the dog in town. */

/* Searching the barrel (CELL_BEHAVIOR_LOG_ROOMS_A_INN_BARREL) takes the Bone,
 */
/* unless the party's bags are full. */
static void scVaultFindBone(void) {
	fdEventStart();
	if(gmPartyInventoryHasSpace() == 0) {
		fdActorSetAnimationAndWait(ACTOR_GIRL_AT_THE_INN, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		fdEventSetMessageId(MsgVaultWowHaveManyThingsArent);
		/* "Wow, you have so many things. Aren't you a greedy lad!" */
		fdEventShowMessageAndWait(ACTOR_GIRL_AT_THE_INN, 0);
	}
	else {
		fdItemDispShowItem(ITEM_BONE,
						   ITEM_DISPLAY_SPARKLES | ITEM_DISPLAY_RAISED);
		fdPartyInventoryGiveItem(ITEM_BONE, 0);
	}
	fdEventEnd();
}
