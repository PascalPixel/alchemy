#ifndef ALCHEMY_MENU_RESULT_H
#define ALCHEMY_MENU_RESULT_H

/// @file
/// The paged list a menu page draws and moves through.

#include "types.h"
#include "static_assert.h"

/// A menu's paged list: which page and row the cursor is on, how many pages
/// and entries there are, and the entry the cursor selects.
struct MenuResult {
  s32 ownerState;
  s32 unk04;
  s32 page;          ///< the page shown, from 0
  s32 pageCount;     ///< pages in the list
  s32 row;           ///< the cursor's row on the page
  s32 entryCount;    ///< entries in the list
  s32 selectedIndex; ///< the entry under the cursor: page * rows + row
};
STATIC_ASSERT(sizeof(struct MenuResult) == 28);

#endif
