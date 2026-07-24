typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;

struct Result {
    s32 value0;
    s32 unused4;
    s32 value8;
    s32 valueC;
    s32 value10;
    s32 value14;
    s32 value18;
};

s32 Func_08077008(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);

s32 Func_080a6a00(struct Result *result, s32 index)
{
    s32 encoded;
    u8 *base = *(u8 **)0x03001F2C;
    s32 offset = index + 0x218;
    u8 *entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;
    s32 slot;

    encoded = Func_08077008(entries[offset]);
    limit = base[0x218];
    slot = entries[offset] + 0x260;
    value = ((s8 *)base)[slot];
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    quotient = Func_080022ec(value, 5);
    remainder = Func_080022fc(value, 5);
    groups = Func_080022ec(limit, 5);
    if (Func_080022fc(limit, 5) != 0) {
        groups++;
    }
    result->value0 = encoded;
    result->value8 = quotient;
    result->valueC = groups;
    result->value10 = remainder;
    result->value14 = limit;
    result->value18 = value;
    return 1;
}
