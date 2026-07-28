#include "node_chain.h"
#include "types.h"

struct NodeChainNode *Func_080b0694(struct NodeChainState *state)
{
    struct NodeChainNode *node = state->node;
    s32 index;

    for (index = 0; index != state->count; ++index) {
        node = node->next;
    }
    return node;
}
