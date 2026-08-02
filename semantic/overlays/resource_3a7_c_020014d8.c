typedef signed int s32;
typedef unsigned int u32;

struct ConvertedValue_020014d8 {
    s32 unknown00;
    s32 direction;
    s32 amount;
    s32 numerator;
    s32 denominator;
};

extern void Func_02002c5a(const s32 input[2], struct ConvertedValue_020014d8 *out);
extern s32 Func_02002a54(const struct ConvertedValue_020014d8 *value);
extern s32 Func_02002a3e(const struct ConvertedValue_020014d8 *value);
extern s32 Func_02002a5c(const struct ConvertedValue_020014d8 *value);
extern s32 Func_02002aa6(s32 numerator, s32 denominator, s32 scale);

/* Convert an input pair and return its signed, scaled value when valid. */
s32 Func_020014d8(s32 first, s32 second)
{
    s32 input[2];
    struct ConvertedValue_020014d8 value;
    s32 result;

    input[0] = first;
    input[1] = second;
    Func_02002c5a(input, &value);

    if (Func_02002a54(&value) != 0 || Func_02002a3e(&value) != 0)
        return 0;

    if (Func_02002a5c(&value) != 0 || value.amount < 0 || value.amount > 30)
        return value.direction != 0 ? (s32)0x80000000u : 0x7fffffff;

    result = Func_02002aa6(value.numerator, value.denominator, 60 - value.amount);
    return value.direction != 0 ? -result : result;
}
