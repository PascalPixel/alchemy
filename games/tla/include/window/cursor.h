#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

// The cursor that approaches its target, shared by shops, the name entry screen, the settings menu
// and the file screen's sprites.

#include "static_assert.h"
#include "types.h"

struct WINDOW_SPR;

// Cursor that approaches its target, shared by shops, the name entry screen and the settings menu.
typedef struct MENU_CURSOR
{
	struct WINDOW_SPR *anchor;  // the cursor's sprite, moved with its OAM
	Uint16             x;       // screen pixels: where the current move started
	Uint16             y;       // screen pixels: where the current move started
	Sint16             targetX; // screen pixels
	Sint16             targetY; // screen pixels
	Sint8              frame;   // ticks of the current move done; reset when it ends
	Sint8              ticks;   // ticks the move takes; 0 when the cursor is still
} MENU_CURSOR;

STATIC_ASSERT(sizeof(MENU_CURSOR) == 16);
STATIC_ASSERT(offsetof(MENU_CURSOR, targetX) == 8);
STATIC_ASSERT(offsetof(MENU_CURSOR, frame) == 12);

#endif
