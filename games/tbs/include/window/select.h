#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

/// @file
/// The selection screen's sprites and nodes.

#include "static_assert.h"
#include "types.h"
#include "system/affine.h"
#include "window/window.h"

/// A selection screen sprite: a list link and its OAM attributes.
struct SelectionSprite {
  struct SelectionSprite *next; ///< the sprite queue's link
  u8 y;                         ///< screen pixels
  u8 affine : 2;                ///< 0 plain, 1 affine, 3 affine at double size
  u8 mode : 2;                  ///< 0 normal, 1 semi-transparent, 2 OBJ window
  u8 mosaic : 1;
  u8 colors : 1;  ///< 1 for 256 colours
  u8 shape : 2;   ///< 0 square, 1 wide, 2 tall
  u16 x : 9;      ///< screen pixels
  u16 matrix : 5; ///< the affine matrix
  u16 size : 2;   ///< with shape, the sprite's size
  u16 tile : 10;  ///< first sprite tile in VRAM
  u16 priority : 2;
  u16 palette : 4; ///< 16-colour palette
};

/// An icon of the selection screen: its list links, the entry it shows, its
/// place and its slide toward a target, its zoom and its sprite.
struct SelectionNode {
  struct SelectionNode *previous; ///< the icon before it in its row
  struct SelectionNode *next;     ///< the icon after it in its row
  /// The entry's number within its kind, as listed in SelectionMenuWork.bases;
  /// for an arrow, the frames left of its nudge after a scroll.
  u16 base;
  /// The entry's kind, as listed in SelectionMenuWork.kinds, or ICON_KIND_NONE
  /// for a free record; for an arrow, TRUE while it shows.
  u16 kind;
  u16 slot;      ///< VRAM cache slot of its picture
  u16 tile;      ///< first sprite tile of its picture
  s16 x;         ///< screen pixels
  s16 y;         ///< screen pixels
  s16 dx;        ///< pixels a frame
  s16 dy;        ///< pixels a frame
  s16 targetX;   ///< screen pixels
  s16 targetY;   ///< screen pixels
  s16 homeX;     ///< screen pixels, where it stood when chosen
  s16 homeY;     ///< screen pixels, where it stood when chosen
  u16 message;   ///< its name, shown in the screen's window
  s16 scale;     ///< 8.8 fixed point, Q_8_8(1) full size
  s16 scaleStep; ///< 8.8 fixed point a frame
  s16 scaleEnd;  ///< 8.8 fixed point, Q_8_8(1) full size
  struct SelectionSprite sprite;
};

/// The entries of SelectionMenuWork.records.
enum SelectionRecord {
  SELECTION_RECORD_ROW = 2,    ///< the first of the row's seven icons
  SELECTION_RECORD_CHOSEN = 9, ///< the first of the five icons chosen
  SELECTION_RECORD_HAND = 14,  ///< the pointing hand
  SELECTION_RECORD_LARGE_ICON = 15,
  SELECTION_RECORD_COUNT = 16
};

/// The selection screen (heap block 18): the row of icons the player picks
/// from, with arrows at each end and a pointing hand over the chosen target.
struct SelectionMenuWork {
  /// 0 and 1 the arrows at the row's ends, 2 to 8 the row's icons, 9 to 13 the
  /// icons chosen so far, 14 the pointing hand, whose picture is loaded at
  /// setup, and 15 the large icon.
  struct SelectionNode records[SELECTION_RECORD_COUNT];
  struct AffineTransform transform; ///< a zooming icon's scale
  struct SelectionNode *head;       ///< the first icon of the row
  struct SelectionNode *path;       ///< the icons chosen so far
  struct Window *window;            ///< the window naming the chosen entry
  /// The listed entries, up to 16 (SelectionMenu_AddEntry): each one's kind
  /// (enum IconKind) and its command, item or ability number.
  ///@{
  u16 kinds[16];
  u16 bases[16];
  u16 count;
  ///@}
  u16 baseX;  ///< screen pixels
  u16 baseY;  ///< screen pixels
  u16 unread; ///< cleared with the list and on each step down; nothing reads it
  u16 top;    ///< the first listed entry shown
  /// The entry under the hand, from top; SELECTION_CURSOR_RESET asks the next
  /// list to start again from its first entry.
  u16 cursorIndex;
  u16 verticalScroll; ///< pixels added to the icons' y; 32 at setup
  /// NODE_STATUS_SCROLLING while the cursor steps, NODE_STATUS_SHOWN after
  /// (window_selection.c); each frame drawn adds 1.
  u16 status;
  u16 pathTop[5];    ///< top, kept for each icon chosen on the path
  u16 pathCursor[5]; ///< cursorIndex, kept for each icon chosen on the path
  /// 999 from setup and once a yes or no window opens, 1 from
  /// Menu_RunSelectionForValue.
  u16 locked;
  u8 unused[42];
};

/// The bit of SelectionMenuWork.cursorIndex set as an entry is chosen, on
/// which SelectionMenu_Clear puts the next list's top and cursor back to its
/// first entry.
#define SELECTION_CURSOR_RESET 0x80

STATIC_ASSERT(sizeof(struct SelectionSprite) == 0x0c);
STATIC_ASSERT(sizeof(struct SelectionNode) == 0x34);
STATIC_ASSERT(offsetof(struct SelectionNode, message) == 0x20);
STATIC_ASSERT(offsetof(struct SelectionNode, sprite) == 0x28);
STATIC_ASSERT(offsetof(struct SelectionMenuWork, head) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionMenuWork, kinds) == 0x354);
STATIC_ASSERT(offsetof(struct SelectionMenuWork, cursorIndex) == 0x39e);
STATIC_ASSERT(offsetof(struct SelectionMenuWork, locked) == 0x3b8);
STATIC_ASSERT(sizeof(struct SelectionMenuWork) == 0x3e4);

#endif
