typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;

s32 Func_08011ddc(s8 *values, s32 start, s32 end) {
    s32 first;
    s32 second;
    s32 third;
    s32 difference;
    s32 delta;

    first = *values++ << 19;
    second = values[0] << 19;
    third = values[1];
    difference = end - start;
    start = difference + 15;
    third <<= 19;

    if (start == 15)
        return second;

    if ((u32)start <= 14) {
        delta = second - first;
        difference = delta * start;
        if (difference < 0)
            difference += 15;
        difference >>= 4;
        return first + difference;
    } else {
        difference *= third - second;
        if (difference < 0)
            difference += 15;
        difference >>= 4;
        return second + difference;
    }
}
