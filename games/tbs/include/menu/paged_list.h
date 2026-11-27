#ifndef ALCHEMY_PAGED_LIST_H
#define ALCHEMY_PAGED_LIST_H

/* @file */
/* PAGED_LIST, what a menu returns. */

#include "types.h"
#include "menu/menu.h"

/* A paged list for one party member: their unit, the list's length and pages,
 */
/* and the page and row of the entry chosen last. */
typedef struct PAGED_LIST {
	s32 memberState; /* the party member's UNIT, as a word */
	s32 unused;
	s32 page;		 /* the chosen entry's page, from 0 */
	s32 pageCnt;	 /* pages the list fills, the last one perhaps part full */
	s32 row;		 /* the chosen entry's row on its page */
	s32 entryCnt;	 /* entries in the list */
	s32 selectedIdx; /* the entry chosen last, kept within the list; 0 if */
					 /* empty */
} PAGED_LIST;

#endif
