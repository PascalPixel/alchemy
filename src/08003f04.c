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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

extern u8 Data_03001810[];
s32 Func_08003f04(s32 arg0) {
    s32 count = 0;
    s32 i;
    u8 *p;
    u8 fill;
    if ((u32)arg0 > 0x5F) {
        return -1;
    }
    p = Data_03001810;
    fill = 0xFF;
    i = 0x200;
    do {
        if (*p == arg0) { *p = fill; count++; }
        i--; p++;
    } while (i != 0);
    if (count != 0) {
        return -1;
    }
    return 0;
}
