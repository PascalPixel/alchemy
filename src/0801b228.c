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

s32 Func_0801b248(s32, s32);

void Func_0801b228(void) {
    s32 temp_r5;

    temp_r5 = *(s32 *)0x03001E98;
    Func_0801b248(temp_r5, 0);
    Func_0801b248(temp_r5, 1);
}
