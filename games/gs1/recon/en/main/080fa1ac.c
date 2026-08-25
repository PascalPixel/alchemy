#include "types.h"

void Func_080fa1ac(void *unused, u8 *node)
{
    s32 flags;

    node[0x16] = 0;
    node[0x1a] = 0;
    if (node[0x18] == 0) {
        flags = 12;
    } else {
        flags = 3;
    }
    node[0] = node[0] | flags;
}
