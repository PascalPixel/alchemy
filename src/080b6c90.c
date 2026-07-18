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

s32 Func_080b6c08(s32, void *);
s32 Func_080b7b6c(void *, s32);

void Func_080b6c90(void) {
    u8 local[28];
    Func_080b6c08(3, local);
    Func_080b7b6c(local, 1);
}
