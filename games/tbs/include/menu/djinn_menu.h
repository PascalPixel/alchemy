#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/* The Djinn menu: its list tables, backdrop and state, and the fields of a
   Djinni's list entry. */

#include "djinn_ids.h"
#include "types.h"
#include "system.h"
#include "window/window_sprite.h"

/* A Djinni's list entry, as mnDjinnListUnitEntries writes it into
   DJINN_LIST_TABLE.ids. bits: 15 set | 11..8 member | 7..5 element | 4..0
   index */
#define DJINN_ENTRY_INDEX	31	   /* its number within its element */
#define DJINN_ENTRY_ELEMENT 0xe0   /* its element (ELEMENT) */
#define DJINN_ENTRY_MEMBER	0xf00  /* the party member holding it */
#define DJINN_ENTRY_IS_SET	0x8000 /* set; an unset one is drawn in colour 2 */

#define DJINN_ELEMENTS	   4  /* Venus, Mercury, Mars and Jupiter (ELEMENT) */
#define DJINN_LIST_MEMBERS 8  /* list columns: one per party member */
#define DJINN_LIST_ROWS	   10 /* entries one party member's list holds */

/* The Djinn menu's lists: each party member's Djinn entries and their count. */
typedef struct DJINN_LIST_TABLE {
	Uint16 ids[DJINN_LIST_MEMBERS][DJINN_LIST_ROWS];
	Sint8  cnts[DJINN_LIST_MEMBERS];
} DJINN_LIST_TABLE;

/* The Djinn menu's lists, the field tiles and palette it restores on closing,
   and the tutorial's state. */
typedef struct DJINN_MENU_BACKDROP {
	DJINN_LIST_TABLE list;
	Uint8			 tiles[8192]; /* BG character block 1's first 8 KB */
	Uint16			 pal[64];	  /* BG palettes 4 to 7 */
	Sint32			 tutorialTmr; /* frames into the tutorial's current step */
	/* The Djinn tutorial's step (DJINN_TUTORIAL_ in menu_internal.h), chosen to
	   start by FLAG_DJINN_LESSON_PART_A and FLAG_DJINN_LESSON_PART_B. */
	long tutorialStep;
} DJINN_MENU_BACKDROP;

/* DJINN_MENU_STATE.pending's bits: what the chosen pair still waits for. */
typedef enum DJINN_PENDING {
	DJINN_PENDING_SECOND = 1, /* a second Djinni is to be chosen */
	DJINN_PENDING_CONFIRM = 2 /* a give or trade awaits its confirmation */
} DJINN_PENDING;

/* The Djinn menu's state, in the HEAP_SLOT_MENU heap slot from the menu's
   opening to its closing. */
typedef struct DJINN_MENU_STATE {
	/* no view of the menu state names it (see menu/README.md) */
	Uint8		   unused[8];
	Sint32		   member;
	struct WINDOW *moneyWin; /* SHOP_STATE's; not named here */
	struct WINDOW *statWin;
	WINDOW_SPR	  *cursor;
	/* The second of ITEM_MENU_STATE's sideIcons; not named here. */
	WINDOW_SPR *secondSideIcon;
	/* each side's party member, an index into party members */
	Sint8 selectedColumn[2];
	Sint8 sideCnt[2]; /* ITEM_MENU_STATE's; not named here */
	/* ITEM_MENU_STATE's and SHOP_STATE's itemWindow; not named here. */
	struct WINDOW *itemWin;
	struct WINDOW *win;
	/* Never given a window, so NULL from mnInitState on; the preview still
	   redraws it, which reads the window's place from the BIOS. */
	struct WINDOW *unopenedWin;
	struct WINDOW *infoWin; /* ITEM_MENU_STATE's; not named here */
	struct WINDOW *listWin;
	struct WINDOW *secondWin;
	/* ITEM_MENU_STATE's last windows and its icons; not named here. */
	Uint8		itemWins[16];
	WINDOW_SPR *entryIcons[32];
	WINDOW_SPR *statIcons[8];
	Uint8		unusedAfterIcons[36]; /* no view of the menu state names it */
	struct WINDOW *helpWin;
	/* ITEM_MENU_STATE's cursor grid, selectedColumn to rowCount; not named
	   here. */
	Uint8 cursorGrid[4];
	/* ITEM_MENU_STATE's party figures; not named here. */
	struct SPR *memberSprs[8];
	Sint16		memberX[8];		/* screen pixels */
	Sint16		memberY[8];		/* screen pixels */
	Sint32		memberScale[8]; /* ITEM_MENU_STATE's; not named here */
	Uint16		selection[2];	/* each side's Djinni row chosen */
	Uint16		pairEntries[2]; /* 255 at first */
	WINDOW_SPR *secondCursor;
	Uint8 unusedBeforeBackdrop[4]; /* no view of the menu state names it */
	DJINN_MENU_BACKDROP *backdrop;
	Uint8  unusedAfterBackdrop[64]; /* no view of the menu state names it */
	Uint16 entries[32]; /* ITEM_MENU_STATE's items; not named here */
	Uint16 members[DJINN_LIST_MEMBERS]; /* the party in order */
	Uint8  pairCnt;
	Uint8  memberCnt;
	Uint8  shownMember[2]; /* the party member giving and the one receiving */
	/* ITEM_MENU_STATE's selectedItemIcon; not named here. */
	WINDOW_SPR *selectedIcon;
	Uint16		pending;	   /* DJINN_PENDING bits */
	Sint16		completionFlg; /* ITEM_MENU_STATE's; not named here */
	/* ItemMenuFiguresView's animations, the figures the Djinn screen moves
	   through that view. */
	struct SPR *figures[4];
	Uint16		slotX[4];
	Uint16		slotY[4];
	Sint32 figurePhases[4]; /* ItemMenuFiguresView's phases; not named here */
	/* The Djinn chosen in each side: index in their element, element, party
	   member. */
	Uint8 pairIdx[2];
	Uint8 pairElement[2];
	Uint8 pairMember[2];
} DJINN_MENU_STATE;

STATIC_ASSERT(offsetof(DJINN_MENU_STATE, statWin) == 16);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, selectedColumn) == 28);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, memberX) == 308);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, memberY) == 324);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, selection) == 372);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, pending) == 544);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, pairIdx) == 596);
STATIC_ASSERT(offsetof(DJINN_MENU_BACKDROP, tutorialTmr) == 8488);
STATIC_ASSERT(offsetof(DJINN_MENU_BACKDROP, tutorialStep) == 8492);

#endif
