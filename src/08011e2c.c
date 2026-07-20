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

extern u8 Data_080132fc[];

s32 Func_08011e2c(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_r0;

    temp_r0 = M2C_FIELD(arg0, s8, 0) << 0x13;
    return temp_r0 + (((M2C_FIELD(arg0, s8, 1) << 0x13) - temp_r0) * Data_080132fc[arg1 + (arg2 * 0x10)]);
}
