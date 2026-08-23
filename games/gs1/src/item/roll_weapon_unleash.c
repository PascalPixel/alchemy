#include "inventory.h"
#include "battle_random.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);

u16 RollWeaponUnleash(void *owner) {
    struct ItemDefinition *item;
    s32 success_threshold;

    if (M2C_FIELD(owner, u8, 0x129) == 0) {
        return 1;
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item == NULL) {
        return 1;
    }
    if (M2C_FIELD(item, u16, 0xE) == 0) {
        return 1;
    }
    success_threshold = FixedPoint_Ratio(
        (Equipment_GetUnleashRateBonus((s32)owner) +
         (M2C_FIELD(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (success_threshold > (s32) (BattleRandom16() & 0xFFFF)) {
        return M2C_FIELD(item, u16, 0xE);
    }
    return 1;
}
