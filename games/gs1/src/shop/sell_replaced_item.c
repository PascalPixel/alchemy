#include "battle_runtime.h"
#include "shop.h"

void Func_080b1f4c(s32, s32, s32);

s32 Shop_SellReplacedItem(s32 unit_id, s32 slot)
{
    struct BattleUnit *unit;
    s32 item_offset;
    s32 item_id;

    unit = BattleUnit_Get(unit_id);
    if (slot == -1)
    {
        return 0;
    }
    item_offset = slot * 2 + 216;
    item_id = 0x1ff & *(u16 *)((u8 *)unit + item_offset);
    if (Item_Get(item_id)->type == 6)
    {
        return 0;
    }
    if (Item_Get(item_id)->flags & 8)
    {
        return 0;
    }
    Func_080b1f4c(unit_id, slot, -1);
    return 1;
}
