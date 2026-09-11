#include "types.h"
#include "scene.h"
#include "abi/owner/scale_value_by_offset_curve.h"

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
