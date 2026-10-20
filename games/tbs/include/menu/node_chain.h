#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

/// @file
/// NodeChain, a chain of menu nodes.

#include "edition.h"
#include "types.h"
#include "static_assert.h"

/// A chain node seen only as its link to the next.
/// XXX: likely a SelectionNode, because next sits where SelectionNode.next
/// does; see NodeChainState.
struct NodeChainNode {
  u8 padding00[4]; ///< SelectionNode.prev, if it is one
  struct NodeChainNode *next;
};

/// A chain's first node and how many links to follow from it, which the
/// unused NodeChain_GetNodeAtIndex reads.
/// XXX: likely a SelectionScreen (its row's first icon and the entry under the
/// hand), because head and cursorIndex sit at these offsets (0x348 and
/// 0x39e); a caller of NodeChain_GetNodeAtIndex would settle it.
struct NodeChainState {
  u8 padding000[840];
  struct NodeChainNode *node; ///< SelectionScreen.head, if it is one
  u8 padding34c[82];
  u16 count; ///< SelectionScreen.cursorIndex, if it is one
};

STATIC_ASSERT(sizeof(struct NodeChainNode) == 8);
STATIC_ASSERT(offsetof(struct NodeChainState, node) == 0x348);
STATIC_ASSERT(offsetof(struct NodeChainState, count) == 0x39e);

#endif
