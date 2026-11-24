#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

/// @file
/// The state of a scene change's screen transition: the window edges a window
/// transition draws line by line, the tile a dissolve fills dot by dot, and the
/// two lines between which the centre-line transition blanks the screen.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/// The screen transition state as the dissolve and the centre-line transition
/// read it: the same block as ScreenTransitionState, with its solid tile read
/// a byte (two pixels) at a time.
struct ScreenTransitionDissolveState {
  u16 pages[2][322]; ///< ScreenTransitionState's pages, unread here
  /// ScreenTransitionState's solidTile, as 4-bit pixels two a byte: the
  /// dissolve sets two more each frame and copies the tile to BG character
  /// block 0.
  u8 solidTilePixels[34];
  /// The progress, moved from start to end over duration frames: the
  /// TRANSITION_OPEN bit says whether the dissolve fills or clears the tile,
  /// the bits below it how far it has gone.
  u16 value;
  /// ScreenTransitionState's centre, layers and spare byte; the dissolve
  /// reads none of them.
  ///@{
  s32 centerX;
  s32 centerY;
  s16 insideLayers;
  s16 outsideLayers;
  u8 unused;
  ///@}
  /// ScreenTransitionState's page: the dissolve flips it each frame and
  /// never reads it.
  u8 page;
  s8 start;    ///< the value's first step
  s8 end;      ///< the value's last step
  s8 duration; ///< frames; 0 when the value is not moving
  s8 step;     ///< frames
};

/// The map grid state as a transition reads it: the display control bits the
/// scene shows, and the two lines where the centre-line transition blanks the
/// screen and shows it again, with the HBlank interrupt's copies of them.
struct ScreenTransitionDisplay {
  /// The map grid state's view, shake and script (WorldMapView's); no
  /// transition reads them.
  ///@{
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  ///@}
  /// Display control bits ORed into DISPCNT; ScreenTransition_HideBackgrounds
  /// clears those of backgrounds 1 to 3.
  u16 dispcnt;
  /// The map grid state's fields from fadeStep to its window rows
  /// (WorldMapView's); no transition reads them.
  u8 viewState[234];
  /// Scanlines: where the screen goes blank, below the screen
  /// (CENTER_LINE_HIDDEN_FIRST) while it is wholly shown.
  u16 firstLine;
  /// Scanlines: where the screen shows again, CENTER_LINE_HIDDEN_SECOND while
  /// it is wholly shown.
  u16 secondLine;
  u16 irqFirstLine;  ///< scanlines; firstLine as the HBlank interrupt reads it
  u16 irqSecondLine; ///< scanlines; secondLine as the HBlank interrupt reads it
  /// Where the HBlank interrupt is in the frame, an enum
  /// ScreenTransitionIrqPhase.
  u16 irqPhase;
};

/// The screen transition state (HEAP_SLOT_SCREEN_TRANSITION): two pages of
/// window edges for every line, the dissolve's solid tile, and the
/// transition's shape and progress.
struct ScreenTransitionState {
  /// Window edges, two pages: WININ and WINOUT, then a WIN0H and WIN1H pair,
  /// each (left << 8 | right), for each of the 160 lines.
  u16 pages[2][322];
  /// The dissolve's solid tile, one 8 by 8 tile of 4-bit pixels: as words,
  /// or as bytes of two pixels each, which the dissolve sets two at a time.
  union {
    u32 solidTile[8];
    u8 solidTilePixels[32];
  };
  /// The window transition's shape, an enum WindowTransitionMode, which
  /// ScreenTransition_UpdateScanlineTable draws.
  u16 mode;
  /// The progress, moved from start to end over duration frames: 0 is the
  /// screen covered, TRANSITION_OPEN fully open and TRANSITION_CLOSED covered
  /// again.
  u16 value;
  /// 16.16 fixed-point pixels: the centre of Reveal's rippling ellipse
  /// (WINDOW_TRANSITION_RIPPLE), the leader's place on the map.
  FIXED centerX;
  FIXED centerY; ///< as centerX: the leader's z less its height
  /// The layers shown inside the window: WININ, or WINOUT in the modes that
  /// swap the two.
  s16 insideLayers;
  /// The layers shown outside it, BG0 alone when a transition starts: WINOUT,
  /// or WININ in the modes that swap the two.
  s16 outsideLayers;
  u8 unused;
  u8 page;     ///< the pages entry shown; the next is built in the other
  s8 start;    ///< the value's first step
  s8 end;      ///< the value's last step
  s8 duration; ///< frames; 0 when the value is not moving
  s8 step;     ///< frames
  /// The mode taken when the value reaches its end; 0 ends the transition.
  s8 nextMode;
};

STATIC_ASSERT(offsetof(struct ScreenTransitionDissolveState, value) == 1322);
STATIC_ASSERT(offsetof(struct ScreenTransitionDissolveState, start) == 1338);
STATIC_ASSERT(sizeof(struct ScreenTransitionDissolveState) == 1344);
STATIC_ASSERT(offsetof(struct ScreenTransitionDisplay, dispcnt) == 20);
STATIC_ASSERT(offsetof(struct ScreenTransitionDisplay, firstLine) == 256);
STATIC_ASSERT(offsetof(struct ScreenTransitionDisplay, irqPhase) == 264);
STATIC_ASSERT(sizeof(struct ScreenTransitionDisplay) == 268);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, pages) == 0);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, solidTile) == 1288);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, page) == 1337);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, mode) == 1320);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, centerX) == 1324);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, insideLayers) == 1332);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, nextMode) == 1342);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, duration) == 1340);
STATIC_ASSERT(sizeof(struct ScreenTransitionState) == 1344);

#endif
