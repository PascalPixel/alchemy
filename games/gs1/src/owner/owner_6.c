#include "types.h"
#include "scene.h"

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
