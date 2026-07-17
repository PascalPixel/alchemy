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

extern u8 Data_08185024[];

u8 *Func_08185008(u32 arg0) {
    return &Data_08185024[(arg0 & 0xfff) * 20];
}
