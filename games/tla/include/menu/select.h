#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

#include "types.h"
#include "static_assert.h"
#include "window.h"
#include "affine.h"

/// A selection screen sprite: a list link and its OAM attributes.
struct SelectionSprite {
  struct SelectionSprite *next;
  u8 y;
  u8 affine : 2;
  u8 mode : 2;
  u8 mosaic : 1;
  u8 colors : 1;
  u8 shape : 2;
  u16 x : 9;
  u16 param : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

/// An icon of the selection screen: its list links, the entry it shows, its
/// place and its slide toward a target, its zoom and its sprite.
struct SelectionNode {
  struct SelectionNode *prev; ///< the icon before it in its row
  struct SelectionNode *next; ///< the icon after it in its row
  u16 base; ///< the entry's id, as listed in SelectionScreen.bases
  u16 kind; ///< the entry's kind, as listed in SelectionScreen.kinds
  u16 slot;
  u16 tile;
  s16 x;       ///< screen pixels
  s16 y;       ///< screen pixels
  s16 dx;      ///< pixels a frame
  s16 dy;      ///< pixels a frame
  s16 targetX; ///< screen pixels
  s16 targetY; ///< screen pixels
  s16 homeX;
  s16 homeY;
  u16 message; ///< its name, shown in the screen's window
  s16 scale;
  s16 scaleStep;
  s16 scaleEnd;
  struct SelectionSprite sprite;
};

/// The selection screen: the row of icons the player picks from.
struct SelectionScreen {
  struct SelectionNode records[16];
  struct AffineTransform transform;
  struct SelectionNode *head; ///< the first icon of the row
  struct SelectionNode *path; ///< the icons chosen so far
  struct Window *window;      ///< the window naming the chosen entry
  /// The listed entries, up to 16.
  ///@{
  u16 kinds[16];
  u16 bases[16];
  u16 count;
  ///@}
  u16 baseX;       ///< screen pixels
  u16 baseY;       ///< screen pixels
  u16 unk39a;      ///< cleared with the list; not read by name in ⚓️
  u16 top;         ///< the first listed entry shown
  u16 cursorIndex; ///< the entry under the hand, from top
  u16 verticalScroll;
  u16 status; ///< 1 shown, 33 while scrolling
  u16 pathTop[5];
  u16 pathCursor[5];
  /// 1 from Menu_RunSelectionForValue, 999 once the window opens. While 0,
  /// a mode 6 window opens at the top of the screen.
  u16 locked;
  u8 unk3ba[42]; ///< not named by ⚓️ code
};

STATIC_ASSERT(sizeof(struct SelectionSprite) == 0x0c);
STATIC_ASSERT(sizeof(struct SelectionNode) == 0x34);
STATIC_ASSERT(offsetof(struct SelectionNode, message) == 0x20);
STATIC_ASSERT(offsetof(struct SelectionNode, sprite) == 0x28);
STATIC_ASSERT(offsetof(struct SelectionScreen, head) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionScreen, kinds) == 0x354);
STATIC_ASSERT(offsetof(struct SelectionScreen, cursorIndex) == 0x39e);
STATIC_ASSERT(offsetof(struct SelectionScreen, locked) == 0x3b8);
STATIC_ASSERT(sizeof(struct SelectionScreen) == 0x3e4);

#endif
