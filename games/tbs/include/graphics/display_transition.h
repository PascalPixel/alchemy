#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

/// @file
/// The display transition's frames, windows and state.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// The display transition state as the centre-line transition and the dissolve
/// read it: the dissolve's tile pixels and the transition's progress.
struct DisplayTransitionFrame {
  u16 pages[2][322]; ///< DisplayTransitionState's; not named here
  /// DisplayTransitionState's solidTile, as 4-bit pixels two a byte: the
  /// dissolve sets two more each frame and copies the tile to BG character
  /// block 0.
  u8 paletteNibbles[0x22];
  /// The progress, moved from start to end over duration frames: bit 5 says
  /// whether the dissolve fills or clears, the low 5 bits how far it is.
  u16 value;
  /// DisplayTransitionState's centreX to its unused byte; not named here.
  ///@{
  s32 centreX;
  s32 centreY;
  s16 mask;
  s16 active;
  u8 unused;
  ///@}
  /// DisplayTransitionState's page: the dissolve flips it each frame and
  /// never reads it.
  u8 ditherToggle;
  s8 start;    ///< the value's first step
  s8 end;      ///< the value's last step
  s8 duration; ///< frames; 0 when the value is not moving
  s8 step;     ///< frames
};

/// The map work's display fields that transitions use: its display control
/// bits and the lines where the centre-line transition shows and hides the
/// screen, with the VCount interrupt's copies and phase.
struct DisplayTransitionWindow {
  /// The map work's origin, shake and script (PerspectiveWork's); no transition
  /// names them.
  ///@{
  s32 *origin;
  s32 shakeX, shakeY, shakeDecay;
  u8 *script;
  ///@}
  /// Display control bits ORed into DISPCNT; DisplayState_ClearFlags clears
  /// those of backgrounds 1 to 3.
  u16 dispcnt;
  /// The map work's fields from fadeStep to its window rows
  /// (PerspectiveWork's); no transition names them.
  u8 viewState[0xea];
  /// scanlines: where the screen is hidden, 200 when it is not
  u16 firstLine;
  /// scanlines: where the screen is shown again, 250 when it is not hidden
  u16 secondLine;
  u16 irqFirstLine;  ///< scanlines; firstLine as the VCount interrupt reads it
  u16 irqSecondLine; ///< scanlines; secondLine as the VCount interrupt reads it
  /// The VCount interrupt's phase: 0 waiting for a line above 159, 1 neither
  /// line passed, 2 the first passed, 3 the second passed first, 9 done.
  u16 irqPhase;
};

/// The display transition state (heap block 31): two pages of window edges for
/// every line, the dissolve's solid tile, and the transition's mode and
/// progress.
struct DisplayTransitionState {
  /// Window edges, two pages: WININ and WINOUT, then a WIN0H and WIN1H pair,
  /// each (left << 8 | right), for each of the 160 lines.
  u16 pages[2][322];
  /// The dissolve's solid tile, one 8 by 8 tile of 4-bit pixels.
  u32 solidTile[8];
  /// The window transition's shape, an enum WindowTransitionMode, which
  /// DisplayTransition_UpdateScanlineTable draws.
  u16 mode;
  /// The progress, moved from start to end over duration frames: bit 5 says
  /// whether the transition opens or closes, the low 5 bits how far it is.
  u16 value;
  /// 16.16 fixed-point pixels: the centre of the ellipse around a point
  /// (WINDOW_TRANSITION_RIPPLE), set by the Reveal effect.
  s32 centreX;
  s32 centreY; ///< as centreX: the target's z less its y
  /// WINOUT, or WININ by mode: 0x3f3f or 63 when a transition starts.
  s16 mask;
  /// WININ, or WINOUT by mode: 1 when a transition starts.
  s16 active;
  u8 unused;   ///< no code names it
  u8 page;     ///< the pages entry shown; the next is built in the other
  s8 start;    ///< the value's first step
  s8 end;      ///< the value's last step; 64 hides BG1 to BG3 and sprites
  s8 duration; ///< frames; 0 when the value is not moving
  s8 step;     ///< frames
  /// The mode taken when the value reaches its end; 0 ends the transition.
  s8 nextMode;
};

STATIC_ASSERT(offsetof(struct DisplayTransitionFrame, value) == 0x52a);
STATIC_ASSERT(offsetof(struct DisplayTransitionFrame, start) == 0x53a);
STATIC_ASSERT(sizeof(struct DisplayTransitionFrame) == 0x540);
STATIC_ASSERT(offsetof(struct DisplayTransitionWindow, dispcnt) == 0x14);
STATIC_ASSERT(offsetof(struct DisplayTransitionWindow, firstLine) == 0x100);
STATIC_ASSERT(offsetof(struct DisplayTransitionWindow, irqPhase) == 0x108);
STATIC_ASSERT(sizeof(struct DisplayTransitionWindow) == 0x10c);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, pages) == 0);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, solidTile) == 0x508);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, page) == 0x539);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, mode) == 0x528);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, centreX) == 0x52c);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, mask) == 0x534);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, nextMode) == 0x53e);
STATIC_ASSERT(offsetof(struct DisplayTransitionState, duration) == 0x53c);
STATIC_ASSERT(sizeof(struct DisplayTransitionState) == 0x540);

#endif
