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

s32 Func_08090a5c(s32, s32, s32, s32);

void Func_08091200(s32 arg0, s32 arg1) {
    s32 temp_r1;

    temp_r1 = *(s32 *)0x03001ED0;
    if (temp_r1 != 0) {
        Func_08090a5c(arg0, temp_r1, temp_r1 + 0xE00, arg1);
    }
}
