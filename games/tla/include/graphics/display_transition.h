#ifndef ALCHEMY_DISPLAY_TRANSITION_H
#define ALCHEMY_DISPLAY_TRANSITION_H

/// @file
/// The screen transition's frames, windows and state.

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"

/// The screen transition state as the dissolve reads it: the solid tile as
/// bytes, two 4-bit pixels to a byte, so that it can set one pixel at a time.
struct ScreenTransitionDissolveState {
  u16 pages[2][322]; ///< as ScreenTransitionState's; the dissolve leaves them
  /// ScreenTransitionState's solidTile and mode: the dissolve sets two more of
  /// the tile's pixels each frame and copies the tile to BG character block 0.
  u8 paletteNibbles[34];
  /// The progress, moved from start to end over duration frames: bit 5 says
  /// whether the dissolve fills or clears, the low 5 bits how far it is.
  u16 value;
  /// As ScreenTransitionState's; the dissolve leaves them.
  ///@{
  s32 centreX;
  s32 centreY;
  s16 mask;
  s16 active;
  u8 unused;
  ///@}
  /// ScreenTransitionState's page: the dissolve flips it each frame, though
  /// nothing reads it while the dissolve runs.
  u8 ditherToggle;
  s8 start;    ///< the value's first step
  s8 end;      ///< the value's last step
  s8 duration; ///< frames; 0 when the value is not moving
  s8 step;     ///< frames
};

/// The screen transition state: two pages of window edges for every line,
/// the dissolve's solid tile, and the transition's mode and progress.
struct ScreenTransitionState {
  /// Window edges, two pages: WININ and WINOUT, then a WIN0H and WIN1H pair,
  /// each (left << 8 | right), for each of the 160 lines.
  u16 pages[2][322];
  /// The dissolve's solid tile, one 8 by 8 tile of 4-bit pixels.
  u32 solidTile[8];
  u16 mode; ///< the window transition's shape
  /// The progress, moved from start to end over duration frames.
  u16 value;
  /// 16.16 fixed-point pixels: the centre of the ellipse around a point, set
  /// by the Reveal effect.
  FIXED centreX;
  FIXED centreY; ///< as centreX: the target's z less its y
  /// WININ for the inside of the shape: both windows show every layer.
  s16 mask;
  /// WINOUT for the outside: BG0 alone, with the sprite window's layers kept.
  /// A mode that inverts its shape writes the two the other way round.
  s16 active;
  u8 unused;
  u8 page;     ///< the pages entry the HBlank DMA copies from
  s8 start;    ///< the value's first step
  s8 end;      ///< the value's last step
  s8 duration; ///< frames; 0 when the value is not moving
  s8 step;     ///< frames
  /// The mode taken when the value reaches its end; 0 ends the transition.
  s8 nextMode;
  /// Set by the fade out to the centre line (ScreenTransition_Finish); while
  /// it is set the icon menus leave window 1's lines alone.
  s8 centreLine;
};

STATIC_ASSERT(offsetof(struct ScreenTransitionDissolveState, value) == 1322);
STATIC_ASSERT(offsetof(struct ScreenTransitionDissolveState, start) == 1338);
STATIC_ASSERT(sizeof(struct ScreenTransitionDissolveState) == 1344);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, pages) == 0);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, solidTile) == 1288);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, page) == 1337);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, mode) == 1320);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, centreX) == 1324);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, mask) == 1332);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, nextMode) == 1342);
STATIC_ASSERT(offsetof(struct ScreenTransitionState, duration) == 1340);
STATIC_ASSERT(sizeof(struct ScreenTransitionState) == 1344);

void ScreenTransition_StartWindowHeld(s32 mode);
void ScreenTransition_SuspendCallbacks(void);
struct ScreenTransitionState *ScreenTransition_AllocateAndClearState(void);
void ScreenTransition_Start(s32 mode, s32 frames);
void ScreenTransition_Finish(s32 mode, s32 frames);
void ScreenTransition_Update(void);
void ScreenTransition_UpdateFromCenter(void);
void ScreenTransition_UpdateScanline(void);
void ScreenTransition_UpdateDissolve(void);
void ScreenTransition_FillTilemapAndColorTile(s32 color);
void ScreenTransition_StartDissolveHeld(s32 mode);
void ScreenTransition_UpdateScanlineTable(void);
void FieldEffect_StartWindowHBlankDma(void);

#endif
