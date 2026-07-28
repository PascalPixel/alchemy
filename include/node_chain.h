#ifndef ALCHEMY_NODE_CHAIN_H
#define ALCHEMY_NODE_CHAIN_H

#include "types.h"
#include "layout_guard.h"

struct NodeChainNode {
    u8 padding_00[4];
    struct NodeChainNode *next;
};

struct NodeChainState {
    u8 padding_000[840];
    struct NodeChainNode *node;
    u8 padding_34c[82];
    u16 count;
};

LAYOUT_SIZE_GUARD(NodeChainNode_Size, struct NodeChainNode, 8);
LAYOUT_OFFSET_GUARD(
    NodeChainState_NodeOffset, struct NodeChainState, node, 0x348);
LAYOUT_OFFSET_GUARD(
    NodeChainState_CountOffset, struct NodeChainState, count, 0x39e);

struct NodeChainNode *Func_0801b36c(struct NodeChainState *);
struct NodeChainNode *Func_080b0694(struct NodeChainState *);

#endif
