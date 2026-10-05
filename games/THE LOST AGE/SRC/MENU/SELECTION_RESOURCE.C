#include "TYPES.H"
#include "NODE_CHAIN.H"

void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    struct NodeChainNode *node = ((struct NodeChainState *)arg0)->node;
    s32 count = 0;

    while (node != 0) {
        node = node->next;
        count++;
    }
    data[count] = 0xff;
    BattlePres_SetActorModesFar(data, 0);
}
