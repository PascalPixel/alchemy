/* mnYesNoMenuRun, the same in both games. Both games'
   window/screens_icon_menu.c include it where the function stands. */

/* Selects Yes or No, starting on `selection`; cancelling answers No. The icons
   stand on the bottom row when `bottom` is set, else at the top; `msg` is
   the first label message, or 0 for the command names, and `labelWidth` the
   label window's width, 3 when 0. */
Sint32 mnYesNoMenuRun(
	Sint32 bottom, Sint32 msg, Sint32 labelWidth, Sint32 selection)
{
	Sint32 row;

	row = 0;

	mnIconMenuOpen();

	if(labelWidth == 0) {
		labelWidth = 3;
	}
	if(bottom != 0) {
		row = ICON_MENU_BOTTOM_ROW;
	}

	mnIconMenuAddIcon(COMMAND_ICON_YES);
	mnIconMenuAddIcon(COMMAND_ICON_NO);
	mnIconMenuCenter(row, labelWidth, msg);

	selection = mnIconMenuRun(selection);
	mnIconMenuClose();
	if(selection == -1) {
		selection = NO;
	}
	return selection;
}
