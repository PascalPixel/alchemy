#include "TYPES.H"
#include "SCENE.H"
#include "ITEM.H"
#include "INVENTORY.H"
#include "BATTLE_RANDOM.H"

/* battle/random16.c */

s32 Owner_ScaleValueByCurve(s32 value, s32 no, s32 multiplier)
{
    s32 result;
    s32 shifted;
    s32 zero;

    result = (s32)((u32)Curve_LookupScaledValue(no, 0) *
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
