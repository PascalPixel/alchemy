/* mnWindowSprAppendToList, the same in both games. Both games'
   window/window_windows.c include it where the function stands. */

/* Adds a window sprite to the end of a list; NULL lists are ignored. */
void mnWindowSprAppendToList(WINDOW_SPR_LIST *list, WINDOW_SPR *node)
{
	if(list != NULL) {
		*list->tailLink = node;
		/* UB: the new tail is stored as node (next is its first member) through
		   a WINDOW_SPR * view of tailLink, which keeps both stores in one alias
		   set and in this order. */
#if FIX_TYPE_PUNNING
		/* Store the address of next through the tail link's actual pointer
		   type. */
		list->tailLink = &node->next;
#else
		*(WINDOW_SPR **)&list->tailLink = node;
#endif
	}
}
