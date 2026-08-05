#include "types.h"

struct ConvertedValue_020014d8 {
    s32 unknown00;
    s32 direction;
    s32 amount;
    s32 numerator;
    s32 denominator;
};

extern void Func_02001770(const s32 input[2], struct ConvertedValue_020014d8 *out);
extern s32 Func_02001564(const struct ConvertedValue_020014d8 *value);
extern s32 Func_02001544(const struct ConvertedValue_020014d8 *value);
extern s32 Func_02001554(const struct ConvertedValue_020014d8 *value);
extern s32 Func_02001574(s32 numerator, s32 denominator, s32 scale);

/* Convert an input pair and return its signed, scaled value when valid. */
s32 Func_020014d8(s32 first, s32 second)
{
    s32 input[2];
    struct ConvertedValue_020014d8 value;
    s32 result;

    input[0] = first;
    Func_02001770(input, &value);
    input[1] = second;

    if (Func_02001564(&value) != 0 || Func_02001544(&value) != 0)
        return 0;

    if (Func_02001554(&value) != 0 || value.amount < 0 || value.amount > 30)
        return value.direction != 0 ? (s32)0x80000000u : 0x7fffffff;

    result = Func_02001574(value.numerator, value.denominator, 60 - value.amount);
    return value.direction != 0 ? -result : result;
}
