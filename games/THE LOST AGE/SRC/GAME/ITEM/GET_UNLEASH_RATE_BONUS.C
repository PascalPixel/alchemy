#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner)
{
    s32 sum = 0;
    u16 *slot = owner->inventory;
    s32 index = 15;
    s32 j;
    struct ItemEffect *effect;
    u16 value;

    while (--index >= 0) {
        value = *slot;
        if (value & 0x200) {
            effect = Item_GetDirect(*slot)->effects;
            j = 4;
            while (--j >= 0) {
                if (effect->kind == 23)
                    sum += effect->amount;
                effect++;
            }
        }
        slot++;
    }
    if (sum < 0)
        sum = 0;
    return sum;
}
