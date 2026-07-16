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

s32 Func_08004278(s32);
s32 Func_080770c8(s32);
s32 Func_080f9010(s32);

void Func_080a2444(void) {
    if (*(s32 *)0x03001C94 & 8) {
        Func_080f9010(0x71);
        Func_080770c8(0x150);
        Func_08004278(0x080A2445);
    }
}
