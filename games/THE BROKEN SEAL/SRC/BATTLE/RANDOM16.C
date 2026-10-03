#include "BATTLE_CALC.H"
#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"
#include "BATTLE_TYPES.H"

/* battle/random16.c */
u32 BattleRandom16(void)
{
    s32 battle_value;
    battle_value = (gBattleRandomSeed * 0x41c64e6d) + 0x3039;
    gBattleRandomSeed = battle_value;
    return (u32)(battle_value << 8) >> 16;
}

/* battle/random_percent.c */
u32 BattleRandomPercent(void)
{
    return (u32)(BattleRandom16() * 0x64) >> 0x10;
}

/* owner/scale_adjusted_amount.c */
s32 Curve_LookupScaledValue(s32 selector, s32 mode);

s32 Owner_ScaleAdjustedAmount(s32 amount, s32 reduction, s32 adjustment, s32 selector)
{
    s32 result;
    s32 product;

    amount -= reduction;
    result = selector;
    if (amount < 0) {
        amount = 0;
    }
    product = Curve_LookupScaledValue(result, 1) * (amount + (adjustment * 2));
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
s32 Owner_ScaleValueByCurve(s32 value, s32 selector, s32 multiplier)
{
    s32 result;

    result = (s32)((u32)Curve_LookupScaledValue(selector, 0) *
        (u32)value * (u32)multiplier);
    if (result < 0)
        result = (s32)((u32)result + 0xffff);
    return result >> 16;
}

/* owner/scale_value_by_offset_curve.c */
s32 Owner_ScaleValueByOffsetCurve(s32 value, s32 selector, s32 multiplier)
{
    s32 result;
    u32 product;

    product = (u32)Curve_LookupScaledValue(
        (s32)((u32)selector * 2 - 0xC8), 0) * (u32)value;
    product = (u32)multiplier * product;
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
    struct BattleUnit *owner;

    owner = Owner_GetState();
    if (owner->class_index == 0) {
        return Owner_GetDefaultElement(owner);
    }
    item = Inventory_GetEquippedDefinition(
        (struct BattleUnit *)owner, 1);
    if (item != NULL) {
        return item->element;
    }
    return 4;
}

/* item/get_unleash_rate_bonus.c */
s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner)
{
    /* FAKEMATCH: retain the existing inventory offset and byte-effect cursors;
       direct array cursors remove the four-byte call spill and reorder operands. */
    s32 sum;
    s32 offset;
    s32 index;
    u8 *data;
    s32 j;
    s32 mask;
    u16 v;

    sum = 0;
    offset = (u32)&((struct BattleUnit *)0)->inventory;
    mask = 0x200;
    index = 15;
    while (--index >= 0) {
        v = *(u16 *)((u8 *)offset + (u32)owner);
        if (v & mask) {
            data = (u8 *)Item_GetDirect(
                *(u16 *)((u8 *)offset + (u32)owner)) + (u32)&((struct ItemDefinition *)0)->effects;
            j = 4;
            while (--j >= 0) {
                if (((struct ItemEffect *)data)->kind == 23)
                    sum += ((struct ItemEffect *)data)->amount;
                data += sizeof(struct ItemEffect);
            }
        }
        offset += sizeof(owner->inventory[0]);
    }
    if (sum < 0)
        sum = 0;
    return sum;
}
