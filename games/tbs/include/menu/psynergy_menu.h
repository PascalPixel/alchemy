#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

/* The Psynergy menu's state. */

#include "fixed_math.h"
#include "static_assert.h"
#include "menu/menu.h"
#include "menu/paged_list.h"
#include "system.h"
#include "text/msg_ids.h"
#include "window/window_sprite.h"

struct SPR;

/* The Psynergy menu: its windows, icons and cursors, the party members' tabs
   and the Psynergy listed for the chosen party member. */
typedef struct PSYNERGY_MENU_STATE {
	/* no view of the menu state names it (see menu/README.md) */
	Uint8		   unused[8];
	Sint32		   selectedMember; /* the party member chosen */
	struct WINDOW *moneyWindow;	   /* SHOP_STATE's; not named here */
	struct WINDOW *auxiliaryWindow;
	WINDOW_SPR	  *sideIcon[2];	 /* each side's cursor */
	Sint8		   tabIndex[2];	 /* each side's party member, in memberTable */
	Sint8		   tabCounts[2]; /* party members each side chooses from */
	struct WINDOW *psynergyWindow;
	struct WINDOW *statusWindow;
	struct WINDOW *shortcutWindow;
	struct WINDOW *infoWindow;
	struct WINDOW *equipWindow; /* ITEM_MENU_STATE's; not named here */
	struct WINDOW *listWindow;
	/* ITEM_MENU_STATE's modalWindow and the two windows it never opens; not
	   named here. */
	Uint8		itemWindows[12];
	WINDOW_SPR *entryGridCursor;
	WINDOW_SPR *entryIcons[32];
	WINDOW_SPR *statusIcons[8];		  /* ITEM_MENU_STATE's; not named here */
	Uint8		unusedAfterIcons[36]; /* no view of the menu state names it */
	struct WINDOW *msgWindow;
	/* The cursor grid: its place and its size. */
	Uint8		selectedColumn;
	Uint8		selectedRow;
	Uint8		columnCount;
	Uint8		rowCount;
	struct SPR *tabSprites[8];	 /* the party members' figures */
	Sint16		tabX[8];		 /* screen pixels */
	Sint16		rowPositions[8]; /* screen pixels */
	FIXED		memberScale[8];	 /* 16.16 fixed point, toFIXED(1) full size */
	Uint16		sideRow[2];		 /* each side's Psynergy row chosen */
	Uint16
		/* each side's Psynergy chosen, as a party member action entry */
				sideAction[2];
	WINDOW_SPR *cursor; /* ITEM_MENU_STATE's; not named here */
	/* DJINN_MENU_STATE's backdrop pointer at 388; no view names the rest. */
	Uint8  aroundBackdrop[72];
	Uint16 psynergies[32]; /* the chosen party member's Psynergy listed */
	Uint16 memberTable[8]; /* the party in order (gmPartyListMembers) */
	Uint8  psynergyCount;
	Uint8  memberCount;
	/* The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy on
	   the whole party. */
	Uint8		memberIds[2];
	WINDOW_SPR *cursorIcon;
	/* bits: 1 the list shows the shortcut help | 0 set after a use or a
	   cancelled target */
	Uint16 flags;
	Uint16 skipSlide; /* 1 skips the cursor's next slide once */
	/* ItemMenuFiguresView's figures and their x; not named here. */
	Uint8  figureRow[24];
	Sint16 slotY[4]; /* screen pixels */
	/* ItemMenuFiguresView's phases and DJINN_MENU_STATE's pair; not named
	   here. */
	Uint8  figurePhases[22];
	Sint16 useResult;	 /* the use's result message, after MsgItemUseResult */
	Sint8  equipPreview; /* ITEM_MENU_STATE's; not named here */
	Uint8  listMode;	 /* ITEM_MENU_STATE's; not named here */
	Uint8  unusedAfterListMode[2];	 /* no view of the menu state names it */
	Sint8  selectedIndexByMember[8]; /* each party member's entry chosen last */
	Uint8  mode; /* 0 to use Psynergy; 1 or 2 to set the L or R shortcut */
} PSYNERGY_MENU_STATE;

STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, auxiliaryWindow) == 16);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergyWindow) == 32);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, entryGridCursor) == 68);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, entryIcons) == 72);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, msgWindow) == 268);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedColumn) == 272);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedRow) == 273);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, columnCount) == 274);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, rowCount) == 275);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, sideAction[0]) == 376);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergies) == 456);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergyCount) == 536);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, memberIds) == 538);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedIndexByMember) == 608);

STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, skipSlide) == 546);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, tabSprites) == 276);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, memberScale) == 340);

#endif
