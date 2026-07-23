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

extern u8 *Data_03001f2c;
s32 Func_08077008(s32);
s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);

static __inline__ u8 LoadByte(s32 base, s32 offset)
{
    return *(u8 *)(base + offset);
}

static __inline__ s8 LoadSignedByte(s32 base, s32 offset)
{
    return *(s8 *)(base + offset);
}

s32 Func_080a8b8c(struct Result *result, s32 index)
{
    s32 encoded;
    s32 base = *(s32 *)0x03001F2C;
    s32 offset = index + 0x218;
    s32 entries = base + 2;
    s32 limit;
    s32 remainder;
    s32 quotient;
    s32 groups;
    s32 value;

    encoded = Func_08077008(LoadByte(entries, offset));
    limit = LoadByte(base, 0x218);
    value = LoadSignedByte(base, LoadByte(entries, offset) + 0x260);
    if ((s32)(value + 1) > limit) {
        value = limit - 1;
    }
    if (limit == 0) {
        value = 0;
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
