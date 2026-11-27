#ifndef ALCHEMY_CURSOR_H
#define ALCHEMY_CURSOR_H

/* @file */
/* The cursor that approaches its target, shared by shops, the name entry */
/* screen, the settings menu and the file screen's sprites. */

#include "static_assert.h"
#include "types.h"

struct WINDOW_SPR;

/* Cursor that approaches its target, shared by shops, the name entry screen */
/* and the settings menu. */
typedef struct MENU_CURSOR {
	struct WINDOW_SPR *anchor; /* the cursor's sprite, moved with its OAM */
	u16 x;	   /* screen pixels: where the current move started */
	u16 y;	   /* screen pixels: where the current move started */
	s16 tgtX;  /* screen pixels */
	s16 tgtY;  /* screen pixels */
	s8 frm;	   /* frames of the current move done; reset when it ends */
	s8 frames; /* frames the move takes; 0 when the cursor is still */
} MENU_CURSOR;

STATIC_ASSERT(sizeof(MENU_CURSOR) == 16);
STATIC_ASSERT(offsetof(MENU_CURSOR, tgtX) == 8);
STATIC_ASSERT(offsetof(MENU_CURSOR, frm) == 12);

#endif
