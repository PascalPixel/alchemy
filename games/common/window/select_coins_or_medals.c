/* mnSelectCoinsOrMedals, the same in both games. Both games'
   window/screens_icon_menu.c include it where the function stands. */

/* Selects Coins or Lucky Medals, centred on the bottom row, starting on
   position `selection`. Returns the position chosen, or -1 when cancelled. */
Sint32 mnSelectCoinsOrMedals(Sint32 selection)
{
	Sint32 result;

	mnIconMenuOpen();
	mnIconMenuAddIcon(COMMAND_ICON_COINS);
	mnIconMenuAddIcon(COMMAND_ICON_LUCKY_MEDALS);
	mnIconMenuCenter(ICON_MENU_BOTTOM_ROW, PAIR_WIDTH, 0);
	result = mnIconMenuRun(selection);
	mnIconMenuClose();
	return result;
}
