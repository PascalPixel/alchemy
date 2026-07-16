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
#define M2C_FIELD(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Func_08012330(s32 arg0, s32 arg1, s32 arg2) {
    void *temp_r3;

    temp_r3 = *(void **)0x03001E70;
    if (arg0 >= 0) {
        M2C_FIELD(temp_r3, s32 *, 4) = arg0;
    }
    if (arg1 >= 0) {
        M2C_FIELD(temp_r3, s32 *, 8) = arg1;
    }
    if (arg2 >= 0) {
        M2C_FIELD(temp_r3, s32 *, 0xC) = arg2;
    }
}
