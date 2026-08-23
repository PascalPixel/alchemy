#include "inventory.h"
#include "battle_random.h"

#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);

u16 RollWeaponUnleash(void *owner) {
    struct ItemDefinition *item;
    s32 value;

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
    value = Func_080022ec(
        (Equipment_GetUnleashRateBonus((s32)owner) +
         (M2C_FIELD(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (value > (s32) (BattleRandom16() & 0xFFFF)) {
        return M2C_FIELD(item, u16, 0xE);
    }
    return 1;
}
