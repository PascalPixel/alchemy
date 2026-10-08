//--------------------------------------------------------------------------
// GET_NODE_AT_COUNT.C
// メニュー：つながったノードの count 番目をさがす
//--------------------------------------------------------------------------

#include "TYPES.H"
#include "NODE_CHAIN.H"

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
