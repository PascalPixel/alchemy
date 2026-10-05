#include "TYPES.H"
#include "BATTLE_UNIT.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"

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

/* Rolls the equipped weapon's unleash: its rate in fifths of a percent plus
   the equipment's bonus against a 16-bit random number. Returns the
   unleash's message, or 1 when there is none or the roll fails. */
s32 RollWeaponUnleash(struct BattleUnit *owner)
{
    struct ItemDefinition *item;
    s32 rate;

    if (owner->class_index == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (item->description_message == 0) {
        return 1;
    }
    rate = ((Equipment_GetUnleashRateBonus(owner) +
         (item->secondary_flags * 5)) << 0x10) / 100;
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return item->description_message;
    }
    return 1;
}
