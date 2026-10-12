#include "types.h"
#include "inventory.h"

s32 Func_020004e0(void)
{
    s32 item;

    for (item = 429; item <= 437; item++) {
        if (PartyInventory_FindOwner(item) != -1)
            return 1;
    }
    for (item = 417; item <= 427; item++) {
        if (PartyInventory_FindOwner(item) != -1)
            return 1;
    }
    return 0;
}
