#include "scene.h"
#include "abi/item/get_definition.h"
#include "item.h"

extern struct ItemDefinition gRom[];

struct ItemDefinition *Item_GetDirect(s32 item_id)
{
    return gRom + (item_id & 0x1ff);
}
