#ifndef ALCHEMY_ICON_MENU_H
#define ALCHEMY_ICON_MENU_H

/// @file
/// The icon menu's entries and state.

#include "static_assert.h"
#include "types.h"
#include "system/heap.h"
#include "window/window.h"

/// A command icon of the icon menu: its OAM attributes, its place and its
/// VRAM cache slot.
struct IconMenuEntry {
  /// The sprite queue's link, then OAM attributes 0 and 1 and attribute 2,
  /// written each frame by IconMenu_DrawIcons.
  u32 sprite[3];
  s16 x; ///< screen pixels; an icon at 0 is not drawn
  s16 y; ///< screen pixels
  u16 unused;
  u16 slot; ///< VRAM cache entry of the icon's picture
};

/// The icon menu: up to six command icons in a row, the windows of their
/// labels, the icons' resource ids, the selection and the layout.
struct IconMenuState {
  struct IconMenuEntry entries[6];
  struct Window *window; ///< the label window naming the selected icon
  /// The password level and transfer menus' explanation window, below the
  /// heading.
  struct Window *lowerWindow;
  struct Window *upperWindow; ///< those menus' heading window
  /// Each icon's number (ResourceId_CommandIcons), which is also its command
  /// name's: MsgCommandName plus it.
  u8 commandIcons[8];
  s16 selection; ///< the icon chosen, 0 to count - 1
  s16 count;     ///< icons in the row, up to 6
  s16 width;     ///< tiles
  /// A nonzero first label message, the selection added to it; 0 labels each
  /// icon by its command name.
  s16 labelBase;
  s16 row; ///< tiles
  /// The selection the explanation was last drawn for; -1 draws it anew.
  s16 drawnSelection;
};

STATIC_ASSERT(sizeof(struct IconMenuEntry) == 20);
STATIC_ASSERT(offsetof(struct IconMenuEntry, slot) == 18);
STATIC_ASSERT(sizeof(struct IconMenuState) == 152);
STATIC_ASSERT(offsetof(struct IconMenuState, window) == 120);
STATIC_ASSERT(offsetof(struct IconMenuState, selection) == 140);
STATIC_ASSERT(offsetof(struct IconMenuState, labelBase) == 146);
STATIC_ASSERT(offsetof(struct IconMenuState, drawnSelection) == 150);

/// Heap slot 58: the icon menu while one is open.
#define gIconMenuState                                                         \
  (*(struct IconMenuState **)&gHeapSlots[HEAP_SLOT_ICON_MENU])

#endif
