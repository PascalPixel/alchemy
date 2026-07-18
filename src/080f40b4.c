typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_080f40b4(s32 arg0, s32 arg1) {
    s16 left = arg0;
    s16 right = arg1;
    s32 product;
    s32 adjusted;
    s32 multiplier;

    multiplier = right;
    product = left;
    product *= multiplier;
    if (product >= 0) {
        adjusted = product;
    } else {
        adjusted = product + 255;
    }
    return (s16)(adjusted >> 8);
}
