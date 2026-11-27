#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

/* The cursor that approaches its target, shared by shops, the name entry screen
   and the settings menu. */

#include "static_assert.h"
#include "types.h"
#include "window/window_sprite.h"

/* Cursor that approaches its target, shared by shops, name entry screen and
   settings menu. */
typedef struct MENU_CURSOR {
	WINDOW_SPR *anchor;	 /* the cursor's sprite, moved with its OAM */
	Uint16		x;		 /* screen pixels: where the current move started */
	Uint16		y;		 /* screen pixels: where the current move started */
	Sint16		targetX; /* screen pixels */
	Sint16		targetY; /* screen pixels */
	Sint8		frame; /* ticks of the current move done; reset when it ends */
	Sint8		ticks; /* ticks the move takes; 0 when the cursor is still */
} MENU_CURSOR;

STATIC_ASSERT(sizeof(MENU_CURSOR) == 16);
STATIC_ASSERT(offsetof(MENU_CURSOR, targetX) == 8);
STATIC_ASSERT(offsetof(MENU_CURSOR, frame) == 12);

/* A menu cursor: its window sprite, or the cursor that approaches its target.
   Screens that set only the image pointer write it through the union. */
typedef union MENU_CURSOR_REF {
	WINDOW_SPR *sprite; /* the image alone; cursor.anchor */
	MENU_CURSOR cursor;
} MENU_CURSOR_REF;

#endif
