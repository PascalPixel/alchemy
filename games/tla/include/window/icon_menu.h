#ifndef ALCHEMY_ICON_MENU_H
#define ALCHEMY_ICON_MENU_H

/// @file
/// The icon menu's work: the row of command icons the field menu, the File
/// Screen, the shops and the Yes/No menu are chosen from.

#include "types.h"
#include "static_assert.h"
#include "window/window.h"

/// A command icon of the icon menu: its OAM attributes, its place and its
/// VRAM cache slot.
struct IconMenuEntry {
  /// The sprite queue's link, then OAM attributes 0 and 1 and attribute 2.
  u32 link;
  u32 attr01;
  u32 attr2;
  s16 x; ///< screen pixels; an icon at 0 is not drawn
  s16 y; ///< screen pixels
  u16 unused;
  u16 slot; ///< VRAM cache entry of the icon's picture
};

/// The icon menu: up to six command icons in a row, the windows of their
/// labels, the icons' resource ids, the selection and the layout.
struct IconMenuWork {
  struct IconMenuEntry entries[6];
  struct Window *window; ///< the label window naming the selected icon
  /// The password level and transfer menus' explanation window, below the
  /// heading.
  struct Window *lowerWindow;
  struct Window *upperWindow; ///< those menus' heading window
  /// Each icon's number, which is also its command name's: MsgCommandName
  /// plus it.
  u8 commandIcons[8];
  s16 selection; ///< the icon chosen, 0 to count - 1
  s16 count;     ///< icons in the row, up to 6
  s16 width;     ///< tiles
  /// A nonzero first label message, the selection added to it; 0 labels each
  /// icon by its command name.
  s16 labelBase;
  s16 row; ///< tiles
  /// The selection the explanation was last drawn for; 0xffff draws it anew.
  s16 previousMode;
};

STATIC_ASSERT(sizeof(struct IconMenuEntry) == 20);
STATIC_ASSERT(offsetof(struct IconMenuEntry, slot) == 18);
STATIC_ASSERT(sizeof(struct IconMenuWork) == 152);
STATIC_ASSERT(offsetof(struct IconMenuWork, window) == 120);
STATIC_ASSERT(offsetof(struct IconMenuWork, selection) == 140);
STATIC_ASSERT(offsetof(struct IconMenuWork, labelBase) == 146);
STATIC_ASSERT(offsetof(struct IconMenuWork, previousMode) == 150);

#endif
