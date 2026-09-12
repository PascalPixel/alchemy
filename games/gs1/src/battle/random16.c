#include "types.h"
#include "scene.h"
#include "item.h"
#include "inventory.h"

/* battle/random16.c */
u32 BattleRandom16(void)
{
    s32 battle_value;
    battle_value = (*(s32 *)0x020023a8 * 0x41c64e6d) + 0x3039;
    *(s32 *)0x020023a8 = battle_value;
    return (u32)(battle_value << 8) >> 16;
}

/* battle/random_percent.c */
u32 BattleRandomPercent(void)
{
    return (u32)(BattleRandom16() * 0x64) >> 0x10;
}

/* owner/scale_adjusted_amount.c */
s32 Sys_Apply(s32 selector, s32 mode);

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
s32 Owner_GetDefaultElement(void *state);

s32 Item_GetEquippedElement(void)
{
    struct ItemDefinition *item;
    void *owner;

    owner = OwnerState_Get();
    if (FIELD_AT_OFFSET(owner, u8 *, 0x129) == 0) {
        return Owner_GetDefaultElement(owner);
    }
    item = Inventory_GetEquippedDefinition(
        (struct OwnerInventoryState *)owner, 1);
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
