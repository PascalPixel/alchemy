#ifndef ALCHEMY_SELECT_H
#define ALCHEMY_SELECT_H

/// @file
/// The selection screen: the row of icons a command, item or Psynergy is
/// picked from, its icons and their sprites.

#include "types.h"
#include "static_assert.h"
#include "window/window.h"
#include "system/affine.h"

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
  u16 matrix : 5; ///< the affine matrix
  u16 size : 2;
  u16 tile : 10;
  u16 priority : 2;
  u16 palette : 4;
};

/// An icon of the selection screen: its list links, the entry it shows, its
/// place and its slide toward a target, its zoom and its sprite.
struct SelectionNode {
  struct SelectionNode *previous; ///< the icon before it in its row
  struct SelectionNode *next;     ///< the icon after it in its row
  u16 base; ///< the entry's id, as listed in SelectionMenuState.bases
  u16 kind; ///< the entry's kind, as listed in SelectionMenuState.kinds
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

/// The kinds of icon the selection screen lists (SelectionNode.kind) and
/// Resource_LoadByMode loads.
enum IconKind {
  ICON_KIND_NONE = 0,    ///< a free selection icon entry
  ICON_KIND_COMMAND = 1, ///< a command's icon, from its number
  ICON_KIND_ITEM = 2,    ///< an item's icon
  ICON_KIND_ABILITY = 4, ///< a Psynergy's icon, from its ability number
  ICON_KIND_YES_NO = 6,  ///< a yes or no command, drawn as a command
  /// An item's icon without the broken mark, for the equipment view.
  ICON_KIND_EQUIPMENT_ITEM = 7,
  ICON_KIND_PATTERN = 8 ///< one of the other icons
};

/// The entries of SelectionMenuState.records.
enum SelectionRecord {
  SELECTION_RECORD_ROW = 2,    ///< the first of the row's seven icons
  SELECTION_RECORD_CHOSEN = 9, ///< the first of the five icons chosen
  SELECTION_RECORD_HAND = 14,  ///< the pointing hand
  SELECTION_RECORD_LARGE_ICON = 15,
  SELECTION_RECORD_COUNT = 16
};

/// The selection screen (HEAP_SLOT_SELECTION_MENU): the row of icons the player
/// picks from, with arrows at each end and a pointing hand over the chosen
/// target.
struct SelectionMenuState {
  /// 0 and 1 the arrows at the row's ends, 2 to 8 the row's icons, 9 to 13 the
  /// icons chosen so far, 14 the pointing hand and 15 the large icon.
  struct SelectionNode records[SELECTION_RECORD_COUNT];
  struct AffineTransform transform; ///< a zooming icon's scale
  struct SelectionNode *head;       ///< the first icon of the row
  struct SelectionNode *path;       ///< the icons chosen so far
  struct Window *window;            ///< the window naming the chosen entry
  /// The listed entries, up to 16.
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
  /// Menu_RunSelectionForValue. Nothing sets 0, which would open a yes or no
  /// row and its window at the top of the screen.
  u16 locked;
  u8 unused[42]; ///< no code names it
};

/// SelectionMenuState.status: the cursor stepping, or the row shown.
#define NODE_STATUS_SCROLLING 33
#define NODE_STATUS_SHOWN 1

STATIC_ASSERT(sizeof(struct SelectionSprite) == 12);
STATIC_ASSERT(sizeof(struct SelectionNode) == 52);
STATIC_ASSERT(offsetof(struct SelectionNode, message) == 32);
STATIC_ASSERT(offsetof(struct SelectionNode, sprite) == 40);
STATIC_ASSERT(offsetof(struct SelectionMenuState, head) == 840);
STATIC_ASSERT(offsetof(struct SelectionMenuState, kinds) == 852);
STATIC_ASSERT(offsetof(struct SelectionMenuState, cursorIndex) == 926);
STATIC_ASSERT(offsetof(struct SelectionMenuState, locked) == 952);
STATIC_ASSERT(sizeof(struct SelectionMenuState) == 996);

/// The bit of SelectionMenuState.cursorIndex set as an entry is chosen, on
/// which SelectionMenu_Clear puts the next list's top and cursor back to its
/// first entry.
#define SELECTION_CURSOR_RESET 0x80

#endif
