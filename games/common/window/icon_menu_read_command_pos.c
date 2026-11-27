/* mnIconMenuReadCommandPos, the same in both games. Both games'
   window/screens_icon_menu.c include it where the function stands. */

/* Reads one entry of a field menu table (MenuTopEntryPositionByCommand). */
static __inline__ Sint32 mnIconMenuReadCommandPos(Sint8 *tbl, Sint32 idx)
{
	return tbl[idx];
}
