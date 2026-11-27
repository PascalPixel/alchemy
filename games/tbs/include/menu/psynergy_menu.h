#ifndef ALCHEMY_PSYNERGY_MENU_H
#define ALCHEMY_PSYNERGY_MENU_H

/* @file */
/* The Psynergy menu's state. */

#include "fixed_math.h"
#include "static_assert.h"
#include "menu/menu.h"
#include "menu/paged_list.h"
#include "system.h"
#include "text/msg_ids.h"
#include "window/window_sprite.h"

struct SPR;

/* The Psynergy menu: its windows, icons and cursors, the party members' tabs */
/* and the Psynergy listed for the chosen party member. */
typedef struct PSYNERGY_MENU_STATE {
	u8 unused[8]; /* no view of the menu state names it (see menu/README.md) */
	s32 selectedMember;		 /* the party member chosen */
	struct WINDOW *moneyWin; /* SHOP_STATE's; not named here */
	struct WINDOW *auxiliaryWin;
	WINDOW_SPR *sideIcon[2]; /* each side's cursor */
	s8 tabIdx[2];			 /* each side's party member, in memberTbl */
	s8 tabCnts[2];			 /* party members each side chooses from */
	struct WINDOW *psynergyWin;
	struct WINDOW *statWin;
	struct WINDOW *shortcutWin;
	struct WINDOW *infoWin;
	struct WINDOW *equipWin; /* ITEM_MENU_STATE's; not named here */
	struct WINDOW *listWin;
	/* ITEM_MENU_STATE's modalWin and the two windows it never opens; */
	/* not named here. */
	u8 itemWins[12];
	WINDOW_SPR *entryGridCursor;
	WINDOW_SPR *entryIcons[32];
	WINDOW_SPR *statIcons[8]; /* ITEM_MENU_STATE's; not named here */
	u8 unusedAfterIcons[36];  /* no view of the menu state names it */
	struct WINDOW *msgWin;
	/* The cursor grid: its place and its size. */
	/* @{ */
	u8 selectedColumn;
	u8 selectedRow;
	u8 columnCnt;
	u8 rowCnt;
	/* @} */
	struct SPR *tabSprs[8]; /* the party members' figures */
	s16 tabX[8];			/* screen pixels */
	s16 rowPositions[8];	/* screen pixels */
	FIXED memberScale[8];	/* 16.16 fixed point, FIXED_ONE full size */
	u16 sideRow[2];			/* each side's Psynergy row chosen */
	u16 sideAction[2];	/* each side's Psynergy chosen, as a party member action
						 */
						/* entry */
	WINDOW_SPR *cursor; /* ITEM_MENU_STATE's; not named here */
	/* DJINN_MENU_STATE's backdrop pointer at 388; no view names the rest. */
	u8 aroundBackdrop[72];
	u16 psynergies[32]; /* the chosen party member's Psynergy listed */
	u16 memberTbl[8];	/* the party in order (gmPartyListMembers) */
	u8 psynergyCnt;
	u8 memberCnt;
	/* The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy */
	/* on the whole party. */
	u8 memberIds[2];
	WINDOW_SPR *cursorIcon;
	/* bits: 1 the list shows the shortcut help | 0 set after a use or a */
	/* cancelled target */
	u16 flg;
	u16 skipSlide; /* 1 skips the cursor's next slide once */
	/* ItemMenuFiguresView's figures and their x; not named here. */
	u8 figureRow[24];
	s16 slotY[4]; /* screen pixels */
	/* ItemMenuFiguresView's phases and DJINN_MENU_STATE's pair; not named here.
	 */
	u8 figurePhases[22];
	s16 useResult;	 /* the use's result message, after MsgItemUseResult */
	s8 equipPreview; /* ITEM_MENU_STATE's; not named here */
	u8 listMode;	 /* ITEM_MENU_STATE's; not named here */
	u8 unusedAfterListMode[2]; /* no view of the menu state names it */
	s8 selectedIdxByMember[8]; /* each party member's entry chosen last */
	u8 mode; /* 0 to use Psynergy; 1 or 2 to set the L or R shortcut */
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
