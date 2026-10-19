#ifndef ALCHEMY_MENU_RESULT_H
#define ALCHEMY_MENU_RESULT_H

/// @file
/// MenuResult, what a menu returns.

#include "unit/menu/menu.h"
#include "types.h"

/// A paged list for one party member: their unit, the list's length and pages,
/// and the page and row of the entry chosen last.
struct MenuResult {
  s32 ownerState;
  s32 unused04;
  s32 page;
  s32 pageCount;
  s32 row;
  s32 entryCount;
  s32 selectedIndex;
};

#endif
