#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

/* The selection screen's sprites and nodes. */

#include "static_assert.h"
#include "types.h"
#include "system.h"
#include "window/window.h"

/* A selection screen sprite: a list link and its OAM attributes. */
typedef struct SELECTION_SPR {
	struct SELECTION_SPR *next; /* the sprite queue's link */
	Uint8				  y;	/* screen pixels */
	Uint8  affine : 2;			/* 0 plain, 1 affine, 3 affine at double size */
	Uint8  mode : 2;			/* 0 normal, 1 semi-transparent, 2 OBJ window */
	Uint8  mosaic : 1;
	Uint8  cols : 1;   /* 1 for 256 colours */
	Uint8  shape : 2;  /* 0 square, 1 wide, 2 tall */
	Uint16 x : 9;	   /* screen pixels */
	Uint16 matrix : 5; /* the affine matrix */
	Uint16 size : 2;   /* with shape, the sprite's size */
	Uint16 tile : 10;  /* first sprite tile in VRAM */
	Uint16 pri : 2;
	Uint16 pal : 4; /* 16-colour palette */
} SELECTION_SPR;

/* An icon of the selection screen: its list links, the entry it shows, its
   place and its slide toward a target, its zoom and its sprite. */
typedef struct SELECTION_NODE {
	struct SELECTION_NODE *prev; /* the icon before it in its row */
	struct SELECTION_NODE *next; /* the icon after it in its row */
	/* The entry's number within its kind, as listed in
	   SELECTION_MENU_STATE.bases; for an arrow, the ticks left of its nudge
	   after a scroll. */
	Uint16 base;
	/* The entry's kind, as listed in SELECTION_MENU_STATE.kinds, or
	   ICON_KIND_NONE for a free record; for an arrow, TRUE while it shows. */
	Uint16		  kind;
	Uint16		  slot;		 /* VRAM cache slot of its picture */
	Uint16		  tile;		 /* first sprite tile of its picture */
	Sint16		  x;		 /* screen pixels */
	Sint16		  y;		 /* screen pixels */
	Sint16		  dx;		 /* pixels a tick */
	Sint16		  dy;		 /* pixels a tick */
	Sint16		  tgtX;		 /* screen pixels */
	Sint16		  tgtY;		 /* screen pixels */
	Sint16		  homeX;	 /* screen pixels, where it stood when chosen */
	Sint16		  homeY;	 /* screen pixels, where it stood when chosen */
	Uint16		  msg;		 /* its name, shown in the screen's window */
	Sint16		  scale;	 /* 8.8 fixed point, Q_8_8(1) full size */
	Sint16		  scaleStep; /* 8.8 fixed point a tick */
	Sint16		  scaleEnd;	 /* 8.8 fixed point, Q_8_8(1) full size */
	SELECTION_SPR spr;
} SELECTION_NODE;

/* The entries of SELECTION_MENU_STATE.records. */
typedef enum SELECTION_RECORD {
	SELECTION_RECORD_ROW = 2,	 /* the first of the row's seven icons */
	SELECTION_RECORD_CHOSEN = 9, /* the first of the five icons chosen */
	SELECTION_RECORD_HAND = 14,	 /* the pointing hand */
	SELECTION_RECORD_LARGE_ICON = 15,
	SELECTION_RECORD_COUNT = 16
} SELECTION_RECORD;

/* The selection screen (heap block 18): the row of icons the player picks from,
   with arrows at each end and a pointing hand over the chosen target. */
typedef struct SELECTION_MENU_STATE {
	/* 0 and 1 the arrows at the row's ends, 2 to 8 the row's icons, 9 to 13 the
	   icons chosen so far, 14 the pointing hand, whose picture is loaded at
	   setup, and 15 the large icon. */
	SELECTION_NODE	 records[SELECTION_RECORD_COUNT];
	AFFINE_TRANSFORM transform; /* a zooming icon's scale */
	SELECTION_NODE	*head;		/* the first icon of the row */
	SELECTION_NODE	*path;		/* the icons chosen so far */
	WINDOW			*win;		/* the window naming the chosen entry */
	/* The listed entries, up to 16 (mnSelectionMenuAddEntry): each one's kind
	   (ICON_KIND) and its command, item or ability number. */
	Uint16 kinds[16];
	Uint16 bases[16];
	Uint16 cnt;
	Uint16 baseX; /* screen pixels */
	Uint16 baseY; /* screen pixels */
	Uint16
		/* cleared with the list and on each step down; nothing reads it */
		   unread;
	Uint16 top; /* the first listed entry shown */
	/* The entry under the hand, from top; SELECTION_CURSOR_RESET asks the next
	   list to start again from its first entry. */
	Uint16 cursorIdx;
	Uint16 verticalScroll; /* pixels added to the icons' y; 32 at setup */
	/* SELECTION_STATUS_SCROLLING while the cursor steps, SELECTION_STATUS_SHOWN
	   after (window_selection.c); each tick drawn adds 1. */
	Uint16 stat;
	Uint16 pathTop[5]; /* top, kept for each icon chosen on the path */
	/* cursorIndex, kept for each icon chosen on the path */
	Uint16 pathCursor[5];
	/* 999 from setup and once a yes or no window opens, 1 from
	   mnRunSelectionForValue. */
	Uint16 locked;
	Uint8  unused[42];
} SELECTION_MENU_STATE;

/* The bit of SELECTION_MENU_STATE.cursorIdx set as an entry is chosen, on which
   mnSelectionMenuClear puts the next list's top and cursor back to its first
   entry. */
#define SELECTION_CURSOR_RESET 0x80

STATIC_ASSERT(sizeof(SELECTION_SPR) == 12);
STATIC_ASSERT(sizeof(SELECTION_NODE) == 52);
STATIC_ASSERT(offsetof(SELECTION_NODE, msg) == 32);
STATIC_ASSERT(offsetof(SELECTION_NODE, spr) == 40);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, head) == 840);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, kinds) == 852);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, cursorIdx) == 926);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, locked) == 952);
STATIC_ASSERT(sizeof(SELECTION_MENU_STATE) == 996);

#endif
