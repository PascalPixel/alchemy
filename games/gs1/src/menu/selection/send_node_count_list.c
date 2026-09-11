#include "types.h"
#include "scene.h"
#include "abi/menu/selection/send_node_count_list.h"

void Menu_SendNodeCountList(u8 *arg0)
{
    u16 data[6];
    u8 *node = *(u8 **)(arg0 + 0x348);
    s32 count = 0;

    while (node != 0) {
        node = *(u8 **)(node + 4);
        count++;
    }
    data[count] = 0xff;
    Menu_Apply(data, 0);
}
