#include "BATTLE_CALC.H"
#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"
#include "BATTLE_TYPES.H"

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner)
{
    s32 sum;
    s32 i;
    s32 j;
    struct ItemDefinition *item;

    sum = 0;
    for (i = 0; i < 15; i++) {
        if (owner->inventory[i] & 0x200) {
            item = Item_GetDirect(owner->inventory[i]);
            for (j = 0; j < 4; j++) {
                if (item->effects[j].kind == 23)
                    sum += item->effects[j].amount;
            }
        }
    }
    if (sum < 0)
        sum = 0;
    return sum;
}
