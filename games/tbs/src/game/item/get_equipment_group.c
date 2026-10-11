#include "item.h"

s32 Item_GetEquipmentGroup(s32 item_id)
{
    s32 group;
    struct ItemDefinition *item;

    item = Item_GetDirect(item_id);
    group = 0;
    if (item->type == 1)
        group = 1;
    else if (item->type == 2)
        group = 2;
    else if (item->type == 3)
        group = 2;
    else if (item->type == 4)
        group = 2;
    else if (item->type == 5)
        group = 2;
    else if (item->type == 9)
        group = 2;
    return group;
}
