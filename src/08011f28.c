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

extern u8 Data_080133fc[];

s32 Func_08011f28(s8 *arg0, u32 arg1, u32 arg2) {
    return arg0[Data_080133fc[((arg2 << 4) - arg1) + 15]] << 19;
}
