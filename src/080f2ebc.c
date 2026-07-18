typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080f2ebc(s16 *arg0, s16 *arg1, s16 *arg2, s32 arg3) {
    s32 index;
    s32 first;
    s32 second;
    s32 (*divide)(s32, s32);

    if (arg3 > 0) {
        divide = (s32 (*)(s32, s32))0x03000380;
        index = 0x5FF;
        do {
            first = *arg0;
            second = *arg1;
            *arg2 = divide(second - first, arg3);
            index--;
            arg0++;
            arg1++;
            arg2++;
        } while (index >= 0);
    }
}
