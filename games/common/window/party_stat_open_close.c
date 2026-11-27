/* mnPartyStatOpenAndWait and mnPartyStatCloseWithMenu, the same in both
   games. Both games' window/window_selection.c include it where the
   functions stand. */

/* Opens the party status window that shows with the field menu (flags 1) and
   waits a tick. */
void mnPartyStatOpenAndWait(void)
{
	mnPartyStatCreateWindow(1);
	sySynch(1);
}

/* Closes the party status window, kept in heap block 16 (PartyStatusState), and
   frees the block. */
void mnPartyStatCloseWithMenu(void)
{
	mnPartyStatClose();
}
