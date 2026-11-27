#ifndef ALCHEMY_DJINN_MENU_H
#define ALCHEMY_DJINN_MENU_H

/* The Djinn menu: its lists, backdrop and state, the fields of a Djinni's list
   entry, and the Djinn tutorial's lines. */

#include "static_assert.h"
#include "types.h"
#include "fixed_math.h"

/* A Djinni's list entry, as mnDjinnListUnitEntries writes it into
   DJINN_MENU_BACKDROP.ids. bits: 15 set | 11..8 member | 7..5 element | 4..0
   index */
#define DJINN_ENTRY_INDEX		  31	/* its number within its element */
#define DJINN_ENTRY_ELEMENT		  0xe0	/* its element (ELEMENT) */
#define DJINN_ENTRY_MEMBER		  0xf00 /* the party member holding it */
#define DJINN_ENTRY_IS_SET		  0x8000 /* set; a standby one is TEXT_COLOR_RED */
#define DJINN_ENTRY_ELEMENT_SHIFT 5
#define DJINN_ENTRY_MEMBER_SHIFT  8

#define DJINN_ELEMENTS	   4
#define DJINN_LIST_MEMBERS 8  /* list columns: one per party member */
#define DJINN_LIST_ROWS	   10 /* entries one party member's list holds */

/* The Djinn tutorial's lines, counted from its first (mnGetDjinnGiveMessage and
   btTutorialGetDjinnGiveMessage: one of its two copies, by
   FLAG_ALREADY_KNOWS_DJINN). */
typedef enum DJINN_TUTORIAL_LINE {
	/* "To give your Djinni to someone else, move it like this." */
	DJINN_TUTORIAL_LINE_GIVE = 0,
	/* "After choosing who gets the Djinni, you can \"set\" it." */
	DJINN_TUTORIAL_LINE_SET_AFTER_GIVE = 1,
	/* "When your class changes, the Psynergy you can use may also change." */
	DJINN_TUTORIAL_LINE_CLASS_PSYNERGY = 2,
	/* "Setting a Djinni changes your attributes and sometimes even your
	   class." */
	DJINN_TUTORIAL_LINE_SET_CHANGES = 3,
	/* "Each Djinni has different, unique powers." */
	DJINN_TUTORIAL_LINE_POWERS = 4,
	/* "Once we have been set, you can unleash us in battle to use our
	   powers." */
	DJINN_TUTORIAL_LINE_UNLEASH = 5,
	/* "So, {protagonist_name}, let me demonstrate the power of my attack
	   now." */
	DJINN_TUTORIAL_LINE_DEMONSTRATE = 6,
	/* "See! What did I tell you? When you unleashed me, I used my special
	   attack ability!" */
	DJINN_TUTORIAL_LINE_UNLEASHED = 7,
	/* "After being unleashed, we Djinn will then stand by, ready to be summoned
	   back again..." */
	DJINN_TUTORIAL_LINE_STANDBY = 8,
	/* "If you have multiple standby Djinn, you can summon even greater powers
	   into battle!" */
	DJINN_TUTORIAL_LINE_SUMMON = 9,
	/* "Choose this command when you want to unleash me in battle, all
	   right?" */
	DJINN_TUTORIAL_LINE_UNLEASH_COMMAND = 10,
	/* "After a Djinni has been used to summon, it must spend time in
	   recovery." */
	DJINN_TUTORIAL_LINE_RECOVERY = 12,
	/* "And once we've recovered, we automatically set ourselves again, so we're
	   ready to attack!" */
	DJINN_TUTORIAL_LINE_SET_AGAIN = 13,
	/* "That's all you have to do to unleash a set Djinni!" */
	DJINN_TUTORIAL_LINE_UNLEASH_HINT = 14
} DJINN_TUTORIAL_LINE;

/* The Djinn menu's lists, the field tiles and palette it restores on closing,
   and the tutorial's state, allocated when the menu opens and released when it
   closes. */
typedef struct DJINN_MENU_BACKDROP {
	/* each party member's Djinn */
	Uint16 ids[DJINN_LIST_MEMBERS][DJINN_LIST_ROWS];
	Sint8  counts[DJINN_LIST_MEMBERS];
	Uint8  tiles[8192];	  /* BG character block 1's first 8 KB */
	Uint8  palette[128];  /* BG palettes 4 to 7 */
	Sint32 tutorialTimer; /* ticks into the tutorial's current step */
	Sint32 tutorialStep;  /* DJINN_TUTORIAL_STEP */
} DJINN_MENU_BACKDROP;

/* A window as the Djinn menu's preview reads it: a Window whose y it reads
   signed, in tiles. */
typedef struct DJINN_PREVIEW_WINDOW {
	Uint8  sprites[8]; /* the window's sprite list; not read here */
	Uint16 width;
	Uint16 height;
	Uint16 x;
	Sint16 y;
} DJINN_PREVIEW_WINDOW;

/* DJINN_MENU_STATE.pending's bits: what the chosen pair still waits for. */
typedef enum DJINN_PENDING {
	DJINN_PENDING_SECOND = 1, /* Unused: a second Djinni is to be chosen */
	DJINN_PENDING_CONFIRM = 2 /* a give or trade awaits its confirmation */
} DJINN_PENDING;

/* The Djinn menu's state, in the HEAP_SLOT_MENU heap slot from the menu's
   opening to its closing, laid out as ITEM_MENU_STATE is: it names only what
   the Djinn screen reads, and says whose the rest is. */
typedef struct DJINN_MENU_STATE {
	Uint8			   unused[8];	/* no view of the menu state names it */
	Sint32			   member;		/* the party member whose Djinn are shown */
	Sint32			   moneyWindow; /* SHOP_STATE's; not named here */
	Sint32			   statusWindow;
	struct WINDOW_SPR *cursor;
	/* The second of ITEM_MENU_STATE's sideIcons; not named here. */
	struct WINDOW_SPR *secondSideIcon;
	/* each side's party member, an index into members */
	Sint8  selectedColumn[2];
	Sint8  sideCount[2];		/* ITEM_MENU_STATE's; not named here */
	Uint8  unusedAfterSides[4]; /* no view of the menu state names it */
	Sint32 itemWindow; /* ITEM_MENU_STATE's and SHOP_STATE's; not named here */
	Sint32 window;
	Sint32 unopenedWindow;
	Sint32 infoWindow; /* ITEM_MENU_STATE's; not named here */
	Sint32 listWindow;
	DJINN_PREVIEW_WINDOW *secondWindow;
	/* ITEM_MENU_STATE's last windows and its icons; not named here. */
	Uint8			   itemWindows[16];
	struct WINDOW_SPR *entryIcons[32];
	Uint8  unusedAfterIcons[36]; /* no view of the menu state names it */
	Sint32 helpWindow;
	Uint8  cursorGrid[4];	 /* ITEM_MENU_STATE's cursor grid; not named here */
	void  *memberSprites[8]; /* the party members' figures */
	Sint16 memberX[8];		 /* screen pixels */
	Sint16 memberY[8];		 /* screen pixels */
	FIXED  memberScale[8];	 /* ITEM_MENU_STATE's; not named here */
	Sint16 memberSpriteIds[8]; /* ITEM_MENU_STATE's; not named here */
	Uint16 selection[2];   /* each side's cursor (DJINN_CURSOR_ROW_STRIDE) */
	Uint16 pairEntries[2]; /* each side's chosen list entry */
	struct WINDOW_SPR *secondCursor;
	Uint8 unusedBeforeArrows[4]; /* no view of the menu state names it */
	/* The up and down arrows beside the party row (mnCursorCreate). */
	struct WINDOW_SPR	*pageArrows[2];
	DJINN_MENU_BACKDROP *backdrop;
	Uint8  unusedAfterBackdrop[64]; /* no view of the menu state names it */
	Uint16 entries[32]; /* ITEM_MENU_STATE's items; not named here */
	Uint16 members[DJINN_LIST_MEMBERS]; /* the party in order */
	Uint8  pairCount;					/* not read by name here */
	Uint8  memberCount;
	Uint8  shownMember[2]; /* the party member giving and the one receiving */
	/* ITEM_MENU_STATE's selectedItemIcon; not named here. */
	struct WINDOW_SPR *selectedIcon;
	Uint16			   pending;		   /* DJINN_PENDING bits */
	Sint16			   completionFlag; /* ITEM_MENU_STATE's; not named here */
	void			  *figures[4];	   /* ITEM_MENU_STATE's; not named here */
	Uint16			   slotX[4];
	Uint16			   slotY[4];
	Sint32 figurePhases[4]; /* ITEM_MENU_STATE's figureScales; not named here */
	Uint16 figureFacings[4]; /* ITEM_MENU_STATE's; not named here */
	/* The Djinn chosen on each side: index in their element, element, party
	   member. */
	Uint8  pairIndex[2];
	Uint8  pairElement[2];
	Uint8  pairMember[2];
	Sint16 msgOffset; /* ITEM_MENU_STATE's; not named here */
} DJINN_MENU_STATE;

STATIC_ASSERT(offsetof(DJINN_MENU_BACKDROP, tiles) == 168);
STATIC_ASSERT(offsetof(DJINN_MENU_BACKDROP, tutorialTimer) == 8488);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, window) == 40);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, helpWindow) == 240);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, memberY) == 296);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, backdrop) == 384);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, members) == 516);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, shownMember) == 534);
STATIC_ASSERT(offsetof(DJINN_MENU_STATE, pairMember) == 604);

#endif
