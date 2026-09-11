#include "types.h"
#include "scene.h"
#include "inventory.h"
#include "item.h"
#include "battle_random.h"

/* owner/scale_adjusted_amount.c */
/* owner/scale_adjusted_amount.c */
s32 Owner_ScaleAdjustedAmount(s32 amount, s32 reduction, s32 adjustment, s32 selector)
{
    s32 result;
    s32 product;

    amount -= reduction;
    result = selector;
    if (amount < 0) {
        amount = 0;
    }
    product = Sys_Apply(result, 1) * (amount + (adjustment * 2));
    if (product < 0) {
        product += 0x1FF;
    }
    result = product >> 9;
    if (result < 0) {
        result = 0;
    }
    return result;
}

/* owner/scale_value_by_curve.c */
s32 Owner_ScaleValueByCurve(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    s32 shifted;
    s32 zero;

    result = (s32)((u32)Sys_Apply(no, 0) *
        (u32)value *(u32)multiplier);
    zero = 0;
    do {
        if (result < zero) {
            result = (s32)((u32)result + 0xFFFF);
        }
        shifted = result >> 0x10;
        return shifted;
    } while (zero);
}

/* owner/scale_value_by_offset_curve.c */
s32 Owner_ScaleValueByOffsetCurve(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    u32 product;

    product = (u32)Sys_Apply(
        (s32)((u32)no * 2 - 0xC8), 0) * (u32)value;
    product = (u32)multiplier *product;
    result = (s32)product;
    if (result < 0) {
        result = (s32)((u32)result + 0xFFFF);
    }
    return result >> 0x10;
}

/* item/get_equipped_element.c */
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
