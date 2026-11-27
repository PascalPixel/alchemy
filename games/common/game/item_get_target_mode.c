/* gmItemGetTargetMode, the same in both games. Both games'
   game/game_inventory.c include it where the function stands. */

/* Returns how the action an item performs when used picks its targets. Nothing
   calls it by name; in The Broken Seal a bare stub reaches it, in The Lost Age
   nothing does. */
Uint8 gmItemGetTargetMode(Sint32 itemId)
{
	return gmAbilityGetDirect(gmItemGetDirect(itemId)->abilityId)->tgtMode;
}
