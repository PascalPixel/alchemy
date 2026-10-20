#ifndef ALCHEMY_RESOURCE_MENU_H
#define ALCHEMY_RESOURCE_MENU_H

/// @file
/// The resource menu's entries and work.

#include "types.h"
#include "static_assert.h"
#include "window.h"

/// A command icon of the resource menu: its OAM attributes, its place and its
/// VRAM cache slot.
struct ResourceMenuEntry {
  /// The sprite queue's link, then OAM attributes 0 and 1 and attribute 2,
  /// written each frame by AffineEffect_UpdateFrame.
  ///@{
  u32 link;
  u32 attr01;
  u32 attr2;
  ///@}
  s16 x;     ///< screen pixels; an icon at 0 is not drawn
  s16 y;     ///< screen pixels
  u16 unk10; ///< not named by TBS code
  u16 slot;  ///< VRAM cache entry of the icon's picture
};

/// The resource menu: up to six command icons in a row, the windows of their
/// labels, the icons' resource ids, the selection and the layout.
/// It lives in heap slot 58 (gMenuSelectWork) from AffineEffect_InitializeWork
/// to Menu_EndResourceSelection.
struct ResourceMenuWork {
  struct ResourceMenuEntry entries[6];
  struct UiWindow *window; ///< the label window naming the selected icon
  /// The password level and transfer menus' explanation window, below the
  /// heading.
  struct UiWindow *lowerWindow;
  struct UiWindow *upperWindow; ///< those menus' heading window
  /// Each icon's number (ResourceId_CommandIcons), which is also its command
  /// name's: MsgCommandName plus it.
  u8 resourceIds[8];
  s16 selection; ///< the icon chosen, 0 to count - 1
  s16 count;     ///< icons in the row, up to 6
  s16 width;     ///< tiles
  /// A nonzero first label message, the selection added to it; 0 labels each
  /// icon by its command name.
  s16 resourceBase;
  s16 row; ///< tiles
  /// The selection the explanation was last drawn for; 0xffff draws it anew.
  s16 previousMode;
};

STATIC_ASSERT(sizeof(struct ResourceMenuEntry) == 0x14);
STATIC_ASSERT(offsetof(struct ResourceMenuEntry, slot) == 0x12);
STATIC_ASSERT(sizeof(struct ResourceMenuWork) == 0x98);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, window) == 0x78);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, selection) == 0x8c);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, resourceBase) == 0x92);
STATIC_ASSERT(offsetof(struct ResourceMenuWork, previousMode) == 0x96);

/// Work slot 58: the resource menu while one is open.
extern struct ResourceMenuWork *gMenuSelectWork;

#endif
