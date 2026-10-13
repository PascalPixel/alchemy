//--------------------------------------------------------------------------
// get_node_at_count.c
// メニュー：つながったノードの count 番目をさがす
//--------------------------------------------------------------------------

#include "types.h"
#include "node_chain.h"

//--------------------------------------------------------------------------
// st->node から next を st->count 回たどったノードを返す
//--------------------------------------------------------------------------
struct NodeChainNode *NodeChain_GetNodeAtCount(struct NodeChainState *st)
{
	struct NodeChainNode *node = st->node;
	s32					  i;

	for (i = 0; i != st->count; ++i)
	{
		node = node->next;
	}
	return node;
}
