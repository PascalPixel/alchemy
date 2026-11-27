#ifndef ALCHEMY_ITEM_MENU_H
#define ALCHEMY_ITEM_MENU_H

/* @file */
/* The item menu's state and commands. */

#include "fixed_math.h"
#include "static_assert.h"
#include "stubs.h"
#include "menu/menu.h"
#include "menu/paged_list.h"
#include "system.h"
#include "text/msg_ids.h"
#include "window/window_sprite.h"

struct SPR;

struct WINDOW;

/* The icons the menus keep for their entries: items, Psynergy and Djinn. */
#define MENU_ENTRY_ICONS 32

/* Where an entry or category icon waits, just past the screen's bottom right */
/* corner, until a screen places it. */
#define MENU_ICON_HIDDEN_X (DISPLAY_WIDTH + 8)
#define MENU_ICON_HIDDEN_Y (DISPLAY_HEIGHT + 8)
/* The y a party or Djinni figure is parked at, below the screen, while a */
/* screen has no place for it. */
#define MENU_FIGURE_HIDDEN_Y 200

/* The item menu's state, in the HEAP_SLOT_MENU heap slot: its windows, the */
/* party figures, the items listed and the two choices a command makes. */
typedef struct ITEM_MENU_STATE {
	u8 unused[8]; /* no view of the menu state names it (see menu/README.md) */
	s32 selectedMember;		 /* the party member chosen */
	struct WINDOW *moneyWin; /* SHOP_STATE's; not named here */
	struct WINDOW *mainWin;
	/* The two sides: the party member whose items are shown, and the one an
	 * item */
	/* is given to. */
	/* @{ */
	WINDOW_SPR *sideIcons[2];
	s8 sideIdx[2]; /* the party member chosen, in memberIds; -1 for none at */
				   /* first */
	s8 sideCnt[2]; /* party members to choose from; 1 at first */
	/* @} */
	struct WINDOW *itemWin;
	struct WINDOW *statWin;
	struct WINDOW *buttonHelpWin;
	struct WINDOW *descriptionWin;
	struct WINDOW *equipWin;
	struct WINDOW *listWin;
	struct WINDOW *unopenedWin; /* never given a window; only closed */
	struct WINDOW *modalWin;
	struct WINDOW *secondUnopenedWin; /* never given a window; only closed */
	WINDOW_SPR *gridCursor;			  /* only prepared by name */
	WINDOW_SPR *entryIcons[MENU_ENTRY_ICONS];
	WINDOW_SPR *statIcons[8];
	u8 unusedAfterIcons[36]; /* no view of the menu state names it */
	struct WINDOW *questionWin;
	/* The cursor grid: its place and its size, 1 by 1 at first. */
	/* @{ */
	u8 selectedColumn;
	u8 selectedRow;
	u8 columnCnt;
	u8 rowCnt;
	/* @} */
	struct SPR *memberSprs[8]; /* the party members' figures */
	s16 memberX[8];			   /* screen pixels */
	s16 memberY[8];			   /* screen pixels */
	FIXED memberScale[8];	   /* 16.16 fixed point, FIXED_ONE full size */
	u16 selectedSlots[2];	   /* each side's inventory slot chosen */
	u16 selectedItems[2];	   /* each side's inventory entry chosen */
	WINDOW_SPR *cursor;
	/* DJINN_MENU_STATE's backdrop pointer at 388; no view names the rest. */
	u8 aroundBackdrop[72];
	u16 items[32];	  /* the inventory entries listed */
	u16 memberIds[8]; /* the party in order (gmPartyListMembers) */
	u8 itemCnt;
	u8 partyCnt;
	u8 sideMember[2]; /* each side's party member */
	WINDOW_SPR *selectedItemIcon;
	u16 flg;		   /* 1 while the item's name and command grid are drawn */
	s16 completionFlg; /* set to 1 as item commands end; not read by name */
	/* The row of four figures the Djinn and status screens show */
	/* (menu_djinn_figures.c): each one's sprite and place in screen pixels, and
	 */
	/* its scale in 16.16, which closes a third of its gap to FIXED_ONE each */
	/* frame, or holds -scale while negative. */
	/* @{ */
	struct SPR *figures[4];
	s16 figureX[4];
	s16 figureY[4];
	s32 figureScales[4];
	/* @} */
	u8 unusedAfterFigures[6]; /* DJINN_MENU_STATE's pair; not named here */
	s16 msgOfs;		 /* the item use's result, after MsgItemUseResult */
	s8 equipPreview; /* 1 while the stats preview a removal, not an equip */
	u8 listMode; /* the command chosen last (ITEM_MENU_COMMAND); -1 for none */
	u8 unusedAfterListMode[2]; /* no view of the menu state names it */
	s8 selectedIdxByMember[8]; /* each party member's entry chosen last */
	u8 mode; /* not named through this struct; PSYNERGY_MENU_STATE.mode */
	u8 shopStock[297];	  /* SHOP_STATE's stock, party and cursor; not named */
						  /* here */
	u16 resourceSlots[2]; /* VRAM cache slots of two arrow pictures */
} ITEM_MENU_STATE;

STATIC_ASSERT(offsetof(ITEM_MENU_STATE, sideIcons) == 20);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, sideIdx) == 28);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, sideCnt) == 30);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, itemWin) == 32);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, descriptionWin) == 44);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, gridCursor) == 68);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, entryIcons) == 72);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, statIcons) == 200);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, questionWin) == 268);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedColumn) == 272);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedRow) == 273);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, columnCnt) == 274);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, rowCnt) == 275);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberSprs) == 276);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberX) == 308);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberY) == 324);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberScale) == 340);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedSlots) == 372);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedItems) == 376);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, cursor) == 380);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, items) == 456);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberIds) == 520);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, itemCnt) == 536);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, sideMember) == 538);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedItemIcon) == 540);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, figures) == 548);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, msgOfs) == 602);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, equipPreview) == 604);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedIdxByMember) == 608);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, mode) == 616);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, resourceSlots) == 914);
STATIC_ASSERT(sizeof(ITEM_MENU_STATE) == 920);

/* The item command grid, by row * 3 + column: Use, Equip and Details on top, */
/* Give, Remove and Drop below (mnItemMenuChooseCommand). */
typedef enum ITEM_MENU_COMMAND {
	ITEM_COMMAND_USE,
	ITEM_COMMAND_EQUIP,
	ITEM_COMMAND_DETAILS, /* Details */
	ITEM_COMMAND_GIVE,
	ITEM_COMMAND_REMOVE,
	ITEM_COMMAND_DROP
} ITEM_MENU_COMMAND;

#endif
