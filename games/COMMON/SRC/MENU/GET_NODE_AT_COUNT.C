#include "TYPES.H"
#include "NODE_CHAIN.H"

/* The node the selection's count reaches, walking the chain from its head. */
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}
