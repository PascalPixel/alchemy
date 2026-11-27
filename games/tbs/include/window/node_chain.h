#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

/* @file */
/* NodeChain, a chain of menu nodes. */

#include "edition.h"
#include "static_assert.h"
#include "types.h"

/* XXX: likely a SELECTION_NODE, because next sits where SELECTION_NODE.next
 * does;
 */
/* see SELECTION_MENU_CURSOR_VIEW. */
/* A chain node seen only as its link to the next. */
typedef struct SELECTION_NODE_LINK_VIEW {
	u8 paddingBeforeNext[4]; /* SELECTION_NODE.prev, if it is one */
	struct SELECTION_NODE_LINK_VIEW *next;
} SELECTION_NODE_LINK_VIEW;

/* XXX: likely a SELECTION_MENU_STATE (its row's first icon and the entry under
 */
/* the hand), because head and cursorIndex sit at these offsets (840 and */
/* 926); a caller of syNodeChainGetNodeAtIndex would settle it. */
/* A chain's first node and how many links to follow from it, which the unused
 */
/* syNodeChainGetNodeAtIndex reads. */
typedef struct SELECTION_MENU_CURSOR_VIEW {
	u8 paddingBeforeNode[840];
	SELECTION_NODE_LINK_VIEW
	*node; /* SELECTION_MENU_STATE.head, if it is one */
	u8 paddingBeforeCnt[82];
	u16 cnt; /* SELECTION_MENU_STATE.cursorIdx, if it is one */
} SELECTION_MENU_CURSOR_VIEW;

STATIC_ASSERT(sizeof(SELECTION_NODE_LINK_VIEW) == 8);
STATIC_ASSERT(offsetof(SELECTION_MENU_CURSOR_VIEW, node) == 840);
STATIC_ASSERT(offsetof(SELECTION_MENU_CURSOR_VIEW, cnt) == 926);

#endif
