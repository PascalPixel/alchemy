#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "LIB.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "BATTLE_CALC.H"
#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"
#include "BATTLE_TYPES.H"

u32 BtlRnd(void)
{
    s32 battle_value;
    battle_value = (gBattleRandomSeed * 0x41c64e6d) + 0x3039;
    gBattleRandomSeed = battle_value;
    return (u32)(battle_value << 8) >> 16;
}

u32 BattleRandomPercent(void)
{
    return (BtlRnd() * 0x64) >> 0x10;
}


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

s32 Owner_ScaleValueByCurve(s32 value, s32 selector, s32 multiplier)
{
    s32 result;

    result = Curve_LookupScaledValue(selector, 0) *
        value * multiplier;
    if (result < 0)
        result = result + 0xffff;
    return result >> 16;
}

s32 Owner_ScaleValueByOffsetCurve(s32 value, s32 selector, s32 multiplier)
{
    s32 result;
    u32 product;

    product = Curve_LookupScaledValue(
        selector * 2 - 0xC8, 0) * value;
    product = multiplier * product;
    result = product;
    if (result < 0) {
        result = result + 0xFFFF;
    }
    return result >> 0x10;
}

s32 Owner_GetDefaultElement(void *state);

s32 Item_GetEquippedElement(s32 owner_id)
{
    struct ItemDefinition *item;
    struct BtlUnit *owner;

    owner = UnitGet(owner_id);
    if (owner->class_index == 0) {
        return Owner_GetDefaultElement(owner);
    }
    item = Inventory_GetEquippedDefinition((void *)owner, 1);
    if (item != NULL) {
        return item->element;
    }
    return 4;
}
