#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

/// @file
/// NodeChain, a chain of menu nodes.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A chain node seen only as its link to the next.
/// XXX: likely a SelectionNode, because next sits where SelectionNode.next
/// does; see SelectionScreenCursorView.
struct SelectionNodeLinkView {
  u8 padding00[4]; ///< SelectionNode.prev, if it is one
  struct SelectionNodeLinkView *next;
};

/// A chain's first node and how many links to follow from it, which the
/// unused NodeChain_GetNodeAtIndex reads.
/// XXX: likely a SelectionScreenWork (its row's first icon and the entry under
/// the hand), because head and cursorIndex sit at these offsets (0x348 and
/// 0x39e); a caller of NodeChain_GetNodeAtIndex would settle it.
struct SelectionScreenCursorView {
  u8 padding000[840];
  struct SelectionNodeLinkView
      *node; ///< SelectionScreenWork.head, if it is one
  u8 padding34c[82];
  u16 count; ///< SelectionScreenWork.cursorIndex, if it is one
};

STATIC_ASSERT(sizeof(struct SelectionNodeLinkView) == 8);
STATIC_ASSERT(offsetof(struct SelectionScreenCursorView, node) == 0x348);
STATIC_ASSERT(offsetof(struct SelectionScreenCursorView, count) == 0x39e);

#endif
