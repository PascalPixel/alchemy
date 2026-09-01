#include "types.h"

#define Curve_LookupScaledValue Func_08079b24

s32 FixedPoint_Ratio(s32 numerator, s32 denominator);

struct CurvePoint {
    s16 input;
    s16 value;
};

extern struct CurvePoint Data_08089258[];

s32 Curve_LookupScaledValue(s32 input, s32 halve)
{
    s32 clamped = input;
    struct CurvePoint *table;
    s32 mode;
    s32 upper;
    s32 lower;
    s32 count;
    s32 value;
    s32 index;
    s32 offset;
    s32 scan_offset;

    table = Data_08089258;
    mode = halve;
    upper = table[0].input;
    lower = table[4].input;
    count = 5;

    if (clamped > upper) {
        clamped = upper;
    } else if (clamped < lower) {
        clamped = lower;
    }

    index = 0;
    offset = 0;
    if (index < count) {
        if (clamped <= *(s16 *)((u8 *)table + offset)) {
            scan_offset = 0;
            do {
                index++;
                scan_offset += 4;
                if (index >= count) {
                    offset = index * 4;
                    break;
                }
                offset = scan_offset;
            } while (clamped <= *(s16 *)((u8 *)table + offset));
        }
    }

    if (index == count) {
        value = *(s16 *)((u8 *)table + offset - 2);
    } else {
        s32 previous_x = *(s16 *)((u8 *)table + offset - 4);
        s32 current_x = *(s16 *)((u8 *)table + offset);
        s32 previous_value = *(s16 *)((u8 *)table + offset - 2);
        s32 current_value = *(s16 *)((u8 *)table + offset + 2);

        value = FixedPoint_Ratio(
            (clamped - current_x) * (previous_value - current_value),
            previous_x - current_x);
        value += current_value;
    }

    if (mode == 1)
        value /= 2;
    return value + 0x100;
}
