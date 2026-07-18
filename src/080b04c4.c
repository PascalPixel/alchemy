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

s32 Func_080030f8(s32);
s32 Func_080f9048(void);

void Func_080b04c4(void) {
    while (Func_080f9048() != 0) {
        Func_080030f8(1);
    }
}
