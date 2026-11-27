#ifndef ALCHEMY_ICON_MENU_H
#define ALCHEMY_ICON_MENU_H

/* The icon menu's entries and state. */

#include "static_assert.h"
#include "types.h"
#include "system.h"
#include "window/window.h"

/* A command icon of the icon menu: its OAM attributes, its place and its VRAM
   cache slot. */
typedef struct ICON_MENU_ENTRY {
	/* The sprite queue's link, then OAM attributes 0 and 1 and attribute 2,
	   written each tick by mnIconMenuDrawIcons. */
	Uint32 sprite[3];
	Sint16 x; /* screen pixels; an icon at 0 is not drawn */
	Sint16 y; /* screen pixels */
	Uint16 unused;
	Uint16 slot; /* VRAM cache entry of the icon's picture */
} ICON_MENU_ENTRY;

/* The icon menu: up to six command icons in a row, the windows of their labels,
   the icons' resource ids, the selection and the layout. */
typedef struct ICON_MENU_STATE {
	ICON_MENU_ENTRY entries[6];
	WINDOW		   *window; /* the label window naming the selected icon */
	/* The password level and transfer menus' explanation window, below the
	   heading. */
	WINDOW *lowerWindow;
	WINDOW *upperWindow; /* those menus' heading window */
	/* Each icon's number (ResourceIdCommandIcons), which is also its command
	   name's: MsgCommandName plus it. */
	Uint8  commandIcons[8];
	Sint16 selection; /* the icon chosen, 0 to count - 1 */
	Sint16 count;	  /* icons in the row, up to 6 */
	Sint16 width;	  /* tiles */
	/* A nonzero first label message, the selection added to it; 0 labels each
	   icon by its command name. */
	Sint16 labelBase;
	Sint16 row; /* tiles */
	/* The selection the explanation was last drawn for; -1 draws it anew. */
	Sint16 drawnSelection;
} ICON_MENU_STATE;

STATIC_ASSERT(sizeof(ICON_MENU_ENTRY) == 20);
STATIC_ASSERT(offsetof(ICON_MENU_ENTRY, slot) == 18);
STATIC_ASSERT(sizeof(ICON_MENU_STATE) == 152);
STATIC_ASSERT(offsetof(ICON_MENU_STATE, window) == 120);
STATIC_ASSERT(offsetof(ICON_MENU_STATE, selection) == 140);
STATIC_ASSERT(offsetof(ICON_MENU_STATE, labelBase) == 146);
STATIC_ASSERT(offsetof(ICON_MENU_STATE, drawnSelection) == 150);

/* Heap slot 58: the icon menu while one is open. */
#define IconMenuStateWork (*(ICON_MENU_STATE **)&HeapSlots[HEAP_SLOT_ICON_MENU])

#endif
