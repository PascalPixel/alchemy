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

s32 Func_08016418(s32, s32);

void Func_080174d8(void) {
    s32 *temp_r5;
    s32 temp_r0;

    temp_r5 = *(s32 **)0x03001EE4;
    temp_r0 = *temp_r5;
    if (temp_r0 != 0) {
        Func_08016418(temp_r0, 1);
        *temp_r5 = 0;
    }
}
