#ifndef ALCHEMY_PAGED_LIST_H
#define ALCHEMY_PAGED_LIST_H

/* @file */
/* The paged list a menu page draws and moves through. */

#include "types.h"
#include "static_assert.h"

/* A menu's paged list: which page and row the cursor is on, how many pages */
/* and entries there are, and the entry the cursor selects. */
typedef struct PAGED_LIST {
	s32 memberState; /* the party member's UNIT, as a word */
	s32 unused;
	s32 page;		 /* the page shown, from 0 */
	s32 pageCnt;	 /* pages in the list */
	s32 row;		 /* the cursor's row on the page */
	s32 entryCnt;	 /* entries in the list */
	s32 selectedIdx; /* the entry under the cursor: page * rows + row */
} PAGED_LIST;
STATIC_ASSERT(sizeof(PAGED_LIST) == 28);

#endif
