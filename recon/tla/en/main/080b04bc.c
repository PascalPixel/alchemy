/*
 * Draft: RollWeaponUnleash does not yet match; 2 halfwords differ from ☀️'s C, first at +0x30 (movs r1, #100).
 * Links as recon/tla/raw/080b04bc.s.
 */
#include "INVENTORY.H"

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

u16 RollWeaponUnleash(struct BattleUnit *owner)
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
    rate = __divsi3(
        (Equipment_GetUnleashRateBonus(owner) +
         (FIELD_AT_OFFSET(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return FIELD_AT_OFFSET(item, u16, 0xE);
    }
    return 1;
}
