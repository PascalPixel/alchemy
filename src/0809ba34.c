typedef signed char s8;
typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

struct Fields_0809ba34 {
    u8 filler0c[0x0c];
    s32 field0c;
    u8 filler10[0x31];
    s8 field41;
};

u32 Func_0809ba34(struct Fields_0809ba34 *arg0) {
    s32 value;

    if (arg0->field41 == 0) {
        return 0;
    }
    value = arg0->field0c ^ 0x80000000;
    return (u32)((-value) | value) >> 31;
}
