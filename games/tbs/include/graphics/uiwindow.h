#ifndef ALCHEMY_UIWINDOW_H
#define ALCHEMY_UIWINDOW_H

/// @file
/// UiWindow, a window's work, and marking its tilemap rows for transfer.

#include "render_input.h"

/// A window: a framed rectangle of the tilemap with its sprite outputs, its
/// opening or closing animation and the rectangle it last drew.
struct UiWindow {
  struct RenderOutputList output; ///< the sprites the window carries
  u16 width;                      ///< tiles
  u16 height;                     ///< tiles
  u16 x;                          ///< tiles
  u16 y;                          ///< tiles
  u16 unk10; ///< 1 once opened, 0 once freed; not read by name in TBS
  /// The effect a message's TEXT_WINDOW_STATE code sets. Only
  /// UI_WINDOW_EFFECT_SWELL (4) is acted on; its channel types nothing more
  /// until the swell ends and clears it.
  u16 effect;
  /// 0 while a message channel is typing into the window; 1 once its text has
  /// ended or closed the window, which frees the channel for the next text.
  u16 state;
  u16 flags; ///< UI_WINDOW_ flags; 0 once closing or free
  /// The zoom's step: counts down from 7 to 0 while opening, and up from 0 to
  /// duration while closing.
  u16 frame;
  s16 duration;       ///< frames of the zoom under way; 0 when none
  u16 previousX;      ///< tiles
  u16 previousY;      ///< tiles
  u16 previousWidth;  ///< tiles
  u16 previousHeight; ///< tiles
};

STATIC_ASSERT(sizeof(struct UiWindow) == 0x24);
STATIC_ASSERT(offsetof(struct UiWindow, width) == 8);
STATIC_ASSERT(offsetof(struct UiWindow, frame) == 0x18);
STATIC_ASSERT(offsetof(struct UiWindow, previousHeight) == 0x22);

/// A text channel, one of three, which types decoded text into a window a few
/// characters a frame and runs its control codes.
struct UiChannelSlot {
  struct UiWindow *work; ///< the window typed into; NULL when free
  u16 x;                 ///< pen, 8.8 fixed-point pixels
  u16 y;                 ///< pen, 8.8 fixed-point pixels
  /// Extra advance per line, in 1/256 pixels: after every character in
  /// Japanese, after a space in the other editions.
  u16 spacing[4];
  u16 line;        ///< line of the page, indexes spacing; at most 3
  u16 entry;       ///< next code in UiRenderWork.entries
  u16 countdown;   ///< frames left of a pause, a page wait or a swell
  u16 colour;      ///< text colour; 15 by default
  u16 outline;     ///< outline colour; 0 for none
  u16 lineSpacing; ///< 10 by default
  u16 transition;  ///< frames left of the canvas scrolling up a line
  u16 startX;      ///< pen x each new line returns to, 8.8 fixed-point pixels
  u16 ended;       ///< 1 once a TEXT_END code is reached
  u16 glyphDelay;  ///< frames to wait after a glyph while no button is held
  u16 flags;       ///< nonzero keeps the window's size from page to page
  u16 unk26;       ///< not named by TBS code
};

STATIC_ASSERT(sizeof(struct UiChannelSlot) == 0x28);
STATIC_ASSERT(offsetof(struct UiChannelSlot, entry) == 0x12);
STATIC_ASSERT(offsetof(struct UiChannelSlot, countdown) == 0x14);
STATIC_ASSERT(offsetof(struct UiChannelSlot, transition) == 0x1c);
STATIC_ASSERT(offsetof(struct UiChannelSlot, flags) == 0x24);

/// Requests transfer of the four-row tilemap band containing window row y.
// A statement macro (S2: a layer, 10 sites): the dirty-row store stays where it
// is written at every use; as a plain expression, tbs-en differs.
#define UiRender_MarkRow(render, y)                                            \
  do {                                                                         \
    (render)->dirty |= 2 << ((u32)(y) >> 2);                                   \
  } while (0)

#endif
