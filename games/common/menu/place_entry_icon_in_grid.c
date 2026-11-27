/* mnPlaceEntryIconInGrid, the same in both games. Both games'
   menu/menu_status.c include it where the function stands. */

/* Places icon at cell index (an index above 31 is taken as 0) of a grid of
   ENTRY_ICON_CELL_SIZE square cells, columns wide, from (originX, originY), and
   shows it. */
void mnPlaceEntryIconInGrid(WINDOW_SPR *icon, Sint32 idx, Sint32 originX,
	Sint32 originY, Sint32 columns)
{
	Sint32 entryIdx;

	entryIdx = idx;
	if(entryIdx > MENU_ENTRY_ICONS - 1) {
		entryIdx = 0;
	}
	icon->y = (entryIdx / columns * ENTRY_ICON_CELL_SIZE) + originY;
	icon->x = (entryIdx % columns * ENTRY_ICON_CELL_SIZE) + originX;
	mnIconShow(icon);
}
