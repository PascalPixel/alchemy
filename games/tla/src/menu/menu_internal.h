#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/* What every part of the menu object shares: its includes and the declarations
   its parts disagreed on. menu.c includes it before the parts, and each part
   includes it too, so that a part read on its own still resolves. */

#include "types.h"
#include "battle/battle.h"
#include "battle/battle_effect.h"
#include "battle/battle_system.h"
#include "battle/battle_type.h"
#include "battle/battle_unit.h"
#include "debug/debug.h"
#include "djinn_ids.h"
#include "edition.h"
#include "field/field.h"
#include "field/field_script.h"
#include "field/sprite.h"
#include "fixed_math.h"
#include "flags.h"
#include "game/class_djinn.h"
#include "game/game.h"
#include "game/inventory.h"
#include "game/item.h"
#include "shop/stubs.h"
#include "graphics/display_transition.h"
#include "graphics/glyph.h"
#include "graphics/render_input.h"
#include "graphics/render_placement.h"
#include "graphics/resource.h"
#include "graphics/uiwindow.h"
#include "graphics/vram_tab.h"
#include "item_ids.h"
#include "macros.h"
#include "menu/djinn_menu.h"
#include "menu/menu.h"
#include "menu/paged_list.h"
#include "menu/select.h"
#include "sound/sound.h"
#include "static_assert.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "text/msg_ids.h"
#include "tla_edition.h"
#include "window/item_icon.h"
#include "window/screens.h"
#include "window/window.h"

#define TEXT_COLOR_NORMAL \
	TEXT_COLOR_WHITE /* the window text's default colour */

/* WINDOW_SPR.motion as the menu sets it: shown, or hidden but kept. */
#define ICON_SHOWN	1  /* WINDOW_SPRITE_MOTION_STILL */
#define ICON_HIDDEN 13 /* WINDOW_SPRITE_MOTION_HIDDEN */

/* The menus' list entries, each with an icon sprite. */
#define MENU_ENTRY_ICONS	  32
/* Where an entry icon waits, just past the screen's bottom right corner, until
   a screen places it. */
#define MENU_ICON_HIDDEN_X	  (DISPLAY_WIDTH + 8)
#define MENU_ICON_HIDDEN_Y	  (DISPLAY_HEIGHT + 8)
/* The y a party or Djinn figure is parked at below the screen, where the
   projected placement does not draw it. */
#define MENU_FIGURE_HIDDEN_Y  200
/* The bytes of BG character block 1 the Item menu (in every edition but the
   Japanese) and the script's item picker (in the German, Spanish, French and
   Italian editions) keep aside and fill with a pattern while they are open. */
#define MENU_SAVED_TILE_BYTES 8192

/* The Djinn change mnDjinnMenuDrawStatPreview previews, with the Djinn in the
   state's sides removePairSlot and addPairSlot. */
typedef enum DJINN_PREVIEW_MODE {
	DJINN_PREVIEW_ADD_AND_SET = 0, /* the other's Djinni is added and set */
	DJINN_PREVIEW_UNSET = 1,	   /* the chosen Djinni is unset */
	/* A trade: the own Djinni is unset if set, the other's is added, and set if
	   it was. */
	DJINN_PREVIEW_TRADE = 2,
	DJINN_PREVIEW_NO_CHANGE = 3, /* nothing changes; maximum HP and PP shown */
	DJINN_PREVIEW_ADD = 4 /* the other's Djinni is added, set if it was */
} DJINN_PREVIEW_MODE;

/* What the Djinn menu's choosers return. */
typedef enum DJINN_MENU_RESULT {
	DJINN_RESULT_START = -2, /* Start: the Djinn menu closes */
	DJINN_RESULT_BACK = -1,
	DJINN_RESULT_CHOSEN = 1,	/* A on a Djinni */
	DJINN_RESULT_CONFIRMED = 1, /* A on the preview's "Is this OK?" */
	DJINN_RESULT_SET = 1,
	DJINN_RESULT_STANDBY = 2,
	DJINN_RESULT_GIVE = 3,
	DJINN_RESULT_TRADE = 4,
	/* The four commands applied without their preview, which no chooser
	   returns. */
	DJINN_RESULT_SET_NOW = 5,
	DJINN_RESULT_STANDBY_NOW = 6,
	DJINN_RESULT_STATUS = 7, /* L: the party member's status */
	DJINN_RESULT_GIVE_NOW = 8,
	DJINN_RESULT_TRADE_NOW = 9,
	DJINN_RESULT_HELP = 10 /* Select: the help */
} DJINN_MENU_RESULT;

/* DJINN_MENU_BACKDROP.tutorialStep: where the Djinn tutorial is. The tutorial
   plays the menu itself, one scripted press a step, after a wait counted in
   ticks (mnDjinnMenuSelectDjinn and, on the preview screens,
   mnDjinnMenuRunPreview). */
typedef enum DJINN_TUTORIAL_STEP {
	DJINN_TUTORIAL_OFF = 0,
	/* Giving a Djinni and setting it: "To give your Djinni to someone else,
	   move it like this." */
	DJINN_TUTORIAL_GIVE = 1,
	DJINN_TUTORIAL_GIVE_PICK = 2,	 /* A on the Djinni */
	DJINN_TUTORIAL_GIVE_MOVE = 3,	 /* Right, to the next party member */
	DJINN_TUTORIAL_GIVE_DROP = 4,	 /* A there */
	DJINN_TUTORIAL_GIVE_PREVIEW = 5, /* A on the preview */
	/* "After choosing who gets the Djinni, you can "set" it." */
	DJINN_TUTORIAL_SET = 6,
	/* as DJINN_TUTORIAL_SET; no step leads here */
	DJINN_TUTORIAL_SET_SPARE = 7,
	DJINN_TUTORIAL_SET_PICK = 8,   /* A on the Djinni */
	DJINN_TUTORIAL_SET_CHOOSE = 9, /* A on its own column */
	/* The preview: "Setting a Djinni changes your attributes and sometimes even
	   your class." */
	DJINN_TUTORIAL_SET_CHANGES = 10,
	/* The preview: "When your class changes, the Psynergy you can use may also
	   change." */
	DJINN_TUTORIAL_CLASS_PSYNERGY = 11,
	DJINN_TUTORIAL_SET_PREVIEW = 12, /* A on the preview */
	DJINN_TUTORIAL_SET_DONE = 13,	 /* B, out of the menu */
	/* The Djinn's powers: a wait, then "Each Djinni has different, unique
	   powers." and the two lines after it. */
	DJINN_TUTORIAL_POWERS = 14,
	DJINN_TUTORIAL_POWERS_TALK = 15,
	DJINN_TUTORIAL_POWERS_WAIT = 16,
	DJINN_TUTORIAL_POWERS_WAIT_MORE = 17,
	DJINN_TUTORIAL_POWERS_PICK = 18, /* A on the Djinni */
	/* As DJINN_TUTORIAL_POWERS_PICK; no step leads here. */
	DJINN_TUTORIAL_POWERS_PICK_SPARE = 20,
	DJINN_TUTORIAL_POWERS_MOVE = 21,	/* Left */
	DJINN_TUTORIAL_POWERS_CHOOSE = 22,	/* A there */
	DJINN_TUTORIAL_POWERS_PREVIEW = 23, /* A on the preview */
	DJINN_TUTORIAL_POWERS_DONE = 24,	/* B, out of the menu */
	DJINN_TUTORIAL_POWERS_END = 25,		/* nothing more is pressed */
	/* Recovery after a summon. */
	DJINN_TUTORIAL_RECOVERY = 27,
	/* Waits for the player's A, then presses B. */
	DJINN_TUTORIAL_CLOSE = 28
} DJINN_TUTORIAL_STEP;

/* What mnDjinnMenuGetEntryState says of a Djinni in the list. */
typedef enum DJINNI_STATE {
	DJINNI_STATE_STANDBY = 0,	/* on standby: R sets it */
	DJINNI_STATE_SET = 1,		/* set: R puts it on standby */
	DJINNI_STATE_RECOVERING = 2 /* neither: recovering from a summon */
} DJINNI_STATE;

/* mnDjinnMenuSelectDjinn's two choosers. */
typedef enum DJINN_SELECT_MODE {
	DJINN_SELECT_DJINNI = 0, /* a Djinni in any party member's column */
	DJINN_SELECT_ACTION = 1	 /* what to do with the Djinni chosen */
} DJINN_SELECT_MODE;

/* The tile of BG character block 1 the Djinn backdrop's border tile is copied
   to (menu_djinn.c); the preview screens blink it (menu_djinn_preview.c). */
#define DJINN_BACKDROP_BORDER_TILE 150
#define DJINN_BACKDROP_BORDER_VRAM \
	(BG_CHAR_BLOCK(1) + DJINN_BACKDROP_BORDER_TILE * TILE_SIZE_4BPP)

/* A Djinn list entry's party member, element and index, shifted down. */
#define DJINN_MEMBER(v)			  (((v) & DJINN_ENTRY_MEMBER) >> 8)
#define DJINN_ENTRY_ELEMENT_OF(v) (((v) & DJINN_ENTRY_ELEMENT) >> 5)
#define DJINN_NUMBER(v)			  ((v) & DJINN_ENTRY_INDEX)

/* The preview loop's four operations, as its confirmation text describes them
   (mnDjinnMenuRunPreview's mode). */
typedef enum DJINN_COMPARE_OPERATION {
	DJINN_COMPARE_TRADE = 0,
	DJINN_COMPARE_GIVE = 1,
	DJINN_COMPARE_STANDBY = 2,
	DJINN_COMPARE_SET = 3
} DJINN_COMPARE_OPERATION;

void mnPlaceEntryIconInGrid(WINDOW_SPR *icon, Sint32 idx, Sint32 originX,
	Sint32 originY, Sint32 columns);
void mnPsynergyMenuPlaceEntryIcon(
	WINDOW_SPR **icons, Sint32 idx, Sint32 x, Sint32 y, Sint32 columns);
void mnItemMenuPlaceIcon(
	WINDOW_SPR **icons, Sint32 idx, Sint32 x, Sint32 y, Sint32 columns);
void mnWindowSetBounds(
	WINDOW *win, Sint32 x, Sint32 y, Sint32 width, Sint32 height);

/* The item menu's state, RamHeapSlots->menuState (HEAP_SLOT_MENU): its
   windows, the party figures, the items listed and the two choices a command
   makes. The Broken Seal's ITEM_MENU_STATE, whose names its members take: from
   itemWindow to the entry icons it lies 4 bytes further on than The Broken
   Seal's, and from questionWin on 28 bytes closer, with the figures' ids after
   their scales. The status, Psynergy and Djinn screens read the members they
   share with it through it too. */
typedef struct ITEM_MENU_STATE {
	Uint8	unused[8];		/* no view of the menu state names it */
	Sint32	selectedMember; /* the party member chosen */
	WINDOW *moneyWin;		/* SHOP_STATE's; not named here */
	WINDOW *mainWin;
	/* The two sides: the party member whose items are shown, and the one an
	   item is given to. */
	WINDOW_SPR *sideIcons[2];
	Sint8		sideIdx[2];			 /* the party member chosen, in memberIds */
	Sint8		sideCnt[2];			 /* party members to choose from */
	Uint8		unusedAfterSides[4]; /* no view of the menu state names it */
	WINDOW	   *itemWin;
	WINDOW	   *statWin;
	WINDOW	   *buttonHelpWin;
	WINDOW	   *infoWin;
	WINDOW	   *equipWin;
	WINDOW	   *listWin;
	WINDOW	   *unopenedWin; /* never given a window; only closed */
	WINDOW	   *modalWin;
	WINDOW	   *secondUnopenedWin; /* never given a window; only closed */
	WINDOW_SPR *gridCursor;
	WINDOW_SPR *entryIcons[MENU_ENTRY_ICONS];
	Uint8		unusedAfterIcons[36]; /* no view of the menu state names it */
	WINDOW	   *questionWin;
	/* The cursor grid: its place and its size. */
	Uint8  selectedColumn;
	Uint8  selectedRow;
	Uint8  columnCnt;
	Uint8  rowCnt;
	void  *memberSprs[8];  /* the party members' figures */
	Sint16 memberX[8];	   /* screen pixels */
	Sint16 memberY[8];	   /* screen pixels */
	FIXED  memberScale[8]; /* 16.16 fixed point, toFIXED(1) full size */
	/* The party member each figure shows, -1 for none
	   (mnCreatePartyFigures). */
	Sint16		memberSprIds[8];
	Uint16		selectedSlots[2]; /* each side's inventory slot chosen */
	Uint16		selectedItems[2]; /* each side's inventory entry chosen */
	WINDOW_SPR *cursor;
	Uint8		unusedBeforeArrows[4]; /* no view of the menu state names it */
	/* The up and down arrows beside the party row (mnCursorCreate). */
	WINDOW_SPR *pageArrows[2];
	/* DJINN_MENU_STATE's backdrop pointer at offset 384; no view names the
	   rest. */
	Uint8		aroundBackdrop[68];
	Uint16		items[32];	  /* the inventory entries listed */
	Uint16		memberIds[8]; /* the party in order (gmPartyListMembersStub) */
	Uint8		itemCnt;
	Uint8		partyCnt;
	Uint8		sideMember[2]; /* each side's party member */
	WINDOW_SPR *selectedItemIcon;
	/* bits: 1 the item's name and command grid are drawn (Details sets it in
	   Japanese) */
	Uint16 flg;
	Sint16 completionFlg; /* set to 1 as item commands end */
	/* The row of four figures the Djinn and status screens and the file list
	   show (mnDjinnFiguresInitTopRow): each one's sprite, place in screen
	   pixels and depth, and its scale in 16.16, which closes a quarter of its
	   gap to toFIXED(1) each tick, or holds -scale while negative. */
	void  *figures[4];
	Sint16 figureX[4];
	Sint16 figureY[4];
	FIXED  figureScales[4];
	Uint16 figureFacings[4];
	Uint8  unusedAfterFigures[6]; /* DJINN_MENU_STATE's pair; not named here */
	Sint16 msgOfs;		 /* the item use's result, after MsgItemUseResult */
	Sint8  equipPreview; /* 1 while the stats preview a removal, not an equip */
	Sint8  listMode;	 /* the command chosen last; -1 for none */
	Uint8  unusedAfterListMode[2]; /* no view of the menu state names it */
	Sint8  selectedIdxByMember[8]; /* each party member's entry chosen last */
	Uint8  mode;				   /* PSYNERGY_MENU_STATE's; not named here */
	Uint8  unusedAfterMode[3];	   /* no view of the menu state names it */
	Uint8  shopStock[638];	 /* SHOP_STATE's stock and party; not named here */
	Uint16 resourceSlots[2]; /* VRAM cache slots of two arrow pictures */
	Uint8  shopKeeper[23]; /* SHOP_STATE's cursor and keeper; not named here */
	/* Nonzero while mnUpdatePartyFigures places the party figures (memberSprs);
	   the shops clear it while a details window covers them. */
	Sint8 memberSprsShown;
	/* Nonzero while the row of four figures is drawn
	   (mnDjinnFiguresSetShown). */
	Sint8 figuresShown;
	Uint8 shopEffs[1729]; /* SHOP_STATE's particles; not named here */
	/* The five menu icons (mnIconLoadSlots): a sprite and a VRAM cache slot
	   each, queued while menuIconsShown is nonzero. */
	ICON_SPR menuIcons[5];
	Uint16	 menuIconSlots[5];
	Sint8	 menuIconsShown;
} ITEM_MENU_STATE;

STATIC_ASSERT(offsetof(ITEM_MENU_STATE, itemWin) == 36);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, gridCursor) == 72);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, entryIcons) == 76);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, questionWin) == 240);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberSprs) == 248);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberY) == 296);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberSprIds) == 344);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedSlots) == 360);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, pageArrows) == 376);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, items) == 452);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, sideMember) == 534);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, flg) == 540);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, completionFlg) == 542);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, msgOfs) == 606);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, listMode) == 609);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, selectedIdxByMember) == 612);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, figures) == 544);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, figureFacings) == 592);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, resourceSlots) == 1262);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, memberSprsShown) == 1289);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, menuIcons) == 3020);
STATIC_ASSERT(offsetof(ITEM_MENU_STATE, menuIconsShown) == 3090);

/* The state of the party member chooser and the Status menu (HEAP_SLOT_MENU),
   which each takes as it opens and frees before it returns. The Broken Seal's
   STAT_SCREEN_STATE, laid out as ITEM_MENU_STATE is here. */
typedef struct STAT_SCREEN_STATE {
	Uint8		unused[8];		/* no view of the menu state names it */
	Sint32		selectedMember; /* the party member chosen */
	WINDOW	   *moneyWin;		/* SHOP_STATE's; not named here */
	WINDOW	   *memberWin;
	WINDOW_SPR *memberCursors[2];
	Sint8		memberIdx[2]; /* each side's party member, in memberIds */
	Sint8		memberCnt[2]; /* party members each side chooses from */
	Uint8		unusedAfterSides[4]; /* no view of the menu state names it */
	WINDOW	   *actionWin;
	WINDOW	   *statWin;
	WINDOW	   *unopenedWin; /* never given a window; only cleared */
	WINDOW	   *helpWin;
	/* ITEM_MENU_STATE's equipWin to gridCursor; not named here. */
	Uint8		itemWins[24];
	WINDOW_SPR *entryIcons[MENU_ENTRY_ICONS];
	Uint8		unusedAfterIcons[36]; /* no view of the menu state names it */
	WINDOW	   *selectorWin;
	Uint8		selectedColumn;
	Uint8		selectedRow;
	Uint8		columnCnt;
	Uint8		rowCnt;
	void	   *memberSprs[8];
	Sint16		memberX[8];		 /* screen pixels */
	Sint16		memberY[8];		 /* screen pixels */
	FIXED		memberScale[8];	 /* 16.16 fixed point, toFIXED(1) full size */
	Sint16		memberSprIds[8]; /* ITEM_MENU_STATE's; not named here */
	Uint16		selectedSlots[2];
	Uint16		selectedActions[2];
	WINDOW_SPR *statCursor;
	Uint8		unusedBeforeArrows[4]; /* no view of the menu state names it */
	/* The up and down arrows beside the party row (mnCursorCreate). */
	WINDOW_SPR *pageArrows[2];
	/* DJINN_MENU_STATE's backdrop pointer at offset 384; no view names the
	   rest. */
	Uint8		aroundBackdrop[68];
	Uint16		entries[32];
	Uint16		memberIds[8]; /* the party in order (gmPartyListMembersStub) */
	Uint8		entryCnt;
	Uint8		partyCnt;
	Uint8		selectedIds[2]; /* each side's party member */
	WINDOW_SPR *selectedIcon;
	Uint16 page; /* 3 for the party member chooser, 0 for the Status menu */
	Sint16 completionFlg; /* ITEM_MENU_STATE's; not named here */
	void  *rowSprs[4];
	Sint16 rowX[4]; /* screen pixels */
	Sint16 rowY[4]; /* screen pixels */
} STAT_SCREEN_STATE;

STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, memberWin) == 16);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, actionWin) == 36);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, entryIcons) == 76);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, selectorWin) == 240);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, selectedColumn) == 244);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, memberY) == 296);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, selectedSlots) == 360);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, statCursor) == 368);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, pageArrows) == 376);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, memberIds) == 516);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, selectedIds) == 534);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, page) == 540);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, rowSprs) == 544);
STATIC_ASSERT(offsetof(STAT_SCREEN_STATE, rowX) == 560);
STATIC_ASSERT(sizeof(STAT_SCREEN_STATE) == 576);

/* The Psynergy menu's state (HEAP_SLOT_MENU): its windows, icons and cursors,
   the party members' tabs and the Psynergy listed for the chosen party member.
   The Broken Seal's PSYNERGY_MENU_STATE, laid out as ITEM_MENU_STATE is here.
   The menu's shared functions (mnCursorCreate, mnCursorSlide,
   mnPlacePartyFigures) read their cursor and figures through it, as The Broken
   Seal's do. */
typedef struct PSYNERGY_MENU_STATE {
	Uint8		unused[8];		/* no view of the menu state names it */
	Sint32		selectedMember; /* the party member chosen */
	WINDOW	   *moneyWin;		/* SHOP_STATE's; not named here */
	WINDOW	   *auxiliaryWin;
	WINDOW_SPR *sideIcon[2]; /* each side's cursor */
	Sint8		tabIdx[2];	 /* each side's party member, in memberTbl */
	Sint8		tabCnts[2];	 /* party members each side chooses from */
	Uint8		unusedAfterSides[4]; /* no view of the menu state names it */
	WINDOW	   *psynergyWin;
	WINDOW	   *statWin;
	WINDOW	   *shortcutWin;
	WINDOW	   *infoWin;
	WINDOW	   *equipWin; /* ITEM_MENU_STATE's; not named here */
	WINDOW	   *listWin;
	/* ITEM_MENU_STATE's modal window and the words around it; not named
	   here. */
	Uint8		modalWins[12];
	WINDOW_SPR *entryGridCursor;
	WINDOW_SPR *entryIcons[MENU_ENTRY_ICONS];
	Uint8		unusedAfterIcons[36]; /* no view of the menu state names it */
	WINDOW	   *msgWin;
	Uint8  cursorGrid[4];	/* ITEM_MENU_STATE's cursor grid; not named here */
	void  *tabSprs[8];		/* the party members' figures */
	Sint16 tabX[8];			/* screen pixels */
	Sint16 rowPositions[8]; /* screen pixels */
	FIXED  memberScale[8];	/* 16.16 fixed point, toFIXED(1) full size */
	Sint16 memberSprIds[8]; /* ITEM_MENU_STATE's; not named here */
	Uint16 sideRow[2];		/* each side's Psynergy row chosen */
	/* each side's Psynergy chosen, as a party member action entry */
	Uint16 sideAction[2];
	/* ITEM_MENU_STATE's cursor and the word after it; not named here. */
	Uint8 itemCursor[8];
	/* The up and down arrows beside the party row (mnCursorCreate). */
	WINDOW_SPR *pageArrows[2];
	/* DJINN_MENU_STATE's backdrop pointer at offset 384; no view names the
	   rest. */
	Uint8  aroundBackdrop[68];
	Uint16 psynergies[32]; /* the chosen party member's Psynergy listed */
	Uint16 memberTbl[8];   /* the party in order (gmPartyListMembersStub) */
	Uint8  psynergyCnt;
	Uint8  memberCnt;
	/* The user and the target: [1] is MENU_TARGET_WHOLE_PARTY for Psynergy on
	   the whole party. */
	Uint8		memberIds[2];
	WINDOW_SPR *cursorIcon;
	/* bits: 0 set after a use or a cancelled target */
	Uint16 flg;
	Uint16 skipSlide; /* 1 skips the cursor's next slide once */
	/* ITEM_MENU_STATE's row of figures and the pair after it; not named
	   here. */
	Uint8  figureRow[62];
	Sint16 useResult; /* the use's result message, after MsgItemUseResult */
	/* ITEM_MENU_STATE's equipPreview and listMode; not named here. */
	Uint8 listState[4];
	Sint8 selectedIdxByMember[8]; /* each party member's entry chosen last */
	Uint8 mode; /* 0 to use Psynergy; 1 or 2 to set the L or R shortcut */
	Uint8 unusedAfterMode[3]; /* no view of the menu state names it */
} PSYNERGY_MENU_STATE;

STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, auxiliaryWin) == 16);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergyWin) == 36);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, entryGridCursor) == 72);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, msgWin) == 240);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, tabSprs) == 248);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, memberScale) == 312);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, sideAction) == 364);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, pageArrows) == 376);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, psynergies) == 452);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, memberIds) == 534);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, skipSlide) == 542);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, useResult) == 606);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, selectedIdxByMember) == 612);
STATIC_ASSERT(offsetof(PSYNERGY_MENU_STATE, mode) == 620);

/* The menu object's functions its parts call before their definitions, or with
   declarations of their own that disagree. */
void mnStatPanelDrawHpPp(UNIT *unit, Sint32 win);
void mnTextDrawStatComparison(
	const UNIT *after, const UNIT *before, Sint32 win);
void mnTextDrawMessageAsTilesStub(Sint32 msg, WINDOW *win, Sint32 x, Sint32 y);

/* Tags the parts name in prototypes before the part that defines them. */
struct DJINN_MENU_BACKDROP;

/* What several parts call, declared once here for all of them. */
Sint32 gmInventoryEquipStub(Sint32 owner, Sint32 slot);
void   mnTextDrawStringAtOffsetStub(
	  const Uint8 *text, Sint32 win, Sint32 ofsX, Sint32 ofsY);
void mnTextDrawStringInWindowStub(Uint8 *text, Sint32 win, Uint32 x, Uint32 y);
void mnTextDrawNumAtOffsetStub(
	Sint32 value, Sint32 width, Sint32 win, Sint32 x, Sint32 y);
void mnTextDrawNumInWindowStub(
	Sint32 value, Sint32 width, Sint32 win, Sint32 x, Sint32 y);
Sint32 mnTextSetColStub(Sint32 col);
void   mnAbilityBuildIconTilesStub(
	  Sint32 action, Sint32 withBase, Sint32 *slot, Sint32 *tile, Sint32 reuse);
void mnWindowDrawDividerLineStub(
	Sint32 win, Sint32 x1, Sint32 y1, Sint32 x2, Sint32 y2);
void mnPortraitReloadStub(Sint32 slot, Sint32 key, Sint32 unused);
/* Draws an empty window at tile (x, y), width by height tiles, and frees the
   text tiles the rectangle showed. */
void mnWindowDrawBorderStub(Sint32 x, Sint32 y, Sint32 width, Sint32 height);
/* Erases a rectangle of the windows' tilemap, in tiles, and frees the text
   tiles it showed. */
void mnWindowEraseBorderRectStub(
	Sint32 x, Sint32 y, Sint32 width, Sint32 height);
/* Frees the second range of text tiles and, with flag set, lets text take them
   too. */
void mnTextUseExtraTilesStub(Sint32 flg);
/* Marks the text tiles the tilemap shows and frees those it no longer shows. */
void   mnWindowMarkVisibleTextTilesStub(void);
void   mnMarkChosenMember(Sint32 unused, Sint32 idx, Sint32 cnt);
Sint32 mnMoveGridCursor(Sint32 *selection, Sint32 cnt, Sint32 columns);
/* Loads entry id's icon of kind mode into sprite tile slot slot (a free one
   when -1). */
void mnSelectionIconLoadIntoSlotStub(
	Sint32 mode, Sint32 id, Sint32 slot, Sint32 flg);
Sint32 fdSprStartAnimationStub(void *object, Sint32 argument);
/* Animates the page arrows on a window's top border, from tile offset. */
void   mnPageArrowsDrawStub(Sint32 ofs);
Sint32 mnStatScreenListStatuses(
	Uint8 *ailments, Sint32 requested, Sint32 owner);
void   mnItemMenuDrawEquipment(Sint32 win, Sint32 ownerId, Sint32 mode);
Sint32 mnAbilityApplyToTargets(
	Sint32 abilityId, Sint32 userId, Sint32 tgtId, Sint32 plainPower);
void mnAbilityPlayUseSnd(Sint32 abilityId);
void mnFreeArrows(void);
/* One 4-bit tile (class_change_arrow.4bpp) the Djinn menus copy into the
   backdrop's frame; the Djinn preview blinks it, drawing it and a fill of the
   window background in turn every four ticks. */
extern Uint8 MenuBackdropBorderTile[];
void		 mnWindowSprPrepareForRedrawStub(Sint32 win);
Sint32		 mnMessageAllTypedStub(void);
/* The draw slot of the party and Djinn figures the menus place, among the
   windows' sprites. */
#define MENU_FIGURE_DEPTH (OAM_DEPTH_WINDOW + 10)
/* Places a sprite through the field's projected placement, facing as an actor
   faces, into draw slot depth. */
void fdRenderApplyProjectedPlacementAtDepthStub(SPR *object, SPR_PLACEMENT *pos,
	SPR_SCALE *scale, Sint32 facing, Sint32 depth);

/* The result of using an item or Psynergy from the menu: its message, counted
   from MsgItemUseResult (mnAbilityApplyToTargets). */
typedef enum ITEM_USE_RESULT {
	ITEM_USE_RESULT_HP_FULLY_RECOVERED = 0, /* "You recovered all HP!" */
	ITEM_USE_RESULT_HP_RECOVERED = 1,		/* "You recovered HP!" */
	ITEM_USE_RESULT_HP_NOT_RECOVERED = 2,	/* "You couldn't recover HP." */
	ITEM_USE_RESULT_PARTY_HP_RECOVERED = 3, /* "Everyone recovered HP!" */
	ITEM_USE_RESULT_HP_FULL = 4,			/* "Your HP is maxed out!" */
	ITEM_USE_RESULT_PP_FULLY_RECOVERED = 5, /* "You recovered all PP!" */
	ITEM_USE_RESULT_PP_RECOVERED = 6,		/* "You recovered some PP!" */
	ITEM_USE_RESULT_PP_FULL = 7,			/* "Your PP is maxed out!" */
	ITEM_USE_RESULT_PARTY_PP_RECOVERED = 8, /* "Everyone recovered PP!" */
	/* A second "Your PP is maxed out!", which no use chooses. */
	ITEM_USE_RESULT_PP_FULL_UNUSED = 9,
	ITEM_USE_RESULT_POISON_PURGED = 10, /* "The poison was purged!" */
	ITEM_USE_RESULT_NOT_POISONED = 11,	/* "You're not poisoned." */
	ITEM_USE_RESULT_REVIVED = 12,		/* "You were revived!" */
	ITEM_USE_RESULT_NOT_DOWN = 13,		/* "You're not down." */
	/* "The curse was removed." and "You are not cursed.", which no use
	   chooses. */
	ITEM_USE_RESULT_CURSE_REMOVED = 14,
	ITEM_USE_RESULT_NOT_CURSED = 15,
	ITEM_USE_RESULT_MAX_HP_UP = 16,	 /* "Your maximum HP increased!" */
	ITEM_USE_RESULT_MAX_PP_UP = 17,	 /* "Your maximum PP increased!" */
	ITEM_USE_RESULT_AGILITY_UP = 18, /* "Your Agility increased!" */
	ITEM_USE_RESULT_LUCK_UP = 19,	 /* "Your Luck increased!" */
	ITEM_USE_RESULT_ATTACK_UP = 20,	 /* "Your Attack increased!" */
	ITEM_USE_RESULT_DEFENSE_UP = 21	 /* "Your Defense increased!" */
} ITEM_USE_RESULT;

/* STAT_SCREEN_STATE.page: which screen the party member row belongs to. */
typedef enum STAT_SCREEN_PAGE {
	STATUS_SCREEN_PAGE_STATUS = 0, /* the Status menu (mnStatScreenRun) */
	/* The party member chooser (mnPartyMemberChooserOpen). */
	STATUS_SCREEN_PAGE_CHOOSER = 3
} STAT_SCREEN_PAGE;

/* Which Psynergy mnPsynergyMenuCollectActions lists. */
typedef enum PSYNERGY_LIST_MODE {
	/* Those with a field effect or usable in the field first, then the rest. */
	PSYNERGY_LIST_ALL = 0,
	PSYNERGY_LIST_SHORTCUTS = 1, /* only those that work on the field */
	/* Those with a field effect or usable in the field. */
	PSYNERGY_LIST_FIELD = 2
} PSYNERGY_LIST_MODE;

/* How the item menu can use an item (mnItemClassifyUseMode). */
typedef enum ITEM_USE_MODE {
	/* It cannot be used here: it has no action, it is equipment the party
	   member cannot wear or that teaches Psynergy, or its action is usable only
	   in battle. */
	ITEM_USE_MODE_UNUSABLE = -1,
	/* It works on the field (fdHasPsynergyCueStub), or its action is usable
	   neither in the field nor in battle, and is used once the menu closes. */
	ITEM_USE_MODE_FROM_FIELD = 0,
	ITEM_USE_MODE_CHOOSE_MEMBER = 1, /* choose a party member to use it on */
	/* ABILITY_RANGE_ALL: the whole party at once */
	ITEM_USE_MODE_WHOLE_PARTY = 2
} ITEM_USE_MODE;

/* Inventory icons occupy five columns in the fifteen-slot list. */
#define MENU_ITEM_GRID_COLUMNS 5

/* The item and Psynergy lists' page numbers end, with their right arrow, at
   tile 15 of the list window's top border. */
#define PAGE_NUMBERS_RIGHT 15

/* The x of the item and Psynergy lists' help line, "L R: Change Char"
   (MsgChangeCharacterHelp), on the list window's bottom row: the Japanese line
   starts further in. */
#define CHANGE_HELP_X JAPANESE_OR(16, 0)

/* The statuses the Status screens show, as mnStatScreenListStatuses fills them:
   one byte each, nonzero while the party member has it. */
typedef enum UNIT_STAT {
	CHARACTER_DOWN,	  /* HP 0; listed only when the caller asks for it */
	CHARACTER_POISON, /* UNIT.poison 1 */
	CHARACTER_VENOM,  /* UNIT.poison any other nonzero value */
	CHARACTER_CURSE,  /* UNIT.curse nonzero */
	CHARACTER_HAUNT,  /* UNIT.haunt nonzero */
	CHARACTER_AILMENT_COUNT
} UNIT_STAT;

/* Status labels use the Latin font's pixel positions or the Japanese tile
   grid. */
#define MENU_DRAW_STATUS_LABEL \
	JAPANESE_OR(mnTextDrawStringInWindowStub, mnTextDrawStringAtOffsetStub)

/* mnDjinnMenuRunPreview's pages for a give or trade, which R cycles; the first
   two also index its per-member page counts. */
typedef enum DJINN_COMPARE_PAGE {
	DJINN_COMPARE_PAGE_GIVER = 0,	 /* the giver's stats and Psynergy */
	DJINN_COMPARE_PAGE_RECEIVER = 1, /* the receiver's stats and Psynergy */
	DJINN_COMPARE_PAGE_BOTH = 2 /* both party members' stats, shown first */
} DJINN_COMPARE_PAGE;

/* The Japanese status panel's PP row, "EP    /": label and slash in one
   string, drawn by the unit summary. */
extern const Uint8 StatusPanelPpRowString[];
/* The Japanese status panel's HP row, "HP    /", the slash already in its
   cell at 48. */
extern const Uint8 StatusPanelHpRowString[];
/* The HP label outside Japan, in each language's form ("KP" in German, "PV" in
   Spanish and French, "PS" in Italian). */
extern const Uint8 StatusPanelHpString[];
/* The PP label outside Japan, which every language writes "PP". */
extern const Uint8 StatusPanelPpString[];
/* The slash between a current and maximum figure on the status panel, outside
   Japan. */
extern const Uint8 StatusPanelSlashString[];
/* Draws a number right-aligned in a field ending at pixel x of a window: the HP
   and PP figures outside Japan. */
void mnTextDrawNumRightAligned(Sint32 num, Sint32 win, Sint32 x, Sint32 y);

#endif
