#include "types.h"
#include "inventory.h"

static void SceneActor_ApplyValueAndMatchingSlots(s32 a, s32 b)
{
    struct BattleUnit *t = UnitGet(a);
    s32                i;

    Inventory_AddItem(a, b);
    for (i = 0; i <= 14; i++)
    {
        if (t->inventory[i] == b)
        {
            Inventory_Equip(a, i);
        }
    }
}
