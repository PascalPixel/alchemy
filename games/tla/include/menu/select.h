#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

/* @file */
/* The selection screen: the row of icons a command, item or Psynergy is */
/* picked from, its icons and their sprites. */

#include "types.h"
#include "static_assert.h"
#include "window/window.h"
#include "system.h"

/* A selection screen sprite: a list link and its OAM attributes. */
typedef struct SELECTION_SPR {
	struct SELECTION_SPR *next;
	u8 y;
	u8 affine : 2;
	u8 mode : 2;
	u8 mosaic : 1;
	u8 cols : 1;
	u8 shape : 2;
	u16 x : 9;
	u16 matrix : 5; /* the affine matrix */
	u16 size : 2;
	u16 tile : 10;
	u16 pri : 2;
	u16 pal : 4;
} SELECTION_SPR;

/* An icon of the selection screen: its list links, the entry it shows, its */
/* place and its slide toward a target, its zoom and its sprite. */
typedef struct SELECTION_NODE {
	struct SELECTION_NODE *prev; /* the icon before it in its row */
	struct SELECTION_NODE *next; /* the icon after it in its row */
	u16 base; /* the entry's id, as listed in SELECTION_MENU_STATE.bases */
	u16 kind; /* the entry's kind, as listed in SELECTION_MENU_STATE.kinds */
	u16 slot;
	u16 tile;
	s16 x;	  /* screen pixels */
	s16 y;	  /* screen pixels */
	s16 dx;	  /* pixels a frame */
	s16 dy;	  /* pixels a frame */
	s16 tgtX; /* screen pixels */
	s16 tgtY; /* screen pixels */
	s16 homeX;
	s16 homeY;
	u16 msg; /* its name, shown in the screen's window */
	s16 scale;
	s16 scaleStep;
	s16 scaleEnd;
	SELECTION_SPR spr;
} SELECTION_NODE;

/* The kinds of icon the selection screen lists (SELECTION_NODE.kind) and */
/* mnSelectionIconLoad loads. */
typedef enum ICON_KIND {
	ICON_KIND_NONE = 0,	   /* a free selection icon entry */
	ICON_KIND_COMMAND = 1, /* a command's icon, from its number */
	ICON_KIND_ITEM = 2,	   /* an item's icon */
	ICON_KIND_ABILITY = 4, /* a Psynergy's icon, from its ability number */
	ICON_KIND_YES_NO = 6,  /* a yes or no command, drawn as a command */
	/* An item's icon without the broken mark, for the equipment view. */
	ICON_KIND_EQUIPMENT_ITEM = 7,
	ICON_KIND_PATTERN = 8 /* one of the other icons */
} ICON_KIND;

/* The entries of SELECTION_MENU_STATE.records. */
typedef enum SELECTION_RECORD {
	SELECTION_RECORD_ROW = 2,	 /* the first of the row's seven icons */
	SELECTION_RECORD_CHOSEN = 9, /* the first of the five icons chosen */
	SELECTION_RECORD_HAND = 14,	 /* the pointing hand */
	SELECTION_RECORD_LARGE_ICON = 15,
	SELECTION_RECORD_COUNT = 16
} SELECTION_RECORD;

/* The selection screen (HEAP_SLOT_SELECTION_MENU): the row of icons the player
 */
/* picks from, with arrows at each end and a pointing hand over the chosen */
/* target. */
typedef struct SELECTION_MENU_STATE {
	/* 0 and 1 the arrows at the row's ends, 2 to 8 the row's icons, 9 to 13 the
	 */
	/* icons chosen so far, 14 the pointing hand and 15 the large icon. */
	SELECTION_NODE records[SELECTION_RECORD_COUNT];
	AFFINE_TRANSFORM transform; /* a zooming icon's scale */
	SELECTION_NODE *head;		/* the first icon of the row */
	SELECTION_NODE *path;		/* the icons chosen so far */
	WINDOW *win;				/* the window naming the chosen entry */
	/* The listed entries, up to 16. */
	/* @{ */
	u16 kinds[16];
	u16 bases[16];
	u16 cnt;
	/* @} */
	u16 baseX;	/* screen pixels */
	u16 baseY;	/* screen pixels */
	u16 unread; /* cleared with the list and on each step down; nothing reads it
				 */
	u16 top;	/* the first listed entry shown */
	/* The entry under the hand, from top; SELECTION_CURSOR_RESET asks the next
	 */
	/* list to start again from its first entry. */
	u16 cursorIdx;
	u16 verticalScroll; /* pixels added to the icons' y; 32 at setup */
	/* SELECTION_STATUS_SCROLLING while the cursor steps, SELECTION_STATUS_SHOWN
	 */
	/* after (window_selection.c); each frame drawn adds 1. */
	u16 stat;
	u16 pathTop[5];	   /* top, kept for each icon chosen on the path */
	u16 pathCursor[5]; /* cursorIdx, kept for each icon chosen on the path */
	/* 999 from setup and once a yes or no window opens, 1 from */
	/* mnRunSelectionForValue. Nothing sets 0, which would open a yes or no */
	/* row and its window at the top of the screen. */
	u16 locked;
	u8 unused[42]; /* no code names it */
} SELECTION_MENU_STATE;

/* SELECTION_MENU_STATE.stat: the cursor stepping, or the row shown. */
#define SELECTION_STATUS_SCROLLING 33
#define SELECTION_STATUS_SHOWN	   1

STATIC_ASSERT(sizeof(SELECTION_SPR) == 12);
STATIC_ASSERT(sizeof(SELECTION_NODE) == 52);
STATIC_ASSERT(offsetof(SELECTION_NODE, msg) == 32);
STATIC_ASSERT(offsetof(SELECTION_NODE, spr) == 40);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, head) == 840);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, kinds) == 852);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, cursorIdx) == 926);
STATIC_ASSERT(offsetof(SELECTION_MENU_STATE, locked) == 952);
STATIC_ASSERT(sizeof(SELECTION_MENU_STATE) == 996);

/* The bit of SELECTION_MENU_STATE.cursorIdx set as an entry is chosen, on */
/* which mnSelectionMenuClear puts the next list's top and cursor back to its */
/* first entry. */
#define SELECTION_CURSOR_RESET 0x80

void *mnIconMenuOpen(void);
void mnIconMenuAddIcon(s32 icon);
void mnIconMenuClose(void);
void mnSelectionMenuFreeLargeIcon(void);
void mnIconMenuLayoutIcons(s32 x, s32 y, s32 w, s32 h);
s32 mnIconMenuRun(s32 initial);

/* Loads the icon of a selection entry of kind mode (ICON_KIND) into a free
 */
/* sprite tile slot and returns the slot, or -1. */
s32 mnSelectionIconLoad(u32 mode, s32 value);

#endif
