typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
#define NULL ((void *)0)

s32 Func_080a3ddc(u8 *arg0, u16 *arg1) {
    u16 *src;
    u16 *out;
    s32 count;
    s32 i;
    s32 j;
    s32 offset;

    for (i = 31; i >= 0; i--) {
        arg1[i] = 0;
    }

    offset = 0;
    src = (u16 *)(arg0 + 216);
    out = arg1;
    count = 0;
    j = 14;
    do {
        *(u16 *)(offset + (s32)arg1) = 0;
        if (*src != 0) {
            *out = *src;
            count++;
            out++;
        }
        src++;
        offset += 2;
        j--;
    } while (j >= 0);
    return count;
}
