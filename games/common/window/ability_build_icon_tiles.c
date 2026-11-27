/* mnAbilityBuildIconTiles, the same in both games. Both games'
   window/window_icons.c include it where the function stands. */

/* Builds the Psynergy icon of a battle action, as
   mnIconBuildAbilityIconTiles. */
void mnAbilityBuildIconTiles(
	Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse)
{
	mnIconBuildAbilityIconTiles(
		gmAbilityGetDirectStub(action)->icon, withBase, slot, tile, reuse);
}
