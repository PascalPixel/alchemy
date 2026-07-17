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

extern s32 Data_08088db8[];

s32 Func_080797ec(s32 arg0, s32 arg1) {
    return Data_08088db8[(arg0 * 4) + arg1];
}
