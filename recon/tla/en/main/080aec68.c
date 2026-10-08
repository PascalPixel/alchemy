#include "types.h"
#include "item.h"

struct ItemDefinition *Item_GetDirect(s32 item_id);

s32 Item_GetEquipmentGroup(s32 item_id)
{
    s32 type = Item_GetDirect(item_id)->type;

    if (type == 1)
        return 1;
    if (type == 2)
        return 2;
    if (type == 3)
        return 2;
    if (type == 4)
        return 2;
    if (type == 5)
        return 2;
    if (type == 9)
        return 2;
    if (type == 7)
        return 1;
    return type == 10;
}
