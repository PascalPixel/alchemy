/* mnIconCountItemIcons and mnIconCountPsynergyIcons, the same in both games.
   Both games' window/window_icons.c include it where the functions stand. */

/* Returns how many item icons the icon table holds; mnIconBuildItemIconTiles
   draws icon 0 for a number past them. */
Sint32 mnIconCountItemIcons(void)
{
	return ItemIconsEnd - ItemIcons;
}

/* Returns how many Psynergy icons the icon table holds;
   mnIconBuildAbilityIconTiles draws icon 0 for a number past them. */
Sint32 mnIconCountPsynergyIcons(void)
{
	return PsynergyIconsEnd - PsynergyIcons;
}
