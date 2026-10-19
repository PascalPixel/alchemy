/// @file
/// Menu: find the node at index count in a linked list.

#include "types.h"
#include "node_chain.h"

/// Starting at st->node, follow next st->count times and return that node.
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st) {
  struct NodeChainNode *node = st->node;
  s32 i;

  for (i = 0; i != st->count; ++i) {
    node = node->next;
  }
  return node;
}
