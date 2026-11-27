#ifndef ALCHEMY_MENU_INTERNAL_H
#define ALCHEMY_MENU_INTERNAL_H

/* What every part of the menu object shares: its includes, and the macros and
   types more than one part uses. menu.c includes it before the parts, and each
   part includes it too, so that a part read on its own resolves. It places no
   bytes. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/djinn_menu.h"
#include "menu/item_menu.h"
#include "menu/psynergy_menu.h"
#include "menu/status_screen.h"
#include "game/main_routine_state.h"
#include "window/battle_status.h"
#include "window/message_speed.h"

/* Game flags of the menus. Start was pressed: every menu screen unwinds and the
   whole menu closes. */
#define FLAG_MENU_START_PRESSED 0x150
/* The info window holds a message the next selection must not draw over. */
#define FLAG_MENU_INFO_KEPT		0x151

/* The target id the Psynergy screen passes for an action whose range is
   ABILITY_RANGE_ALL. */
#define MENU_TARGET_WHOLE_PARTY 9

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

/* PSYNERGY_MENU_STATE.mode: what choosing a Psynergy does. */
typedef enum PSYNERGY_MENU_MODE {
	PSYNERGY_MODE_USE = 0,
	PSYNERGY_MODE_SET_L = 1, /* make it the L shortcut */
	PSYNERGY_MODE_SET_R = 2	 /* make it the R shortcut */
} PSYNERGY_MENU_MODE;

/* The Psynergy shortcuts (mnPsynergyMenuSetShortcut). */
typedef enum PSYNERGY_SHORTCUT {
	PSYNERGY_SHORTCUT_L = 0,
	PSYNERGY_SHORTCUT_R = 1
} PSYNERGY_SHORTCUT;

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
	/* It works on the field (fdHasItemCueStub), or its action is usable neither
	   in the field nor in battle, and is used once the menu closes. */
	ITEM_USE_MODE_FROM_FIELD = 0,
	ITEM_USE_MODE_CHOOSE_MEMBER = 1, /* choose a party member to use it on */
	/* ABILITY_RANGE_ALL: the whole party at once */
	ITEM_USE_MODE_WHOLE_PARTY = 2
} ITEM_USE_MODE;

/* Whom mnItemMenuSelectTarget chooses. */
typedef enum ITEM_TARGET_MODE {
	ITEM_TARGET_USE = 0, /* the party member to use the item on */
	ITEM_TARGET_GIVE = 1 /* the party member to give it to */
} ITEM_TARGET_MODE;

/* The bytes of the menu state, heap block HEAP_SLOT_MENU. Each screen's run
   function takes the block as it opens and releases it as it closes. */
#define MENU_STATE_SIZE 2672

/* Inventory icons occupy five columns in the fifteen-slot list. */
#define MENU_ITEM_GRID_COLUMNS 5
/* The icon grids are eight wide: mnSpawnIconEntries and
   mnPsynergyMenuCreateEntryGrid make their icons a row at a time. */
#define ICON_GRID_ROW		   8
/* The tile of the Item and Psynergy list windows' top border where the page
   numbers end, with their right arrow (mnPagedListDrawPageNumbers). */
#define PAGE_NUMBERS_RIGHT	   15

/* What mnStatPanelDraw shows below the name and statuses: the low byte of its
   mode. */
typedef enum STAT_PANEL_KIND {
	STATUS_PANEL_EXPERIENCE = 0, /* class, HP, PP and experience */
	/* Stats if the item were equipped, or removed when equipPreview is set.
	   STATUS_PANEL_EQUIP + 1 draws the same. */
	STATUS_PANEL_EQUIP = 2,
	STATUS_PANEL_LEARN = 4, /* the Psynergy a Psynergy item teaches */
	STATUS_PANEL_CLASS = 6, /* class, HP and PP; no call passes it */
	/* Attack, Defense, Agility and Luck, while R is held on the Item screen. */
	STATUS_PANEL_STATS = 8,
} STAT_PANEL_KIND;

/* The panel kind in mnStatPanelDraw's mode; the bits above it are
   STATUS_PANEL_LOWER_ONLY. */
#define STATUS_PANEL_KIND_MASK	0xff
/* Keeps the open window, the name and the statuses, and redraws only the part
   below them. */
#define STATUS_PANEL_LOWER_ONLY 0x100

/* The information side's text: outside Japanese it is drawn character by
   character into the window's tiles, so clearing it redraws the empty window;
   the Japanese edition decodes it into the text canvas (mnTextDrawMessage), so
   clearing it releases the canvas's window sprites. */
#define MENU_RESET_DESCRIPTION_WINDOW \
	JAPANESE_OR(mnWindowSprClearListStub, mnWindowSprRedrawSavedRectStub)
#define MENU_DRAW_DESCRIPTION \
	JAPANESE_OR(mnTextDrawMessageStub, mnTextDrawMessageAsTilesStub)

/* Status labels use the Latin font's pixel positions or the Japanese tile
   grid. */
#define MENU_DRAW_STATUS_LABEL \
	JAPANESE_OR(mnTextDrawStringInWindowStub, mnTextDrawStringAtOffsetStub)

/* Text colours of the status panel's numbers. */
#define TEXT_COLOR_LOW_HP \
	TEXT_COLOR_YELLOW					/* HP under a quarter of the maximum */
#define TEXT_COLOR_NO_HP TEXT_COLOR_RED /* HP 0 */
#define TEXT_COLOR_NORMAL \
	TEXT_COLOR_WHITE /* the window text's default colour */

/* WINDOW_SPR.motion as the menu sets it: shown, or hidden but kept. */
#define ICON_SHOWN	1  /* WINDOW_SPRITE_MOTION_STILL */
#define ICON_HIDDEN 13 /* WINDOW_SPRITE_MOTION_HIDDEN */

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

/* mnDjinnMenuRunPreview's pages for a give or trade, which R cycles; the first
   two also index its per-member page counts. */
typedef enum DJINN_COMPARE_PAGE {
	DJINN_COMPARE_PAGE_GIVER = 0,	 /* the giver's stats and Psynergy */
	DJINN_COMPARE_PAGE_RECEIVER = 1, /* the receiver's stats and Psynergy */
	DJINN_COMPARE_PAGE_BOTH = 2 /* both party members' stats, shown first */
} DJINN_COMPARE_PAGE;

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

/* DJINN_MENU_BACKDROP.tutorialStep: where the Djinn tutorial is. Each lesson's
   own steps follow it: each waits its frames, then presses a key for the player
   and goes on to the next. mnDjinnMenuSelectDjinn runs most;
   mnDjinnMenuRunPreview presses A on each preview. */
typedef enum DJINN_TUTORIAL_STEP {
	DJINN_TUTORIAL_OFF = 0,
	/* Giving a Djinni and setting it: "To give your Djinni to someone
	   else..." */
	DJINN_TUTORIAL_GIVE = 1,
	DJINN_TUTORIAL_GIVE_PICK = 2,	 /* A, taking the Djinni */
	DJINN_TUTORIAL_GIVE_MOVE = 3,	 /* Right, to the next party member */
	DJINN_TUTORIAL_GIVE_DROP = 4,	 /* A there */
	DJINN_TUTORIAL_GIVE_PREVIEW = 5, /* A on the give preview */
	/* "After choosing who will take the Djinni, you can set it." */
	DJINN_TUTORIAL_SET_AFTER_GIVE = 6,
	/* As DJINN_TUTORIAL_SET_AFTER_GIVE; no step leads here. */
	DJINN_TUTORIAL_SET_SPARE = 7,
	DJINN_TUTORIAL_SET_PICK = 8,   /* A, taking the Djinni */
	DJINN_TUTORIAL_SET_CHOOSE = 9, /* A on its own column */
	/* "Setting a Djinni changes your attributes and maybe even your class." */
	DJINN_TUTORIAL_SET_CHANGES = 10,
	/* "When your class changes, the Psynergy you can use may also change." */
	DJINN_TUTORIAL_CLASS_PSYNERGY = 11,
	DJINN_TUTORIAL_SET_PREVIEW = 12, /* A on the set preview */
	DJINN_TUTORIAL_SET_DONE = 13,	 /* B, out of the menu */
	/* The Djinn's powers: "Each Djinni has different powers." */
	DJINN_TUTORIAL_POWERS = 14,
	DJINN_TUTORIAL_POWERS_TALK = 15, /* Flint's three lines on the powers */
	DJINN_TUTORIAL_POWERS_WAIT = 16, /* a wait */
	DJINN_TUTORIAL_POWERS_WAIT_MORE = 17, /* another */
	DJINN_TUTORIAL_POWERS_PICK = 18,	  /* A, taking the Djinni */
	/* As DJINN_TUTORIAL_POWERS_PICK; no step leads here. */
	DJINN_TUTORIAL_POWERS_PICK_SPARE = 20,
	DJINN_TUTORIAL_POWERS_MOVE = 21,	/* Left */
	DJINN_TUTORIAL_POWERS_CHOOSE = 22,	/* A there */
	DJINN_TUTORIAL_POWERS_PREVIEW = 23, /* A on the preview */
	DJINN_TUTORIAL_POWERS_DONE = 24,	/* B, out of the menu */
	DJINN_TUTORIAL_POWERS_END = 25,		/* nothing more is pressed */
	/* Recovery after a summon: "After a Djinni has been used to summon..." */
	DJINN_TUTORIAL_RECOVERY = 27,
	/* Waits for the player's A, then presses B. */
	DJINN_TUTORIAL_CLOSE = 28
} DJINN_TUTORIAL_STEP;

/* mnDjinnMenuSelectDjinn's two choosers. */
typedef enum DJINN_SELECT_MODE {
	DJINN_SELECT_DJINNI = 0, /* a Djinni in any party member's column */
	DJINN_SELECT_ACTION = 1	 /* what to do with the Djinni chosen */
} DJINN_SELECT_MODE;

/* The window map entry of the first element icon, in BASIC_COLORS_PALETTE; the
   four elements follow it in order (ELEMENT). */
#define DJINN_ELEMENT_TILE (BG_TILE_PALETTE(BASIC_COLORS_PALETTE) | 1)

/* The tile of BG character block 1 the Djinn backdrop's border tile is copied
   to (menu_djinn.c); the preview screens blink it (menu_djinn_preview.c). */
#define DJINN_CHANGE_ARROW_TILE 150
#define DJINN_CHANGE_ARROW_VRAM \
	(BG_CHAR_BLOCK(1) + DJINN_CHANGE_ARROW_TILE * TILE_SIZE_4BPP)

/* A Djinn list entry's party member, element and index (DJINN_ENTRY_MEMBER,
   _ELEMENT and _INDEX), shifted down. */
#define DJINN_MEMBER(v)	 (((v) & 0xf00) >> 8)
#define DJINN_ELEMENT(v) (((v) & 0xe0) >> 5)
#define DJINN_NUMBER(v)	 ((v) & 0x1f)

/* The preview loop's four operations, as its confirmation text describes them
   (mnDjinnMenuRunPreview's mode). */
typedef enum DJINN_COMPARE_OPERATION {
	DJINN_COMPARE_TRADE = 0,
	DJINN_COMPARE_GIVE = 1,
	DJINN_COMPARE_STANDBY = 2,
	DJINN_COMPARE_SET = 3
} DJINN_COMPARE_OPERATION;

#endif
