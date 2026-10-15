#include "types.h"
#include "owner_state.h"

// owner の持ちもののうち空でない欄の数
s32 ItemMenu_Count(s32 owner_id)
{
    s32                         count = 0;
    struct OwnerInventoryState *state = UnitGet(owner_id);
    s32                         i;

    for (i = 0; i < 15; i++)
    {
        s32 item_id = state->inventory[i] & 0x1ff;

        if (item_id != 0)
            count++;
    }
    return count;
}

s32 InventoryMenu_GetItemQuantity(s32 owner, s32 item)
{
    struct OwnerInventoryState *state;
    s32                         i;
    s32                         quantity;

    quantity = 0;
    state = UnitGet(owner);
    i = 0;
    do
    {
        if (state->inventory[i] != 0 && (state->inventory[i] & 0x1ff) == item)
        {
            quantity += (state->inventory[i] & 0xf800) >> 11;
            quantity++;
        }
        i++;
    } while (i <= 14);
    return quantity;
}
