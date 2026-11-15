#ifndef ALCHEMY_PAGED_LIST_H
#define ALCHEMY_PAGED_LIST_H

/// @file
/// The paged list a menu page draws and moves through.

#include "types.h"
#include "static_assert.h"

/// A menu's paged list: which page and row the cursor is on, how many pages
/// and entries there are, and the entry the cursor selects.
struct PagedList {
  s32 memberState; ///< the party member's struct Unit, as a word
  s32 unused;
  s32 page;          ///< the page shown, from 0
  s32 pageCount;     ///< pages in the list
  s32 row;           ///< the cursor's row on the page
  s32 entryCount;    ///< entries in the list
  s32 selectedIndex; ///< the entry under the cursor: page * rows + row
};
STATIC_ASSERT(sizeof(struct PagedList) == 28);

#endif
