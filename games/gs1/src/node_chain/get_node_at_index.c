#include "node_chain.h"
#include "types.h"

struct NodeChainNode *NodeChain_GetNodeAtIndex(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}
