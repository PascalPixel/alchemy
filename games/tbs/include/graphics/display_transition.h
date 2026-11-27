#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

/* @file */
/* The state of a scene change's screen transition: the window edges a window */
/* transition draws line by line, the tile a dissolve fills dot by dot, and the
 */
/* two lines between which the centre-line transition blanks the screen. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/* The map grid state as a transition reads it: the display control bits the */
/* scene shows, and the two lines where the centre-line transition blanks the */
/* screen and shows it again, with the HBlank interrupt's copies of them. */
typedef struct SCR_TRANSITION_DISPLAY {
	/* The map grid state's view, shake and script (WORLD_MAP_VIEW's); no */
	/* transition reads them. */
	/* @{ */
	s32 *origin;
	s32 shakeX, shakeY, shakeDecay;
	u8 *script;
	/* @} */
	/* Display control bits ORed into DISPCNT; fdScrTransitionHideBackgrounds */
	/* clears those of backgrounds 1 to 3. */
	u16 dispcnt;
	/* The map grid state's fields from fadeStep to its window rows */
	/* (WORLD_MAP_VIEW's); no transition reads them. */
	u8 viewState[234];
	/* Scanlines: where the screen goes blank, below the screen */
	/* (CENTER_LINE_HIDDEN_FIRST) while it is wholly shown. */
	u16 firstLine;
	/* Scanlines: where the screen shows again, CENTER_LINE_HIDDEN_SECOND while
	 */
	/* it is wholly shown. */
	u16 secondLine;
	u16 irqFirstLine;  /* scanlines; firstLine as the HBlank interrupt reads it
						*/
	u16 irqSecondLine; /* scanlines; secondLine as the HBlank interrupt reads it
						*/
	/* Where the HBlank interrupt is in the frame, an enum */
	/* SCR_TRANSITION_IRQ_PHASE. */
	u16 irqPhase;
} SCR_TRANSITION_DISPLAY;

/* One line's horizontal window edges, each a WIN_RANGE value. */
typedef struct WINDOW_LINE {
	u16 win0; /* for WIN0H */
	u16 win1; /* for WIN1H */
} WINDOW_LINE;

/* A page of the screen transition's window edges */
/* (SCR_TRANSITION_STATE.pages): WININ and WINOUT, then each line's edges. */
typedef struct WINDOW_BUFFER {
	u8 winInOut[4]; /* WININ and WINOUT, not touched here */
	WINDOW_LINE lines[DISPLAY_HEIGHT];
} WINDOW_BUFFER;

/* The screen transition state (HEAP_SLOT_SCREEN_TRANSITION): two pages of */
/* window edges for every line, the dissolve's solid tile, and the */
/* transition's shape and progress. */
typedef struct SCR_TRANSITION_STATE {
	/* Window edges, two pages: WININ and WINOUT, then a WIN0H and WIN1H pair,
	 */
	/* each (left << 8 | right), for each of the 160 lines. */
	u16 pages[2][322];
	/* The dissolve's solid tile, one 8 by 8 tile of 4-bit pixels: as words, */
	/* or as bytes of two pixels each, which the dissolve sets two at a time. */
	union {
		u32 solidTile[8];
		u8 solidTilePixels[32];
	};
	/* The window transition's shape, an WINDOW_TRANSITION_MODE, which */
	/* fdScrTransitionUpdateScanlineTable draws. */
	u16 mode;
	/* The progress, moved from start to end over duration frames: 0 is the */
	/* screen covered, TRANSITION_OPEN fully open and TRANSITION_CLOSED covered
	 */
	/* again. */
	u16 value;
	/* 16.16 fixed-point pixels: the centre of Reveal's rippling ellipse */
	/* (WINDOW_TRANSITION_RIPPLE), the leader's place on the map. */
	FIXED centerX;
	FIXED centerY; /* as centerX: the leader's z less its height */
	/* The layers shown inside the window: WININ, or WINOUT in the modes that */
	/* swap the two. */
	s16 insideLayers;
	/* The layers shown outside it, BG0 alone when a transition starts: WINOUT,
	 */
	/* or WININ in the modes that swap the two. */
	s16 outsideLayers;
	u8 unused;
	u8 page;	 /* the pages entry shown; the next is built in the other */
	s8 start;	 /* the value's first step */
	s8 end;		 /* the value's last step */
	s8 duration; /* frames; 0 when the value is not moving */
	s8 step;	 /* frames */
	/* The mode taken when the value reaches its end; 0 ends the transition. */
	s8 nextMode;
} SCR_TRANSITION_STATE;

STATIC_ASSERT(offsetof(SCR_TRANSITION_DISPLAY, dispcnt) == 20);
STATIC_ASSERT(offsetof(SCR_TRANSITION_DISPLAY, firstLine) == 256);
STATIC_ASSERT(offsetof(SCR_TRANSITION_DISPLAY, irqPhase) == 264);
STATIC_ASSERT(sizeof(SCR_TRANSITION_DISPLAY) == 268);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, pages) == 0);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, solidTile) == 1288);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, page) == 1337);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, mode) == 1320);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, centerX) == 1324);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, insideLayers) == 1332);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, nextMode) == 1342);
STATIC_ASSERT(offsetof(SCR_TRANSITION_STATE, duration) == 1340);
STATIC_ASSERT(sizeof(SCR_TRANSITION_STATE) == 1344);

#endif
