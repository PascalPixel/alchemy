#include "types.h"
#include "ram_buffer.h"

struct ShopItemList
{
    u8  unk000[0x270];
    u16 items[0x149];
    u16 count;
    u8  unk504[2];
    s8  group;
};

s32 Item_GetEquipmentGroupFar(s32 item);
s32 Item_AdjustCounterFar(s32 item_id, s32 delta);

s32 Func_081080a8(void)
{
    struct ShopItemList *menu = Ram_HeapSlots->menuRuntime;
    s32                  count = 0;
    s32                  item;

    for (item = 0; item < 0x200; item++)
    {
        s32 group = menu->group;

        if (group != 3)
        {
            if (group != 4)
            {
                if (group != Item_GetEquipmentGroupFar(item))
                    continue;
            }
        }
        if (Item_AdjustCounterFar(item, 0) != 0)
            menu->items[count++] = item;
    }
    menu->items[count] = 0;
    menu->count = count;
    return count;
}
