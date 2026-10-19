#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

#include "edition.h"
#include "types.h"
#include "static_assert.h"

struct NodeChainNode {
  u8 padding00[4];
  struct NodeChainNode *next;
};

struct NodeChainState {
  u8 padding000[840];
  struct NodeChainNode *node;
  u8 padding34c[82];
  u16 count;
};

STATIC_ASSERT(sizeof(struct NodeChainNode) == 8);
STATIC_ASSERT(offsetof(struct NodeChainState, node) == 0x348);
STATIC_ASSERT(offsetof(struct NodeChainState, count) == 0x39e);

struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *state);
struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *);

#endif
