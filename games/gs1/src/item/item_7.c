#include "scene.h"
#include "inventory.h"
#include "types.h"
#include "item.h"
#include "battle_random.h"

/* item/get_equipped_element.c */
s32 Owner_GetDefaultElement(void *state);

s32 Item_GetEquippedElement(void)
{
    struct ItemDefinition *item;
    void *owner;

    owner = OwnerState_Get();
    if (FIELD_AT_OFFSET(owner, u8 *, 0x129) == 0) {
        return Owner_GetDefaultElement(owner);
    }
    item = Inventory_GetEquippedDefinition(owner, 1);
    if (item != NULL) {
        return FIELD_AT_OFFSET(item, s32 *, 0x14);
    }
    return 4;
}

/* item/get_unleash_rate_bonus.c */
s32 Equipment_GetUnleashRateBonus(s32 owner)
{
    s32 sum;
    s32 offset;
    s32 index;
    u8 *data;
    s32 j;
    s32 mask;
    u16 v;

    sum = 0;
    offset = 216;
    mask = 0x200;
    index = 15;
    while (--index >= 0) {
        v = *(u16 *)((u8 *)offset + owner);
        if (v & mask) {
            data = (u8 *)Item_GetDirect(
                *(u16 *)((u8 *)offset + owner)) + 24;
            j = 4;
            while (--j >= 0) {
                if (data[0] == 23) { sum += (s8)data[1]; }
                data += 4;
            }
        }
        offset += 2;
    }
    if (sum < 0) sum = 0;
    return sum;
}

/* item/roll_weapon_unleash.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 FixedPoint_Ratio(s32, s32);

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
    rate = FixedPoint_Ratio(
        (Equipment_GetUnleashRateBonus((s32)owner) +
         (FIELD_AT_OFFSET(item, u8, 0xB) * 5)) << 0x10,
        100);
    if (rate > (s32)(BattleRandom16() & 0xFFFF)) {
        return FIELD_AT_OFFSET(item, u16, 0xE);
    }
    return 1;
}
