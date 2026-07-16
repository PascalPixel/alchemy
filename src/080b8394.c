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

s32 Func_08009080(s32, s32);
s32 Func_08009140(s32);
s32 *Func_080b7dd0();

void Func_080b8394(void) {
    s32 temp_r5;

    temp_r5 = *Func_080b7dd0();
    Func_08009140(temp_r5);
    Func_08009080(temp_r5, 2);
}
