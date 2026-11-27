#ifndef ALCHEMY_MENU_LIST_H
#define ALCHEMY_MENU_LIST_H

/* The battle menus' shared state, and the sprite shapes the menus queue. */

#include "types.h"

struct WINDOW;

/* A menu cursor's sprite: its render slot link and OAM attributes 0 to 2. */
typedef struct CURSOR_SPR {
	struct CURSOR_SPR *next;
	Uint16			   y : 8;
	Uint16			   affine : 2;
	Uint16			   mode : 2;
	Uint16			   mosaic : 1;
	Uint16			   colors : 1;
	Uint16			   shape : 2;
	Uint16			   x : 9;
	Uint16			   affineIndex : 5;
	Uint16			   size : 2;
	Uint16			   tile : 10;
	Uint16			   priority : 2;
	Uint16			   palette : 4;
	Uint16 affineParam; /* OAM's fourth halfword, the affine parameter slot */
} CURSOR_SPR;

/* A point on the screen, in pixels. */
typedef struct MENU_POINT {
	Sint32 x;
	Sint32 y;
} MENU_POINT;

/* A menu sprite: a list link and its OAM attributes, as two words or as
   fields. */
typedef struct MENU_SPR {
	struct MENU_SPR *next;

	union {
		struct {
			unsigned long attr01;
			unsigned long attr23;
		} word;

		struct {
			Uint16 y : 8;
			Uint16 affine : 2;
			Uint16 mode : 2;
			Uint16 mosaic : 1;
			Uint16 colors : 1;
			Uint16 shape : 2;
			Uint16 x : 9;
			Uint16 affineIndex : 5;
			Uint16 size : 2;
			Uint16 tile : 10;
			Uint16 priority : 2;
			Uint16 palette : 4;
			Uint16 unused;
		} f;
	} oam;
} MENU_SPR;

/* The battle menus' state (HEAP_SLOT_BATTLE_MENU): what the command row and the
   lists remember between them while the party's commands are chosen. */
typedef struct BATTLE_MENU_STATE {
	/* The sprites mnBattleMenuUpdateEachTick draws each tick: the chosen
	   command's icon, the chosen ability's icon and the party member's
	   portrait. */
	MENU_SPR icons[3];
	Uint8	 iconShown[3]; /* whether each of icons is drawn */
	Uint8	 padding;	   /* up to the word targetViewOffset starts on */
	/* Where the battle view slides to (btCameraSetCommandView's vertical
	   offset): -2 or 16 by the target menu's mode, 0 otherwise. */
	Sint32 targetViewOffset;
	/* where the slide has reached; a third of the way a tick */
	Sint32 viewOffset;
	Sint32 row;			 /* the list row chosen last */
	Sint32 page;		 /* the list page chosen last */
	Sint32 preferredRow; /* the row to return to */
	/* The command row's choice: [0] for the party's row (fight, flee, status),
	   [1] for a party member's commands. */
	Sint32 commandSelection[2];
	/* The window that shows the link battle's seconds left, or NULL. */
	struct WINDOW *timerWindow;
	/* Set while the status screen is open, so the timer opens no window. */
	Sint32 timerHidden;
	/* Ticks left to choose commands in a link battle; -1 outside link battles.
	   At 0 a menu ends as if cancelled, or takes the first command. */
	Sint32 timer;
	Sint32 linkTimer; /* 1 while the link battle's timer runs */
	/* The party's standby Djinn by element before each command: what the
	   summons chosen by the earlier commands leave. */
	Uint8  standbyDjinn[8][4];
	Uint8  listSlots[34];	  /* the Psynergy list's action slots, ended by 0 */
	Uint16 listAbilities[33]; /* the Psynergy list's ability ids, ended by 0 */
	/* The Djinn tutorial's step: 0 off, 1 from the start of each turn while
	   FLAG_DJINN_LESSON is set, then one more for each hint shown. */
	Sint32 tutorialStep;
	Sint32 tutorialDelay; /* ticks until the tutorial's next key press */
	/* The party member whose commands are being chosen; -1 between members. */
	Sint32 commandActor;
	/* The Djinni each command has chosen, element << 8 | index, or
	   BATTLE_MENU_NO_DJINNI. */
	Uint32 chosenDjinn[8];
	Sint32 swappedUnit;	 /* the unit a reserve swap replaced, -1 for none */
	Sint32 swappedPlace; /* its party place */
	/* The target mnBattleTargetRunSelection chose last, where it starts
	   again. */
	Sint32 lastTarget;
} BATTLE_MENU_STATE;

/* BATTLE_MENU_STATE.chosenDjinn of a command that has chosen no Djinni. */
#define BATTLE_MENU_NO_DJINNI 0x80000000

/* The party status window and the rectangle it last drew. */
typedef struct PARTY_STAT_WINDOW {
	struct WINDOW *window;
	Uint16		   x;
	Uint16		   y;
	Uint16		   width;
	Uint16		   height;
	Uint16		   flags;
} PARTY_STAT_WINDOW;

/* The rows a battle list window shows. */
enum { BATTLE_LIST_ROWS = 5 };

/* Gives a menu sprite its shape and clears the rest of its attributes, its tile
   and place still to come. The battle lists, the status screen and the command
   row start their sprites so. */
#define MENU_SPR_INIT(sprite, shape) \
	{ \
		(sprite).oam.word.attr01 = (shape); \
		(sprite).oam.word.attr23 = 0; \
		ASM_NOTHING(); \
	}

/* The battle menus' shared state, in its heap slot. */
#define BattleMenuStateWork ((BATTLE_MENU_STATE *)RamHeapSlots->battleMenuState)

/* Restores the previous selection: page, row and return row. */
#define BATTLE_MENU_GET_CURSOR(page_, row_, preferred_) \
	do { \
		(page_) = BattleMenuStateWork->page; \
		(row_) = BattleMenuStateWork->row; \
		(preferred_) = BattleMenuStateWork->preferredRow; \
	} while(0)

/* Saves the selection: page, row and return row. */
#define BATTLE_MENU_SET_CURSOR(page_, row_, preferred_) \
	do { \
		BattleMenuStateWork->page = (page_); \
		BattleMenuStateWork->row = (row_); \
		BattleMenuStateWork->preferredRow = (preferred_); \
	} while(0)

#endif
