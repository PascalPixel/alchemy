#ifndef ALCHEMY_STATUS_SCREEN_H
#define ALCHEMY_STATUS_SCREEN_H

/* @file */
/* The Status screen's state, and the statuses it shows. */

#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/sprite.h"
#include "menu/menu.h"
#include "system.h"
#include "window/screens.h"
#include "window/window.h"

struct UNIT;

/* The state of the party member chooser and the Status menu, in the */
/* HEAP_SLOT_MENU heap slot, which each takes as it opens and frees before it */
/* returns. */
typedef struct STAT_SCR_STATE {
	u8 unused[8]; /* no view of the menu state names it (see menu/README.md) */
	s32 selectedMember; /* the party member chosen */
	WINDOW *moneyWin;	/* SHOP_STATE's; not named here */
	WINDOW *memberWin;
	WINDOW_SPR *memberCursors[2];
	s8 memberIdx[2]; /* each side's party member, in memberIds */
	s8 memberCnt[2]; /* party members each side chooses from */
	WINDOW *actionWin;
	WINDOW *statWin;
	WINDOW *unopenedWin; /* never given a window; only cleared */
	WINDOW *helpWin;
	/* ITEM_MENU_STATE's equipWin to gridCursor; not named here. */
	u8 itemWins[24];
	WINDOW_SPR *entryIcons[32];
	WINDOW_SPR *statIcons[8];
	u8 unusedAfterIcons[36]; /* no view of the menu state names it */
	WINDOW *selectorWin;
	u8 selectedColumn;
	u8 selectedRow;
	u8 columnCnt;
	u8 rowCnt;
	SPR *memberSprs[8];
	s16 memberX[8];		  /* screen pixels */
	s16 memberY[8];		  /* screen pixels */
	FIXED memberScale[8]; /* 16.16 fixed point, FIXED_ONE full size */
	u16 selectedSlots[2];
	u16 selectedActions[2];
	WINDOW_SPR *statCursor;
	/* DJINN_MENU_STATE's backdrop pointer at 388; no view names the rest. */
	u8 aroundBackdrop[72];
	u16 entries[32];
	u16 memberIds[8]; /* the party in order (gmPartyListMembers) */
	u8 entryCnt;
	u8 partyCnt;
	u8 selectedIds[2]; /* each side's party member */
	WINDOW_SPR *selectedIcon;
	u16 page; /* 3 for the party member chooser, 0 for the Status menu */
	s16 completionFlg; /* ITEM_MENU_STATE's; not named here */
	SPR *rowSprs[4];
	s16 rowX[4]; /* screen pixels */
	s16 rowY[4]; /* screen pixels */
} STAT_SCR_STATE;

STATIC_ASSERT(offsetof(STAT_SCR_STATE, memberWin) == 16);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, memberCursors) == 20);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, statWin) == 36);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, entryIcons) == 72);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, selectorWin) == 268);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, selectedColumn) == 272);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, memberSprs) == 276);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, memberY) == 324);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, selectedSlots) == 372);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, statCursor) == 380);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, memberIds) == 520);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, selectedIds) == 538);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, page) == 544);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, rowSprs) == 548);
STATIC_ASSERT(offsetof(STAT_SCR_STATE, rowX) == 564);
STATIC_ASSERT(sizeof(STAT_SCR_STATE) == 580);

/* The statuses the Status screens show, as mnStatScrListStatuses */
/* fills them: one byte each, nonzero while the party member has it. */
typedef enum UNIT_STAT {
	CHARACTER_DOWN,	  /* HP 0; listed only when the caller asks for it */
	CHARACTER_POISON, /* UNIT.poison 1 */
	CHARACTER_VENOM,  /* UNIT.poison any other nonzero value */
	CHARACTER_CURSE,  /* UNIT.curse nonzero */
	CHARACTER_HAUNT,  /* UNIT.haunt nonzero */
	CHARACTER_AILMENT_COUNT
} UNIT_STAT;

#endif
