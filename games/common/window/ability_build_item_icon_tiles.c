/* mnAbilityBuildItemIconTiles, the same in both games. Both games'
   window/window_icons.c include it where the function stands. */

/* Unused: builds the icon of a battle action as an item's icon
   (mnIconBuildItemIconTiles), where mnAbilityBuildIconTiles builds its Psynergy
   icon. */
UNUSED void mnAbilityBuildItemIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse)
{
	mnIconBuildItemIconTiles(
		gmAbilityGetDirectStub(action)->icon, withBase, slot, tile, reuse);
}
