#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"
#include "FIXED_MATH.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

u16 RollWeaponUnleash(void *owner)
{
    struct ItemDefinition *item;
    s32 rate;

    if (FIELD_AT_OFFSET(owner, u8, 0x129) == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (FIELD_AT_OFFSET(item, u16, 0xE) == 0) {
        return 1;
    }
    rate = Math_Div(
        (Equipment_GetUnleashRateBonus((s32)owner) +
         (FIELD_AT_OFFSET(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return FIELD_AT_OFFSET(item, u16, 0xE);
    }
    return 1;
}
