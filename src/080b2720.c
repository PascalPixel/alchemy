typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;

extern s16 Data_080b41ac[][33];

s32 Func_080b2720(s32 arg0, u16 *arg1) {
    s16 *src;
    u16 *dst;
    s32 count;

    count = 0;
    if (Data_080b41ac[arg0][0] != 0) {
        dst = arg1;
        src = Data_080b41ac[arg0];
        do {
            *dst = *src;
            count++;
            src++;
            dst++;
        } while (count <= 23 && *src != 0);
    }
    arg1[count] = 0;
    return count;
}
