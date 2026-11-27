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
	struct WINDOW *moneyWin;	   /* SHOP_STATE's; not named here */
	struct WINDOW *auxiliaryWin;
	WINDOW_SPR	  *sideIcon[2]; /* each side's cursor */
	Sint8		   tabIdx[2];	/* each side's party member, in memberTbl */
	Sint8		   tabCnts[2];	/* party members each side chooses from */
	struct WINDOW *psynergyWin;
	struct WINDOW *statWin;
	struct WINDOW *shortcutWin;
	struct WINDOW *infoWin;
	struct WINDOW *equipWin; /* ITEM_MENU_STATE's; not named here */
	struct WINDOW *listWin;
	/* ITEM_MENU_STATE's modalWin and the two windows it never opens; not named
	   here. */
	Uint8		itemWins[12];
	WINDOW_SPR *entryGridCursor;
	WINDOW_SPR *entryIcons[32];
	WINDOW_SPR *statIcons[8];		  /* ITEM_MENU_STATE's; not named here */
	Uint8		unusedAfterIcons[36]; /* no view of the menu state names it */
	struct WINDOW *msgWin;
	/* The cursor grid: its place and its size. */
	Uint8		selectedColumn;
	Uint8		selectedRow;
	Uint8		columnCnt;
	Uint8		rowCnt;
	struct SPR *tabSprs[8];		 /* the party members' figures */
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
	Uint16 memberTbl[8];   /* the party in order (gmPartyListMembers) */
	Uint8  psynergyCnt;
	Uint8  memberCnt;
	/* The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy on
	   the whole party. */
	Uint8		memberIds[2];
	WINDOW_SPR *cursorIcon;
	/* bits: 1 the list shows the shortcut help | 0 set after a use or a
	   cancelled target */
	Uint16 flg;
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
	Uint8  unusedAfterListMode[2]; /* no view of the menu state names it */
	Sint8  selectedIdxByMember[8]; /* each party member's entry chosen last */
	Uint8  mode; /* 0 to use Psynergy; 1 or 2 to set the L or R shortcut */
} PSYNERGY_MENU_STATE;

STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, auxiliaryWin) == 16);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergyWin) == 32);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, entryGridCursor) == 68);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, entryIcons) == 72);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, msgWin) == 268);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedColumn) == 272);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedRow) == 273);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, columnCnt) == 274);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, rowCnt) == 275);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, sideAction[0]) == 376);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergies) == 456);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergyCnt) == 536);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, memberIds) == 538);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedIdxByMember) == 608);

STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, skipSlide) == 546);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, tabSprs) == 276);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, memberScale) == 340);

#endif
