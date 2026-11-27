#ifndef ALCHEMY_PAGED_LIST_H
#define ALCHEMY_PAGED_LIST_H

// The paged list a menu page draws and moves through.

#include "types.h"
#include "static_assert.h"

// A menu's paged list: which page and row the cursor is on, how many pages and entries there are,
// and the entry the cursor selects.
typedef struct PAGED_LIST
{
	Sint32 memberState;   // the party member's UNIT, as a word
	Sint32 unused;
	Sint32 page;          // the page shown, from 0
	Sint32 pageCount;     // pages in the list
	Sint32 row;           // the cursor's row on the page
	Sint32 entryCount;    // entries in the list
	Sint32 selectedIndex; // the entry under the cursor: page * rows + row
} PAGED_LIST;

STATIC_ASSERT(sizeof(PAGED_LIST) == 28);

#endif
