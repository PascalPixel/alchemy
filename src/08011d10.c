typedef signed char s8;
typedef signed int s32;

s32 Func_08011d10(s8 *values, s32 unused, s32 factor) {
    s32 base;
    s32 offset;

    base = values[0] << 19;
    offset = ((values[1] << 19) - base) * factor;
    if (offset < 0) {
        offset += 15;
    }
    return base + (offset >> 4);
}
