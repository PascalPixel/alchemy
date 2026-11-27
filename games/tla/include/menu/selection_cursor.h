#ifndef ALCHEMY_SELECTION_CURSOR_H
#define ALCHEMY_SELECTION_CURSOR_H

/* The selection screen's icon under the hand. */

#include "types.h"

struct SELECTION_MENU_STATE;
struct SELECTION_NODE;

/* Returns the icon of the row the hand points at (cursorIndex). */
struct SELECTION_NODE *mnSelectionMenuGetCursorNode(
	struct SELECTION_MENU_STATE *screen);

#endif
