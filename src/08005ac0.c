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

u32 Func_08005b24();
s32 Func_08005b64();

u32 Func_08005ac0(void) {
    s32 temp_r0;

    if (Func_08005b24() > 0xFU) {
        return 1U;
    }
    temp_r0 = Func_08005b64();
    return (u32) ((0 - temp_r0) | temp_r0) >> 0x1F;
}
