#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

// The screen transition's ticks, windows and state.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

// The screen transition state as the dissolve reads it: the solid tile as bytes, two 4-bit pixels
// to a byte, so that it can set one pixel at a time.
typedef struct SCREEN_TRANSITION_DISSOLVE_STATE
{
	// as SCREEN_TRANSITION_STATE's; the dissolve leaves them
	Uint16 pages[2][322];
	// SCREEN_TRANSITION_STATE's solidTile and mode: the dissolve sets two more of the tile's pixels
	// each tick and copies the tile to BG character block 0.
	Uint8  solidTilePixels[34];
	// The progress, moved from start to end over duration ticks: bit 5 says whether the dissolve
	// fills or clears, the low 5 bits how far it is.
	Uint16 value;
	// As SCREEN_TRANSITION_STATE's; the dissolve leaves them.
	Sint32 centerX;
	Sint32 centerY;
	Sint16 insideLayers;
	Sint16 outsideLayers;
	Uint8  unused;
	// SCREEN_TRANSITION_STATE's page: the dissolve flips it each tick, though nothing reads it
	// while the dissolve runs.
	Uint8  page;
	Sint8  start;    // the value's first step
	Sint8  end;      // the value's last step
	Sint8  duration; // ticks; 0 when the value is not moving
	Sint8  step;     // ticks
} SCREEN_TRANSITION_DISSOLVE_STATE;

// The screen transition state: two pages of window edges for every line, the dissolve's solid tile,
// and the transition's mode and progress.
typedef struct SCREEN_TRANSITION_STATE
{
	// Window edges, two pages: WININ and WINOUT, then a WIN0H and WIN1H pair, each (left << 8 |
	// right), for each of the 160 lines.
	Uint16 pages[2][322];
	// The dissolve's solid tile, one 8 by 8 tile of 4-bit pixels.
	Uint32 solidTile[8];
	Uint16 mode; // the window transition's shape
	// The progress, moved from start to end over duration ticks.
	Uint16 value;
	// 16.16 fixed-point pixels: the centre of the ellipse around a point, set by the Reveal effect.
	FIXED  centerX;
	FIXED  centerY; // as centerX: the target's z less its y
	// WININ for the inside of the shape: both windows show every layer.
	Sint16 insideLayers;
	// WINOUT for the outside: BG0 alone, with the sprite window's layers kept. A mode that inverts
	// its shape writes the two the other way round.
	Sint16 outsideLayers;
	Uint8  unused;
	Uint8  page;     // the pages entry the HBlank DMA copies from
	Sint8  start;    // the value's first step
	Sint8  end;      // the value's last step
	Sint8  duration; // ticks; 0 when the value is not moving
	Sint8  step;     // ticks
	// The mode taken when the value reaches its end; 0 ends the transition.
	Sint8  nextMode;
	// Set by the fade out to the centre line (fdScreenTransitionFinish); while it is set the icon
	// menus leave window 1's lines alone.
	Sint8  centreLine;
} SCREEN_TRANSITION_STATE;

STATIC_ASSERT(offsetof(SCREEN_TRANSITION_DISSOLVE_STATE, value) == 1322);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_DISSOLVE_STATE, start) == 1338);
STATIC_ASSERT(sizeof(SCREEN_TRANSITION_DISSOLVE_STATE) == 1344);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, pages) == 0);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, solidTile) == 1288);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, page) == 1337);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, mode) == 1320);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, centerX) == 1324);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, insideLayers) == 1332);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, nextMode) == 1342);
STATIC_ASSERT(offsetof(SCREEN_TRANSITION_STATE, duration) == 1340);
STATIC_ASSERT(sizeof(SCREEN_TRANSITION_STATE) == 1344);

void                     fdScreenTransitionSuspendCallbacks(void);
SCREEN_TRANSITION_STATE *fdScreenTransitionAllocateAndClearState(void);
void                     fdScreenTransitionUpdate(void);
void                     fdScreenTransitionUpdateFromCenter(void);
void                     fdScreenTransitionUpdateScanline(void);
void                     fdScreenTransitionUpdateDissolve(void);
void                     fdScreenTransitionFillTilemapAndColorTile(Sint32 color);
void                     fdScreenTransitionUpdateScanlineTable(void);
void                     fdEffectStartWindowHBlankDMA(void);

#endif
