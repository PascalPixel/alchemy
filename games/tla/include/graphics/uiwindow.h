#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

#include "render_input.h"

/// A window: a bordered rectangle of the tilemap with its sprite outputs, its
/// opening or closing animation and the rectangle it last drew.
struct Window {
  struct WindowSpriteList sprites; ///< the sprites the window carries
  u16 width;                       ///< tiles
  u16 height;                      ///< tiles
  u16 x;                           ///< tiles
  u16 y;                           ///< tiles
  u16 opened; ///< 1 once opened, 0 once freed; nothing reads it
  /// The animation a message's window-state code sets.
  u16 animation;
  /// 0 while a message channel is typing into the window; nonzero once it is
  /// done, which frees the channel for the next text.
  u16 state;
  u16 flags; ///< 0 once closing or free
  /// The zoom's stage: counts down while opening, up to duration while
  /// closing.
  u16 zoomStage;

  s16 duration;       ///< frames of the zoom under way; 0 when none
  u16 previousX;      ///< tiles
  u16 previousY;      ///< tiles
  u16 previousWidth;  ///< tiles
  u16 previousHeight; ///< tiles
};

STATIC_ASSERT(sizeof(struct Window) == 36);
STATIC_ASSERT(offsetof(struct Window, width) == 8);
STATIC_ASSERT(offsetof(struct Window, zoomStage) == 24);
STATIC_ASSERT(offsetof(struct Window, previousHeight) == 34);

/// A text channel, one of three, which types decoded text into a window a few
/// characters a frame and runs its control codes.
struct UiChannelSlot {
  struct Window *work; ///< the window typed into; NULL when free
  u16 x;               ///< pen, 8.8 fixed-point pixels
  u16 y;               ///< pen, 8.8 fixed-point pixels
  u16 spacing[4];
  u16 line;
  u16 entry;
  u16 countdown;
  u16 colour;
  u16 outline;
  u16 lineSpacing;
  u16 transition;
  u16 startX;
  u16 ended;
  u16 glyphDelay;
  u16 flags;
  /// Copied from spacing[3] when queued; with WindowWork.textMode set it
  /// places the glyphs by y / 320.
  u16 compressLines;
};

STATIC_ASSERT(sizeof(struct UiChannelSlot) == 40);
STATIC_ASSERT(offsetof(struct UiChannelSlot, entry) == 18);
STATIC_ASSERT(offsetof(struct UiChannelSlot, countdown) == 20);
STATIC_ASSERT(offsetof(struct UiChannelSlot, transition) == 28);
STATIC_ASSERT(offsetof(struct UiChannelSlot, flags) == 36);

/// Requests transfer of the four-row tilemap band containing window row y.
// A statement macro (S2: a layer, 10 sites): the dirty-row store stays where
// it is written at every use; as a plain expression, tbs-en differs.
#define UiRender_MarkRow(render, y)                                            \
  do {                                                                         \
    (render)->dirty |= 2 << ((u32)(y) >> 2);                                   \
  } while (0)

#endif
