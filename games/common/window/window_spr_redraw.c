/* mnWindowSprPrepareForRedraw, mnWindowSprRedrawSavedRect and
   mnWindowSprClearList, the same in both games. Both games'
   window/window_windows.c include it where the functions stand. */

/* Clears a window's contents: redraws it empty and releases the window sprites
   it carries. A text canvas window is left alone. */
void mnWindowSprPrepareForRedraw(WINDOW *win)
{
	if(!(WINDOW_FLAG_TEXT_CANVAS & win->flg)) {
		mnWindowSprRedrawSavedRect(win);
		mnWindowSprClearList(win);
	}
}

/* Redraws a window empty, its border and fill, at its current rectangle. */
void mnWindowSprRedrawSavedRect(WINDOW *win)
{
	mnWindowDrawBorder(win->x, win->y, win->width, win->height);
}

/* Releases every window sprite in a list and empties it. sprites is the list,
   or a window (whose sprite list comes first); NULL is ignored. */
void mnWindowSprClearList(void *sprs)
{
	WINDOW_SPR_LIST *list = sprs;
	WINDOW_SPR		*node;
	WINDOW_SPR		*next;

	next = NULL;
	if(list != NULL) {
		node = list->head;
		/* Detach the chain before returning its nodes to the shared free
		   pool. */
		list->tailLink = &list->head;
		list->head = next;
		while(node != NULL) {
			next = node->next;
			mnWindowSprRelease(node);
			node = next;
		}
	}
}
