#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

/// @file
/// NodeChain, a chain of menu nodes.

#include "edition.h"
#include "static_assert.h"
#include "types.h"

// XXX: likely a SelectionNode, because next sits where SelectionNode.next does;
// see SelectionMenuCursorView.
/// A chain node seen only as its link to the next.
struct SelectionNodeLinkView {
  u8 paddingBeforeNext[4]; ///< SelectionNode.prev, if it is one
  struct SelectionNodeLinkView *next;
};

// XXX: likely a SelectionMenuState (its row's first icon and the entry under
// the hand), because head and cursorIndex sit at these offsets (840 and
// 926); a caller of NodeChain_GetNodeAtIndex would settle it.
/// A chain's first node and how many links to follow from it, which the unused
/// NodeChain_GetNodeAtIndex reads.
struct SelectionMenuCursorView {
  u8 paddingBeforeNode[840];
  struct SelectionNodeLinkView *node; ///< SelectionMenuState.head, if it is one
  u8 paddingBeforeCount[82];
  u16 count; ///< SelectionMenuState.cursorIndex, if it is one
};

STATIC_ASSERT(sizeof(struct SelectionNodeLinkView) == 8);
STATIC_ASSERT(offsetof(struct SelectionMenuCursorView, node) == 840);
STATIC_ASSERT(offsetof(struct SelectionMenuCursorView, count) == 926);

#endif
