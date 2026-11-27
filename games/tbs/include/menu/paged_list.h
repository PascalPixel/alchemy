#ifndef ALCHEMY_PAGED_LIST_H
#define ALCHEMY_PAGED_LIST_H

/* PAGED_LIST, what a menu returns. */

#include "types.h"
#include "menu/menu.h"

/* A paged list for one party member: their unit, the list's length and pages,
   and the page and row of the entry chosen last. */
typedef struct PAGED_LIST {
	Sint32 memberState; /* the party member's UNIT, as a word */
	Sint32 unused;
	Sint32 page;	 /* the chosen entry's page, from 0 */
	Sint32 pageCnt;	 /* pages the list fills, the last one perhaps part full */
	Sint32 row;		 /* the chosen entry's row on its page */
	Sint32 entryCnt; /* entries in the list */
	/* the entry chosen last, kept within the list; 0 if empty */
	Sint32 selectedIdx;
} PAGED_LIST;

#endif
