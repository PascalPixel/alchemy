#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

/* A window and the messages that type text into it. */

#include "render_input.h"

/* A window: a bordered rectangle of the tilemap with its sprite outputs, its
   opening or closing animation and the rectangle it last drew. */
typedef struct WINDOW {
	WINDOW_SPR_LIST sprs;	/* the sprites the window carries */
	Uint16			width;	/* tiles */
	Uint16			height; /* tiles */
	Sint16			x;		/* tiles */
	Sint16			y;		/* tiles */
	Uint16			opened; /* 1 once opened, 0 once freed; nothing reads it */
	/* The animation a message's window-state code sets. */
	Uint16 anm;
	/* 0 while a message is typing into the window; nonzero once it is done,
	   which frees the message for the next text. */
	Uint16 state;
	Uint16 flg; /* 0 once closing or free */
	/* The zoom's stage: counts down while opening, up to duration while
	   closing. */
	Sint16 zoomStage;

	Sint16 duration;   /* frames of the zoom under way; 0 when none */
	Sint16 prevX;	   /* tiles */
	Sint16 prevY;	   /* tiles */
	Sint16 prevWidth;  /* tiles */
	Sint16 prevHeight; /* tiles */
} WINDOW;

STATIC_ASSERT(sizeof(WINDOW) == 36);
STATIC_ASSERT(offsetof(WINDOW, width) == 8);
STATIC_ASSERT(offsetof(WINDOW, zoomStage) == 24);
STATIC_ASSERT(offsetof(WINDOW, prevHeight) == 34);

/* A message, one of the three in WIN_STATE.msgs, which types decoded text into
   a window a few characters a frame and runs its control codes. */
typedef struct MESSAGE {
	WINDOW *win; /* the window typed into; NULL when free */
	Uint16	x;	 /* pen, 8.8 fixed-point pixels */
	Uint16	y;	 /* pen, 8.8 fixed-point pixels */
	/* Extra advance for each line of the page, in 1/256 pixels: after every
	   character in Japanese, after a space in the other editions but German,
	   which takes none. */
	Uint16 spacing[4];
	Uint16 line;		/* line of the page, indexes spacing; at most 3 */
	Uint16 textIdx;		/* next code in WIN_STATE.textBuf */
	Uint16 countdown;	/* frames left of a pause, a page wait or a swell */
	Uint16 colour;		/* text colour; TEXT_COLOR_WHITE at start */
	Uint16 outline;		/* an TXT_STYLE; TEXT_STYLE_PLAIN at start */
	Uint16 lineSpacing; /* TEXT_DEFAULT_LINE_SPACING at start */
	Uint16 scrollFrm;	/* frames left of the canvas scrolling up a line */
	Uint16 startX; /* pen x each new line returns to, 8.8 fixed-point pixels */
	Uint16 ended;  /* 1 once a TEXT_END code is reached */
	/* frames to wait after a glyph while no button is held */
	Uint16 glyphDelay;
	Uint16 flg; /* nonzero keeps the window's size from page to page */
	/* Copied from spacing[3] when the message starts; with WIN_STATE.textMode
	   set it places the glyphs by y / 320. */
	Uint16 compressLines;
} MESSAGE;

STATIC_ASSERT(sizeof(MESSAGE) == 40);
STATIC_ASSERT(offsetof(MESSAGE, textIdx) == 18);
STATIC_ASSERT(offsetof(MESSAGE, countdown) == 20);
STATIC_ASSERT(offsetof(MESSAGE, scrollFrm) == 28);
STATIC_ASSERT(offsetof(MESSAGE, flg) == 36);

/* Marks the 4-row band of the window tilemap that holds `row` for upload
   (WIN_STATE.dirty): the battle lists mark their top border's row so, and the
   Djinn list its preview's. */
#define WINDOW_MARK_TILEMAP_ROW(state, row) \
	do { \
		(state)->dirty |= 2 << ((Uint32)(row) >> 2); \
	} while(0)

#endif
