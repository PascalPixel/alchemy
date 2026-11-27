#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

/* The state of a scene change's screen transition: the window edges a window
   transition draws line by line, the tile a dissolve fills dot by dot, and the
   two lines between which the centre-line transition blanks the screen. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* The map grid state as a transition reads it: the display control bits the
   scene shows, and the two lines where the centre-line transition blanks the
   screen and shows it again, with the HBlank interrupt's copies of them. */
typedef struct SCREEN_TRANSITION_DISPLAY {
	/* The map grid state's view, shake and script (WORLD_MAP_VIEW's); no
	   transition reads them. */
	Sint32 *origin;
	Sint32	shakeX, shakeY, shakeDecay;
	Uint8  *script;
	/* Display control bits ORed into DISPCNT; fdScreenTransitionHideBackgrounds
	   clears those of backgrounds 1 to 3. */
	Uint16 dispcnt;
	/* The map grid state's fields from fadeStep to its window rows
	   (WORLD_MAP_VIEW's); no transition reads them. */
	Uint8 viewState[234];
	/* Scanlines: where the screen goes blank, below the screen
	   (CENTER_LINE_HIDDEN_FIRST) while it is wholly shown. */
	Uint16 firstLine;
	/* Scanlines: where the screen shows again, CENTER_LINE_HIDDEN_SECOND while
	   it is wholly shown. */
	Uint16 secondLine;
	/* scanlines; firstLine as the HBlank interrupt reads it */
	Uint16 irqFirstLine;
	/* scanlines; secondLine as the HBlank interrupt reads it */
	Uint16 irqSecondLine;
	/* Where the HBlank interrupt is in the tick, an enum
	   SCREEN_TRANSITION_IRQ_PHASE. */
	Uint16 irqPhase;
} SCREEN_TRANSITION_DISPLAY;

/* One line's horizontal window edges, each a WIN_RANGE value. */
typedef struct WINDOW_LINE {
	Uint16 win0; /* for WIN0H */
	Uint16 win1; /* for WIN1H */
} WINDOW_LINE;

/* A page of the screen transition's window edges
   (SCREEN_TRANSITION_STATE.pages): WININ and WINOUT, then each line's edges. */
typedef struct WINDOW_BUFFER {
	Uint8		windowInOut[4]; /* WININ and WINOUT, not touched here */
	WINDOW_LINE lines[DISPLAY_HEIGHT];
} WINDOW_BUFFER;

/* The screen transition state (HEAP_SLOT_SCREEN_TRANSITION): two pages of
   window edges for every line, the dissolve's solid tile, and the transition's
   shape and progress. */
typedef struct SCREEN_TRANSITION_STATE {
	/* Window edges, two pages: WININ and WINOUT, then a WIN0H and WIN1H pair,
	   each (left << 8 | right), for each of the 160 lines. */
	Uint16 pages[2][322];

	/* The dissolve's solid tile, one 8 by 8 tile of 4-bit pixels: as words, or
	   as bytes of two pixels each, which the dissolve sets two at a time. */
	union {
		Uint32 solidTile[8];
		Uint8  solidTilePixels[32];
	};

	/* The window transition's shape, an WINDOW_TRANSITION_MODE, which
	   fdScreenTransitionUpdateScanlineTable draws. */
	Uint16 mode;
	/* The progress, moved from start to end over duration ticks: 0 is the
	   screen covered, TRANSITION_OPEN fully open and TRANSITION_CLOSED covered
	   again. */
	Uint16 value;
	/* 16.16 fixed-point pixels: the centre of Reveal's rippling ellipse
	   (WINDOW_TRANSITION_RIPPLE), the leader's place on the map. */
	FIXED centerX;
	FIXED centerY; /* as centerX: the leader's z less its height */
	/* The layers shown inside the window: WININ, or WINOUT in the modes that
	   swap the two. */
	Sint16 insideLayers;
	/* The layers shown outside it, BG0 alone when a transition starts: WINOUT,
	   or WININ in the modes that swap the two. */
	Sint16 outsideLayers;
	Uint8  unused;
	Uint8  page;	 /* the pages entry shown; the next is built in the other */
	Sint8  start;	 /* the value's first step */
	Sint8  end;		 /* the value's last step */
	Sint8  duration; /* ticks; 0 when the value is not moving */
	Sint8  step;	 /* ticks */
	/* The mode taken when the value reaches its end; 0 ends the transition. */
	Sint8 nextMode;
} SCREEN_TRANSITION_STATE;

STATIC_ASSERT(offsetof(SCREEN_TRANSITION_DISPLAY, dispcnt) == 20);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_DISPLAY, firstLine) == 256);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_DISPLAY, irqPhase) == 264);
STATIC_ASSERT(sizeof(SCREEN_TRANSITION_DISPLAY) == 268);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, pages) == 0);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, solidTile) == 1288);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, page) == 1337);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, mode) == 1320);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, centerX) == 1324);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, insideLayers) == 1332);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, nextMode) == 1342);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, duration) == 1340);
STATIC_ASSERT(sizeof(SCREEN_TRANSITION_STATE) == 1344);

#endif
