#include "node_chain.h"
#include "types.h"

#define NodeChain_GetNodeAtCount Func_0801b36c

struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}
