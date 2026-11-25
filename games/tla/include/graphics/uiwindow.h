#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

/// @file
/// A window and the messages that type text into it.

#include "render_input.h"

/// A window: a bordered rectangle of the tilemap with its sprite outputs, its
/// opening or closing animation and the rectangle it last drew.
struct Window {
  struct WindowSpriteList sprites; ///< the sprites the window carries
  u16 width;                       ///< tiles
  u16 height;                      ///< tiles
  s16 x;                           ///< tiles
  s16 y;                           ///< tiles
  u16 opened; ///< 1 once opened, 0 once freed; nothing reads it
  /// The animation a message's window-state code sets.
  u16 animation;
  /// 0 while a message is typing into the window; nonzero once it is done,
  /// which frees the message for the next text.
  u16 state;
  u16 flags; ///< 0 once closing or free
  /// The zoom's stage: counts down while opening, up to duration while
  /// closing.
  s16 zoomStage;

  s16 duration;       ///< frames of the zoom under way; 0 when none
  s16 previousX;      ///< tiles
  s16 previousY;      ///< tiles
  s16 previousWidth;  ///< tiles
  s16 previousHeight; ///< tiles
};

STATIC_ASSERT(sizeof(struct Window) == 36);
STATIC_ASSERT(offsetof(struct Window, width) == 8);
STATIC_ASSERT(offsetof(struct Window, zoomStage) == 24);
STATIC_ASSERT(offsetof(struct Window, previousHeight) == 34);

/// A message, one of the three in WindowState.messages, which types decoded
/// text into a window a few characters a frame and runs its control codes.
struct Message {
  struct Window *window; ///< the window typed into; NULL when free
  u16 x;                 ///< pen, 8.8 fixed-point pixels
  u16 y;                 ///< pen, 8.8 fixed-point pixels
  /// Extra advance for each line of the page, in 1/256 pixels: after every
  /// character in Japanese, after a space in the other editions but German,
  /// which takes none.
  u16 spacing[4];
  u16 line;         ///< line of the page, indexes spacing; at most 3
  u16 textIndex;    ///< next code in WindowState.textBuffer
  u16 countdown;    ///< frames left of a pause, a page wait or a swell
  u16 colour;       ///< text colour; TEXT_COLOR_WHITE at start
  u16 outline;      ///< an enum TextStyle; TEXT_STYLE_PLAIN at start
  u16 lineSpacing;  ///< TEXT_DEFAULT_LINE_SPACING at start
  u16 scrollFrames; ///< frames left of the canvas scrolling up a line
  u16 startX;       ///< pen x each new line returns to, 8.8 fixed-point pixels
  u16 ended;        ///< 1 once a TEXT_END code is reached
  u16 glyphDelay;   ///< frames to wait after a glyph while no button is held
  u16 flags;        ///< nonzero keeps the window's size from page to page
  /// Copied from spacing[3] when the message starts; with WindowState.textMode
  /// set it places the glyphs by y / 320.
  u16 compressLines;
};

STATIC_ASSERT(sizeof(struct Message) == 40);
STATIC_ASSERT(offsetof(struct Message, textIndex) == 18);
STATIC_ASSERT(offsetof(struct Message, countdown) == 20);
STATIC_ASSERT(offsetof(struct Message, scrollFrames) == 28);
STATIC_ASSERT(offsetof(struct Message, flags) == 36);

/// Marks the 4-row band of the window tilemap that holds `row` for upload
/// (WindowState.dirty): the battle lists mark their top border's row so, and
/// the Djinn list its preview's.
#define WINDOW_MARK_TILEMAP_ROW(state, row)                                    \
  do {                                                                         \
    (state)->dirty |= 2 << ((u32)(row) >> 2);                                  \
  } while (0)

#endif
