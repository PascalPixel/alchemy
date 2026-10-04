/* Draft: RollWeaponUnleash, 88-byte EN extent.
 * 2026-10-04: ☀️'s body (DJINN.C) differs by one instruction's place: the
 * native sets r1 = 100 for __divsi3 right after loading secondary_flags,
 * in the load's shadow; this build sets it after the sum. The same body
 * matches with -mtune=arm9tdmi (load scheduling on), which is not ⚓️'s
 * option set (K1), so no device was added.
 */
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner);

s32 RollWeaponUnleash(struct BattleUnit *owner)
{
    struct ItemDefinition *item;
    s32 rate;

    if (owner->class_index == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition((struct OwnerInventoryState *)owner, 1);
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
