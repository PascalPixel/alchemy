#include "item.h"

extern struct ItemDefinition Data_0807b6a8[];

struct ItemDefinition *Item_GetDirect(s32 item_id)
{
    return Data_0807b6a8 + (item_id & 0x1ff);
}
