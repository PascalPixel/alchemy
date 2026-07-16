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

s32 Func_08079bc4();

u32 Func_08079be8(void) {
    return (u32) (Func_08079bc4() * 0x64) >> 0x10;
}
