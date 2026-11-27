#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

/* @file */
/* Menu lists: sprites, cursors, the party status window and paged selections.
 */

#include "edition.h"
#include "static_assert.h"
#include "tbs_edition.h"
#include "types.h"
#include "stubs.h"
#include "graphics/battle_sprite_id.h"
#include "system/globals.h"
#include "window/screens.h"
#include "window/window.h"
#include "window/window_sprite.h"

/* The items a battle's item list shows at once, two tiles a row. */
enum { BATTLE_LIST_ROWS = 5 };

/* A menu sprite: a list link and its OAM attributes, as two words or as */
/* fields. */
typedef struct MENU_SPR {
	struct MENU_SPR *next;
	union {
		struct {
			unsigned long attr01;
			unsigned long attr23;
		} word;
		struct {
			u16 y : 8;
			u16 affine : 2;
			u16 mode : 2;
			u16 mosaic : 1;
			u16 cols : 1;
			u16 shape : 2;
			u16 x : 9;
			u16 affineIdx : 5;
			u16 size : 2;
			u16 tile : 10;
			u16 pri : 2;
			u16 pal : 4;
			u16 unused;
		} f;
	} oam;
} MENU_SPR;

/* A cursor or prompt sprite: MENU_SPR's layout with its OAM fields named */
/* directly, for code that never writes the attributes as words. */
typedef struct CURSOR_SPR {
	struct CURSOR_SPR *next;
	u16 y : 8;
	u16 affine : 2;
	u16 mode : 2;
	u16 mosaic : 1;
	u16 cols : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affineIdx : 5;
	u16 size : 2;
	u16 tile : 10;
	u16 pri : 2;
	u16 pal : 4;
	u16 unused;
} CURSOR_SPR;

/* Sets sprite shape (attr0/attr1) and clears the remaining fields. */
#define syMenuSprInit(sprite, shape)                                           \
	do {                                                                       \
		(sprite).oam.word.attr01 = (shape);                                    \
		(sprite).oam.word.attr23 = 0;                                          \
	} while(0)

/* The party status window's layout: its window, its place and size, and what */
/* it shows (PARTY_STATUS_ flags). */
typedef struct PARTY_STAT_WINDOW {
	WINDOW *win;
	u16 x;		/* tiles */
	u16 y;		/* tiles */
	u16 width;	/* tiles */
	u16 height; /* tiles */
	u16 flg;	/* PARTY_STATUS_ flags */
} PARTY_STAT_WINDOW;

STATIC_ASSERT(sizeof(PARTY_STAT_WINDOW) == 16);
STATIC_ASSERT(offsetof(PARTY_STAT_WINDOW, flg) == 12);

/* A point on the screen where a menu shows its cursor. */
typedef struct MENU_POINT {
	s32 x; /* screen pixels */
	s32 y; /* screen pixels */
} MENU_POINT;

/* The sprites BATTLE_MENU_STATE.icons holds. */
typedef enum BATTLE_MENU_ICON {
	BATTLE_MENU_ICON_COMMAND,  /* the chosen command's icon, 32x32 */
	BATTLE_MENU_ICON_ABILITY,  /* the chosen Psynergy, summon or item's icon */
	BATTLE_MENU_ICON_PORTRAIT, /* the party member's portrait */
	BATTLE_MENU_ICON_COUNT
} BATTLE_MENU_ICON;

/* The battle command menus' state, HEAP_SLOT_BATTLE_MENU (260 bytes), reached
 */
/* as BattleMenuStateWork. */
typedef struct BATTLE_MENU_STATE {
	/* The sprites mnBattleMenuUpdate draws each frame: the chosen */
	/* command's icon, the chosen ability's icon and the party member's
	 * portrait. */
	MENU_SPR icons[BATTLE_MENU_ICON_COUNT];
	u8 iconShown[BATTLE_MENU_ICON_COUNT]; /* whether each of icons is drawn */
	u8 padding; /* up to the word tgtViewOfs starts on */
	/* Where the battle view slides to (btCameraSetCommandView's vertical */
	/* offset): -2 or 16 by the target menu's mode, 0 otherwise. */
	s32 tgtViewOfs;
	s32 viewOfs;	  /* where the slide has reached; a third of the way a frame
					   */
	s32 row;		  /* the list row chosen last */
	s32 page;		  /* the list page chosen last */
	s32 preferredRow; /* the row to return to */
	/* The command row's choice: [0] for the party's row (fight, flee, */
	/* status), [1] for a party member's commands. */
	s32 commandSelection[2];
	/* The window that shows the link battle's seconds left, or NULL. */
	WINDOW *tmrWin;
	/* Set while the status screen is open, so the timer opens no window. */
	s32 tmrHidden;
	/* Frames left to choose commands in a link battle; -1 outside link battles.
	 */
	/* At 0 a menu ends as if cancelled, or takes the first command. */
	s32 tmr;
	s32 linkTmr; /* 1 while the link battle's timer runs */
	/* The party's standby Djinn by element before each command: what the */
	/* summons chosen by the earlier commands leave. */
	u8 standbyDjinn[8][4];
	u8 listSlots[34];	   /* the Psynergy list's action slots, ended by 0 */
	u16 listAbilities[33]; /* the Psynergy list's ability ids, ended by 0 */
	/* The Djinn tutorial's step: 0 off, 1 from the start of each turn while */
	/* FLAG_DJINN_LESSON is set, then one more for each hint shown. */
	s32 tutorialStep;
	s32 tutorialDelay; /* frames until the tutorial's next key press */
	/* The party member whose commands are being chosen; -1 between members. */
	s32 commandActor;
	/* The Djinni each command has chosen, element << 8 | index, or */
	/* BATTLE_MENU_NO_DJINNI. */
	u32 chosenDjinn[8];
} BATTLE_MENU_STATE;

/* BATTLE_MENU_STATE.chosenDjinn of a command that has chosen no Djinni. */
#define BATTLE_MENU_NO_DJINNI 0x80000000

STATIC_ASSERT(sizeof(BATTLE_MENU_STATE) == 260);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, tgtViewOfs) == 40);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, row) == 48);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, commandSelection) == 60);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, tmrWin) == 68);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, tmr) == 76);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, standbyDjinn) == 84);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, tutorialStep) == 216);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, commandActor) == 224);
STATIC_ASSERT(offsetof(BATTLE_MENU_STATE, chosenDjinn) == 228);

/* Restores the previous selection: page, row and return row. */
#define BATTLE_MENU_GET_CURSOR(page_, row_, preferred_)                        \
	do {                                                                       \
		(page_) = BattleMenuStateWork->page;                                   \
		(row_) = BattleMenuStateWork->row;                                     \
		(preferred_) = BattleMenuStateWork->preferredRow;                      \
	} while(0)

/* Saves the selection: page, row and return row. */
#define BATTLE_MENU_SET_CURSOR(page_, row_, preferred_)                        \
	do {                                                                       \
		BattleMenuStateWork->page = (page_);                                   \
		BattleMenuStateWork->row = (row_);                                     \
		BattleMenuStateWork->preferredRow = (preferred_);                      \
	} while(0)

#define BattleMenuStateWork                                                    \
	(*(BATTLE_MENU_STATE **)&HeapSlots[HEAP_SLOT_BATTLE_MENU])
/* The animated cursor's frames, 16 by 16 at a byte a pixel */
/* (graphics/ui/sprite_cursors/spin). */
extern const u8 Menu_AnimatedCursorTiles[];

#endif
