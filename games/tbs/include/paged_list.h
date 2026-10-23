#ifndef ALCHEMY_PAGED_LIST_H
#define ALCHEMY_PAGED_LIST_H

/// @file
/// PagedList, what a menu returns.

#include "unit/menu/menu.h"
#include "types.h"

/// A paged list for one party member: their unit, the list's length and pages,
/// and the page and row of the entry chosen last.
struct PagedList {
  s32 memberState;   ///< the member's struct Unit, as a word
  s32 unused04;      ///< not named by TBS code
  s32 page;          ///< the chosen entry's page, from 0
  s32 pageCount;     ///< pages the list fills, the last one perhaps part full
  s32 row;           ///< the chosen entry's row on its page
  s32 entryCount;    ///< entries in the list
  s32 selectedIndex; ///< the entry chosen last, kept within the list; 0 if
                     ///< empty
};

#endif
